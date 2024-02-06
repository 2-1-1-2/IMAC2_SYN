#include "glimac/default_shader.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    /* initialisation vbo - Vertex Buffer Object - tableau */
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /*tableau de coordonnées*/
    GLfloat vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};

    // target, taille du tableau en oct, pointeur vers les données, usage =>
    // dessin, les donnéesne changeront jamais
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    // debinder pour éviter de modifier par erreur
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* initialisation vao */
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // pas besoin de préciser GL_ARRAY... car il y a un unique vao

    /* activer les attributs */
    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);
    /* comment lire les infos */

    /* on lie le vbo à GLARRAYBUFFER*/

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // index de l'attribut - nb de composant de l'attribut, type, normalized,
    // taille en oct, offset en oct

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

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
        glimac::bind_default_shader();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}