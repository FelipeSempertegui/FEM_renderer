#pragma once

#include <glm.hpp>
#include"Shader.h"


class Material {
private:
    glm::vec3 lightColorAmbi;
    glm::vec3 lightColorDiff;
    glm::vec3 lightColorSpec;
    int shininess;
public:

    Material();

    ~Material();

    Material(glm::vec3 lightColorAmbi, glm::vec3 lightColorDiff, glm::vec3 lightColorSpec, int shininess);

    void setLightning(glm::vec3 lightColorAmbi, glm::vec3 lightColorDiff, glm::vec3 lightColorSpec, int shininess);

    void applyToShader(Shader& program);
};