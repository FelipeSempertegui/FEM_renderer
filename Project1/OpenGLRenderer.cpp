#include"OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer() {}

OpenGLRenderer::~OpenGLRenderer() {
    glfwTerminate();
}

void OpenGLRenderer::initWindow(const char* title, int width, int height) {
    //  Initialize GLFW
    glfwInit();

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(4);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!" << std::endl;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Dark blue background
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

}

void OpenGLRenderer::setMaterial(FEMalgorithm& result, const std::string& fileName, float ambient, float diffuse, float specular, int shininess) {

    mesh.initializeMesh(result.allFreqMeshes, result.allFreqColors, result.allFreqNormals, result.frames, result.nrFreqs);

    matrices.initVariables();
    matrices.initMatrices();

    shaderProgram.createId(fileName); // prepared to use phong shading

    material.setLightning(glm::vec3(ambient), glm::vec3(diffuse), glm::vec3(specular), shininess);

}

int OpenGLRenderer::getWindowShouldClose() {
    return glfwWindowShouldClose(window);
}

void OpenGLRenderer::terminateCondition() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void OpenGLRenderer::update() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh.selectCurrentTimeStep();
    matrices.updateMatrices(window);
    matrices.updateUniforms(shaderProgram);
    material.applyToShader(shaderProgram);

}

void OpenGLRenderer::draw() {


    mesh.drawCall();

    // Swap front and back buffers 
    glfwSwapBuffers(window);
    // Poll for and process events
    glfwPollEvents();


}

void OpenGLRenderer::incrementFrame() {
    mesh.changeFrequency(window);
    mesh.incrementFrames();
}


/*
class OpenGLRenderer {

private:

    GLFWwindow* window;
    Shader shaderProgram;
    Material material;
    transfMatrices matrices;
    dynamicMesh mesh;

    std::vector < std::vector<std::vector<float>> > dynMesh;
    std::vector < std::vector<std::vector<float>> > dynColors;
    std::vector < std::vector<std::vector<float>> > dynNormals;

public:
    OpenGLRenderer() {}

    ~OpenGLRenderer() {
        glfwTerminate();
    }

    void initWindow(const char* title, int width, int height) {
        //  Initialize GLFW
        glfwInit();

        // Create a windowed mode window and its OpenGL context 
        window = glfwCreateWindow(width, height, title, NULL, NULL);

        // Make the window's context current
        glfwMakeContextCurrent(window);

        glfwSwapInterval(4);

        if (glewInit() != GLEW_OK)
            std::cout << "ERROR!" << std::endl;

        // Ensure we can capture the escape key being pressed below
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        // Hide the mouse and enable unlimited mouvement
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Dark blue background
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

    }

    void setMaterial(FEMalgorithm& result, const std::string& fileName, float ambient, float diffuse, float specular, int shininess) {

        mesh.initializeMesh(result.allFreqMeshes, result.allFreqColors, result.allFreqNormals, result.frames, result.nrFreqs);

        matrices.initVariables();
        matrices.initMatrices();

        shaderProgram.createId(fileName); // prepared to use phong shading

        material.setLightning(glm::vec3(ambient), glm::vec3(diffuse), glm::vec3(specular), shininess);

    }

    int getWindowShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void terminateCondition() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void update() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh.selectCurrentTimeStep();
        matrices.updateMatrices(window);
        matrices.updateUniforms(shaderProgram);
        material.applyToShader(shaderProgram);

    }

    void draw() {


        mesh.drawCall();

        // Swap front and back buffers 
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();


    }

    void incrementFrame() {
        mesh.changeFrequency(window);
        mesh.incrementFrames();
    }

};
*/
