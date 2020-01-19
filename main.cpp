#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <cmath>

#include "shader.h"
#include "camera.h"
#include "model.h"

#define ATTENUATION_CONSTANT    1.0f
#define ATTENUATION_LINEAR      0.14f
#define ATTENUATION_QUADRATIC   0.07f

void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);
void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
unsigned int loadTexture(const char*);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 600;

const bool wireframe = false;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


float deltaTime = 0.0f;		// Time between current frame and last frame
float lastFrame = 0.0f;		// Time of last frame
float mixValue = 0.0f;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;


int main()
{

    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: Create window object
    //-----------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Lighting", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // glad: Load all OpenGL function pointers
    //-------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // load shaders
    Shader shader("shaders/modelVertexShader.glsl", "shaders/modelFragShader.glsl");
    //Shader lampShader("shaders/lampVertexShader.glsl", "shaders/lampFragmentShader.glsl");

    // load models
    Model ourModel("models/nanosuit/nanosuit.obj");

    //------------------------------------------------
    // GLFW: Render loop (displays individual FRAMES)
    //------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // handle input
        processInput(window);

        // rendering commands
        glClearColor(0.21f, 0.18f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Wireframe?
        if (wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        // The below used for a rotating light around an object
        //------------------------------------------------------
        /*float lightX = sin(glfwGetTime());
        float lightY = sin(glfwGetTime()) - cos(glfwGetTime());
        float lightZ = cos(glfwGetTime());
        glm::vec3 lightPos(lightX, lightY, lightZ);*/

        // Draw model 
        shader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // it's a bit too big so scale the model down
        shader.setMat4("model", model);
        ourModel.Draw(shader);



        // check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // terminate glfw and clean up memory allocations
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) // initially true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.01f;
        if (mixValue > 1.0f)
        {
            mixValue = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.01f;
        if (mixValue < 0.0f)
        {
            mixValue = 0.0f;
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        if (mixValue == 0.0f)
        {
            mixValue = 1.0f;
        }
        else if (mixValue == 1.0f)
        {
            mixValue = 0.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        mixValue = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(RUNNING, deltaTime);
        }
        else
            camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    /*if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RUNNING, deltaTime);
    }*/
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Make sure that the viewport matches the windows size, especially when 
    // the main window gets resized by the user
    glViewport(0, 0, width, height);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
