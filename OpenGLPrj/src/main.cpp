#include <OpenGLPrj.hpp>

#include <GLFW/glfw3.h>

#include <Shader.hpp>

#include <iostream>
#include <string>
#include <vector>

const std::string program_name = ("Transformation basics");

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


static glm::vec3 transl = glm::vec3(0.0f, 0.0f, 0.0f);

static glm::vec3 up = glm::vec3(0.0f, 0.01f, 0.0f);
static glm::vec3 down = glm::vec3(0.0f, -0.01f, 0.0f);
static glm::vec3 left = glm::vec3(-0.01f, 0.0f, 0.0f);
static glm::vec3 right = glm::vec3(0.01f, 0.0f, 0.0f);
float const PI_OVER_4 = glm::quarter_pi<float>();
float const PI = glm::pi<float>();
float const PI_OVER_2 = glm::pi<float>() / 2.0f;
float agol = 0.0f;

int w = 0, s = 0, a = 0, d = 1;
// Starting angle is not 0, but PI/8
float angle = PI_OVER_4 / 2.0f;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
        program_name.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------

    std::string shader_location("D:/Sara fax/Kompjuterska grafika/Lab 4/OpenGLPrj/res/shaders/");

    std::string used_shaders("shader");

    Shader ourShader(shader_location + used_shaders + std::string(".vert"),
        shader_location + used_shaders + std::string(".frag")
    );

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertices;


    //45


    for (auto i = 0; i < 3; ++i)
        vertices.push_back(0.0f);

    for (auto i = 0; i < 9; ++i) {
        vertices.push_back(glm::cos(angle) * 0.1f);
        vertices.push_back(glm::sin(angle) * 0.1f);
        vertices.push_back(0.0f);
        angle += PI_OVER_4;
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_a), vertices_a, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        // create transformations
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, transl);
        transform = glm::rotate(transform, agol,
            glm::vec3(0.0f, 0.0f, 1.0f));


        // render container
        ourShader.use();

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render container
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 9);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
        // etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (transl[1] < 0.8f)
            transl += up;
        w = 1;
        if (s) {
            agol += PI;
            s = 0;
        }
        else if (a) {
            agol -= PI_OVER_2;
            a = 0;
        }
        else if (d) {
            agol += PI_OVER_2;
            d = 0;
        }


    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (transl[1] > -0.8f)
            transl += down;
        s = 1;
        if (w) {
            agol += PI;
            w = 0;
        }
        else if (a) {
            agol += PI_OVER_2;
            a = 0;
        }
        else if (d) {
            agol -= PI_OVER_2;
            d = 0;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (transl[0] > -0.8f)
            transl += left;
        a = 1;
        if (s) {
            agol -= PI_OVER_2;
            s = 0;
        }
        else if (w) {
            agol += PI_OVER_2;
            w = 0;
        }
        else if (d) {
            agol += PI;
            d = 0;
        }


    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (transl[0] < 0.8f)
            transl += right;
        d = 1;
        if (s) {
            agol += PI_OVER_2;
            s = 0;
        }
        else if (a) {
            agol += PI;
            a = 0;
        }
        else if (w) {
            agol -= PI_OVER_2;
            w = 0;
        }

    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
