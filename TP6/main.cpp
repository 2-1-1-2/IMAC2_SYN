
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "header/helper.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normal.fs.glsl"
    );
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    VBO vbo;
    vbo.bind();
    // target, taille du tableau en oct, pointeur vers les données, usage =>
    // dessin, les données ne changeront jamais
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    vbo.debind();

    VAO vao;
    vao.bind();

    /* activer les attributs */
    static constexpr GLuint aVertexPosition  = 0;
    static constexpr GLuint aVertexNormal    = 1;
    static constexpr GLuint aVertexTexCoords = 2;
    glEnableVertexAttribArray(aVertexPosition);
    glEnableVertexAttribArray(aVertexNormal);
    glEnableVertexAttribArray(aVertexTexCoords);

    vbo.bind();

    /* TODO: Voir si problème ici */
    glVertexAttribPointer(aVertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(aVertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(aVertexTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    vbo.debind();
    vao.debind();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        shader.use();

        GLint uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
        GLint uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
        GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

        std::cerr << "uMVPMatrix Location: " << uMVPMatrix << std::endl;
        std::cerr << "uMVMatrix Location: " << uMVMatrix << std::endl;
        std::cerr << "uNormalMatrix Location: " << uNormalMatrix << std::endl;

        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        vao.debind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}