#include <iostream>
#include <algorithm>
#include <vector>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

//Linked files;
#include"Interfaces.h"
#include"getfilelocation.h"
#include"loadobj.h"
#include"shaderloader.h"
#include"object_class.h"
#include "texturecube.h"

using namespace glm;
using namespace std;

int main(int argc, char* argv[])
{
    if (!InitContext()) return -1;

    string path = exePath(false);

    //out / build / x64 - debug / quiz2.exe
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //change before run===============================================================
    string localdirectory = path;
    //=================================================================================

    string shaderPathPrefix = localdirectory + "assets/shaders/";

    GLuint shaderShadow = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");

    GLuint texturedShaderProgram = loadSHADER(shaderPathPrefix + "texturev.glsl", shaderPathPrefix + "texture.glsl");

    string ground = localdirectory + "assets/models/plane.obj";
    string house1 = localdirectory + "assets/models/p1/house1.obj";
    string house2 = localdirectory + "assets/models/p1/house2.obj";
    string house3 = localdirectory + "assets/models/p1/house3.obj";
    string leaves01 = localdirectory + "assets/models/p1/leaves01.obj";
    string leaves02 = localdirectory + "assets/models/p1/leaves02.obj";
    string leaves03 = localdirectory + "assets/models/p1/leaves03.obj";
    string leaves04 = localdirectory + "assets/models/p1/leaves04.obj";
    string tree1 = localdirectory + "assets/models/p1/tree1.obj";
    string tree2 = localdirectory + "assets/models/p1/tree2.obj";





    GLuint red = loadTexture(localdirectory + "assets/texture/red.png");
    GLuint lightmetal = loadTexture(localdirectory + "assets/texture/lightmetal.png");
    GLuint green = loadTexture(localdirectory + "assets/texture/green.png");
    GLuint sky = loadTexture(localdirectory + "assets/texture/sky.png");
    GLuint white = loadTexture(localdirectory + "assets/texture/white.png");
    const unsigned int DEPTH_MAP_TEXTURE_SIZE = WIDTH;
    glUseProgram(texturedShaderProgram);
    glUniform1i(glGetUniformLocation(texturedShaderProgram, "shadow_map"), 0);

    glUniform1i(glGetUniformLocation(texturedShaderProgram, "textureSampler"), 1);

    GLuint depth_map_texture;
    // Get the texture
   // glActiveTexture(GL_TEXTURE0 + 0);
    glGenTextures(1, &depth_map_texture);
    // Bind the texture so the next glTex calls affect it
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);
    // Create the texture and specify it's attributes, including widthn height, components (only depth is stored, no color information)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
        NULL);
    // Set texture sampler parameters.
    // The two calls below tell the texture sampler inside the shader how to upsample and downsample the texture. Here we choose the nearest filtering option, which means we just use the value of the closest pixel to the chosen image coordinate.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // The two calls below tell the texture sampler inside the shader how it should deal with texture coordinates outside of the [0, 1] range. Here we decide to just tile the image.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    GLuint depth_map_fbo;  // fbo: framebuffer object
   // Get the framebuffer
    glGenFramebuffers(1, &depth_map_fbo);
    // Bind the framebuffer so the next glFramebuffer calls affect it
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    // Attach the depth map texture to the depth map framebuffer
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_map_texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
    glDrawBuffer(GL_NONE); //disable rendering colors, only write depth values


    //==============================================================new entity==========================================//
    GLuint worldMatrixLocation = glGetUniformLocation(texturedShaderProgram, "worldMatrix");
    vector<mode1*> entitys;

    mode1* groungA = new mode1(EBO, ground, white, 1.0, 1.0, 1.0);
    groungA->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 1.0, 1.0, 1.0, 1.0);
    entitys.push_back(groungA);

    mode1* sphere = new mode1(EBO, localdirectory + "assets/models/sphere.obj", red, 1.0, 1.0, 1.0);
    sphere->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 1.0, 1.0, 1.0, 1.0);
    entitys.push_back(sphere);

    mode1* houseA = new mode1(EBO, house1, lightmetal, 1.0, 1.0, 1.0);
    houseA->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 1.0, 1.0, 1.0, 1.0);
    entitys.push_back(houseA);
    mode1* houseB = new mode1(EBO, house2, green, 1.0, 1.0, 1.0);
    houseB->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 1.0, 1.0, 1.0, 1.0);
    entitys.push_back(houseB);
    mode1* houseC = new mode1(EBO, house3, sky, 1.0, 1.0, 1.0);
    houseC->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 1.0, 1.0, 1.0, 1.0);
    entitys.push_back(houseC);

    mode1* leavesA = new mode1(EBO, leaves01, red, 1.0, 1.0, 1.0);
    leavesA->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 1.0, 1.0, 1.0, 1.0);
    entitys.push_back(leavesA);
    //==============================================================camera==============================================//
    // Camera parameters for view transform
    vec3 cameraPosition(6.0f, 15.0f, 40.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 5.0f;
    float cameraFastSpeed = 5 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;



    mat4 projectionMatrix = glm::perspective(70.0f,           // field of view in degrees
        WIDTH * 1.0f / HEIGHT, // aspect ratio
        0.01f, 800.0f);  // near and far (near > 0)

// Set initial view matrix on both shaders
                   // up


    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    float dt = glfwGetTime() - lastFrameTime;
    lastFrameTime += dt;
    double mousePosX = 0, mousePosY = 0;
    double lastMousePosX = 0, lastMousePosY = 0;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    mousePosX = lastMousePosX;
    double dx = mousePosX - lastMousePosX;
    double dy = mousePosY - lastMousePosY;

    lastMousePosX = mousePosX;
    lastMousePosY = mousePosY;

    // Convert to spherical coordinates
    const float cameraAngularSpeed = 60.0f;
    cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
    cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

    // Clamp vertical angle to [-85, 85] degrees
    cameraVerticalAngle = max(-85.0f, min(85.0f, cameraVerticalAngle));
    if (cameraHorizontalAngle > 360)
    {
        cameraHorizontalAngle -= 360;
    }
    else if (cameraHorizontalAngle < -360)
    {
        cameraHorizontalAngle += 360;
    }

    float theta = radians(cameraHorizontalAngle);
    float phi = radians(cameraVerticalAngle);

    cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

    float lightAngleOuter = 30.0;
    float lightAngleInner = 20.0;
    SetUniform1Value(texturedShaderProgram, "light_cutoff_inner", cos(radians(lightAngleInner)));
    SetUniform1Value(texturedShaderProgram, "light_cutoff_outer", cos(radians(lightAngleOuter)));

    SetUniformVec3(texturedShaderProgram, "light_color", vec3(1.0, 1.0, 1.0));

    // Set object color on scene shader
    SetUniformVec3(texturedShaderProgram, "object_color", vec3(1.0, 1.0, 1.0));

    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);


    float mode_X = 0.0f;
    float mode_Y = 0.0f;
    float mode_angle = 0.0f;


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    float ab = 0.4f;
    float di = 0.3f;
    float spec = 0.2f;

    bool spolt = true;
    bool camera = false;
    bool dj = false;
    bool change = false;
    bool change2 = false;
    vec3 lightPosition;
    vec3 lightFocus(0.0, 0.0, 0.0);
    const int mlen = 30;
    const int mwidth = 30;
    int map2[mlen][mwidth];

    //========================================================================loop===========================================================//
    while (!glfwWindowShouldClose(window)) {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime = glfwGetTime();
        mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        //cout << cameraLookAt.x << endl;

        if (spolt) {
            // light parameters
            lightPosition = //  vec3(0.6f,50.0f,5.0f); // the location of the light in 3D space
                vec3(0.0, 50.0, 10.0);
            lightFocus = vec3(.0, 0., 0.);
            ab = 0.6f;
            di = 0.1f;
            spec = 0.1f;
            SetUniformVec3(texturedShaderProgram, "light_color", vec3(1.0, 1.0, 1.0));
        }
        else if (camera) {
            lightPosition = //  vec3(0.6f,50.0f,5.0f); // the location of the light in 3D space
                cameraPosition + vec3(3.0, 10.0, 3.0);
            lightFocus = cameraLookAt * 200.0f;
            ab = 0.05f;
            di = 0.9f;
            spec = 0.4f;
            SetUniformVec3(texturedShaderProgram, "light_color", vec3(1.0, 1.0, 1.0));
        }
        else if (dj) {
            cout << static_cast<int>(fmod(glfwGetTime(), 2) + 0.5) << endl;
            if (static_cast<int>(fmod(glfwGetTime(), 2) + 0.5) == 0) {
                if (!change) {
                    float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 50;
                    float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 50;
                    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 1;
                    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 1;
                    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 1;
                    ab = 0.03f;
                    di = 0.6f;
                    spec = 1.0f;
                    SetUniformVec3(texturedShaderProgram, "light_color", vec3(r + 0.3, g + 0.3, b + 0.3));

                    lightPosition = vec3((x - 25) / 4, 50.f, (y - 25) / 4);
                    lightFocus = vec3((x - 25) / 2, x + y - 50, (y - 25) / 2);
                    change = true;
                }
            }
        }
        else {
            lightPosition = lightPosition;
        }
        if (static_cast<int>(fmod(glfwGetTime(), 2) + 0.5) == 1) {
            change = false;
        }

        vec3 lightDirection = normalize(lightFocus - lightPosition);

        float lightNearPlane = 1.0f;
        float lightFarPlane = 180.0f;

        mat4 lightProjectionMatrix = frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
        //perspective(20.0f, (float)DEPTH_MAP_TEXTURE_SIZE / (float)DEPTH_MAP_TEXTURE_SIZE, lightNearPlane, lightFarPlane);
        mat4 lightViewMatrix = lookAt(lightPosition, lightFocus, vec3(0.0f, 1.0f, 0.0f));
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        // Set light space matrix on both shaders
        SetUniformMat4(shaderShadow, "light_view_proj_matrix", lightSpaceMatrix);
        SetUniformMat4(texturedShaderProgram, "light_view_proj_matrix", lightSpaceMatrix);

        SetUniform1Value(texturedShaderProgram, "light_near_plane", lightNearPlane);
        SetUniform1Value(texturedShaderProgram, "light_far_plane", lightFarPlane);



        SetUniform1Value(texturedShaderProgram, "shading_ambient_strength", ab);
        SetUniform1Value(texturedShaderProgram, "shading_diffuse_strength", di);
        SetUniform1Value(texturedShaderProgram, "shading_specular_strength", spec);

        // Set light position on scene shader
        SetUniformVec3(texturedShaderProgram, "light_position", lightPosition);

        // Set light direction on scene shader
        SetUniformVec3(texturedShaderProgram, "light_direction", lightDirection);
        setViewMatrix(texturedShaderProgram, viewMatrix);
        setProjectionMatrix(texturedShaderProgram, projectionMatrix);
        setLightMatrix(texturedShaderProgram, lightSpaceMatrix);

        GLuint worldMatrixLocation = glGetUniformLocation(texturedShaderProgram, "worldMatrix");

        if (static_cast<int>(fmod(glfwGetTime(), 6) + 0.5) == 1) {
            if (!change2) {
                for (int i = 0; i < mlen; i++) {
                    for (int j = 0; j < mwidth; j++) {
                        map2[i][j] = rand() % 10;
                    }
                }
                for (int i = 0; i < mlen; i++) {
                    printf("{");
                    for (int j = 0; j < mwidth; j++) {
                        cout << map2[i][j] << ",";
                    }
                    cout << "}\n";
                }
                change2 = true;
            }
        }
        if (static_cast<int>(fmod(glfwGetTime(), 6) + 0.5) == 5) {
            change2 = false;
        }

        int map[8][8] = {
                        {1,0,1,0,1},
                        {3,0,1,0,0},
                        {2,2,1,2,2},
                        {0,0,1,4,0},
                        {1,1,1,1,1}

        };
        {
            glUseProgram(texturedShaderProgram);
            // Use proper image output size
            // Side note: we get the size from the framebuffer instead of using WIDTH and HEIGHT because of a bug with highDPI displays
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            // Bind screen as output framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Clear color and depth data on framebuffer
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //vector print

            entitys[0]->Setmode(worldMatrixLocation, mat4(1.0f), 100.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            entitys[0]->printmode();




            for (int i = 0; i < mlen; i++) {
                for (int j = 0; j < mwidth; j++) {


                    if (map2[i][j] == 1) {
                        entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 3.0f, -10.0f * mlen + j * 20.0f, 1.0f, -10.0f * mwidth + i * 20.0f, 0.0f);
                        entitys[2]->printmode();
                    }
                    if (map2[i][j] == 2) {
                        entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0f, -10.0f * mlen + j * 20.0f, 1.0f, -10.0f * mwidth + i * 20.0f, 0.0f);
                        entitys[3]->printmode();
                    }
                    if (map2[i][j] == 3) {
                        entitys[4]->Setmode(worldMatrixLocation, mat4(1.0f), 0.02f, -10.0f * mlen + j * 20.0f, 1.0f, -10.0f * mwidth + i * 20.0f, 0.0f);
                        entitys[4]->printmode();
                    }
                    if (map2[i][j] == 4) {
                        entitys[5]->Setmode(worldMatrixLocation, mat4(1.0f), 0.2f, -10.0f * mlen + j * 20.0f, -5.0f, -10.0f * mwidth + i * 20.0f, 0.0f);
                        entitys[5]->printmode();
                    }
                }
            }


        } {
            glUseProgram(shaderShadow);
            // Use proper image output size
            glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);
            // Bind depth map texture as output framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
            // Clear depth data on the framebuffer
            glClear(GL_DEPTH_BUFFER_BIT);
            // Bind geometry
            glBindTexture(GL_TEXTURE_2D, depth_map_texture);
            GLuint worldMatrixLocation = glGetUniformLocation(shaderShadow, "model_matrix");
            for (mode1* model : entitys) {
                model->changeProgram(worldMatrixLocation);
            }

            for (int i = 0; i < mlen; i++) {
                for (int j = 0; j < mwidth; j++) {


                    if (map[i][j] == 1) {
                        entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 3.0f, -10.0f * mlen + j * 20.0f, 1.0f, -10.0f * mwidth + i * 20.0f, 0.0f);
                        entitys[2]->printmodeshadow();
                    }
                    if (map[i][j] == 2) {
                        entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0f, -10.0f * mlen + j * 20.0f, 1.0f, -10.0f * mwidth + i * 20.0f, 0.0f);
                        entitys[3]->printmodeshadow();
                    }
                    if (map[i][j] == 3) {
                        entitys[4]->Setmode(worldMatrixLocation, mat4(1.0f), 0.02f, -10.0f * mlen + j * 20.0f, 1.0f, -10.0f * mwidth + i * 20.0f, 0.0f);
                        entitys[4]->printmodeshadow();
                    }
                    if (map[i][j] == 4) {
                        entitys[5]->Setmode(worldMatrixLocation, mat4(1.0f), 0.2f, -10.0f * mlen + j * 20.0f, -5.0f, -10.0f * mwidth + i * 20.0f, 0.0f);
                        entitys[5]->printmodeshadow();
                    }
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);






        //      - Update camera horizontal and vertical angle
        vec3 cameraSideVector = vec3(1.0f, 1.0f, 1.0f);

        // used

        float height = 20.f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move camera to the left
        {
            /*cameraX = -sin(getangle()) * radius;
            cameraY = -cos(getangle()) * radius;
            cameraPosition = vec3(cameraX, height + cameraZ, cameraY);
           viewMatrix = lookAt(cameraPosition, cameraPosition+cameraLookAt, vec3(0.0f, 1.0f, 0.0f));*/
            world_angleY -= 10.1f * dt;

        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move camera to the right
        {
            world_angleY += 10.1f * dt;

        }
        else

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera up

            {
                world_angleX -= 10.1f * dt;
                /* cameraX = -sin(getangle()) * radius;
                 cameraZ = -cos(getangle()) * radius;
                 cameraPosition = vec3(cameraX, height + cameraZ, cameraY);
                 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, vec3(0.0f, 1.0f, 0.0f));*/
            }
            else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera down
            {
                world_angleX += 10.1f * dt;
                /*   cameraX = -sin(getangleM()) * radius;
                   cameraZ = -cos(getangleM()) * radius;
                   cameraPosition = vec3(cameraX, height + cameraZ, cameraY);
                   viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, vec3(0.0f, 1.0f, 0.0f));*/
            }
            else {
                //  cameraPosition = vec3(cameraX, height + cameraZ, cameraY);

                //  viewMatrix = lookAt(cameraPosition, cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
            }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

            world_angleX = 0.0f;
            world_angleY = 0.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {


            world_angleX = 0.0f;
            world_angleY = 0.0f;
        }

        // if (glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT)) {

        glfwGetCursorPos(window, &mousePosX, &mousePosY);


        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            mode_angle -= 1.1f * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            mode_angle += 1.1f * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            mode_X += 4.4f * dt;
            cameraPosition += vec3(0.4f, 0.0f, 0.0f) * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            mode_X -= 4.4f * dt;
            cameraPosition -= vec3(0.4f, 0.0f, 0.0f) * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            mode_Y += 4.4f * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            mode_Y -= 4.4f * dt;
        }


        // } 
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
            spolt = true;
            camera = false;
            dj = false;
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
            spolt = false;
            camera = true;
            dj = false;
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            spolt = false;
            camera = false;
            dj = false;
        }
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
            spolt = false;
            camera = false;
            dj = true;
        }



        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) ==
            GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


        // - Calculate mouse motion dx and dy
        // - Update camera horizontal and vertical angle
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = max(-85.0f, min(85.0f, cameraVerticalAngle));

        theta = radians(cameraHorizontalAngle);
        phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);

        // Use camera lookat and side vectors to update positions with ASDW
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            cameraPosition += cameraLookAt * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            cameraPosition -= cameraLookAt * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            cameraPosition += cameraSideVector * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            cameraPosition -= cameraSideVector * dt * currentCameraSpeed;
        }
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;




    }

    glfwTerminate();

    return 0;
}