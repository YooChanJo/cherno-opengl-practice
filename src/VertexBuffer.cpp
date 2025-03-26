// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../includes/VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &m_RendererID); // generator one buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // activate that buffer
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // DEFAULT bind data to buffer
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // activate that buffer
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0); // activate that buffer
}