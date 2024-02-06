#include "glimac/default_shader.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX2"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );
    /* initialisation vbo - Vertex Buffer Object - tableau */
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /*tableau de coordonnées*/
    GLfloat vertices[] = {
        -0.5f, -0.5f, 1.f, 0.f, 0.f, // Premier sommet
        0.5f, -0.5f, 0.f, 1.f, 0.f,  // Deuxième sommet
        0.0f, 0.5f, 0.f, 0.f, 1.f    // Troisième sommet
    };
    // target, taille du tableau en oct, pointeur vers les données, usage =>
    // dessin, les donnéesne changeront jamais
    glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    // debinder pour éviter de modifier par erreur
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* initialisation vao */
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // pas besoin de préciser GL_ARRAY... car il y a un unique vao

    /* activer les attributs */
    static constexpr GLuint vertex_attr_position = 3;
    static constexpr GLuint vertex_attr_color    = 8;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_color);
    /* comment lire les infos */

    /* on lie le vbo à GLARRAYBUFFER*/

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // index de l'attribut - nb de composant de l'attribut, type, normalized,
    // taille en oct, offset en oct

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));
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
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}