//
//  main.cpp
//  3D Object Drawing
//  Created by Mishu Baidya.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "cylinder.h"
#include "stb_image.h"
#include "Sphere2.h"
#include "curve.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float currentFrame);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void rail(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L);

// settings
const unsigned int SCR_WIDTH = 1100;
const unsigned int SCR_HEIGHT = 900;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float run_speed = 0.006f;
float speed = 0.1f;
float time_decrement = 0.7f;
float scaleFly = 5.0;
//initial values

float platform1 = -5.5f;
float platform2 = 14.5f;
float light1=0.0f;
float light2=10.0f;
float light3 = 20.0f;
float light4 = 30.0f;
float ball_x = 1.0f;
float ball_y = 0.4f;
float ball_x_inc = 0.0f;
bool b_right = false;
bool b_left = false;
float ball_y_inc = 0.05f;
bool b_jump = false;
int score_time = 24;
bool onMud = false;
bool onMud2 = false;
bool first_block = true;
float inc_angleBall = 10.0f;
bool gameModeON = false;
bool fly = false;
float fly_time = 0.0;
int rocket_point = 0;
int health_point = 0;
bool purchaseMode = false;

float gameBx = 0.0;
float gameBz = 0.0;
float gameBy = 0.0f;
float gameByInc = 0.0f;
float gameBxInc = 0.0f;
int addedCoin = 0;
// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f,1.0f,-3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[9] = {
    glm::vec3(1.50f,  2.50f,  light1),
    glm::vec3(-1.5f,  2.5f,  light1),
    glm::vec3(1.5f,  2.5f,  light2),
    glm::vec3(-1.5f,  2.5f,  light2),
    glm::vec3(1.5f,  2.5f,  light3),
    glm::vec3(-1.5f,  2.5f,  light3),
    glm::vec3(1.5f,  2.5f,  light4),
    glm::vec3(-1.5f,  2.5f,  light4),
    glm::vec3(-1.0f,  7.9f,  7.0f),

};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);

PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);

PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6       // light number
);

PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7       // light number
);
PointLight pointlight8(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    8       // light number
);
PointLight pointlight9(

    pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    9       // light number
);


//curve tree
vector<float> tree_points = {
    -0.0750, 2.4577, 5.1000,
    -0.1313, 2.4365, 5.1000,
    -0.2396, 2.4214, 5.1000,
    -0.3354, 2.3427, 5.1000,
    -0.3583, 2.2519, 5.1000,
    -0.3792, 2.1399, 5.1000,
    -0.3833, 2.0672, 5.1000,
    -0.3833, 1.9915, 5.1000,
    -0.3813, 1.8886, 5.1000,
    -0.3938, 1.8190, 5.1000,
    -0.4083, 1.7887, 5.1000,
    -0.4563, 1.7645, 5.1000,
    -0.5750, 1.6737, 5.1000,
    -0.7271, 1.6283, 5.1000,
    -0.8146, 1.5284, 5.1000,
    -0.8938, 1.4103, 5.1000,
    -0.9417, 1.2892, 5.1000,
    -0.9521, 1.1348, 5.1000,
    -0.8333, 0.9865, 5.1000,
    -0.7125, 0.8987, 5.1000,
    -0.6563, 0.8563, 5.1000,
    -0.7333, 0.7715, 5.1000,
    -0.8479, 0.6959, 5.1000,
    -1.0229, 0.6141, 5.1000,
    -1.1083, 0.5173, 5.1000,
    -1.1792, 0.4143, 5.1000,
    -1.2375, 0.3386, 5.1000,
    -1.2417, 0.2206, 5.1000,
    -1.2292, 0.0874, 5.1000,
    -1.1958, 0.0268, 5.1000,
    -1.0917, -0.0852, 5.1000,
    -0.8896, -0.1760, 5.1000,
    -0.7313, -0.2305, 5.1000,
    -0.4979, -0.2910, 5.1000,
    -0.3417, -0.3334, 5.1000,
    -0.2146, -0.3606, 5.1000,
    -0.0563, -0.3879, 5.1000,
    0.0396, -0.3879, 5.1000,
};
//curve
vector <float> coordinates;
vector <float> normals;
vector <int> indices;
vector <float> vertices;
class point
{
public:
    point()
    {
        x = 0;
        y = 0;
    }
    int x;
    int y;
} clkpt[2];
int mouseButtonFlag = 0;
FILE* fp;
const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;
bool showControlPoints = true;
bool loadBezierCurvePoints = false;
bool showHollowBezier = false;
bool lineMode = false;
unsigned int bezierVAO;
unsigned int treeVAO;




// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
int ind_track = 0;
bool game_over = false;
// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
int ball_type = 0;
float basket_z = 0.0f;
float basketInc = 0.03f;
bool Gameshoot = false;
int score_count = 0;


