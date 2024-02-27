#include <filesystem>
#include <type_traits>
#include <utility>
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

/* ---------- VBO ---------- */
class VBO {
public:
    VBO() { glGenBuffers(1, &_id); }

    ~VBO() { glDeleteBuffers(1, &_id); }

    VBO(const VBO&)            = delete;
    VBO& operator=(const VBO&) = delete;
    VBO(VBO&& other) noexcept // Move constructor
        : _id{other._id}
    {
        other._id = 0; // Make sure that other won't delete the _id we just copied
    }
    VBO& operator=(VBO&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                             // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteBuffers(1, &_id); // Delete the previous object
            _id       = other._id;    // Copy the object
            other._id = 0;            // Make sure that other won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
    }

    void debind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    GLuint _id{};
};

/* ---------- VAO ---------- */

class VAO {
public:
    VAO() { glGenVertexArrays(1, &_id); }

    ~VAO() { glDeleteVertexArrays(1, &_id); }

    VAO(const VAO&)            = delete;
    VAO& operator=(const VAO&) = delete;
    VAO(VAO&& other) noexcept
        : _id{other._id} { other._id = 0; } // Move constructor
    VAO& operator=(VAO&& other) noexcept    // Move assignment operator
    {
        if (this != &other)
        {                             // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteBuffers(1, &_id); // Delete the previous object
            _id       = other._id;    // Copy the object
            other._id = 0;            // Make sure that other won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    void bind() const { glBindVertexArray(_id); } // pas besoin de préciser GL_ARRAY... car il y a un unique vao

    void debind() const { glBindVertexArray(0); }

private:
    GLuint _id{};
};

/* ---------- TEXTURE ----------*/

class Texture {
    img::Image   image;
    GLuint       _id{};
    unsigned int _width;
    unsigned int _height;

public:
    explicit Texture(const std::filesystem::path& path)
        : image(p6::load_image_buffer(path)), _width(image.width()), _height(image.height())
    {
        glGenTextures(1, &_id);
    }

    ~Texture()
    {
        glDeleteTextures(1, &_id);
    }

    void bind() const { glBindTexture(GL_TEXTURE_2D, _id); }

    void debind() const { glBindTexture(GL_TEXTURE_2D, 0); }

    unsigned int   height() const { return _height; }
    unsigned int   width() const { return _width; } //
    const uint8_t* data() const { return image.data(); }
};

int main()
{
    auto ctx = p6::Context{{720, 720, "TP4 EX2"}};
    ctx.maximize_window();

    /*****************************************************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *****************************************************************/
    const p6::Shader shader = p6::load_shader(
        "shaders/tex2D.vs.glsl",
        "shaders/tex2D.fs.glsl"
    );
    /* initialisation vbo - Vertex Buffer Object - tableau */

    VBO vbo;
    vbo.bind();

    /*tableau de coordonnées*/
    /*****************************************************************
     *      NE PAS OUBLIER DE CHANGER LES COORD POUR LA TEXTURE      *
     *****************************************************************/
    std::vector<Vertex2DUV> vertices;
    vertices.emplace_back(glm::vec2(-1, -1), glm::vec2(0, 0));
    vertices.emplace_back(glm::vec2(1, -1), glm::vec2(1, 0));
    vertices.emplace_back(glm::vec2(0, 1), glm::vec2(0.5, 1));

    // target, taille du tableau en oct, pointeur vers les données, usage =>
    // dessin, les donnéesne changeront jamais
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2DUV), vertices.data(), GL_STATIC_DRAW);
    // debinder pour éviter de modifier par erreur
    vbo.debind();

    /* initialisation ibo - Index Buffer Object - tableau */
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    std::vector<uint32_t> indice = {0, 1, 2};

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice.size() * sizeof(uint32_t), indice.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* initialisation vao */
    VAO vao;
    vao.bind();

    // ON BIND L'IBO COMME LE VAO EST ACTUELLEMENT BINDE
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    /* activer les attributs */
    static constexpr GLuint vertex_attr_position = 0;
    static constexpr GLuint vertex_attr_texture  = 1;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_texture);

    /* comment lire les infos */

    /* on lie le vbo à GLARRAYBUFFER*/
    vbo.bind();
    // index de l'attribut - nb de composant de l'attribut, type, normalized,
    // taille en oct, offset en oct

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, position)));

    glVertexAttribPointer(vertex_attr_texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)(offsetof(Vertex2DUV, texture)));

    vbo.debind(); // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // de bind vao
    vao.debind();

    // Declare your infinite update loop.
    float degree = 0;

    /* img::Image _triforce = p6::load_image_buffer("assets/textures/triforce.png"); */
    Texture triforce("assets/textures/triforce.png");
    triforce.bind();

    std::cout << "height : " << triforce.height() << " width : " << triforce.width() << "\n";

    // target, level, internal format, width, height, border, format, type, data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, triforce.width(), triforce.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, triforce.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::cout << "test2\n";
    triforce.debind();

    std::cout << "test3\n";
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);
        vao.bind();
        shader.use();
        // GLint uTime = glGetUniformLocation(shader.id(), "uTime");
        // glUniform1f(uTime, degree);

        GLint uMatrix  = glGetUniformLocation(shader.id(), "uModelMatrix");
        GLint uColor   = glGetUniformLocation(shader.id(), "uColor");
        GLint uTexture = glGetUniformLocation(shader.id(), "uTexture");

        std::cerr << "uMatrix Location: " << uMatrix << std::endl;
        std::cerr << "uColor Location: " << uColor << std::endl;
        std::cerr << "uTexture Location: " << uTexture << std::endl;

        /* glUniform3fv(uColor, 1, glm::value_ptr(glm::vec3(0.1))); */
        std::vector<glm::mat3> transformations = {
            rotateZ(degree) * scale(0.25, 0.25) * translate(-2, 2) * rotateZ(degree),
            rotateZ(degree) * scale(0.25, 0.25) * translate(-2, -2) * rotateZ(degree),
            rotateZ(degree) * scale(0.25, 0.25) * translate(2, 2) * rotateZ(degree),
            rotateZ(degree) * scale(0.25, 0.25) * translate(2, -2) * rotateZ(degree)
        };

        for (auto& transformation : transformations)
        {
            glUniformMatrix3fv(uMatrix, 1, GL_FALSE, glm::value_ptr(transformation));
            triforce.bind(); /*
             glBindTexture(GL_TEXTURE_2D, tex); */
            glUniform1i(uTexture, 0);
            glDrawElements(GL_TRIANGLES, indice.size(), GL_UNSIGNED_INT, 0);
            triforce.debind();
        }

        vao.debind();
        degree += 1;
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
