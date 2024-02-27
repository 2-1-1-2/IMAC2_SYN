#include <filesystem>
#include <type_traits>
#include <utility>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"
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