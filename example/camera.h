#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

class Camera{
    public:
        Camera() = default;

        // Camera attributes
        glm::vec3 Position{1.0f};
        float Fov = 45.0f;
        glm::vec2 Sensitivity{0.1f};
        float dis = 2.0f;

        // Returns view matrix
        glm::mat4 getView(){
    return glm::lookAt(m_Front * dis, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
        // Returns perspective matrix
        glm::mat4 getPerspective(){
    return glm::perspective(Fov, 800.0f/600.0f, 0.1f, 100.0f);
}

    void update(float xOffset, float yOffset){
    m_Yaw += xOffset * Sensitivity.x;
    m_Pitch += yOffset * Sensitivity.y;
    if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

    // calculating look direction
    glm::vec3 Front;    
    Front.x = cos(glm::radians(m_Yaw));
    Front.y = sin(glm::radians(m_Pitch));
    Front.z = sin(glm::radians(m_Yaw));

    // scales between 1 and 0
    m_Front = glm::normalize(Front);
    m_Front = Front;
    //m_Front.x += 0.001;
}
    private:
        glm::vec3 m_Front{1.0f, 0.0f, 0.0f}; // Look direction
        float m_Pitch = 45.0f;
        float m_Yaw = 0.0f;
};
