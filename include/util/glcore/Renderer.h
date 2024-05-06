#pragma once

#include "util/glcore/VertexArray.h"
#include "util/glcore/IndexBuffer.h"
#include "util/glcore/Shader.h"

class Renderer
{
public:
    void DrawTriangles(const VertexArray& va, const IndexBuffer &ib, const Shader& shader) const;
    void DrawPoints(const VertexArray& va, const IndexBuffer &ib, const Shader& shader) const;
    void Clear() const;
};