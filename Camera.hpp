#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace gps {

    enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN};

    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SENSITIVTY = 0.3f;


    class Camera
    {
    public:
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);
        glm::mat4 getViewMatrix();
        glm::vec3 getCameraTarget();
        void move(MOVE_DIRECTION direction, float speed);
        void rotate(float yaw, float pitch, bool constrainPitch = true);
        void actualizeazaPozitiaCamerei(int x, int y, int z);
     


        // Eular Angles
        float Yaw;
        float Pitch;

        // Camera options
        float MouseSensitivity;
     

    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;
    };

}

#endif /* Camera_hpp */