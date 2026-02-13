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
using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f,10.0f,100,100) 
{
    mesh = ObjMesh::load("media/Statue.obj", true);
}



void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    
    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(20.0f), 1.0f, 0.3f, 100.0f);
    
    float x, z;

    for (int i = 0; i < 3; i++) {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);

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
    
    prog.setUniform("lights[3].Position", view * topLightPos);

    prog.setUniform("Material.Kd", glm::vec3(0.4f, 0.4f, 0.4f));
    prog.setUniform("Material.Ka", glm::vec3(0.5f, 0.5f, 0.5f));
    prog.setUniform("Material.Ks", glm::vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Shininess", 180.0f);
    
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
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-1.0f, 0.2f, -0.6f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    mesh->render();

    prog.setUniform("Material.Kd", glm::vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Ka", glm::vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Ks", glm::vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model,vec3(0.0f,-0.45f,0.0f));

    setMatrices();
    plane.render();
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
    if (index >= 0 && index < 4) {
        toggles[index] = !toggles[index];
    }
}
