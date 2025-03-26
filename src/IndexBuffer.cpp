// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../includes/IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    glGenBuffers(1, &m_RendererID); // generator one buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); // activate that buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW); // DEFAULT bind data to buffer
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); // activate that buffer
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // activate that buffer
}