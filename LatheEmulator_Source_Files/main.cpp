

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stb_image.h>
#include "shader.h"
#include "camera.h"
#include "cylinder.h"
#include "particle.h"
#include <iostream>

#define WOOD 0
#define METAL 1

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void knife_move_callback(float xoffset, float yoffset);
unsigned int loadCubemap(vector<std::string> skyboxFaces);
void bezierCurve(float a, float b, float c, float d, vector<float>& bezier);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// controlls
bool knife_active = false;
bool camera_active = false;

bool bound_active = true;

// lighting
glm::vec3 lightPos(-2.0f, 10.0f, 10.0f);

float spinSpeed = 0.0f;
float posA = 0.0f;
float posB = 0.002f;
float knifeWidth = 0.05f;
float sliceRad = 1.0f;

int texIdx = WOOD;

struct Material
{
    float ambientStrength;
    float specularStrength;
    float shininess;

    Material() {
        ambientStrength = 0.5f;
        specularStrength = 0.5f;
        shininess = 8.0f;
    }
    Material(float am, float sp, float sh)
    {
        ambientStrength = am;
        specularStrength = sp;
        shininess = sh;
    }
};

Material matArray[2];

Knife knife(glm::vec3(knifeWidth, sliceRad, 0.0f), knifeWidth);
Cylinder cylinder(250, posB - posA, sliceRad);

// LIGHT LAMP POSITIONING (Pos, TexCoord, Normal)
// ------------------------------------------------------------------
float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.0f, -1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  0.0f,  1.0f,  0.0f
};

