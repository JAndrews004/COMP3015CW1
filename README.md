#COMP3015 CW1

##Project Overview
This project is an OpenGL-based real-time rendering prototype developed as part of the COMP3015 module. It demonstrates the implementation of multiple shaders, material systems, lighting models, and scene management within a modern OpenGL pipeline.
The focus of this prototype is to explore rendering techniques, shader structure, and modular scene organisation rather than full engine architecture.

##Development Environment

#IDE: [Your Visual Studio Version]

Operating System: Windows 11 Pro
OpenGL Version: 4.6
Dependencies/Libraries Used: GLFW, GLAD, GLM stb_image

##How It Works
This project initialises an openGL context using GLFW and loads and compiles the GLSL shaders. Then it sets up the geometry buffers and loads textures including the cubemap used for the skybox. Then the scene is initialised with the material and light uniforms being set using multiple lights. The render loop is where the scene is updated including the keyboard and mouse movement. The materials are switched here to be applied to the different models. The shaders are bound and new uniforms bound then objects are rendered. Finally the skybox shaders are used as it requires no lighting so is simplified.
The view and projection matrices are updated in the SceneBasic_Uniform::handleMouseInput function. Animation of the overhead light is updated in the SceneBasic_Uniform::Update function and is toggled using the spacebar to start/stop animation.
##Code Structure and Navigation Guide
main.cpp - entry point of the application where the window is created and instantiates the scene class. Also handles the render loop.
Scene class (SceneBasic_Uniform) - Initialises the geometry with some helper functions such as objmesh.cpp and plane.cpp. Also compiles, links and uses the various shaders and configures lights and materials. It is also responsible for updating and rendering the scene.
Helper utilities - Used for texture loading, model loading, some error checking for the shaders and OpenGL debugging.

##Rendering Approach
For rendering I  started with the more basic Phong model then changed to Blinn-Phong using attenuation and multiple lights. A struct is used for the lights to make this easier. A material struct is also used to simplify this with Ka, Kd, Ks and shininess. Multiple shaders are used as some models or textures didnt require lighting or the other techniques used. The other techniques used were texture mixing, normal mapping, a mask with varying material properties and gamma correction. This was not needed on the skybox or the graffiti mechanic.
The graffiti mechanic works by adjusting the alpha based on how much lighting it would recieve, alpha discarding was also used to reduce artifacts. A togglable fog is also implemented to showcase multiple techniques
##Controls
1,2,3,4 - enable and disable associated lights
0 - fog toggle
Space bar - play/ pause animation of light
F - Character 'torch' to find graffiti on statue.
WASD - camera movement
Mouse - look around

##Video
