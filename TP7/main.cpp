
#include <iostream>
#include <vector>
#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "header/helper.hpp"
#include "header/shaderProgram.hpp"
#include "p6/p6.h"
int main()
{
    auto ctx = p6::Context{{1280, 720, "TP6 SPHERE"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    // EarthProgram             earthProg{};
    std::vector<std::string> uText;
    uText.emplace_back("uEarthTexture");
    EarthProgram    earthProgram("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl");
    MoonProgram     moonProgram("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl");
    TrackballCamera tbc;
    /*"", ""*/
    const std::vector<glimac::ShapeVertex>
        vertices = glimac::sphere_vertices(1.f, 32, 16);

    VBO vbo;
    vbo.bind();
    vbo.bufferData(vertices);
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
    std::vector<glm::vec3> rotationAxes;
    for (unsigned int i = 0; i < 32; i++)
    {
        rotationAxes.push_back(glm::sphericalRand(2.f));
    }

    Texture earth("assets/textures/EarthMap.jpg");

    Texture moon("assets/textures/MoonMap.jpg");

    Texture cloud("assets/textures/CloudMap.jpg");

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        const glm::mat4 globalMVMatrix = tbc.getViewMatrix();                            /* glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f}); */
        /* glm::mat4       globalMVMatrix = glm::translate(VMatrix, {0.f, 0.f, 0.f}); */ /*glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f})*/
        vao.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        // earthProg.m_Program.use();
        earthProgram.use();

        earthProgram.uniformText();

        const glm::mat4 earthMVMatrix = glm::rotate(globalMVMatrix, ctx.time(), {0.f, 5.f, 0.f});

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        earthProgram.transformation(ProjMatrix, earthMVMatrix, tbc.getViewMatrix());

        earth.activeTexture();
        cloud.activeTexture(1);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // debind
        earth.deactiveTexture();
        cloud.deactiveTexture(1);

        // débind sur l'unité GL_TEXTURE0

        moonProgram.use();
        for (unsigned int i = 0; i < 32; i++)
        {
            moonProgram.uniformText();
            /* MOON MATRIX - TRANSFORMATION*/
            glm::mat4 moonMVMatrix = glm::rotate(globalMVMatrix, ctx.time() * (rotationAxes[i].x + rotationAxes[i].y + rotationAxes[i].z), {0.f, 1.f, 0.f});
            // Translation * Rotation
            moonMVMatrix = glm::translate(moonMVMatrix, rotationAxes[i]); // Translation * Rotation * Translation
            moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3{0.2f});

            moonProgram.transformation(ProjMatrix, moonMVMatrix, tbc.getViewMatrix());
            moon.activeTexture();
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            moon.deactiveTexture();

            // Fix the typo here
        }
        vao.debind();
    };
    ctx.mouse_scrolled = [&tbc](p6::MouseScroll mouseScroll) {
        tbc.moveFront(mouseScroll.dy);
    };
    ctx.mouse_dragged = [&tbc](p6::MouseDrag mouseMove) {
        tbc.rotateUp(mouseMove.position.x);
        tbc.rotateLeft(mouseMove.position.y);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}