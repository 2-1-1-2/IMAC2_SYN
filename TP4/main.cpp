#include "glimac/default_shader.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
    Vertex2DColor(glm::vec2 vp, glm::vec3 vc)
        : position(vp), color(vc) {}
};

int main()
{
    auto ctx = p6::Context{{720, 720, "TP3 EX2"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    const p6::Shader shader = p6::load_shader(
        "shaders/color2D.vs.glsl",
        "shaders/grey2D.fs.glsl"
    );
    /* initialisation vbo - Vertex Buffer Object - tableau */
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /*tableau de coordonnées*/
    Vertex2DColor vertices[] = {
        Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
        Vertex2DColor(glm::vec2(-0.5, 0.5), glm::vec3(0, 1, 0)),
        Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0, 0, 1)),
        Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
        Vertex2DColor(glm::vec2(0.5, 0.5), glm::vec3(0, 0, 1)),
        Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0))
    };

    // target, taille du tableau en oct, pointeur vers les données, usage =>
    // dessin, les donnéesne changeront jamais
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);
    // debinder pour éviter de modifier par erreur
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* initialisation vao */
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // pas besoin de préciser GL_ARRAY... car il y a un unique vao

    /* activer les attributs */
    static constexpr GLuint vertex_attr_position = 0;
    static constexpr GLuint vertex_attr_color    = 1;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_color);
    /* comment lire les infos */

    /* on lie le vbo à GLARRAYBUFFER*/

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // index de l'attribut - nb de composant de l'attribut, type, normalized,
    // taille en oct, offset en oct

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, position)));

    glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, color)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // de bind vao
    glBindVertexArray(0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        shader.use();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}