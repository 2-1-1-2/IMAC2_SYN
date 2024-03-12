#pragma once
#include <string>
#include <vector>
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
// #include "glfw/src/internal.h"

class ShaderProgram {
protected:
    p6::Shader m_Program;

    GLint              uMVPMatrix;
    GLint              uMVMatrix;
    GLint              uNormalMatrix;
    std::vector<GLint> uTextures;

public:
    ShaderProgram(const std::filesystem::path& vs, const std::filesystem::path& fs)
        : m_Program{p6::load_shader(vs, fs)}, uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix"))
    {
        // uTextures.push_back(glGetUniformLocation(m_Program.id(), nomVardansGLSL));

        // std::cout << "dans cette fonction\n";
        // addTextures();
    }

    virtual void addTextures() = 0;

    void addTexture(const std::string tex)
    {
        std::cout << "dans addTextures sans s\n";
        std::cout << tex << "\n";
        uTextures.push_back(glGetUniformLocation(m_Program.id(), tex.c_str()));
    }

    void use()
    {
        m_Program.use();
    }

    void uniformText()
    {
        for (int i = 0; i < uTextures.size(); i++)
        {
            glUniform1i(uTextures.at(i), i);
        }
    }

    void transformation(glm::mat4 ProjMatrix, const glm::mat4 earthMVMatrix)
    {
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(earthMVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * earthMVMatrix));
    }
};

class EarthProgram : public ShaderProgram {
public:
    EarthProgram(const std::filesystem::path& vs, const std::filesystem::path& fs)
        : ShaderProgram(vs, fs)
    {
        addTextures();
    };
    void addTextures() override
    {
        std::cout << "dans addTextures avec s\n";
        addTexture("uEarthTexture");
        addTexture("uCloudTexture");
    }
};

class MoonProgram : public ShaderProgram {
public:
    MoonProgram(const std::filesystem::path& vs, const std::filesystem::path& fs)
        : ShaderProgram(vs, fs)
    {
        addTextures();
    };
    void addTextures() override
    {
        std::cout << "dans addTextures avec s\n";
        addTexture("uTexture");
    }
};