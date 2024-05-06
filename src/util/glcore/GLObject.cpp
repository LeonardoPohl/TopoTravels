#include "imgui.h"
#include "util/glcore/GLObject.h"

namespace GLObject
{
	TestMenu::TestMenu(GLObject *&currentTestPointer, const Camera *cam)
		: m_CurrentTest(currentTestPointer)		
	{
		camera = cam;
	}


	void TestMenu::OnImGuiRender()
	{
		for (auto const &test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second(camera);
			}
		}
	}
}