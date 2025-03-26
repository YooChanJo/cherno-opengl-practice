// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// STD Headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

// Custom Headers
#include "../config.h" // in real life this needs its own file and functionality

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filePath) {
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

static unsigned int compileShader(unsigned int type, const std::string& source) {
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

static int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs  = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs  = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs); // should technically calling detach shader
    glDeleteShader(fs);
    
    return program;
}

static float timeOscillator(std::time_t start, int rate) { // rate in milliseconds
    int timeVal = ((std::clock() - start) / rate) % 512;
    return timeVal < 255 ? static_cast<float>(timeVal) / 255 : static_cast<float>(511 - timeVal) / 255;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* glfw configs */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // needs a vao

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK) {
        std::cout << "Glew init error" << std::endl;
        return 0;
    }
    // std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    unsigned vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer;
    glGenBuffers(1, &buffer); // generator one buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // activate that buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW); // bind data of 6 * sizeof(float) of position and this is only going to be rendered once with not modifications
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    // 0th attribute for each 2 * sizeof(float) strides start from offset 0 counting 2 elements size of GL_FLOAT
    // links buffer with vao

    unsigned int ibo;
    glGenBuffers(1, &ibo); // generator one buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // activate that buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW); // bind data of 6 * sizeof(float) of position and this is only going to be rendered once with not modifications
    
    ShaderProgramSource source = parseShader(ProjectConfig::projectDir + "shaders/basic.shader");
    
    unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    if(location == -1) std::cout << "Uniform named u_Color not found" << std::endl;
    
    std::time_t start = std::clock();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        // my code
        glUniform4f(location, timeOscillator(start, 3), timeOscillator(start, 4), timeOscillator(start, 5), 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}