#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <thread>

#include <unordered_map>
#include <curl/curl.h>
#include <imgui.h>

#include "Tile.h"

enum State {
  DOWNLOADING,
  IDLE,
  COMPLETED
};

class EarthDataCollector {
public:
  EarthDataCollector () {
    readUrls();
    areFilesLoaded = checkFileStatus();
  }

  void showDataCollector() {
    ImGui::Begin("Data Collector");

    ImGui::SliderInt("Latitude", &latitude, -90, 90);
    ImGui::SliderInt("Longitude", &longitude, -180, 180);

    ImGui::End();
  }

  bool checkFileStatus() {
    if (!std::filesystem::exists(this->rawDir)) {
      std::filesystem::create_directory(this->rawDir);
      return false;
    }

    int fileCount = 0;

    for (auto _ : std::filesystem::directory_iterator(this->rawDir)){
        fileCount++;
    }
    std::cout << fileCount << std::endl;
    // TODO improve and add file name check
    return fileCount == urls.size();
  }

  void readUrls() {
    std::ifstream infile(this->urlPath);

    std::string url;

    while (infile >> url) {
      this->urls.push_back(url);
    }
  }

  float getProgress() {
    if(this->fileCheckFrameClock++ % 10 == 0) {
      this->foundFiles = 0;

      for (auto& p : std::filesystem::directory_iterator({this->rawDir}))
      {
          ++foundFiles;
      }
    }

    return (float)foundFiles / this->urls.size();
  }

  static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
  {
    size_t written = fwrite(ptr, size, nmemb, stream);
    fclose(stream);
    return written;
  }

  static std::string extractFilename(const std::string& url) {
    std::stringstream ss(url);
    std::string token;
    std::string lastPart;

    while (std::getline(ss, token, '/')) {
        if (!token.empty())
            lastPart = token;
    }

    // Remove the prefix "ASTGTMV003_" if it exists
    size_t pos = lastPart.find("ASTGTMV003_");
    if (pos != std::string::npos) {
        lastPart.erase(0, pos + std::string("ASTGTMV003_").length());
    }

    return lastPart;
  }

  static bool file_exists(const std::string& path) {
      std::ifstream file(path);
      return file.good();
  }
 
  static void add_transfer(CURLM *cm, unsigned int i, int *left, FILE *fp, const char * url)
  {
    CURL *eh = curl_easy_init();
    curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(eh, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(eh, CURLOPT_URL, url);
    curl_easy_setopt(eh, CURLOPT_PRIVATE, url);
    curl_multi_add_handle(cm, eh);
    (*left)++;
  }

  static std::string getFilemaneFromURL(const std::filesystem::path *rawDir, std::string url){
    return (*rawDir) / extractFilename(url);
  }

  static void loadFiles(const std::vector<std::string> *urls, const std::filesystem::path *rawDir, State *state, int maxParallel) {
    state = reinterpret_cast<State*>(State::DOWNLOADING);

    // Create the directory if it does not exist
    std::filesystem::create_directory(*rawDir);

    CURLM *cm;
    CURLMsg *msg;
    unsigned int transfers = 0;
    unsigned int transferPointer = 0;
    int msgs_left = -1;
    int left = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    cm = curl_multi_init();

    // Limit the amount of simultaneous connections curl should allow:
    curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)maxParallel);

    for(transfers = 0; transfers < maxParallel && transferPointer < urls->size(); transfers++) {
        auto fileName = getFilemaneFromURL(rawDir, (*urls)[transferPointer]);
        std::cout << (transfers < maxParallel && transferPointer < urls->size()) << std::endl;
        if (file_exists(fileName)){
          std::cout << fileName << " found" << std::endl;
            transferPointer++;
        }
        else{
          FILE* tileFile = fopen(fileName.c_str(), "wb");
          add_transfer(cm, transferPointer++, &left, tileFile, (*urls)[transferPointer].c_str());
        }
    }

    do {
        std::cout << transferPointer << "/" << urls->size() << "\r";
        int still_alive = 1;
        curl_multi_perform(cm, &still_alive);

        while((msg = curl_multi_info_read(cm, &msgs_left))) {
            if(msg->msg == CURLMSG_DONE) {
                char *url;
                CURL *e = msg->easy_handle;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
                curl_multi_remove_handle(cm, e);
                curl_easy_cleanup(e);
                left--;
            } else {
                std::cerr << "E: CURLMsg (" << msg->msg << ")" << std::endl;
            }

            if(transfers < urls->size()){
                auto fileName = getFilemaneFromURL(rawDir, (*urls)[transferPointer]);
                if (file_exists(fileName)){
                    std::cout << fileName << " found" << std::endl;
                    transferPointer++;
                    continue;
                }

                FILE* tileFile = fopen(fileName.c_str(), "wb");
                add_transfer(cm, transferPointer++, &left, tileFile, (*urls)[transferPointer].c_str());
            }
        }

        if(left)
            curl_multi_wait(cm, NULL, 0, 1000, NULL);

    } while(left);

    curl_multi_cleanup(cm);
    curl_global_cleanup();

    state = reinterpret_cast<State*>(State::COMPLETED);
  }
private:
  bool areFilesLoaded;
  std::filesystem::path urlPath{"resources/urls.txt"};
  std::vector<std::string> urls{};
  std::filesystem::path rawDir{"resources/raw"};
  State state;
  unsigned int foundFiles{0};
  unsigned int fileCheckFrameClock{0};
  int latitude = 0.0f;
  int longitude = 0.0f;
  std::string url;
  //std::unordered_map<std::pair<int, int>, Tile> tileByCoordinate{{}};
};