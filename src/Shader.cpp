// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// STD Headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../includes/Shader.h"

ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);
    
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line)) {
        if(line.find("#shader") != std::string::npos) { // found syntax
            if(line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
                
            } else if(line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
                
            }
        } else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // this source should exist
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<  " shader: " << message << std::endl;
        glDeleteShader(id);
        return 0; 
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs  = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs  = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs); // should technically calling detach shader
    glDeleteShader(fs);
    
    return program;
}


Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}    

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}    

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}    
void Shader::Unbind() const {
    glUseProgram(0);
}    

// set uniforms
void Shader::SetUniform1i(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}    
void Shader::SetUnifromMat4f(const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]); // make one matrix
}

int Shader::GetUniformLocation(const std::string& name) {
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) return m_UniformLocationCache[name];
    int location =  glGetUniformLocation(m_RendererID, name.c_str());
    if(location == -1) std::cout << "Warning: uniform with " + name << " does not exist" << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}    

