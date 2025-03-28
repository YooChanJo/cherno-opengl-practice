#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // set uniforms + needs extensions
    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUnifromMat4f(const std::string& name, glm::mat4 matrix);
private:
    static ShaderProgramSource ParseShader(const std::string& filePath);
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
};

#endif