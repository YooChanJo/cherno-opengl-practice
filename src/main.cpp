// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// STD Headers
#include <iostream>
#include <string>
#include <ctime>

// Custom Headers
#include "../config.h" // in real life this needs its own file and functionality
#include "../includes/IndexBuffer.h"
#include "../includes/VertexBuffer.h"
#include "../includes/VertexArray.h"
#include "../includes/Shader.h"
#include "../includes/Renderer.h"

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
    {
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

        VertexArray va;
        // unsigned vao;
        // glGenVertexArrays(1, &vao);
        // glBindVertexArray(vao);

        VertexBuffer vb(positions, 6 * 2 * sizeof(float));

        // unsigned int buffer;
        // glGenBuffers(1, &buffer); // generator one buffer
        // glBindBuffer(GL_ARRAY_BUFFER, buffer); // activate that buffer
        // glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW); // bind data of 6 * sizeof(float) of position and this is only going to be rendered once with not modifications

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        // 0th attribute for each 2 * sizeof(float) strides start from offset 0 counting 2 elements size of GL_FLOAT
        // links buffer with vao

        IndexBuffer ib(indices, 6);
        // unsigned int ibo;
        // glGenBuffers(1, &ibo); // generator one buffer
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // activate that buffer
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW); // bind data of 6 * sizeof(float) of position and this is only going to be rendered once with not modifications
        
        // ShaderProgramSource source = parseShader(ProjectConfig::projectDir + "shaders/basic.shader");
        
        // unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
        // glUseProgram(shader);
        Shader shader("./shaders/basic.shader");
        
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        // int location = glGetUniformLocation(shader, "u_Color");
        // if(location == -1) std::cout << "Uniform named u_Color not found" << std::endl;
        
        std::time_t start = std::clock();
        Renderer renderer;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            // glClear(GL_COLOR_BUFFER_BIT);
            
            shader.SetUniform4f("u_Color", timeOscillator(start, 4), timeOscillator(start, 5), 1.0f, 1.0f);
            // my code
            // glUniform4f(
            //     location,
            //     timeOscillator(start, 3),
            //     timeOscillator(start, 4),
            //     timeOscillator(start, 5),
            //     1.0f
            // );
            renderer.Draw(va, ib, shader);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}