#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Triangle transformation variables
float rotateAngleTriangle = 0.0f;
float translate_X_Triangle = 0.0f;
float translate_Y_Triangle = 0.0f;
float scale_X_Triangle = 1.0f;
float scale_Y_Triangle = 1.0f;

// Square transformation variables
float rotateAngleSquare = 0.0f;
float translate_X_Square = 0.0f;
float translate_Y_Square = 0.0f;
float scale_X_Square = 1.0f;
float scale_Y_Square = 1.0f;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 objectColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(objectColor, 1.0);\n"
"}\n\0";

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Assignment 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Triangle vertices
    float triangleVertices[] = {
         -0.0364163232092331,-0.899543747728832,0,
        -0.110124824906591,-0.903473708294639,0,
        -0.182249333014464,-0.899678335419442,0,
        -0.236751135971873,-0.893594971803878,0,
        -0.300874923517956,-0.884766019299875,0,
        -0.365004922803606,-0.873352983136162,0,
        -0.417928943911097,-0.856960202419886,0,
        -0.501309124113662,-0.837471904819585,0,
        -0.543014743563861,-0.821267546870163,0,
        -0.599150234027288,-0.802236847417935,0,
        -0.647285003928925,-0.778172568336899,0,
        -0.698625031446931,-0.75405445417962,0,
        -0.735553823170409,-0.725010430546022,0,
        -0.786912485907115,-0.693140065409617,0,
        -0.83188548036935,-0.651041035786867,0,
        -0.862409968599656,-0.619520598646047,0,
        -0.891356674980044,-0.577690744404517,0,
        -0.934758099331927,-0.522698214021345,0,
        -0.955747567327492,-0.457746194533048,0,
        -0.968742526500834,-0.385176511756234,0,
        -0.967326249879647,-0.307680919503102,0,
        -0.949865049957915,-0.238206753610314,0,
        -0.919533125654173,-0.189620597300171,0,
        -0.885995943734063,-0.141088276066271,0,
        -0.852421491376552,-0.108060456790622,0,
        -0.807641060840883,-0.0700528929624095,0,
        -0.766059676182016,-0.0345755777176617,0,
        -0.708445791701737,-0.00195152151384236,0,
        -0.668435977153222,0.0206322929704847,0,
        -0.604392942221504,0.045396428042691,0,
        -0.545132946756074,0.0599049810904293,0,
        -0.497078929468803,0.0694337895856045,0,
        -0.424985480058763,0.0785588350089498,0,
        -0.328852598525954,0.0872801173604662,0,
        -0.229520671116342,0.0985316482954468,0,
        -0.155843228116818,0.115382027159796,0,
        -0.0757179994471553,0.116620233913406,0,
        0.0220423578521045,0.114978264087966,0,
        0.119802715151364,0.113336294262526,0,
        0.207972146559783,0.101519495026984,0,
        0.307378614844193,0.0817620220454637,0,
        0.406803718347304,0.0542522980848172,0,
        0.469362147522602,0.0299457611606843,0,
        0.511098825670634,0.000820984912719868,0,
        0.557637178503653,-0.0258004602899017,0,
        0.583322721611573,-0.0443197265178125,0,
        0.610598470048544,-0.0576977429644283,0,
        0.629867286184159,-0.0735252553801428,0,
        0.66197576800395,-0.0973203590799585,0,
        0.681250795879131,-0.115731955155381,0,
        0.702140876041631,-0.139338636088343,0,
        0.716620440971391,-0.162837646868817,0,
        0.732702634709336,-0.186363575187413,0,
        0.751971450844951,-0.202191087603127,0,
        0.763239546418776,-0.223052179647649,0,
        0.779340375375421,-0.25433035894537,0,
        0.789055536057595,-0.295837202729438,0,
        0.79230427585093,-0.313979623423641,0,
        0.795596497821232,-0.350210629735804,0,
        0.797279879243782,-0.383830634850136,0,
        0.798988107624599,-0.427786974603302,0,
        0.797472443170347,-0.463937228301099,0,
        0.789533840004223,-0.494811644526991,0,
        0.77838997922173,-0.525632225676639,0,
        0.764028437343736,-0.551230804430626,0,
        0.756058775479779,-0.569184802357976,0,
        0.732112519450509,-0.607542294181773,0,
        0.716129713545629,-0.625361704418513,0,
        0.70014690764075,-0.643181114655253,0,
        0.66816266061229,-0.671067684149607,0,
        0.658565522981883,-0.678658429900001,0,
        0.636172201844265,-0.696370169984253,0,
        0.616984138323016,-0.714135745144749,0,
        0.596187234254016,-0.729290319107414,0,
        0.560985306130055,-0.751954886206107,0,
        0.530578840951514,-0.769532038599749,0,
        0.498563535225222,-0.784498189795561,0,
        0.479356836485273,-0.794511513976931,0,
        0.439334598457624,-0.811927161141841,0,
        0.40893434501865,-0.832088397195192,0,
        0.373707569936422,-0.844416629655051,0,
        0.340077211922812,-0.854187695993324,0,
        0.316056415018743,-0.86153618390062,0,
        0.284028685813318,-0.871334167777015,0,
        0.247180646704206,-0.875883231719626,0,
        0.199120417677368,-0.882827956555093,0,
        0.167080264992809,-0.887457773112071,0,
        0.143047044609607,-0.88963809369995,0,
        0.0933903985141517,-0.899139984657003,0,
        0.0645555034459623,-0.903823636290224,0,
        0.0357143966382063,-0.905923204263737,0,
        0.00366182047451468,-0.905384853501298,0,
    };


    // Square vertices
    float squareVertices[] = {
        -0.197679294097915,-0.48081452470357,0,
        -0.622301387392032,-0.504690381017751,0,
        -0.631997912855505,-0.47093578821281,0,
        -0.650117557171298,-0.266497086176498,0,
        -0.670367828158436,0.157615644473156,0,
        -0.677213165160837,0.338609170805238,0,
        -0.542791120939463,0.419038774713664,0,
        -0.227576396631994,0.623046796140025,0,
        -0.224532644244357,0.690179136216201,0,
        -0.211817213351513,0.73389321812627,0,
        -0.171757704886465,0.735804363332929,0,
        -0.144562709063859,0.756019434462524,0,
        -0.110702516686285,0.670179405391582,0,
        -0.0737550897441077,0.633383130778858,0,
        -0.0400564025952647,0.614729276860338,0,
        0.0016305816362343,0.606277169890042,0,
        0.0464731295675697,0.618443897121169,0,
        0.0880545142264364,0.653921212365917,0,
        0.107180460352019,0.697527624123497,0,
        0.134263644862425,0.764256201127844,0,
        0.1438110885763,0.792519616155906,0,
        0.172714312779722,0.768778347532334,0,
        0.18084547987241,0.719546170307263,0,
        0.204860065036913,0.729478741874268,0,
        0.249721248186948,0.73389321812627,0,
        0.251404629609498,0.700273213011938,0,
        0.289873932745495,0.697043108437302,0,
        0.326759242292007,0.686087670421663,0,
        0.37325411294806,0.677554810837,0,
        0.426165710576418,0.666330197440142,0,
        0.474288056998922,0.647434085678523,0,
        0.499967388367275,0.631498903110321,0,
        0.527330101158178,0.581943715427787,0,
        0.538635467169403,0.545578121425014,0,
        0.541952536097971,0.499010780474017,0,
        0.545244758068273,0.462779774161855,0,
        0.553344866463128,0.426468015235326,0,
        0.555040671364812,0.387679842801577,0,
        0.539107559376465,0.349187763287169,0,
        0.508738364635324,0.316106108935276,0,
        0.495960816346813,0.298232863622293,0,
        0.472573616878538,0.0273078424247317,0,
        0.452447580682732,-0.266927766786449,0,
        0.451285985383776,-0.450370789087629,0,
        0.0651331952256568,-0.477476749976446,0
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // Triangle VAO
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Square VAO
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set up transformation for triangle
        glm::mat4 transformTriangle = glm::mat4(1.0f);
        transformTriangle = glm::translate(transformTriangle, glm::vec3(translate_X_Triangle, translate_Y_Triangle, 0.0f));
        transformTriangle = glm::rotate(transformTriangle, glm::radians(rotateAngleTriangle), glm::vec3(0.0f, 0.0f, 1.0f));
        transformTriangle = glm::scale(transformTriangle, glm::vec3(scale_X_Triangle, scale_Y_Triangle, 1.0f));

        // Draw triangle with red color
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // Red color for triangle
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformTriangle));
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 91);

        // Set up transformation for square
        glm::mat4 transformSquare = glm::mat4(1.0f);
        transformSquare = glm::translate(transformSquare, glm::vec3(translate_X_Square, translate_Y_Square, 0.0f));
        transformSquare = glm::rotate(transformSquare, glm::radians(rotateAngleSquare), glm::vec3(0.0f, 0.0f, 1.0f));
        transformSquare = glm::scale(transformSquare, glm::vec3(scale_X_Square, scale_Y_Square, 1.0f));

        // Draw square with yellow color
        glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f); // Yellow color for square
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformSquare));
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 44);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

// process all input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Triangle transformations
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotateAngleTriangle += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        rotateAngleTriangle -= 0.1f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        translate_Y_Triangle += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        translate_Y_Triangle -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        translate_X_Triangle -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        translate_X_Triangle += 0.001f;

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    {
        scale_X_Triangle += 0.001f;
        scale_Y_Triangle += 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        scale_X_Triangle -= 0.001f;
        scale_Y_Triangle -= 0.001f;
    }

    // Square transformations
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        rotateAngleSquare += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        rotateAngleSquare -= 0.1f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        translate_Y_Square += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        translate_Y_Square -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        translate_X_Square -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        translate_X_Square += 0.001f;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        scale_X_Square += 0.001f;
        scale_Y_Square += 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        scale_X_Square -= 0.001f;
        scale_Y_Square -= 0.001f;
    }
}

// GLFW callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
