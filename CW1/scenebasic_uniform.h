#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "glm/glm.hpp"
#include "helper/plane.h"
#include "helper/objmesh.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
    
    Plane plane;
    std::unique_ptr<ObjMesh> mesh;
    GLuint programHandle;
    GLuint location;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float lastTime = 0.0f;
    float deltaTime;

    float pitch = 0.0f;
    float yaw = -90.0f;
    float sensitivity = 90.0f;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0, 1, 0);
    double lastX = width / 2.0;
    double lastY = height / 2.0;
    bool firstMouse = true;

    glm::vec4 topLightPos;
    bool movingPositive = true;
    int toggles[4] = { 1, 1, 1, 1 };
    glm::vec3 lightL[4] = { glm::vec3(0.0f, 0.0f, 0.5f),glm::vec3(0.0f, 0.5f, 0.0f),glm::vec3(0.5f, 0.0f, 0.0f),glm::vec3(0.4f, 0.4f, 0.4f) };

    glm::vec3 lightLa[4] = { glm::vec3(0.6f, 0.6f, 0.6f),glm::vec3(0.6f, 0.6f, 0.6f),glm::vec3(0.6f, 0.6f, 0.6f),glm::vec3(0.6f, 0.6f, 0.6f) };

    GLuint statueTexID, statueNormID;
    GLuint floorTexID, floorNormID;

    void compile();
    void setMatrices();
public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void toggleLight(int index) override;
    void handleInput(int key) override;
    void handleMouseInput(double mouseX, double mouseY) override;
};

#endif // SCENEBASIC_UNIFORM_H
