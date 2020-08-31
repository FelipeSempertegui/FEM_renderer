#pragma once


#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include <iostream>
#include <glm.hpp>

#include"FEMalgorithm.h"

#include"Shader.h"
#include"Material.h"
#include"transfMatrices.h"
#include"dynamicMesh.h"


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
    OpenGLRenderer();

    ~OpenGLRenderer();

    void initWindow(const char* title, int width, int height);

    void setMaterial(FEMalgorithm& result, const std::string& fileName, float ambient, float diffuse, float specular, int shininess);

    int getWindowShouldClose();

    void terminateCondition();

    void update();

    void draw();

    void incrementFrame();

};
