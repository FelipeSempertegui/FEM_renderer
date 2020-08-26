#include"transfMatrices.h"


transfMatrices::transfMatrices() {}

transfMatrices::~transfMatrices() {}

void transfMatrices::initVariables() {
    // Initial variables for the camera
    horizontalAngle = 0.0f;
    verticalAngle = 0.0f;
    radius = 0.03f;
}

void transfMatrices::initMatrices() {
    // Matrices & vectors
    M = glm::mat4(1.0f); // Model matrix : an identity matrix (model will be at the origin)
    P = glm::perspective(glm::radians(45.0f), 1.0f, 0.001f, 1.0f); // Projection matrix
    globalLightPos = glm::vec3(5.f, 5.f, 5.f); // it will be updated in the loop
}

void transfMatrices::updateMatrices(GLFWwindow* window) {

    float speed = 3.0f; // 3 units / second
    float mouseSpeed = 0.001f;

    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024 / 2, 1024 / 2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
    verticalAngle += mouseSpeed * float(1024 / 2 - ypos);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));

    // Right vector
    glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));
    // Up vector
    glm::vec3 up = glm::cross(right, direction);
    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        radius -= radius * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        radius += radius * deltaTime * speed;
    }

    viewPos = radius * direction;

    // Camera matrix
    V = glm::lookAt(
        viewPos,                                // Camera is here
        glm::vec3(0.0011, 0.002, 0.0012),       // and looks here
        up);                                    // Head is up

    lightPos = glm::vec4(globalLightPos, 1.0f) * V; // reposition the coordinates of the light with movement

    lastTime = currentTime;

}

void transfMatrices::updateUniforms(Shader& program) {
    program.setUniform_mat4(V, "V");
    program.setUniform_mat4(M, "M");
    program.setUniform_mat4(P, "P");
    program.setUniform_vec3(viewPos, "viewPos");
    program.setUniform_vec3(lightPos, "lightPos");
}


