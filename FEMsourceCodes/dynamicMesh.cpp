#include"dynamicMesh.h"

void dynamicMesh::initRendererVariables() {
    // for rendering
    freqIndex = 0;
    counter = 0;
    delay %= 7;
}

void dynamicMesh::initDynamicVAO() {

    // 1st attribute buffer : vertices
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, dynMesh[0][0].size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // 2nd attribute buffer : colors
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, dynColors[0][0].size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // 3rd attribute buffer : normals
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, dynNormals[0][0].size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

dynamicMesh::dynamicMesh() {}

dynamicMesh::dynamicMesh(std::vector < std::vector<std::vector<float>> > dynMesh, std::vector < std::vector<std::vector<float>> >dynColors, std::vector < std::vector<std::vector<float>> >dynNormals, int frames, int freqIndexMax) {

        this->dynMesh = dynMesh;
        this->dynColors = dynColors;
        this->dynNormals = dynNormals;

        this->frames = frames;
        this->freqIndexMax = freqIndexMax;


        this->initRendererVariables();

        this->initDynamicVAO();

    }

dynamicMesh::~dynamicMesh() {}

void dynamicMesh::initializeMesh(std::vector < std::vector<std::vector<float>> > dynMesh, std::vector < std::vector<std::vector<float>> >dynColors, std::vector < std::vector<std::vector<float>> >dynNormals, int frames, int freqIndexMax) {
    this->dynMesh = dynMesh;
    this->dynColors = dynColors;
    this->dynNormals = dynNormals;

    this->frames = frames;
    this->freqIndexMax = freqIndexMax;

    this->initRendererVariables();

    this->initDynamicVAO();
}

void dynamicMesh::drawCall() {
    glDrawArrays(GL_TRIANGLES, 0, dynMesh[0][0].size());
}

void dynamicMesh::selectCurrentTimeStep() {
    // Dynamic vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dynMesh[0][0].size() * sizeof(float), dynMesh[freqIndex][counter].data());

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dynColors[0][0].size() * sizeof(float), dynColors[freqIndex][counter].data());

    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dynNormals[0][0].size() * sizeof(float), dynNormals[freqIndex][counter].data());
}

void dynamicMesh::changeFrequency(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_1) != GLFW_RELEASE) {
        std::this_thread::sleep_for(delay);
        freqIndex++;
        std::this_thread::sleep_for(delay);
    }
}

void dynamicMesh::incrementFrames() {
    counter++;
    if (counter == frames - 1) { counter = 0; }
    if (freqIndex == freqIndexMax) { freqIndex = 0; }
}
