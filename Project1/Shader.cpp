#include"Shader.h"


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int ID = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(ID, 1, &src, nullptr);
    glCompileShader(ID);

    return ID;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {

    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[int(type)] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

Shader::Shader() {}

Shader::~Shader() {
    glDeleteProgram(id);
}

Shader::Shader(const std::string& filepath)
{
    ShaderProgramSource source = ParseShader(filepath);
    id = CreateShader(source.VertexSource, source.FragmentSource);
}

void Shader::createId(const std::string& filepath) {
    ShaderProgramSource source = ParseShader(filepath);
    id = CreateShader(source.VertexSource, source.FragmentSource);
}

void Shader::setUniform_mat4(glm::mat4 value, const char* name)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform_vec3(glm::vec3 value, const char* name)
{
    glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);

}

void Shader::setUniform_int(int value, const char* name)
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::use() {
    glUseProgram(id);
}


/*
class Shader {
private:
    unsigned int id;

    unsigned int CompileShader(unsigned int type, const std::string& source) {
        unsigned int ID = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(ID, 1, &src, nullptr);
        glCompileShader(ID);

        return ID;
    }

    ShaderProgramSource ParseShader(const std::string& filepath) {

        std::ifstream stream(filepath);

        enum class ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;

        while (getline(stream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else {
                ss[int(type)] << line << '\n';
            }
        }
        return { ss[0].str(), ss[1].str() };
    }

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }



public:

    Shader() {}

    ~Shader() {
        glDeleteProgram(id);
    }

    Shader(const std::string& filepath)
    {
        ShaderProgramSource source = ParseShader(filepath);
        id = CreateShader(source.VertexSource, source.FragmentSource);
    }

    void createId(const std::string& filepath) {
        ShaderProgramSource source = ParseShader(filepath);
        id = CreateShader(source.VertexSource, source.FragmentSource);
    }

    void setUniform_mat4(glm::mat4 value, const char* name)
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void setUniform_vec3(glm::vec3 value, const char* name)
    {
        glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);

    }

    void setUniform_int(int value, const char* name)
    {
        glUniform1i(glGetUniformLocation(id, name), value);
    }

    void use() {
        glUseProgram(id);
    }
};
*/