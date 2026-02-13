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

    glm::vec4 topLightPos;
    bool movingPositive = true;
    int toggles[4] = { 1, 1, 1, 1 };
    glm::vec3 lightL[4] = { glm::vec3(0.0f, 0.0f, 0.8f),glm::vec3(0.0f, 0.8f, 0.0f),glm::vec3(0.8f, 0.0f, 0.0f),glm::vec3(0.25f, 0.25f, 0.25f) };

    glm::vec3 lightLa[4] = { glm::vec3(0.4f, 0.4f, 0.4f),glm::vec3(0.4f, 0.4f, 0.4f),glm::vec3(0.4f, 0.4f, 0.4f),glm::vec3(0.4f, 0.4f, 0.4f) };
    void compile();
    void setMatrices();
public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void toggleLight(int index) override;

    
};

#endif // SCENEBASIC_UNIFORM_H
