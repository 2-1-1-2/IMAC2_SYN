#include <type_traits>
#include "glimac/default_shader.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
    Vertex2DColor(glm::vec2 vp, glm::vec3 vc)
        : position(vp), color(vc) {}
};

struct Vertex2DUV {
    glm::vec2 position;
    glm::vec2 texture;
    Vertex2DUV();
    Vertex2DUV(glm::vec2 vp, glm::vec2 vc)
        : position(vp), texture(vc) {}
};

glm::mat3 translate(const float tx, const float ty)
{
    glm::mat3 M = glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
    return M;
}

glm::mat3 scale(const float sx, const float sy)
{
    glm::mat3 M = glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
    return M;
}

glm::mat3 rotateZ(const float alpha)
{
    float     a = glm::radians(alpha);
    glm::mat3 M = glm::mat3(glm::vec3(glm::cos(a), glm::sin(a), 0), glm::vec3(-glm::sin(a), glm::cos(a), 0), glm::vec3(0, 0, 1));
    return M;
}

int main()
{
    auto ctx = p6::Context{{720, 720, "TP4 EX2"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    const p6::Shader shader = p6::load_shader(
        "shaders/tex2D.vs.glsl",
        "shaders/tex2D.fs.glsl"
    );
    /* initialisation vbo - Vertex Buffer Object - tableau */
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /*tableau de coordonnées*/
    std::vector<Vertex2DUV> vertices;
    vertices.emplace_back(glm::vec2(-1, -1), glm::vec2(0, 0));
    vertices.emplace_back(glm::vec2(1, -1), glm::vec2(0, 0));
    vertices.emplace_back(glm::vec2(0, 1), glm::vec2(0, 0));

    // target, taille du tableau en oct, pointeur vers les données, usage =>
    // dessin, les donnéesne changeront jamais
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2DUV), vertices.data(), GL_STATIC_DRAW);
    // debinder pour éviter de modifier par erreur
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* initialisation ibo - Index Buffer Object - tableau */
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    std::vector<uint32_t> indice = {0, 1, 2};

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice.size() * sizeof(uint32_t), indice.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* initialisation vao */
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // pas besoin de préciser GL_ARRAY... car il y a un unique vao
    // ON BIND L'IBO COMME LE VAO EST ACTUELLEMENT BINDE
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    /* activer les attributs */
    static constexpr GLuint vertex_attr_position = 0;
    static constexpr GLuint vertex_attr_texture  = 1;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_texture);

    /* comment lire les infos */

    /* on lie le vbo à GLARRAYBUFFER*/

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // index de l'attribut - nb de composant de l'attribut, type, normalized,
    // taille en oct, offset en oct

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, position)));

    glVertexAttribPointer(vertex_attr_texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, texture)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // de bind vao
    glBindVertexArray(0);

    // Declare your infinite update loop.
    float degree = 0;
    ctx.update   = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        shader.use();
        // GLint uTime = glGetUniformLocation(shader.id(), "uTime");
        // glUniform1f(uTime, degree);

        GLint uModelMatrix = glGetUniformLocation(shader.id(), "uModelMatrix");
        GLint uColor       = glGetUniformLocation(shader.id(), "uColor");
        glUniform3fv(uColor, 1, glm::value_ptr(glm::vec3(0.1)));
        glm::mat3 M  = rotateZ(degree);
        glm::mat3 mM = rotateZ(-degree);
        glm::mat3 M1 = translate(-0.5, 0.5);
        glm::mat3 S  = scale(0.25, 0.25);

        glUniformMatrix3fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(M * M1 * S * mM));
        glDrawElements(GL_TRIANGLES, indice.size(), GL_UNSIGNED_INT, 0);

        glm::mat3 M2 = translate(-0.5, -0.5);
        glUniformMatrix3fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(M * M2 * S * mM));
        glDrawElements(GL_TRIANGLES, indice.size(), GL_UNSIGNED_INT, 0);

        glm::mat3 M3 = translate(0.5, 0.5);
        glUniformMatrix3fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(M * M3 * S * M));
        glDrawElements(GL_TRIANGLES, indice.size(), GL_UNSIGNED_INT, 0);

        glm::mat3 M4 = translate(0.5, -0.5);
        glUniformMatrix3fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(M * M4 * S * M));
        glDrawElements(GL_TRIANGLES, indice.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        degree += 1;
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}