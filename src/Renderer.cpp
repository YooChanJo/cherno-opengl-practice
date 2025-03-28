// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../includes/Renderer.h"

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}