int item_cost = 5;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };


    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);


    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);




    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //bazierVAO
    treeVAO = hollowBezier(tree_points.data(), ((unsigned int)tree_points.size() / 3) - 1);
    //coordinates.clear();
    //normals.clear();
    //indices.clear();
    //vertices.clear();




    Sphere sphere = Sphere();
    Sphere sphere2 = Sphere();
    Sphere sphere3 = Sphere();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath = "sand.jpg";
    string specularMapPath = "sand.jpg";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube sand = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "rail_track.jpg";
    specularMapPath = "rail_track.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube rail_track = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "road.jpg";
    specularMapPath = "road.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube road = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "grass.jpg";
    specularMapPath = "grass.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grass = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube track[3] = { rail_track , road, grass };
    

    diffuseMapPath = "under_rail.jpg";
    specularMapPath = "under_rail.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube under_rail = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "rocket.png";
    specularMapPath = "rocket.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube rocket = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "health.png";
    specularMapPath = "health.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube health = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "wood.png";
    specularMapPath = "wood.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wood = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "inventory.jpg";
    specularMapPath = "inventory.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube mcdonald = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);



    string diffuseMapPath2 = "wall1.jpg";
    string specularMapPath2 = "wall1.jpg";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall1 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath3 = "wall2.jpg";
    string specularMapPath3 = "wall2.jpg";
    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall2 = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath4 = "wall3.jpg";
    string specularMapPath4 = "wall3.jpg";
    unsigned int diffMap4 = loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap4 = loadTexture(specularMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall3 = Cube(diffMap4, specMap4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath5 = "wall4.jpg";
    string specularMapPath5 = "wall4.jpg";
    unsigned int diffMap5 = loadTexture(diffuseMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap5 = loadTexture(specularMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall4 = Cube(diffMap5, specMap5, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath2 = "wall5.jpg";
    specularMapPath2 = "wall5.jpg";
    diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall5 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath2 = "wall6.jpg";
    specularMapPath2 = "wall6.jpg";
    diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall6 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath2 = "wall7.jpg";
    specularMapPath2 = "wall7.jpg";
    diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall7 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath2 = "wall8.jpg";
    specularMapPath2 = "wall8.jpg";
    diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall8 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube Wall[8] = { wall1,wall2,wall3,wall4,wall5,wall6,wall7,wall8};


    string Ball = "Skin/skin1.jpg";
    unsigned int diffBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 skin1 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffBall, specBall, 0.0f, 0.0f, 1.0f, 1.0f);

    Ball = "Skin/skin2.png";
    diffBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 skin2 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffBall, specBall, 0.0f, 0.0f, 1.0f, 1.0f);

    Ball = "Skin/skin3.png";
    diffBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 skin3 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffBall, specBall, 0.0f, 0.0f, 1.0f, 1.0f);

    Ball = "Skin/skin4.jpg";
    diffBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 skin4 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffBall, specBall, 0.0f, 0.0f, 1.0f, 1.0f);

    Ball = "Skin/skin5.jpg";
    diffBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 skin5 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffBall, specBall, 0.0f, 0.0f, 1.0f, 1.0f);

    Ball = "Skin/skin6.jpg";
    diffBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 skin6 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffBall, specBall, 0.0f, 0.0f, 1.0f, 1.0f);

    Ball = "Skin/skin7.jpg";
    diffBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 skin7 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffBall, specBall, 0.0f, 0.0f, 1.0f, 1.0f);

    Ball = "Skin/skin8.jpg";
    diffBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specBall = loadTexture(Ball.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 skin8 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffBall, specBall, 0.0f, 0.0f, 1.0f, 1.0f);

    Sphere2 ball[8] = { skin1,skin2,skin3,skin4,skin5,skin6,skin7,skin8};
    float ball_angle = 0.0f;

    //Coin
    string coinPath = "coin.jpg";
    unsigned int diffCoin = loadTexture(coinPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specCoin = loadTexture(coinPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 coin = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffCoin, specCoin, 0.0f, 0.0f, 1.0f, 1.0f);


    //mud
    string mudPath = "mud.jpeg";
    unsigned int diffMud = loadTexture(mudPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMud = loadTexture(mudPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube mud = Cube(diffMud, specMud, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //Score
    string water = "water score/water1.png";
    unsigned int diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water1 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water2.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water2 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water3.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water3 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water4.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water4 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water5.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water5 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water6.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water6 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water7.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water7 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water8.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water8 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water9.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water9 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water10.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water10 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water11.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water11 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water12.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water12 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water13.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water13 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water14.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water14 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water15.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water15 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water16.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water16 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water17.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water17 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water18.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water18 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water19.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water19 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water20.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water20 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water21.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water21 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water22.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water22 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water23.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water23 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/water24.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water24 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    water = "water score/waterfinish.png";
    diffWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specWater = loadTexture(water.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water25 = Cube(diffWater, specWater, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube water_score[30] = { water25,water24,water23,water22,water21,water20,water19,water18,water17,water16,water15,water14,water13,water12,water11,water10,water9,water8,water7,water6,water5,water4,water3,water2,water1 };


    //collected coins

    string numPath = "digits/0.png";
    unsigned int diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube zero = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/1.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube one = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/2.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube two = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/3.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube three = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/4.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube four = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "digits/5.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube five = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/6.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube six = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/7.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube seven = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/8.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube eight = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "digits/9.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube nine = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube score_block[10] = { zero,one,two,three,four,five,six,seven,eight,nine };



    //Game over picture loading
    numPath = "gameOver/1.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g1 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "gameOver/2.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g2 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "gameOver/3.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g3 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "gameOver/4.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g4 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "gameOver/5.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g5 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "gameOver/6.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g6 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "gameOver/7.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g7 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "gameOver/8.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g8 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "gameOver/9.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g9 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "gameOver/10.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g10 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    numPath = "gameOver/11.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g11 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "gameOver/12.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g12 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "gameOver/13.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g13 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    numPath = "gameOver/14.png";
    diffNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specNum = loadTexture(numPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube g14 = Cube(diffNum, specNum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube gameOverCubes[] = { g1,g2,g3,g4,g5,g6,g7,g8,g9,g10,g11,g12,g13,g14 };

    

    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<int> dis(0,3);
    std::uniform_int_distribution<int> dis2(4,7);
    std::uniform_real_distribution<float> dis3(5.0,15.0);
    std::uniform_int_distribution<int> roadN(1, 2);

    // Generate a random number
    int wall_num1 = dis(gen);
    int wall_num2 = dis(gen);
    int wall_num3 = dis(gen);
    int wall_num4 = dis(gen);
    int wall_num5 = dis2(gen);
    int wall_num6 = dis2(gen);
    int wall_num7 = dis2(gen);
    int wall_num8 = dis2(gen);


    float mud_z = dis3(gen);
    int road_num = roadN(gen);
    int road_num2 = roadN(gen);
    float time3 = 0.0f;
    //ourShader.use();
    //lightingShader.use();

    //Tree
    Cylinder tree = Cylinder();
    tree.diffuse = glm::vec3(0.8, 0.3, 0.2);
    tree.ambient = glm::vec3(0.8, 0.3, 0.2);

    Sphere leaf = Sphere();
    leaf.diffuse = glm::vec3(0.0, 1.0, 0.0);
    leaf.ambient = glm::vec3(0.0, 0.8, 0.0);


    float time = 0.0;
    float coin_rot_angle = 0.0f;

    bool coin_get1 = false;
    bool coin_get2 = false;
    bool coin_get3 = false;
    bool coin_get4 = false;
    float coinEffectInc = 0.0f;
    int g_ind = 0;
    int gInc = 1;


    // render loop
    // -----------


    
    camera.Position = glm::vec3(0.0,1.8,-9.0);
    int count = 0;



    while (!glfwWindowShouldClose(window))
    {

        /*glm::vec3 pointLightPositions[8] = {
    glm::vec3(1.50f,  1.50f,  light1),
    glm::vec3(-1.5f,  1.5f,  light1),
    glm::vec3(1.5f,  1.5f,  light2),
    glm::vec3(-1.5f,  1.5f,  light2),
    glm::vec3(1.5f,  1.5f,  light3),
    glm::vec3(-1.5f,  1.5f,  light3),
    glm::vec3(1.5f,  1.5f,  light4),
    glm::vec3(-1.5f,  1.5f,  light4)

        };*/


        //speed = 0.0f;

        if (gameModeON)
        {
            speed = 0.0f;
            camera.Position = glm::vec3(-1.0, 2.5f, platform1 + 17.5);
            camera.setYAW(-180.0);
            basket_z += basketInc;
            if (basket_z >= 2.5)
            {
                basketInc = -0.03f;
            }

            else if (basket_z < 0.0)
            {
                basketInc = 0.03f;
            }
        }
        else
        {
            if (purchaseMode)
            {
                speed = 0.0f;
                camera.Position = glm::vec3(1.0, 1.5f, platform2 + 2.5f);
                camera.setYAW(0.0);
            }
            else
            {
                camera.Position = glm::vec3(0.0, 1.8, -9.0);
                camera.setYAW(-90.0);
            }
            
        }



        //moving

        if(game_over)
           speed = 0.0f;

        light1 -= speed;
        light2 -= speed;
        light3 -= speed;
        light4 -= speed;
        platform1 -= speed;
        platform2 -= speed;

        if (platform1 <= -27.0f)
        {
            platform1 += 40.0f;
            light1 += 40.0f;
            light2 += 40.0f;
            wall_num1 = dis(gen);
            wall_num2 = dis(gen);
            wall_num3 = dis(gen);
            wall_num4 = dis(gen);
            road_num = roadN(gen);
        }
        if (platform2 <= -27.0f)
        {
            platform2 += 40.0f;
            light3 += 40.0f;
            light4 += 40.0f;
            wall_num5 = dis2(gen);
            wall_num6 = dis2(gen);
            wall_num7 = dis2(gen);
            wall_num8 = dis2(gen);
            road_num2 = roadN(gen);
        }



        pointlight1.position = glm::vec3(1.50f, 1.50f, light1);
        pointlight2.position = glm::vec3(-1.5f, 1.5f, light1);
        pointlight3.position = glm::vec3(1.5f, 1.5f, light2);
        pointlight4.position = glm::vec3(-1.5f, 1.5f, light2);
        pointlight5.position = glm::vec3(1.5f, 1.5f, light3);
        pointlight6.position = glm::vec3(-1.5f, 1.5f, light3);
        pointlight7.position = glm::vec3(1.5f, 1.5f, light4);
        pointlight8.position = glm::vec3(-1.5f, 1.5f, light4);

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window,currentFrame);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);
        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        rail(cubeVAO, lightingShader, model);



        //also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights

        float pointLightZ[8] = {light1,light1,light2,light2,light3,light3,light4,light4};

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 8; i++)
        {
            glm::mat4 model2 = glm::mat4(1.0f);
            glm::vec3 ttt = pointLightPositions[i];
            float tttx = ttt[0];
            float ttty= ttt[1];
            float tttz = pointLightZ[i];
            model2 = glm::translate(model2, glm::vec3(tttx,ttty,tttz));
            model2 = glm::scale(model2, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model2);
            ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);


        //---------------------Game Over-----------------//

        if (score_time == 0)
        {
            game_over = true;
        }

        if (!purchaseMode && !gameModeON && game_over)
        {
            if (currentFrame - time3 >= 0.2)
            {
                if (g_ind == 13)
                    gInc = -1;
                if (g_ind == 0)
                    gInc = 1;

                g_ind += gInc;

                time3 = currentFrame;
            }

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 0.0f, 5.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 6.0f, 6.0f));
            glm::mat4 modelForScore = translateMatrix * scaleMatrix;
            gameOverCubes[g_ind].drawCubeWithTexture(lightingShaderWithTexture, modelForScore);

        }


        if (fly && currentFrame-fly_time<=10.0)
        {
            ball_y = 1.5f;
            scaleFly -= 0.0085f;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 6.0f, 10.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, scaleFly, 0.5f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
            glm::mat4 modelForFly = translateMatrix * rotateZMatrix* scaleMatrix;
            tree.drawCylinder(lightingShader, modelForFly, 1.0, 0.0, 0.0);

        }
        if (fly && 10.0<currentFrame - fly_time)
        {
            ball_y = 0.4f;
            fly = false;
        }

        //----------------Character----------------//
        if (b_right)
        {
            ball_x -= 0.2f;
            if (ball_x <= -0.9f)
                b_right = false;
        }
        
        if (b_left)
        {
            ball_x += 0.2f;
            if (ball_x >= 1.0f)
                b_left = false;
        }

        if (b_jump)
        {
            ball_y += ball_y_inc;

            if (ball_y >= 1.5f)
            {
                ball_y_inc = -0.05f;
            }

            if (ball_y <= 0.4f)
            {
                ball_y_inc = 0.05f;
                b_jump = false;
            }
        }

        ball_angle += inc_angleBall;
        if (ball_angle >= 359.0)
            ball_angle = 0.0f;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(ball_x, ball_y, -4.7));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(ball_angle), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 modelForBall = translateMatrix * rotateXMatrix* scaleMatrix;
        ball[ball_type].drawSphereWithTexture(lightingShaderWithTexture, modelForBall);



        //--------------Coin----------------//

        if (coin_rot_angle >= 358.0)
            coin_rot_angle = 0.0f;
        else
            coin_rot_angle += 2.0f;

        float c1_z = platform1 + mud_z + 1.5;
        float c2_z = platform1 + mud_z + 5.0;
        float c3_z = platform2 + mud_z + 1.5;
        float c4_z = platform2 + mud_z + 5.0;
        float c_y = 1.5;
        float c1_x = 1.0;
        float c2_x = -1.0;
        float ball_z = -4.7;

        float disC1 = sqrt((ball_x - c2_x) * (ball_x - c2_x) + (ball_y - c_y) * (ball_y - c_y) + (ball_z-c1_z)* (ball_z - c1_z));
        float disC2 = sqrt((ball_x - c1_x) * (ball_x - c1_x) + (ball_y - c_y) * (ball_y - c_y) + (ball_z - c2_z)* (ball_z - c2_z));
        float disC3 = sqrt((ball_x - c1_x) * (ball_x - c1_x) + (ball_y - c_y) * (ball_y - c_y) + (ball_z - c3_z) * (ball_z - c3_z));
        float disC4 = sqrt((ball_x - c2_x) * (ball_x - c2_x) + (ball_y - c_y) * (ball_y - c_y) + (ball_z - c4_z) * (ball_z - c4_z));

        float coin_X = 5.0;
        float coin_Y = 1.5;
        float coin_Z = 5.0;

        if (disC1 <= 0.4)
        {
            coin_get1 = true;
            coin_X = c2_x;
            coin_Z = c1_z;
        }
        else
        {
            if (!coin_get1)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(c2_x, c_y, c1_z));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.15f, 0.02f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(coin_rot_angle), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 modelForCoin = translateMatrix * rotateYMatrix * scaleMatrix;
                coin.drawSphereWithTexture(lightingShaderWithTexture, modelForCoin);
            }
            
        }
        

        if (disC2 <= 0.4)
        {
            coin_get2 = true;
            coin_X = c1_x;
            coin_Z = c2_z;
        }
        else
        {
            if (!coin_get2)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(c1_x, c_y, c2_z));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.15f, 0.02f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(coin_rot_angle), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 modelForCoin = translateMatrix * rotateYMatrix * scaleMatrix;
                coin.drawSphereWithTexture(lightingShaderWithTexture, modelForCoin);
            }
            
        }

        if (disC3 <= 0.4)
        {
            coin_get3 = true;
            coin_X = c1_x;
            coin_Z = c3_z;
        }
        else
        {
            if (!coin_get3)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(c1_x, c_y, c3_z));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.15f, 0.02f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(coin_rot_angle), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 modelForCoin = translateMatrix * rotateYMatrix * scaleMatrix;
                coin.drawSphereWithTexture(lightingShaderWithTexture, modelForCoin);
            }
            
        }

        if (disC4 <= 0.4)
        {
            coin_get4 = true;
            coin_X = c2_x;
            coin_Z = c4_z;
        }
        else
        {
            if (!coin_get4)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(c2_x, c_y, c4_z));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.15f, 0.02f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(coin_rot_angle), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 modelForCoin = translateMatrix * rotateYMatrix * scaleMatrix;
                coin.drawSphereWithTexture(lightingShaderWithTexture, modelForCoin);
            }
            
        }

        //cout << score_count << endl;

        //coin get effect
        if (coin_get1 || coin_get2 || coin_get3 || coin_get4)
        {
            coinEffectInc += 0.1f;

            translateMatrix = glm::translate(identityMatrix, glm::vec3(coin_X+coinEffectInc, coin_Y, coin_Z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
            glm::mat4 modelForEffect = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForEffect, 1.0f, 0.843f, 0.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(coin_X - coinEffectInc, coin_Y, coin_Z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
            modelForEffect = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForEffect, 1.0f, 0.843f, 0.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(coin_X , coin_Y + coinEffectInc, coin_Z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
            modelForEffect = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForEffect, 1.0f, 0.843f, 0.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(coin_X, coin_Y - coinEffectInc, coin_Z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
            modelForEffect = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForEffect, 1.0f, 0.843f, 0.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(coin_X+ coinEffectInc, coin_Y + coinEffectInc, coin_Z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
            modelForEffect = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForEffect, 1.0f, 0.843f, 0.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(coin_X - coinEffectInc, coin_Y + coinEffectInc, coin_Z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
            modelForEffect = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForEffect, 1.0f, 0.843f, 0.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(coin_X + coinEffectInc, coin_Y - coinEffectInc, coin_Z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
            modelForEffect = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForEffect, 1.0f, 0.843f, 0.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(coin_X - coinEffectInc, coin_Y - coinEffectInc, coin_Z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
            modelForEffect = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForEffect, 1.0f, 0.843f, 0.0f);
        }
        
        if (coinEffectInc >= 2.5f)
        {
            coinEffectInc = 0.0f;
            score_count++;
            score_time=min(24, score_time +5);
            coin_get1 = false;
            coin_get2 = false;
            coin_get3 = false;
            coin_get4 = false;
        }


        //score
        if (!purchaseMode && !gameModeON && currentFrame - time >= time_decrement)
        {
            if (score_time > 0)
                score_time--;
            time = currentFrame;
        }
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0f, 6.0f, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.6f, 2.0f, 0.01f));
        glm::mat4 modelForScore = translateMatrix * scaleMatrix;
        water_score[score_time].drawCubeWithTexture(lightingShaderWithTexture, modelForScore);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.8f, 8.0f, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.02f));
        glm::mat4 modelForScore2 = translateMatrix * scaleMatrix;
        coin.drawSphereWithTexture(lightingShaderWithTexture, modelForScore2);

        

        //---------coin collected-----------//
        int A[3];
        A[0] = 0;
        A[1] = 0;
        A[2] = 0;
        int ff = score_count;
        int inddd = 2;
        while (ff > 0) {
            A[inddd] = ff % 10;
            ff /= 10;
            inddd--;
        }



        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.8, 7.65f, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.6f, 0.02f));
        glm::mat4 modelForScore11 =  translateMatrix * scaleMatrix;
        score_block[A[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore11);

        translateMatrix = glm::translate(identityMatrix, glm::vec3( -3.4, 7.65f, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.6f, 0.02f));
        glm::mat4 modelForScore22 =  translateMatrix * scaleMatrix;
        score_block[A[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore22);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.0, 7.65f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.6f, 0.02f));
        glm::mat4 modelForScore33 =  translateMatrix * scaleMatrix;
        score_block[A[2]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore33);

        // rocket score
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0, 6.8f, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.6f, 0.02f));
        glm::mat4 modelForrocket = translateMatrix * scaleMatrix;
        rocket.drawCubeWithTexture(lightingShaderWithTexture, modelForrocket);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0, 6.8f, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.6f, 0.02f));
        modelForScore33 = translateMatrix * scaleMatrix;
        score_block[ rocket_point].drawCubeWithTexture(lightingShaderWithTexture, modelForScore33);

        //health
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0, 6.0f, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.6f, 0.02f));
        modelForrocket = translateMatrix * scaleMatrix;
        health.drawCubeWithTexture(lightingShaderWithTexture, modelForrocket);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0, 6.0f, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.6f, 0.02f));
        modelForScore33 = translateMatrix * scaleMatrix;
        score_block[health_point].drawCubeWithTexture(lightingShaderWithTexture, modelForScore33);




        //mud - 1st block
        float mud_x = 0.5;
        if (road_num == 2)
            mud_x = -1.5f;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(mud_x, 0.05f, platform1+mud_z));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 3.0f));
        glm::mat4 modelForMud = translateMatrix * scaleMatrix;
        mud.drawCubeWithTexture(lightingShaderWithTexture, modelForMud);

        if ( platform1 + mud_z <= -4.7 && -4.7 <= platform1 + mud_z+3.0 && ball_y<=0.5)
        {
            if ((ball_x < 0.0 && mud_x < 0.0) || (ball_x > 0.0 && mud_x > 0.0))
            {
                onMud = true;
            }
            else
                onMud = false;
        }
        else
            onMud = false;


        //mud - block 2
        float mud_x2 = 0.5;
        if (road_num2 == 2)
            mud_x2 = -1.5f;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(mud_x2, 0.05f, platform2 + mud_z));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 3.0f));
        modelForMud = translateMatrix * scaleMatrix;
        mud.drawCubeWithTexture(lightingShaderWithTexture, modelForMud);

        if (platform2 + mud_z <= -4.7 && -4.7 <= platform2 + mud_z + 3.0 && ball_y <= 0.5)
        {
            if ((ball_x < 0.0 && mud_x2 < 0.0) || (ball_x > 0.0 && mud_x2 > 0.0))
            {
                onMud2 = true;
            }
            else
                onMud2 = false;
        }
        else
            onMud2 = false;



        if (onMud || onMud2)
        {
            inc_angleBall = 5.0f;
            speed = 0.05f;
        }
        else
        {
            inc_angleBall = 10.0f;
            speed = 0.1f;
        }



        

        float baseHeight = 0.1;
        float width = 10;
        float length = 20;


        //Block - 1

        //Platform
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.9f, 0.1f, platform1));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.2f, length));
        glm::mat4 modelMatrixForContainer = translateMatrix * scaleMatrix;
        sand.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.95f, 0.1f, platform1));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.2f, length));
        glm::mat4 modelMatrixForContainer2 = translateMatrix * scaleMatrix;
        sand.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer2);


        //under rail Track
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(width, baseHeight, length));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.8, 0, platform1));
        glm::mat4 modelForBase = translateMatrix * scaleMatrix;
        under_rail.drawCubeWithTexture(lightingShaderWithTexture, modelForBase);

        //Rail track
        //left
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, baseHeight, length/2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.05, platform1));
        glm::mat4 modelForRailTrack = translateMatrix * scaleMatrix;
        track[ind_track].drawCubeWithTexture(lightingShaderWithTexture, modelForRailTrack);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, baseHeight, length / 2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.05, platform1+ length / 2));
        glm::mat4 modelForRailTrack2 = translateMatrix * scaleMatrix;
        track[ind_track].drawCubeWithTexture(lightingShaderWithTexture, modelForRailTrack2);

        //right
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, baseHeight, length / 2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0, 0.05, platform1));
        modelForRailTrack = translateMatrix * scaleMatrix;
        track[ind_track].drawCubeWithTexture(lightingShaderWithTexture, modelForRailTrack);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, baseHeight, length / 2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0, 0.05, platform1 + length / 2));
        modelForRailTrack2 = translateMatrix * scaleMatrix;
        track[ind_track].drawCubeWithTexture(lightingShaderWithTexture, modelForRailTrack2);



        //Block -2
        //Platform
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.9f, 0.1f, platform2));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.2f, length));
        modelMatrixForContainer = translateMatrix * scaleMatrix;
        sand.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.95f, 0.1f, platform2));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.2f, length));
        modelMatrixForContainer2 = translateMatrix * scaleMatrix;
        sand.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer2);


        //under rail Track
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(width, baseHeight, length));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.8, 0, platform2));
        modelForBase = translateMatrix * scaleMatrix;
        under_rail.drawCubeWithTexture(lightingShaderWithTexture, modelForBase);

        //Rail track
        //left
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, baseHeight, length / 2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.05, platform2));
        modelForRailTrack = translateMatrix * scaleMatrix;
        track[ind_track].drawCubeWithTexture(lightingShaderWithTexture, modelForRailTrack);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, baseHeight, length / 2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.05, platform2 + length / 2));
        modelForRailTrack2 = translateMatrix * scaleMatrix;
        track[ind_track].drawCubeWithTexture(lightingShaderWithTexture, modelForRailTrack2);

        //right
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, baseHeight, length / 2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0, 0.05, platform2));
        modelForRailTrack = translateMatrix * scaleMatrix;
        track[ind_track].drawCubeWithTexture(lightingShaderWithTexture, modelForRailTrack);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, baseHeight, length / 2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0, 0.05, platform2 + length / 2));
        modelForRailTrack2 = translateMatrix * scaleMatrix;
        track[ind_track].drawCubeWithTexture(lightingShaderWithTexture, modelForRailTrack2);



        ////-------------------Bench-----------------///////

        glm::mat4 rotOther = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));

        //Back
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.58, 0.95, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 modelForBench = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.4, 0.75, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 modelForBench2 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench2);


        //seat
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.15, 0.6, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 modelForBench3 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench3);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.95, 0.6, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 modelForBench4 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench4);


        //back support
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.05, 0.25, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 modelForBench5 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench5);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.05, 0.25, platform1 + 10.2f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench5 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench5);

        //bottom support
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.35, 0.25, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(40.0f), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 modelForBench6 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench6);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.35, 0.25, platform1 + 10.2f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(40.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench6 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench6);




        //-----------------Bench other side------------------//
        //back
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.58, 0.95, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(40.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.4, 0.75, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(40.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench2 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench2);


        //seat
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45, 0.6, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench3 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench3);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.20, 0.6, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench4 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench4);


        //back support
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.05, 0.25, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(40.0f), glm::vec3(1.0, 0.0, 0.0));
         modelForBench5 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench5);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.05, 0.25, platform1 + 10.2f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(40.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench5 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench5);

        //bottom support
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.35, 0.25, platform1 + 12.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench6 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench6);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5, 0.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.35, 0.25, platform1 + 10.2f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-40.0f), glm::vec3(1.0, 0.0, 0.0));
        modelForBench6 = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForBench6);




        //--------------------McDonald--------------------//

        // ||mm
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.9, 3.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.85, 0.32, platform2 + 1.0f));
        glm::mat4 modelForMcD = translateMatrix * scaleMatrix;
        mcdonald.drawCubeWithTexture(lightingShaderWithTexture, modelForMcD);
        //__
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.9, 0.01));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.33, 0.32, platform2 + 1.0f));
        modelForMcD = translateMatrix * scaleMatrix;
        mcdonald.drawCubeWithTexture(lightingShaderWithTexture, modelForMcD);
        //--
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.9, 0.01));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.33, 0.32, platform2 + 4.0f));
        modelForMcD = translateMatrix * scaleMatrix;
        mcdonald.drawCubeWithTexture(lightingShaderWithTexture, modelForMcD);
        //mm||
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 2.0, 3.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.35, 0.32, platform2 + 1.0f));
        modelForMcD = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForMcD, 1.0, 0.8, 0.8);

        //rakhar jaiga
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.03, 3.3));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.45, 1.2, platform2 + 0.9f));
        modelForMcD = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForMcD, 1.0, 1.0, 1.0);

        //kit
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5, 1.5, platform2 + 1.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.4f));
        modelForrocket = translateMatrix * scaleMatrix;
        health.drawCubeWithTexture(lightingShaderWithTexture, modelForrocket);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5, 1.5, platform2 + 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.4f));
        modelForrocket = translateMatrix * scaleMatrix;
        rocket.drawCubeWithTexture(lightingShaderWithTexture, modelForrocket);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5, 1.3, platform2 + 1.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.2f, 0.2f));
        modelForrocket = translateMatrix * scaleMatrix;
        nine.drawCubeWithTexture(lightingShaderWithTexture, modelForrocket);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5, 1.3, platform2 + 2.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.2f, 0.2f));
        modelForrocket = translateMatrix * scaleMatrix;
        eight.drawCubeWithTexture(lightingShaderWithTexture, modelForrocket);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5, 1.5f, platform2 + 3.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.3f, 0.3f));
        modelForScore11 = translateMatrix * scaleMatrix;
        score_block[A[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore11);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5, 1.5f, platform2 + 3.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.3f, 0.3f));
        modelForScore22 = translateMatrix * scaleMatrix;
        score_block[A[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore22);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5, 1.5f, platform2 + 2.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.3f, 0.3f));
        modelForScore33 = translateMatrix * scaleMatrix;
        score_block[A[2]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore33);


        //stand
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 2.0, 0.03));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.87, -0.00, platform2 + 0.97f));
        modelForMcD = translateMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForMcD);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 2.0, 0.03));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.35, -0.00, platform2 + 0.97f));
        modelForMcD = translateMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForMcD);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 2.0, 0.03));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.87, -0.00, platform2 + 4.03f));
        modelForMcD = translateMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForMcD);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 2.0, 0.03));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.35, -0.00, platform2 + 4.03f));
        modelForMcD = translateMatrix * scaleMatrix;
        wood.drawCubeWithTexture(lightingShaderWithTexture, modelForMcD);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 0.02, 3.0));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(168.0f), glm::vec3(0.0, 0.0, 1.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.55, 1.9, platform2 + 1.0f));
        modelForMcD = translateMatrix * rotateZMatrix * scaleMatrix;
        mcdonald.drawCubeWithTexture(lightingShaderWithTexture, modelForMcD);



        //---------------------- Game Shop 1 -----------------------//
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.8, 3.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45, 0.3, platform1 + 15.9f));
        modelForMcD = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForMcD, 0.1, 0.2, 0.8);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f,3.0, 3.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.4, 0.32, platform1 + 15.9f));
        modelForMcD = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForMcD, 0.6, 0.8, 1.0);

        
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f,0.05,1.0));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0, 0.0, 1.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1, 2.0, platform1 + 16.0f+basket_z));
        glm::mat4 modelForBasket = translateMatrix * rotateZMatrix * scaleMatrix;
        tree.drawCylinder(lightingShader, modelForBasket, 1.0, 0.0f, 0.0f);

        if (gameModeON)
        {
            
            gameBx += gameBxInc;
            gameBy += gameByInc;
            if (gameBy >= 2.0)
                gameByInc = -0.05f;

            if (gameBy <= -1.0f)
            {
                gameBx = 0.0f;
                gameBy = 0.0f;
                gameByInc = 0.0;
                gameBxInc = 0.0f;
                Gameshoot = false;
            }
            translateMatrix = glm::translate(identityMatrix, glm::vec3(gameBx, 2.0+gameBy, gameBz+ platform1+17.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
            glm::mat4 modelForBall = translateMatrix * scaleMatrix;
            ball[ball_type].drawSphereWithTexture(lightingShaderWithTexture, modelForBall);

            float bbx = gameBx, bby = 2.0 + gameBy, bbz = gameBz + platform1 + 17.0;
            float ccx = 4.1, ccy = 2.0, ccz = platform1 + 16.0f + basket_z;

            float distance2 = sqrt((bbx - ccx) * (bbx - ccx) + (bby - ccy) * (bby - ccy) + (bbz - ccz) * (bbz - ccz));
            if (distance2 <= 0.3)
            {
                //cout << addedCoin << endl;
                addedCoin++;
                gameBx = 0.0f;
                gameBy = 0.0f;
                gameByInc = 0.0;
                gameBxInc = 0.0f;
                Gameshoot = false;
            }

            int Arr[3];
            Arr[0] = 0;
            Arr[1] = 0;
            Arr[2] = 0;
            int ff = addedCoin;
            int inddd = 2;
            while (ff > 0) {
                Arr[inddd] = ff % 10;
                ff /= 10;
                inddd--;
            }



            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, 2.5f, platform1 + 17.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.3f, 0.3f));
            glm::mat4 modelForScore11 = translateMatrix * scaleMatrix;
            score_block[Arr[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore11);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, 2.5f, platform1 + 17.8f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.3f, 0.3f));
            glm::mat4 modelForScore22 = translateMatrix * scaleMatrix;
            score_block[Arr[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore22);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, 2.5f, platform1 + 18.1f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.3f, 0.3f));
            glm::mat4 modelForScore33 = translateMatrix * scaleMatrix;
            score_block[Arr[2]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore33);

        }


        //--------------------- Game Shop 2 -----------------//

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.8, 3.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45, 0.3, platform2 + 15.9f));
        modelForMcD = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForMcD, 0.1, 0.8, 0.2);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 3.0, 3.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.4, 0.32, platform2 + 15.9f));
        modelForMcD = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForMcD, 0.6, 1.0, 0.7);



        //----------- Tree 1---------//
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 1.8, 0.3));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45, 0.3, platform1 + 3.0f));
        glm::mat4 modelForTree = translateMatrix * scaleMatrix;
        tree.drawCylinder(lightingShader, modelForTree, 0.545f, 0.275f, 0.176f);

        

        float scale_Lxz = 5.0f;
        float trans_y = 1.8f;

        for (int i = 0; i < 500; i++)
        {
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_Lxz, 0.5, scale_Lxz));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45, trans_y, platform1 + 3.0f));
            glm::mat4 modelForleaf = translateMatrix * scaleMatrix;
            leaf.drawSphere(lightingShader, modelForleaf);
            trans_y += 0.01f;
            scale_Lxz -= 0.03f;
            if (scale_Lxz <= 0.0)
                break;

            if (i == 80)
                scale_Lxz = 4.0f;

            if(i==150)
                scale_Lxz = 3.0f;
        }



        //-------------- Tree 2 ----------------//
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 1.8, 0.3));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.45, 0.3, platform2 + 15.0f));
        modelForTree = translateMatrix * scaleMatrix;
        tree.drawCylinder(lightingShader, modelForTree, 0.545f, 0.275f, 0.176f);



        scale_Lxz = 5.0f;
        trans_y = 1.8f;

        for (int i = 0; i < 500; i++)
        {
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_Lxz, 0.5, scale_Lxz));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.45, trans_y, platform2 + 15.0f));
            glm::mat4 modelForleaf = translateMatrix * scaleMatrix;
            leaf.drawSphere(lightingShader, modelForleaf);
            trans_y += 0.01f;
            scale_Lxz -= 0.03f;
            if (scale_Lxz <= 0.0)
                break;

            if (i == 80)
                scale_Lxz = 4.0f;

            if (i == 150)
                scale_Lxz = 3.0f;
        }


        //---------------tree with curve----------------//
        glm::mat4 identityMatrixCurve = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrixCurve, rotateXMatrixCurve, rotateYMatrixCurve, rotateZMatrixCurve, scaleMatrixCurve, modelforCurve;
        translateMatrixCurve = glm::translate(identityMatrixCurve, glm::vec3(3.45, 2.3, platform2 + 8.2f));
        rotateXMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrixCurve = glm::scale(identityMatrixCurve, glm::vec3(0.8, 0.5, 0.8));
        modelforCurve = translateMatrixCurve * rotateYMatrixCurve * rotateXMatrixCurve  * rotateZMatrixCurve * scaleMatrixCurve;
        lightingShader.setMat4("model", modelforCurve);
        lightingShader.use();
        lightingShader.setVec3("material.ambient", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setVec3("material.diffuse", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setVec3("material.specular", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setFloat("material.shininess", 32.0f);
        glBindVertexArray(treeVAO);
        glDrawElements(GL_TRIANGLE_FAN, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        rotateZMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.8, 0.3));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.2, 2.3, platform2 + 7.5f));
        rotateXMatrix = glm::rotate(identityMatrixCurve, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 modelFortree2 = translateMatrix * rotateZMatrixCurve * rotateXMatrix * scaleMatrix;
        tree.drawCylinder(lightingShader, modelFortree2, 0.545f, 0.275f, 0.176f);

        translateMatrixCurve = glm::translate(identityMatrixCurve, glm::vec3(3.45, 2.3, platform2 + 5.8f));
        rotateXMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrixCurve = glm::scale(identityMatrixCurve, glm::vec3(0.8, 0.5, 0.8));
        modelforCurve = translateMatrixCurve * rotateXMatrixCurve * rotateYMatrixCurve * rotateZMatrixCurve * scaleMatrixCurve;
        lightingShader.setMat4("model", modelforCurve);
        lightingShader.use();
        lightingShader.setVec3("material.ambient", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setVec3("material.diffuse", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setVec3("material.specular", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setFloat("material.shininess", 32.0f);
        glBindVertexArray(treeVAO);
        glDrawElements(GL_TRIANGLE_FAN, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.8, 0.3));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.4, 2.1, platform2 + 6.3f));
        rotateXMatrix = glm::rotate(identityMatrixCurve, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 modelFortree3 = translateMatrix *  rotateXMatrix * scaleMatrix;
        tree.drawCylinder(lightingShader, modelFortree3, 0.545f, 0.275f, 0.176f);


        translateMatrixCurve = glm::translate(identityMatrixCurve, glm::vec3(3.45, 3.3, platform2 + 7.0f));
        rotateXMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrixCurve = glm::rotate(identityMatrixCurve, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrixCurve = glm::scale(identityMatrixCurve, glm::vec3(0.8, 0.6, 0.8));
        modelforCurve = translateMatrixCurve * rotateXMatrixCurve * rotateYMatrixCurve * rotateZMatrixCurve * scaleMatrixCurve;
        lightingShader.setMat4("model", modelforCurve);
        lightingShader.use();
        lightingShader.setVec3("material.ambient", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setVec3("material.diffuse", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setVec3("material.specular", glm::vec3(0.2f, 1.0f, 0.2f));
        lightingShader.setFloat("material.shininess", 32.0f);
        glBindVertexArray(treeVAO);
        glDrawElements(GL_TRIANGLE_FAN, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 5.3, 0.5));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45, -1.3, platform2 + 7.0f));
        glm::mat4 modelFortree = translateMatrix * scaleMatrix;
        tree.drawCylinder(lightingShader, modelFortree, 0.545f, 0.275f, 0.176f);

        

        // ------------------------ Wood ----------------//

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 1.3, 0.7));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.05, 0.6, platform1 + 5.0f));
        glm::mat4 modelForWood = translateMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;
        tree.drawCylinder(lightingShader, modelForWood, 0.545f, 0.275f, 0.176f);

        float xx = -3.45;
        for (int i = 0; i < 3; i++)
        {
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 1.3, 0.7));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.6, platform1 + 5.0f));
            modelForWood = translateMatrix * rotateXMatrix * scaleMatrix;
            tree.drawCylinder(lightingShader, modelForWood, 0.545f, 0.275f, 0.176f);
            xx -= 0.4f;
        }

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 1.3, 0.7));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.25,1.0, platform1 + 5.0f));
        modelForWood = translateMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;
        tree.drawCylinder(lightingShader, modelForWood, 0.50f, 0.3f, 0.2f);
        xx = -3.25;
        for (int i = 0; i < 2; i++)
        {
            xx -= 0.4f;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 1.3, 0.7));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.0, platform1 + 5.0f));
            modelForWood = translateMatrix * rotateXMatrix * scaleMatrix;
            tree.drawCylinder(lightingShader, modelForWood, 0.545f, 0.275f, 0.176f);
            
        }

        //-------------------wall-------------------//

        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 0.1f, platform1));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.0f, 10.0f));
        glm::mat4 modelMatrixForWall = translateMatrix * scaleMatrix;
        Wall[wall_num1].drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForWall);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, 0.1f, platform1));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.0f, 10.0f));
        modelMatrixForWall = translateMatrix * scaleMatrix;
        Wall[wall_num2].drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForWall);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 0.1f, platform1+10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.0f, 10.0f));
        modelMatrixForWall = translateMatrix * scaleMatrix;
        Wall[wall_num3].drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForWall);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, 0.1f, platform1+10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.0f, 10.0f));
        modelMatrixForWall = translateMatrix * scaleMatrix;
        Wall[wall_num4].drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForWall);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 0.1f, platform2));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.0f, 10.0f));
        modelMatrixForWall = translateMatrix * scaleMatrix;
        Wall[wall_num5].drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForWall);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, 0.1f, platform2));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.0f, 10.0f));
        modelMatrixForWall = translateMatrix * scaleMatrix;
        Wall[wall_num6].drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForWall);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 0.1f, platform2+10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.0f, 10.0f));
        modelMatrixForWall = translateMatrix * scaleMatrix;
        Wall[wall_num7].drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForWall);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, 0.1f, platform2+10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.0f, 10.0f));
        modelMatrixForWall = translateMatrix * scaleMatrix;
        Wall[wall_num8].drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForWall);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void rail(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float baseHeight = 0.1;
    float width = 10;
    float length = 20;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);



    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;

    
    //light stand-1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, 0.0f, light1));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 3.0f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 2.7f,light1));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.05f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    //light stand-2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.0f, light1));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 3.0f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 2.7f, light1));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.05f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);


    //light stand-3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, 0.0f, light2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 3.0f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 2.7f, light2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.05f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    //light stand-4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.0f, light2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 3.0f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 2.7f, light2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.05f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);
    


    //---------------------2nd block er jonno-----------------------------//

    //light stand-1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, 0.0f, light3));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 3.0f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 2.7f, light3));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.05f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    //light stand-2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.0f, light3));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 3.0f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 2.7f, light3));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.05f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);


    //light stand-3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, 0.0f, light4));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 3.0f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 2.7f, light4));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.05f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    //light stand-4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.0f, light4));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 3.0f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 2.7f, light4));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.05f, 0.05f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    


    //-------------------------Obstacle----------------------//

    //glm::vec3 pos = camera.Position;
    //float start = pos[3];
    //std::random_device rd; 
    //std::mt19937 gen(rd()); 
    //std::uniform_real_distribution<float> dist(start, start+10.0f);
    //float random_number = dist(gen);



}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, float currentFrame)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if(!Gameshoot && gameBz>0)
            gameBz -= 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        
        if (!Gameshoot &&  gameBz < 3.0)
            gameBz += 0.005f;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        if (ball_type == 7)
            ball_type = 0;
        else
            ball_type++;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (!onMud && !onMud2 && !b_jump && b_left == false && ball_x<0.0f)
            b_left = true;
      
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (!onMud && !onMud2 && b_jump == false && b_right == false && ball_x >0.0f)
            b_right = true;

    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if (!fly && !onMud && !onMud2 && b_jump == false)
            b_jump = true;

    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if(fly==false && rocket_point>0)
        {
            rocket_point--;
            fly = true;
            fly_time = currentFrame;
            scaleFly = 5.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (!purchaseMode)
        {
            gameModeON = true;
            addedCoin = 0;
        }
            
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        if (gameModeON)
        {
            gameModeON = false;
            score_count += addedCoin;
            score_time = min(24, score_time + addedCoin);
        }
        

    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if(!gameModeON)
            purchaseMode = true;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (purchaseMode)
        {
            purchaseMode = false;
        }


    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (gameModeON)
        {
            if (Gameshoot == false)
            {
                Gameshoot = true;
                gameBxInc = 0.05f;
                gameByInc = 0.05f;
            }
                
        }
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (health_point > 0)
        {
            score_time = 24;
            health_point--;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        if (score_time == 0)
        {
            score_time = 24;
            score_count = 0;
            health_point = 0;
            rocket_point = 0;
            game_over = false;
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   

    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointlight5.turnOff();
            pointlight6.turnOff();
            pointlight7.turnOff();
            pointlight8.turnOff();
            pointlight9.turnOn();
            pointLightOn = !pointLightOn;
        }

        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointlight5.turnOn();
            pointlight6.turnOn();
            pointlight7.turnOn();
            pointlight8.turnOn();
            pointlight9.turnOn();
            pointLightOn = !pointLightOn;
        }
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        ind_track = 0;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        ind_track = 1;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        ind_track = 2;
    }

    //level 2
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        time_decrement = 0.5;
    }
    //level 1
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        time_decrement = 0.8;
    }

    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        if (purchaseMode && score_count >= 10)
        {
            rocket_point++;
            score_count -= 10;
        }
    }
    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        if (purchaseMode && score_count >= 10)
        {
            health_point++;
            score_count -= 10;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

//BezierCurve Function
long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}
