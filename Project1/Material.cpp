#include"Material.h"

Material::Material() {}

Material::~Material() {}

Material::Material(glm::vec3 lightColorAmbi, glm::vec3 lightColorDiff, glm::vec3 lightColorSpec, int shininess) {
    this->lightColorAmbi = lightColorAmbi;
    this->lightColorDiff = lightColorDiff;
    this->lightColorSpec = lightColorSpec;
    this->shininess = shininess;
}

void Material::setLightning(glm::vec3 lightColorAmbi, glm::vec3 lightColorDiff, glm::vec3 lightColorSpec, int shininess) {
    this->lightColorAmbi = lightColorAmbi;
    this->lightColorDiff = lightColorDiff;
    this->lightColorSpec = lightColorSpec;
    this->shininess = shininess;
}

void Material::applyToShader(Shader& program) {
    program.setUniform_vec3(lightColorAmbi, "lightColorAmbi");
    program.setUniform_vec3(lightColorDiff, "lightColorDiff");
    program.setUniform_vec3(lightColorSpec, "lightColorSpec");
    program.setUniform_int(shininess, "shininess");

    program.use();
}
