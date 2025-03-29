// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// vender
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

// static float timeOscillator(std::time_t start, int rate) { // rate in milliseconds
//     int timeVal = ((std::clock() - start) / rate) % 512;
//     return timeVal < 255 ? static_cast<float>(timeVal) / 255 : static_cast<float>(511 - timeVal) / 255;
// }

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* glfw configs */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // needs a vao

    int w_Width = 960, w_Height = 540;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(w_Width, w_Height, "Hello World", NULL, NULL);
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
            100.0f, 100.0f, 0.0f, 0.0f, // 0
            200.0f, 100.0f, 1.0f, 0.0f, // 1
            200.0f, 200.0f, 1.0f, 1.0f, // 2
            100.0f, 200.0f, 0.0f, 1.0f, // 3
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

        glm::mat4 proj = glm::ortho(0.0f, (float)w_Width, 0.0f, (float)w_Height, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
        
        glm::mat4 mvp = proj * view * model;

        shader.SetUnifromMat4f("u_MVP", mvp);

        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
        glm::vec4 result = mvp * vp;
        std::cout << "Result vec4: < " << result[0] << ", " << result[1] << ", " << result[2] << ", " << result[3] << " >" << std::endl;
        
        Texture texture("./res/textures/broken_violin.jpg");
        texture.Bind(); // binding texture to slot 0
        shader.SetUniform1i("u_Texture", 0); // texture slot 0
        
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
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