// SKYBOX
// ------------------------------------------------------------------

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // SHADERS
    // ------------------------------------------------------------------
    Shader smoothShader("./shaders/smooth.vs", "./shaders/smooth.fs");
    Shader roughShader("./shaders/rough.vs", "./shaders/rough.fs");
    Shader basiclampShader("./shaders/basic_lighting.vs", "./shaders/basic_lighting.fs");
    Shader lampShader("./shaders/lamp.vs", "./shaders/lamp.fs");
    Shader knifeShader("./shaders/knife.vs", "./shaders/knife.fs");
    Shader skyboxShader("./shaders/skybox.vs", "./shaders/skybox.fs");
    Shader particleShader("./shaders/particle.vs", "./shaders/particle.fs");


    // MATERIAL SHADER PARAM
    // ------------------------------------------------------------------
    Material woodMaterial(0.5f, 0.5f, 8.0f);
    Material metalMaterial(0.5f, 0.5f, 8.0f);
    matArray[0] = woodMaterial;
    matArray[1] = metalMaterial;


    // SMOOTH TEXTURE
    // ------------------------------------------------------------------
    unsigned int smoothTexture[2];
    glGenTextures(2, smoothTexture);
    glBindTexture(GL_TEXTURE_2D, smoothTexture[WOOD]);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int smoothTextureWidth, smoothTextureHeight, smoothNrChannels;
    unsigned char* smoothTextureDataWood = stbi_load("./resources/woodSmooth.jpg", &smoothTextureWidth, &smoothTextureHeight, &smoothNrChannels, 0);
    if (smoothTextureDataWood)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, smoothTextureWidth, smoothTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, smoothTextureDataWood);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(smoothTextureDataWood);

    glBindTexture(GL_TEXTURE_2D, smoothTexture[METAL]);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    unsigned char* smoothTextureDataMetal = stbi_load("./resources/woodRough.jpg", &smoothTextureWidth, &smoothTextureHeight, &smoothNrChannels, 0);
    if (smoothTextureDataMetal)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, smoothTextureWidth, smoothTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, smoothTextureDataMetal);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(smoothTextureDataMetal);


    // ROUGH TEXTURE
    // ------------------------------------------------------------------
    unsigned int roughTexture[2];
    glGenTextures(2, roughTexture);
    glBindTexture(GL_TEXTURE_2D, roughTexture[WOOD]);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int roughTextureWidth, roughTextureHeight, roughNrChannels;
    unsigned char* roughTextureDataWood = stbi_load("./resources/woodRough.jpg", &roughTextureWidth, &roughTextureHeight, &roughNrChannels, 0);
    if (roughTextureDataWood)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, roughTextureWidth, roughTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, roughTextureDataWood);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(roughTextureDataWood);

    glBindTexture(GL_TEXTURE_2D, roughTexture[METAL]);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    unsigned char* roughTextureDataMetal = stbi_load("./resources/metalRough.jpg", &roughTextureWidth, &roughTextureHeight, &roughNrChannels, 0);
    if (roughTextureDataMetal)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, roughTextureWidth, roughTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, roughTextureDataMetal);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(roughTextureDataMetal);


    // CYLINDER INITIALIZATION
    // ------------------------------------------------------------------
    const int n = 400;  // 一层切片上一个圆所包含的总点数
    float PI = 3.1415926f;
    float R = 1.0f;  // 半径

    // 浮点数数量: (3 + 3) + ((3 * n) * 2)
    // 2 圆心，n - 1 圆上点，共 2n
    float vertexPositionArray[((2 * n) * 3)];   // 一层切片所需点的位置
    float vertexNormalArray[((2 * n) * 3)]; // 一层切片所需点的法向量
    const int totalNumOfRoughTriInCircle = 2 * (n - 1);
    const int totalNumOfSmoothTriInCircle = 2 * (n - 1);

    unsigned int circleSmoothIndices[3 * totalNumOfSmoothTriInCircle];  // 光滑层（切面表面）片段位置下标
    unsigned int circleRoughIndices[3 * totalNumOfRoughTriInCircle];    // 粗糙层（圆柱体表面）片段位置下标
    float circleSmoothTextureCoord[2 * (2 * n)];    // 光滑层（切面表面）片段纹理坐标
    float circleRoughTextureCoord[2 * (2 * n)];     // 粗糙层（圆柱体表面）片段纹理坐标

    // 左侧圆心
    vertexPositionArray[0] = posA;
    vertexPositionArray[1] = 0.0f;
    vertexPositionArray[2] = 0.0f;
    vertexNormalArray[0] = 0.0f;
    vertexNormalArray[1] = 0.0f;
    vertexNormalArray[2] = 0.0f;

    // 右侧圆心
    vertexPositionArray[3 * n] = posB;
    vertexPositionArray[3 * n + 1] = 0.0f;
    vertexPositionArray[3 * n + 2] = 0.0f;
    vertexNormalArray[3 * n] = 1.0f;
    vertexNormalArray[3 * n + 1] = 0.0f;
    vertexNormalArray[3 * n + 2] = 0.0f;

    // 圆心纹理坐标
    circleSmoothTextureCoord[0] = 0.0f;
    circleSmoothTextureCoord[1] = 0.0f;
    circleSmoothTextureCoord[2 * n] = 0.0f;
    circleSmoothTextureCoord[2 * n + 1] = 0.0f;

    // 构造点
    for (int i = 1; i <= n - 1; i++)
    {
        // 左侧面
        // [3][4][5] ~ [3n - 3][3n - 2][3n - 1] 
        vertexPositionArray[i * 3] = posA;
        vertexNormalArray[i * 3] = 0.0f;

        float sinVal = sin(2 * PI * (i-1) / (n - 2));
        float cosVal = cos(2 * PI * (i-1) / (n - 2));

        vertexPositionArray[i * 3 + 1] = R * sinVal;
        vertexPositionArray[i * 3 + 2] = R * cosVal;
        vertexNormalArray[i * 3 + 1] = R * sinVal;
        vertexNormalArray[i * 3 + 2] = R * cosVal;

        if (i < n - 1)
        {
            float sinValX = (sinVal / (float)2) + 0.5f;
            float cosValX = (cosVal / (float)2) + 0.5f;
            circleSmoothTextureCoord[i * 2] = (sinValX < 0.0f ? 0.0f : sinValX);
            circleSmoothTextureCoord[i * 2 + 1] = (cosValX < 0.0f ? 0.0f : cosValX);
        }
    }

    for (int i = 1; i <= n - 1; i++)
    {
        // 右侧面
        // [3n + 3][3n + 4][3n + 5] ~ [6n - 3][6n - 2][6n - 1]
        vertexPositionArray[3 * n + i * 3] = posB;
        vertexNormalArray[3 * n + i * 3] = 0.0f;

        float sinVal = sin(2 * PI * (i - 1) / (n - 2));
        float cosVal = cos(2 * PI * (i - 1) / (n - 2));
        vertexPositionArray[3 * n + i * 3+ 1] = R * sinVal;
        vertexPositionArray[3 * n + i * 3+ 2] = R * cosVal;
        vertexNormalArray[3 * n + i * 3 + 1] = R * sinVal;
        vertexNormalArray[3 * n + i * 3 + 2] = R * cosVal;

        if (i < n - 1)
        {
            float sinValX = (sinVal / (float)2) + 0.5f;
            float cosValX = (cosVal / (float)2) + 0.5f;
            circleSmoothTextureCoord[2 * n + i * 2] = (sinValX < 0.0f ? 0.0f : sinValX);
            circleSmoothTextureCoord[2 * n + i * 2 + 1] = (cosValX < 0.0f ? 0.0f : cosValX);
        }
    }

    // 左侧切面
    // e.g: n = 6, n - 1 = 5, 0,4
    for (int j = 0; j < n - 2; j++)
    {
        circleSmoothIndices[j * 3] = 0;
        circleSmoothIndices[j * 3 + 1] = j + 1;
        circleSmoothIndices[j * 3 + 2] = j + 2;
    }
    circleSmoothIndices[(n - 2) * 3] = 0;
    circleSmoothIndices[(n - 2) * 3 + 1] = n - 1;
    circleSmoothIndices[(n - 2) * 3 + 2] = 1;
    

    // 右侧切面
    for (int j = 0; j < n - 2; j++)
    {
        circleSmoothIndices[(n - 1) * 3 + j * 3] = 0 + n;
        circleSmoothIndices[(n - 1) * 3 + j * 3 + 1] = j + 1 + n;
        circleSmoothIndices[(n - 1) * 3 + j * 3 + 2] = j + 2 + n;
    }
    circleSmoothIndices[(n - 1) * 3 + (n - 2) * 3] = 0 + n;
    circleSmoothIndices[(n - 1) * 3 + (n - 2) * 3 + 1] = 2 * n - 1;
    circleSmoothIndices[(n - 1) * 3 + (n - 2) * 3 + 2] = n + 1;


    // 圆柱体粗糙（非切面）表面
    for (int j = 0; j < n - 2; j++)
    {
        circleRoughIndices[j * 3] = j + 1; // 1, n - 2
        circleRoughIndices[j * 3 + 1] = j + 2; // 2, n - 1
        circleRoughIndices[j * 3 + 2] = n + j + 2; // n + 2, 2n - 1
    }
    // [3n - 6], [3n - 5], [3n - 4]
    circleRoughIndices[(n - 2) * 3] = n - 1;
    circleRoughIndices[(n - 2) * 3 + 1] = 1;
    circleRoughIndices[(n - 2) * 3 + 2] = n + 1;
    /*n -1 tri
    0, 3 n - 2*/

    // [3n - 3], [3n - 2], [3n - 1]
    // [6n - 12], [6n - 11], [6n - 10]
    for (int j = 0; j < n - 2; j++)
    {
        circleRoughIndices[(n - 1) * 3 + j * 3] = j + 1;   // 1, n - 2
        circleRoughIndices[(n - 1) * 3 + j * 3 + 1] = n + j + 1; // n + 1, 2n - 2
        circleRoughIndices[(n - 1) * 3 + j * 3 + 2] = n + j + 2; // n + 2, 2n - 1
    }
    // 6n - 9, 6n - 8, 6n - 7
    circleRoughIndices[(n - 1) * 3 + (n - 2) * 3] = n - 1;
    circleRoughIndices[(n - 1) * 3 + (n - 2) * 3 + 1] = 2 * n - 1;
    circleRoughIndices[(n - 1) * 3 + (n - 2) * 3 + 2] = n + 1;

    // LAMP VAO & VBO
    // ------------------------------------------------------------------

    unsigned int lampVBO, lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glGenBuffers(1, &lampVBO);

    glBindVertexArray(lampVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(10);


    // CYLINDER RELATED VAO & VBO ( POS, NORMAL, TEXTURE)
    // ------------------------------------------------------------------


    // CYLINDER CIRCLE(SMOOTH, POLISHED)
    // ------------------------------------------------------------------
    unsigned int VBO[5], VAO[2], EBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(4, VBO);
    glGenBuffers(2, EBO);
  
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionArray), vertexPositionArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleSmoothIndices), circleSmoothIndices, GL_STATIC_DRAW);
    glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, 3 * sizeof(unsigned int), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleSmoothTextureCoord), circleSmoothTextureCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormalArray), vertexNormalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(5);
  
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // CYLINDER SURFACE(ROUGH, UNPOLISHED)
    // ------------------------------------------------------------------

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionArray), vertexPositionArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleRoughIndices) - 3 * sizeof(float), circleRoughIndices, GL_STATIC_DRAW);
    glVertexAttribIPointer(3, 3, GL_UNSIGNED_INT, 3 * sizeof(unsigned int), (void*)0);
    glEnableVertexAttribArray(3);


    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleRoughTextureCoord), circleRoughTextureCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormalArray), vertexNormalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(6);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    
    // CUBE
    // ------------------------------------------------------------------

    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(7);


    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(8);

    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(9);

    glBindVertexArray(0);
    

    // SKYBOX VAO & VBO
    // ------------------------------------------------------------------

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(11);

    vector<std::string> skyboxFaces;
    skyboxFaces.push_back("./resources/FortPoint/right.jpg");
    skyboxFaces.push_back("./resources/FortPoint/left.jpg");
    skyboxFaces.push_back("./resources/FortPoint/top.jpg");
    skyboxFaces.push_back("./resources/FortPoint/bottom.jpg");
    skyboxFaces.push_back("./resources/FortPoint/front.jpg");
    skyboxFaces.push_back("./resources/FortPoint/back.jpg");

    unsigned int cubemapTexture = loadCubemap(skyboxFaces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);


    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    
    bound_active = true;

    float angle = 0.0f;

    std::vector<float> bezierArray;
    int currBezierIdx = 0;
    if (bound_active == true)
    {
        bezierCurve(0.25f, 0.01f, 0.25f, 1.0f, bezierArray);
    }

    ParticleSystem particleSystem(knife, 0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (knife_active == true)
        {
            if (cylinder.CylinderCut(knife, bound_active, bezierArray, currBezierIdx))
            {
                particleSystem.position = knife.position;
                int particleNum = particleSystem.particleNum;
                for (int i = 0; i < particleNum; i++)
                {
                    Particle particle = particleSystem.particles[i];
                    if (particle.active)
                    {
                        particleShader.use();

                        particleShader.setFloat("ambientStrength", matArray[texIdx].ambientStrength);
                        particleShader.setFloat("specularStrength", matArray[texIdx].specularStrength);
                        particleShader.setFloat("shininess", matArray[texIdx].shininess);

                        particleShader.setVec3("cameraPos", camera.Position);

                        particleShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                        particleShader.setVec3("lightPos", lightPos);
                        particleShader.setVec3("viewPos", camera.Position);

                        if(texIdx == WOOD)
                            glBindTexture(GL_TEXTURE_2D, smoothTexture[texIdx]);
                        else
                            glBindTexture(GL_TEXTURE_2D, roughTexture[texIdx]);


                        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                        particleShader.setMat4("projection", projection);

                        glm::mat4 view = camera.GetViewMatrix();
                        particleShader.setMat4("view", view);

                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, particle.position);
                        model = glm::scale(model, particle.scale);
                        particleShader.setMat4("model", model);

                        glBindVertexArray(cubeVAO);
                        glDrawArrays(GL_TRIANGLES, 0, 36);

                        if (particleSystem.particles[i].velocity > 0.2f)
                        {
                            particleSystem.particles[i].position += particle.velocity * particle.direction * deltaTime;
                            particleSystem.particles[i].velocity -= 0.002f;
                        }
                        particleSystem.particles[i].position.y += 0.5f * particleSystem.gravity * deltaTime * deltaTime;


                        if (particleSystem.particles[i].lifetime < 0.0f)
                        {
                            particleSystem.rebornParticle(i, knife.position);
                        }
                        else
                            particleSystem.particles[i].lifetime -= 0.01f;

                    }
                }
            }

        }

        int cylinder_vec_length = cylinder.slices.size();

        if (angle >= 180.0f)
            angle = 0.0f;
        
        angle += spinSpeed;

        float slice_distance = 0.0f;
        for (int i = 0; i < cylinder_vec_length; i++)
        {
            // 计算切片厚度，即每层在世界坐标中的 X轴坐标，以此来移动圆柱体切片
            // ------------------------------------------------------------------
            Slice slice = cylinder.slices[i];
            float slice_posA = slice.posA;
            float slice_posB = slice.posB;
            float slice_rad = slice.rad;
            slice_distance += slice_posB - slice_posA;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(slice_distance, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f, slice_rad, slice_rad));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

            // CYLINDER CIRCLE(ROUGH, POLISHED)
            // ------------------------------------------------------------------

            smoothShader.use();

            smoothShader.setFloat("ambientStrength", matArray[texIdx].ambientStrength);
            smoothShader.setFloat("specularStrength", matArray[texIdx].specularStrength);
            smoothShader.setFloat("shininess", matArray[texIdx].shininess);

            smoothShader.setVec3("cameraPos", camera.Position);

            smoothShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            smoothShader.setVec3("lightPos", lightPos);
            smoothShader.setVec3("viewPos", camera.Position);

            smoothShader.setInt("texIdx", texIdx);
            glBindTexture(GL_TEXTURE_2D, smoothTexture[texIdx]);

            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            smoothShader.setMat4("projection", projection);
            glm::mat4 view = camera.GetViewMatrix();
            smoothShader.setMat4("view", view);
            smoothShader.setMat4("model", model);
            
            glBindVertexArray(VAO[0]);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawElements(GL_TRIANGLES, 3 * totalNumOfSmoothTriInCircle, GL_UNSIGNED_INT, 0);



            // CYLINDER SURFACE(ROUGH, UNPOLISHED)
            // ------------------------------------------------------------------

            roughShader.use();

            roughShader.setFloat("ambientStrength", matArray[texIdx].ambientStrength);
            roughShader.setFloat("specularStrength", matArray[texIdx].specularStrength);
            roughShader.setFloat("shininess", matArray[texIdx].shininess);


            roughShader.setVec3("cameraPos", camera.Position);

            roughShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            roughShader.setVec3("lightPos", lightPos);
            roughShader.setVec3("viewPos", camera.Position);
            
            smoothShader.setInt("texIdx", texIdx);
            glBindTexture(GL_TEXTURE_2D, roughTexture[texIdx]);

            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            roughShader.setMat4("projection", projection);
            view = camera.GetViewMatrix();
            roughShader.setMat4("view", view);
            roughShader.setMat4("model", model);
            
            glBindVertexArray(VAO[1]);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawElements(GL_TRIANGLES, 3 * totalNumOfRoughTriInCircle, GL_UNSIGNED_INT, 0);
        }

        
        // LAMP
        // ------------------------------------------------------------------
        lampShader.use();

        glm::mat4 lampModel = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 lampView = camera.GetViewMatrix();
        glm::mat4 lampProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        lampShader.setMat4("projection", lampProjection);
        lampShader.setMat4("view", lampView);
        lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, lightPos);
        lampModel = glm::scale(lampModel, glm::vec3(2.0f)); // a smaller cube
        lampShader.setMat4("model", lampModel);

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // KNIFE
        // ------------------------------------------------------------------
        knifeShader.use();

        glm::mat4 knifeModel = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 knifeView = camera.GetViewMatrix();
        glm::mat4 knifeProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        knifeShader.setMat4("projection", knifeProjection);
        knifeShader.setMat4("view", knifeView);
        knifeModel = glm::mat4(1.0f);
        knifeModel = glm::translate(knifeModel, knife.position);
        knifeModel = glm::scale(knifeModel, glm::vec3(0.05f, 1.0f, 0.05f)); // a smaller cube
        knifeShader.setMat4("model", knifeModel);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        // SKYBOX
        // ------------------------------------------------------------------
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        
        skyboxShader.use();

        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        glm::mat4 skyboxProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        skyboxShader.setMat4("view", skyboxView);
        skyboxShader.setMat4("projection", skyboxProjection);
        
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (spinSpeed < 5.0f)
            spinSpeed = 5.0f;
        else
            spinSpeed -= 0.5f;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (spinSpeed > 90.0f)
            spinSpeed = 90.0f;
        else
            spinSpeed += 0.5f;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        for (int i = 0; i < cylinder.slice_count; i++)
        {
            cylinder.slices[i].rad = cylinder.slice_rad;
        }
        bound_active = false;
        knife_active = false;
        camera_active = true;
        knife.position.y = cylinder.slice_rad + 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        texIdx = (texIdx == WOOD) ? METAL : WOOD;

    //if (glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    //{
    //    cout << "You're pressing left key" << endl;
    //    controlling = true;
    //    // Process knife
    //}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                cout << "Mosue left button clicked!" << endl;
                if (camera_active == false)
                    knife_active = !knife_active;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                cout << "Mosue middle button clicked!" << endl;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                cout << "Mosue right button clicked!" << endl;
                if (knife_active == false)
                    camera_active = !camera_active;
                break;
            default:
                return;
        }
    if (knife_active)
        cout << "Moving Knife!" << endl;

    if (camera_active)
        cout << "Moving camera!" << endl;
    return;
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed cosce y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (knife_active == false && camera_active == true)
        camera.ProcessMouseMovement(xoffset, yoffset);
    else if(knife_active == true && camera_active == false)
        knife_move_callback(xoffset, yoffset);
}

void knife_move_callback(float xoffset, float yoffset)
{
    knife.position.x += (xoffset * 0.0001f);

    if (knife.position.y < 0.0f)
        knife.position.y = 0.0f;
    else
        knife.position.y += (yoffset * 0.001f);

    cout << "Knife Pos CB " << knife.position.x << " " << knife.position.y << " " << knife.position.z << endl;

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// loads a cubemap texture from 6 individual texture skyboxFaces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> skyboxFaces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < skyboxFaces.size(); i++)
    {
        unsigned char* data = stbi_load(skyboxFaces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << skyboxFaces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}



// a, d : src and dst of curve
// b, c : param for directing the curve
void bezierCurve(float a, float b, float c, float d, vector<float> &bezier)
{
    //knife.width = cylinder.slice_width;
    //float cylinder_len = cylinder.slice_count * cylinder.slice_width;

    for (int i = 0; i < cylinder.slice_count; i++)
    {
        float t = (float)i * cylinder.slice_width;
        float omt = 1.0f - t;
        float result = a * omt * omt * omt + 3 * b * t * omt * omt + 3 * c * t * t * omt + d * t * t * t;
        bezier.push_back(result * cylinder.slice_rad);
    }
}