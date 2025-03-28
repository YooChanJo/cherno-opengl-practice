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
#include "../includes/Texture.h"

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
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
             0.5f, -0.5f, 1.0f, 0.0f, // 1
             0.5f,  0.5f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f, // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha rendering
        glEnable(GL_BLEND); // enable blending

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2); // position
        layout.Push<float>(2); // texture coord
        va.AddBuffer(vb, layout); // need addition in layout

        IndexBuffer ib(indices, 6);
        Shader shader("./shaders/basic.shader");
        shader.Bind();

        Texture texture("./res/textures/broken_violin.jpg");
        texture.Bind(); // binding texture to slot 0
        shader.SetUniform1i("u_Texture", 0); // texture slot 0
        
        // va.Unbind();
        // vb.Unbind();
        // ib.Unbind();
        // shader.Unbind();

        std::time_t start = std::clock();
        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            
            shader.SetUniform4f("u_Color", timeOscillator(start, 4), timeOscillator(start, 5), 1.0f, 1.0f);
            renderer.Draw(va, ib, shader);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}