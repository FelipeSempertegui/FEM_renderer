#pragma once

#include"Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>



class transfMatrices {

private:
    //Initialization variables
    double xpos, ypos;
    float horizontalAngle, verticalAngle, radius;

    //Transformation matrices & vectors
    glm::mat4 V;        // View matrix - it is updated in the loop
    glm::mat4 M;        // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 P;        // Projection matrix
    glm::vec3 viewPos;  // camera in global coordinates - it is updated in the loop
    glm::vec3 globalLightPos;
    glm::vec3 lightPos; // it will be updated in the loop 

public:

    transfMatrices();

    ~transfMatrices();

    void initVariables();

    void initMatrices();

    void updateMatrices(GLFWwindow* window);

    void updateUniforms(Shader& program);

};