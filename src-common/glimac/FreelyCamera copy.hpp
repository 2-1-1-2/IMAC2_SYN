
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"
class FreeflyCamera {
    float     m_Phi;
    float     m_Theta;
    glm::vec3 m_Position;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors()
    {
        m_FrontVector = {glm::cos(m_Theta) * glm::cos(m_Phi), glm::sin(m_Theta), glm::cos(m_Theta) * glm::cos(m_Phi)};
        m_LeftVector  = {glm::sin(m_Phi + glm::pi<float>() / 2.), 0, glm::cos(m_Phi + glm::pi<float>() / 2.)};
        m_UpVector    = m_LeftVector * m_FrontVector;
    }

public:
    FreeflyCamera(float distance = 5.0f, float angleX = 0.0f, float angleY = 0.0f)
        : m_Distance(distance), m_AngleX(angleX), m_AngleY(angleY)
    {
    }
    void rotateLeft(float degrees)
    {
        m_fPhi = m_fPhi + radians(degrees);
        computeDirectionVectors();
    }

    void rotateUp(float degrees)
    {
        m_fTheta = m_fTheta + radians(degrees);
        computeDirectionVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        // glm::vec3 V = glm::vec3(m_Position.x + m_FrontVector.x, m_Position.y + m_FrontVector.y, m_Position.z + m_FrontVector.z);
        return lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    }
};