#include "glimac/default_shader.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
    Vertex2DColor();
    Vertex2DColor(glm::vec2 vp, glm::vec3 vc)
        : position(vp), color(vc) {}
};

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

    static constexpr unsigned int nbTriangle = 64;
    static constexpr float        r          = 0.5;
    static constexpr float        theta      = 2. * glm::pi<float>() / nbTriangle;

    /*tableau de coordonnées*/
    std::vector<Vertex2DColor> vertices;
    for (unsigned int i = 0; i < nbTriangle; i++)
    {
        vertices.emplace_back(glm::vec2(0., 0.), glm::vec3(1, 1, 1));
        vertices.emplace_back(glm::vec2(r * glm::cos(theta * i), r * glm::sin(theta * i)), glm::vec3(1, 1, 1));
        vertices.emplace_back(glm::vec2(r * glm::cos(theta * (i + 1)), r * glm::sin(theta * (i + 1))), glm::vec3(1, 1, 1));
    }

    // target, taille du tableau en oct, pointeur vers les données, usage =>
    // dessin, les donnéesne changeront jamais
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);
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
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}