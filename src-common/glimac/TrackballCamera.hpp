#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"
class TrackballCamera {
    float m_Distance;
    float m_AngleX;
    float m_AngleY;

public:
    TrackballCamera(const float& distance = 5.0f, const float& angleX = 0.0f, const float& angleY = 0.0f)
        : m_Distance(distance), m_AngleX(angleX), m_AngleY(angleY)
    {
    }
    void      moveFront(const float& delta) { m_Distance += delta; }
    void      rotateLeft(const float& degrees) { m_AngleX += degrees; }
    void      rotateUp(const float& degrees) { m_AngleY += degrees; }
    glm::mat4 getViewMatrix() const
    {
        glm::mat4 cameraMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -m_Distance));
        cameraMatrix           = glm::rotate(cameraMatrix, glm::radians(m_AngleX), glm::vec3(1, 0, 0));
        cameraMatrix           = glm::rotate(cameraMatrix, glm::radians(m_AngleY), glm::vec3(0, 1, 0));
        /* glm::mat4 cameraMatrix = glm::translate(glm::mat4{m_Distance}, {0.f, 0.f, -5.f}) * glm::rotate(glm::radians(), const vec<3, T, Q> &v);
        ; */
        return cameraMatrix;
    }
};