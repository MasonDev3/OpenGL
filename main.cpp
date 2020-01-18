#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <cmath>

#include "shader.h"
#include "stb_image.h"
#include "camera.h"
#include "materials.h"

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

    // glad: Load all OpenGL function pointers
    //-------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader cubeShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    Shader lampShader("shaders/lampVertexShader.glsl", "shaders/lampFragmentShader.glsl");
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);




    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };

    glm::vec3 horrorLightColors[] = {
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.3f, 0.1f, 0.1f)
    };


    //-------------------------------
    // Set up VBO, VAO, and
    // pointers to data
    //-------------------------------

    // Create vertex buffer object (VBO) to send data from CPU to GPU
    // and keep it there for faster performance
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    // Bind VBO buffer to GL_ARRAY_BUFFER
    // Copy previously defined vertex data into buffer's memory
    // glBufferData(target_buffer, size_of_data, data, usage)
    // usage is GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
    // Tell OpenGL how to interpret vertex data
    // VAOs[0]

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Load Textures
    //---------------------------
    unsigned int diffuseMap = loadTexture("textures/container2.png");
    unsigned int specularMap = loadTexture("textures/container2_specular.png");
    cubeShader.use();
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);

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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);*/

        // Draw cube object 
        cubeShader.use();
        cubeShader.setVec3("viewPos", camera.Position);
        cubeShader.setFloat("material.shininess", 32.0f);

        /* Set all uniform attributes for the 6 different lights. This all has to be
         * done manually unless we define light types as different classes and set
         * all their attributes that way. Another way could be to us 
         * 'Uniform buffor objects'
         */

        // Kc = attenuation constant variable
        // Kl = attenuation linear variable
        // Kq = attenuation quadratic variable

        // Point Light attributes
        glm::vec3 pointAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
        glm::vec3 pointDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 pointSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

        // Directional Light Shader
        cubeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cubeShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
        cubeShader.setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

        // Point Light 1 Shader
        cubeShader.setVec3("pointLight[0].position", pointLightPositions[0]);
        cubeShader.setVec3("pointLight[0].ambient", horrorLightColors[0] * 0.1f);
        cubeShader.setVec3("pointLight[0].diffuse", horrorLightColors[0]);
        cubeShader.setVec3("pointLight[0].specular", horrorLightColors[0]);
        cubeShader.setFloat("pointLight[0].Kc", ATTENUATION_CONSTANT);
        cubeShader.setFloat("pointLight[0].Kl", ATTENUATION_LINEAR);
        cubeShader.setFloat("pointLight[0].Kq", ATTENUATION_QUADRATIC);

        // Point Light 2 Shader
        cubeShader.setVec3("pointLight[1].position", pointLightPositions[1]);
        cubeShader.setVec3("pointLight[1].ambient", horrorLightColors[1] * 0.1f);
        cubeShader.setVec3("pointLight[1].diffuse", horrorLightColors[1]);
        cubeShader.setVec3("pointLight[1].specular", horrorLightColors[1]);
        cubeShader.setFloat("pointLight[1].Kc", ATTENUATION_CONSTANT);
        cubeShader.setFloat("pointLight[1].Kl", ATTENUATION_LINEAR);
        cubeShader.setFloat("pointLight[1].Kq", ATTENUATION_QUADRATIC);

        // Point Light 3 Shader
        cubeShader.setVec3("pointLight[2].position", pointLightPositions[2]);
        cubeShader.setVec3("pointLight[2].ambient", horrorLightColors[2] * 0.1f);
        cubeShader.setVec3("pointLight[2].diffuse", horrorLightColors[2]);
        cubeShader.setVec3("pointLight[2].specular", horrorLightColors[2]);
        cubeShader.setFloat("pointLight[2].Kc", ATTENUATION_CONSTANT);
        cubeShader.setFloat("pointLight[2].Kl", ATTENUATION_LINEAR);
        cubeShader.setFloat("pointLight[2].Kq", ATTENUATION_QUADRATIC);
        
        // Point Light 4 Shader
        cubeShader.setVec3("pointLight[3].position", pointLightPositions[3]);
        cubeShader.setVec3("pointLight[3].ambient", horrorLightColors[3] * 0.1f);
        cubeShader.setVec3("pointLight[3].diffuse", horrorLightColors[3]);
        cubeShader.setVec3("pointLight[3].specular", horrorLightColors[3]);
        cubeShader.setFloat("pointLight[3].Kc", ATTENUATION_CONSTANT);
        cubeShader.setFloat("pointLight[3].Kl", ATTENUATION_LINEAR);
        cubeShader.setFloat("pointLight[3].Kq", ATTENUATION_QUADRATIC);

        // Spotlight Shader
        cubeShader.setVec3("flashLight.position", camera.Position);
        cubeShader.setVec3("flashLight.direction", camera.Front);
        cubeShader.setVec3("flashLight.ambient", 0.0f, 0.0f, 0.0f);
        cubeShader.setVec3("flashLight.diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("flashLight.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("flashLight.Kc", ATTENUATION_CONSTANT);
        cubeShader.setFloat("flashLight.Kl", 0.09f);
        cubeShader.setFloat("flashLight.Kq", 0.032f);
        cubeShader.setFloat("flashLight.cutOff", glm::cos(glm::radians(10.0f)));
        cubeShader.setFloat("flashLight.outCutOff", glm::cos(glm::radians(15.0f)));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        cubeShader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        
        // render containers
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20 * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Draw lamp object(s)
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);

        // draw as many light bulbs as we have point lights
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lampShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }




        // check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

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
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RUNNING, deltaTime);
    }
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
