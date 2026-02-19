#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include <glm/glm.hpp>
#include <sstream>
#include "helper/texture.h"
#include <GLFW/glfw3.h>
using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f,10.0f,100,100) 
{
    mesh = ObjMesh::load("media/Statue.obj", true,true);  
}



void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    
    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(70.0f), 1.0f, 0.10f, 100.0f);
    
    float x, z;

    for (int i = 0; i < 3; i++) {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 1.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 1.0f * sinf((glm::two_pi<float>() / 3) * i);

        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));

    }

    prog.setUniform("lights[3].Position", view * glm::vec4(0, 1.2f, 0, 1.0f));
    topLightPos = glm::vec4(0, 1.2f, 0, 1.0f);

    prog.setUniform("lights[0].L", lightL[0]);
    prog.setUniform("lights[0].La", lightLa[0]);
   
    prog.setUniform("lights[1].L", lightL[1]);
    prog.setUniform("lights[1].La", lightLa[1]);
   
    prog.setUniform("lights[2].L", lightL[2]);
    prog.setUniform("lights[2].La", lightLa[2]);
    
    prog.setUniform("lights[3].L", lightL[3]);
    prog.setUniform("lights[3].La", lightLa[3]);
    
    statueTexID = Texture::loadTexture("media/Gold.jpg");
    statueNormID = Texture::loadTexture("media/Gold_NormalMap.jpg");
    blankMaskID = Texture::loadTexture("media/BlankMask.png");
    
    floorTexID = Texture::loadTexture("media/Floor.jpg");
    floorNormID = Texture::loadTexture("media/Floor_NormalMap.jpg");
    mossTexID = Texture::loadTexture("media/Moss.png");
    puddleMaskID = Texture::loadTexture("media/PuddleMask.png");
    

    prog.setUniform("Tex1", 0);
    prog.setUniform("NormalMapTex", 1); 
    prog.setUniform("Tex2", 2);
    prog.setUniform("puddleMask", 3);

    prog.setUniform("Spot.L", lightL[4]);
    prog.setUniform("Spot.La", lightLa[4]);
    prog.setUniform("Spot.Exponent", 25.0f);
    prog.setUniform("Spot.Cutoff", glm::radians(30.0f));
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    if (topLightPos.x > 3.0f) {
        
        movingPositive = false;
    }
    else if (topLightPos.x < -5.0f) {
        
        movingPositive = true;
    }

    if (movingPositive) {
        topLightPos += glm::vec4(0.001f, 0.0f, 0.0f, 0.0f);
    }
    else {
        topLightPos -= glm::vec4(0.001f, 0.0f, 0.0f, 0.0f);
    }
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    
    view = glm::lookAt(position, position + front, up);

    glm::vec4 spotPosView = view * glm::vec4(position, 1.0f);
    glm::vec3 spotDirView = glm::mat3(view) * front;

    prog.setUniform("Spot.Position", spotPosView);
    prog.setUniform("Spot.Direction", glm::normalize(spotDirView));

    prog.setUniform("lights[3].Position", view * topLightPos);

    prog.setUniform("Material.Kd", glm::vec3(0.8f, 0.65f, 0.2f));
    prog.setUniform("Material.Ka", glm::vec3(0.1f, 0.07f, 0.02f));
    prog.setUniform("Material.Ks", glm::vec3(1.0f, 0.85f, 0.4f));
    prog.setUniform("Material.Shininess", 128.0f);
    
    for (int i = 0; i < 4; i++) {
        if (toggles[i] ==0) {
            std::stringstream nameL;
            nameL << "lights[" << i << "].L";
            prog.setUniform(nameL.str().c_str(), glm::vec3(0.0f, 0.0f, 0.0f));
            std::stringstream nameLa;
            nameLa << "lights[" << i << "].La";
            prog.setUniform(nameLa.str().c_str(), glm::vec3(0.0f, 0.0f, 0.0f));
        }
        else {
            std::stringstream nameL;
            nameL << "lights[" << i << "].L";
            prog.setUniform(nameL.str().c_str(), lightL[i]);
            std::stringstream nameLa;
            nameLa << "lights[" << i << "].La";
            prog.setUniform(nameLa.str().c_str(), lightLa[i]);
        }
    }
    if (toggles[4] == 0) {
        prog.setUniform("Spot.L", glm::vec3(0.0f, 0.0f, 0.0f));
        prog.setUniform("Spot.La", glm::vec3(0.0f, 0.0f, 0.0f));
    }
    else {
        prog.setUniform("Spot.L", lightL[4]);
        prog.setUniform("Spot.La", lightLa[4]);
    }

    model = mat4(1.0f);
    model = glm::translate(model, vec3(-1.0f, 0.2f, -0.6f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, statueTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, statueNormID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, statueTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, blankMaskID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);


    mesh->render();

    prog.setUniform("Material.Kd", glm::vec3(0.5f, 0.5f, 0.5f));
    prog.setUniform("Material.Ka", glm::vec3(0.1f, 0.07f, 0.02f));
    prog.setUniform("Material.Ks", glm::vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Shininess", 0.3f);

    model = mat4(1.0f);
    model = glm::translate(model,vec3(0.0f,-0.45f,0.0f));

    setMatrices();

   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, floorNormID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mossTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, puddleMaskID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);

    plane.render();

    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);

}

void SceneBasic_Uniform::setMatrices() {
    mat4 mv =view * model;

    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP",  projection* mv);

}

void SceneBasic_Uniform::toggleLight(int index) {
    if (index >= 0 && index < 5) {
        toggles[index] = !toggles[index];
    }
}
void SceneBasic_Uniform::handleInput(int key) {
    float vel = 0.15f * deltaTime;

    if (key == GLFW_KEY_W)
        position += front * vel;
    if (key == GLFW_KEY_S)
        position -= front * vel;
    if (key == GLFW_KEY_A)
        position -= glm::normalize(glm::cross(front, up)) * vel;
    if (key == GLFW_KEY_D)
        position += glm::normalize(glm::cross(front, up)) * vel;

    
}
void SceneBasic_Uniform::handleMouseInput(double mouseX, double mouseY) {
    if (firstMouse) {
        lastX = mouseX;
        lastY = mouseY;
        firstMouse = false;
    }

    double offsetX = mouseX - lastX;
    double offsetY = lastY - mouseY;

    lastX = mouseX;
    lastY = mouseY;

    yaw += offsetX * sensitivity * deltaTime;
    pitch += offsetY * sensitivity * deltaTime;

    
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
}
