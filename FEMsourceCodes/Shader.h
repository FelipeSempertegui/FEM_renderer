#pragma once


#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
//#include <string>

#include <gtc/type_ptr.hpp>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    unsigned int id;

    unsigned int CompileShader(unsigned int type, const std::string& source);

    ShaderProgramSource ParseShader(const std::string& filepath);

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);



public:

    Shader();

    ~Shader();

    Shader(const std::string& filepath);

    void createId(const std::string& filepath);

    void setUniform_mat4(glm::mat4 value, const char* name);

    void setUniform_vec3(glm::vec3 value, const char* name);

    void setUniform_int(int value, const char* name);

    void use();
};