#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"




Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0), m_UniformLocationCache()
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };


    std::string line;
    std::stringstream ss[2]; // two types of shaders
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) // If we find on a line we need to specify a type
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX; // Set mode to vertex
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;// Set mode to fragment
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() }; // Build and return the ShaderProgramSource struct
}


unsigned int Shader::CompileShader(const std::string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // Pointer to beginning of data
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id); // Compile the shader

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // Get info on the compilation

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // Get the length of the message
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message); // Get the message
        std::cout << "Failed " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // Compile shader from code
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs); //attaching to program
    glAttachShader(program, fs);
    glLinkProgram(program); // Linking
    glValidateProgram(program); // Validating

    glDeleteShader(vs); // remove overflow thing, we already compiled the program that is used!
    glDeleteShader(fs); // hmm... detachshader is something to look into

    return program;
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

unsigned int Shader::GetRendererID() const {
    return m_RendererID;
}

void Shader::SetUniform1f(const std::string& name, GLfloat v0)
{
    GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3fv(const std::string& name, std::vector<glm::vec3> inVector)
{
    GLCall(glUniform3fv(GetUniformLocation(name), (GLint)inVector.size(), &inVector[0].x));
}

void Shader::SetUniform1i(const std::string& name, GLint i)
{
    GLCall(glUniform1i(GetUniformLocation(name), i));
}

void Shader::SetUniform1iv(const std::string& name, std::vector<int> inVector)
{
    GLCall(glUniform1iv(GetUniformLocation(name), (GLint)inVector.size(), &inVector[0]));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 vec) {
    GLCall(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}
