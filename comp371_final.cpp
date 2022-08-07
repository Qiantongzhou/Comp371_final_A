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
#include <list>
using namespace glm;
using namespace std;



int main(int argc, char* argv[])
{



    //==============================================================user specified parameters==========================================//
    bool IsWinter = true;
    //Season ; ture = Winter theme; false = Summer theme


    int seed = 123412465;
    //the seed for world generation. determines the rotation & position of trees, item & house orders

    int roadLength = 20;
    int roadWidth = 18;
    //The lengh & width , for the road generation

    int SideForestLength = 8;
    //The length of the forest on the left & right

    //==============================================================user specified parameters==========================================//
    if (!InitContext()) return -1;
    string path = exePath(false);
    string localdirectory = path;
    string shaderPathPrefix = localdirectory + "assets/shaders/";
    string TexturePath = path + "assets/texture/";
    if (!false) {
        TexturePath = path + "assets/texture/Summer/";
    }
    GLuint sky = loadTexture(localdirectory + "assets/texture/sky.png");
    string h1Path = localdirectory + "assets/models/h1.obj";
    GLuint h1T = loadTexture(TexturePath + "h1.png");
    string h2Path = localdirectory + "assets/models/h2.obj";
    GLuint h2T = loadTexture(TexturePath + "h2.png");
    string h3Path = localdirectory + "assets/models/h3.obj";
    GLuint h3T = loadTexture(TexturePath + "h3.png");
    string h4Path = localdirectory + "assets/models/h4.obj";
    GLuint h4T = loadTexture(TexturePath + "h4.png");
    string h5Path = localdirectory + "assets/models/h5.obj";
    GLuint h5T = loadTexture(TexturePath + "h5.png");
    string h6Path = localdirectory + "assets/models/h6.obj";
    GLuint h6T = loadTexture(TexturePath + "h6.png");
    string h7Path = localdirectory + "assets/models/h7.obj";
    GLuint h7T = loadTexture(TexturePath + "h7.png");
    string h8Path = localdirectory + "assets/models/h8.obj";
    GLuint h8T = loadTexture(TexturePath + "h8.png");
    string h9Path = localdirectory + "assets/models/h9.obj";
    GLuint h9T = loadTexture(TexturePath + "h9.png");

    string ground_1_P = localdirectory + "assets/models/snowground1.obj";
    GLuint ground_1_T = loadTexture(TexturePath + "ground1.png");
    GLuint ground_2_T = loadTexture(TexturePath + "ground2.png");
    GLuint grass_1_T = loadTexture(TexturePath + "grass1.png");
    GLuint grass_2_T = loadTexture(TexturePath + "grass2.png");


    string lamp_P = localdirectory + "assets/models/lamp.obj";
    GLuint lamp_T = loadTexture(TexturePath + "lamp.png");

    string wall_P = localdirectory + "assets/models/stone_fence.obj";
    GLuint wall_T = loadTexture(TexturePath + "wall.png");

    string tree1_P = localdirectory + "assets/models/tree1.obj";
    GLuint tree1_T = loadTexture(TexturePath + "tree1.png");
    string tree2_P = localdirectory + "assets/models/tree2.obj";
    GLuint tree2_T = loadTexture(TexturePath + "tree2.png");

    string fence_P = localdirectory + "assets/models/fence.obj";
    GLuint fence_T = loadTexture(TexturePath + "fence.png");

    string barrel_P = localdirectory + "assets/models/barrel.obj";
    GLuint barrel_T = loadTexture(TexturePath + "barrel.png");

    string logs_P = localdirectory + "assets/models/logs.obj";
    GLuint logs_T = loadTexture(TexturePath + "logs.png");

    string rocks_P = localdirectory + "assets/models/rocks.obj";
    GLuint rocks_T = loadTexture(TexturePath + "rocks.png");

    string well_P = localdirectory + "assets/models/well.obj";
    GLuint well_T = loadTexture(TexturePath + "well.png");

    string pot_P = localdirectory + "assets/models/pot.obj";
    GLuint pot_T = loadTexture(TexturePath + "pot.png");

    string Boxes_P = localdirectory + "assets/models/marketBox.obj";
    GLuint Boxes_T = loadTexture(TexturePath + "Boxes.png");

    string littleGrass_P = localdirectory + "assets/models/grass.obj";
    GLuint littleGrass_T = loadTexture(TexturePath + "grass.png");


    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    GLuint shaderShadow = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");
    GLuint texturedShaderProgram = loadSHADER(shaderPathPrefix + "texturev.glsl", shaderPathPrefix + "texture.glsl");

    const unsigned int DEPTH_MAP_TEXTURE_SIZE = WIDTH;
    glUseProgram(texturedShaderProgram);
    glUniform1i(glGetUniformLocation(texturedShaderProgram, "shadow_map"), 0);

    glUniform1i(glGetUniformLocation(texturedShaderProgram, "textureSampler"), 1);

    GLuint depth_map_texture;
    glGenTextures(1, &depth_map_texture);
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
        NULL);
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


    //==============================================================new entity ==========================================//
    GLuint worldMatrixLocation = glGetUniformLocation(texturedShaderProgram, "worldMatrix");
    vector<mode1*> entitys;

    mode1* road1 = new mode1(EBO, ground_1_P, ground_1_T, 1.0, 1.0, 1.0);
    road1->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(road1);
    mode1* road2 = new mode1(EBO, ground_1_P, ground_2_T, 1.0, 1.0, 1.0);
    road2->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(road2);
    mode1* road3 = new mode1(EBO, ground_1_P, grass_1_T, 1.0, 1.0, 1.0);
    road3->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(road3);
    mode1* road4 = new mode1(EBO, ground_1_P, grass_2_T, 1.0, 1.0, 1.0);
    road4->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(road4);

    mode1* model_h1 = new mode1(EBO, h1Path, h1T, 1.0, 1.0, 1.0);
    model_h1->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h1);
    mode1* model_h2 = new mode1(EBO, h2Path, h2T, 1.0, 1.0, 1.0);
    model_h2->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h2);
    mode1* model_h3 = new mode1(EBO, h3Path, h3T, 1.0, 1.0, 1.0);
    model_h3->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h3);
    mode1* model_h4 = new mode1(EBO, h4Path, h4T, 1.0, 1.0, 1.0);
    model_h4->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h4);
    mode1* model_h5 = new mode1(EBO, h5Path, h5T, 1.0, 1.0, 1.0);
    model_h5->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h5);
    mode1* model_h6 = new mode1(EBO, h6Path, h6T, 1.0, 1.0, 1.0);
    model_h6->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h6);
    mode1* model_h7 = new mode1(EBO, h7Path, h7T, 1.0, 1.0, 1.0);
    model_h7->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h7);
    mode1* model_h8 = new mode1(EBO, h8Path, h8T, 1.0, 1.0, 1.0);
    model_h8->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h8);
    mode1* model_h9 = new mode1(EBO, h9Path, h9T, 1.0, 1.0, 1.0);
    model_h9->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(model_h9);


    mode1* lamp = new mode1(EBO, lamp_P, lamp_T, 1.0, 1.0, 1.0);
    lamp->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(lamp);

    mode1* wall = new mode1(EBO, wall_P, wall_T, 0.12, 0.12, 1.0);
    wall->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(wall);

    mode1* tree1 = new mode1(EBO, tree1_P, tree1_T, 1.0, 1.0, 1.0);
    tree1->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(tree1);

    mode1* tree2 = new mode1(EBO, tree2_P, tree2_T, 1.0, 1.0, 1.0);
    tree2->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(tree2);


    mode1* boxes = new mode1(EBO, Boxes_P, Boxes_T, 1.0, 1.0, 1.0);
    boxes->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(boxes);

    mode1* rocks = new mode1(EBO, rocks_P, rocks_T, 1.0, 1.0, 1.0);
    rocks->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(rocks);

    mode1* barrel = new mode1(EBO, barrel_P, barrel_T, 1.0, 1.0, 1.0);
    barrel->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(barrel);

    mode1* littleGrass = new mode1(EBO, littleGrass_P, littleGrass_T, 1.0, 1.0, 1.0);
    littleGrass->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(littleGrass);

    mode1* logs = new mode1(EBO, logs_P, logs_T, 1.0, 1.0, 1.0);
    logs->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(logs);

    mode1* well = new mode1(EBO, well_P, well_T, 1.0, 1.0, 1.0);
    well->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(well);

    mode1* pot = new mode1(EBO, pot_P, pot_T, 1.0, 1.0, 1.0);
    pot->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0, 0.0, 0.0, 0.0);
    entitys.push_back(pot);

    GLuint texturedCubeVAO = createTexturedCubeVertexArrayObject();

    //==============================================================camera==============================================//
    // Camera parameters for view transform
    vec3 cameraPosition(6.0f, 15.0f, 40.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 25.0f;
    float cameraFastSpeed = 5 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;

    mat4 projectionMatrix = glm::perspective(70.0f,           // field of view in degrees
        WIDTH * 1.0f / HEIGHT, // aspect ratio
        0.01f, 800.0f);  // near and far (near > 0)

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
    const int mlen = 1;
    const int mwidth = 1;
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

            srand(seed);
            //set the seed
            if (cameraPosition.y < 1.0) {
                cameraPosition.y = 1.0;
            }
            if (cameraPosition.y > 200.0) {
                cameraPosition.y = 200.0;
            }
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindVertexArray(texturedCubeVAO);
            glDisable(GL_CULL_FACE);
            glBindTexture(GL_TEXTURE_2D, sky);
            mat4 sky =  translate(mat4(1.0f), cameraPosition) * scale(mat4(1.0f), vec3(200.f, 200.f, 200.f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &sky[0][0]);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            //glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);

            float offset = 6.6 * (roadLength);
            //genereate the start & end of the town
            for (int i = 0; i < 3; i++) {
                entitys[1]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -6.6, -10.0, -6.6 + (i * 6.6), 0.0);
                
                entitys[1]->printmode();
                entitys[1]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + offset, -10.0, -6.6 + (i * 6.6), 0.0);
               
                entitys[1]->printmode();
            }
            for (int i = 0; i < 1 + (roadWidth); i++) {
                entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -6.6, -10.0, 13.2 + (i * 6.6), 0.0);
              
                entitys[2]->printmode();
                entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -6.6, -10.0, -13.2 - (i * 6.6), 0.0);
               
                entitys[2]->printmode();

                entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 15.0, -10.6, -10.0, -13.2 - (i * 6.6), randomRotation());
               
                entitys[20]->printmode();
                entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 15.0, -10.6, -10.0, 13.2 + (i * 6.6), randomRotation());
               
                entitys[20]->printmode();

                entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + offset, -10.0, 13.2 + (i * 6.6), 0.0);
                entitys[2]->printmode();
                entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + offset, -10.0, -13.2 - (i * 6.6), 0.0);
                entitys[2]->printmode();

                entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 15.0, -3.3 + offset, -10.0, -13.2 - (i * 6.6), randomRotation());
                entitys[20]->printmode();
                entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 15.0, -3.3 + offset, -10.0, 13.2 + (i * 6.6), randomRotation());
                entitys[20]->printmode();
            }
            int totalRoadWidth = 5 + 2 * (roadWidth);
            for (int i = 0; i < totalRoadWidth; i++) {

                entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -13.2, -10.0, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 6.6), 0.0);
                entitys[3]->printmode();

                entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 6.6 + offset, -10.0, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 6.6), 0.0);
                entitys[3]->printmode();
            }
            for (int i = 0; i < totalRoadWidth; i++) {

                for (int j = 0; j < SideForestLength - 1; j++) {
                    entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -19.8 - (j * 6.6), -10.0, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 6.6), 0.0);
                    entitys[3]->printmode();
                    entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 13.2 + offset + (j * 6.6), -10.0, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 6.6), 0.0);
                    entitys[3]->printmode();
                }
            }
            //generate forest
            for (int i = 0; i < totalRoadWidth / 1; i++) {
                int type = (rand() % (3 - 1)) + 1;
                int treeposition = (rand() % (3 - 1)) + 1;
                if (type == 1) {
                    entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 16.0, -14.2, -10.4, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 1 * 6.6), randomRotation());
                    entitys[15]->checkPosition(cameraPosition);
                    entitys[15]->printmode();

                    entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 16.0, 2.0 + offset, -10.4, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 1 * 6.6), randomRotation());
                    entitys[15]->checkPosition(cameraPosition);
                    entitys[15]->printmode();
                }
                else {
                    entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 16.0, -14.2, -10.4, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 1 * 6.6), randomRotation());
                    entitys[16]->checkPosition(cameraPosition);
                    entitys[16]->printmode();

                    entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 16.0, 2.0 + offset, -10.4, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 1 * 6.6), randomRotation());
                    entitys[16]->checkPosition(cameraPosition);
                    entitys[16]->printmode();
                }
            }
            for (int i = 0; i < totalRoadWidth / 2; i++) {
                for (int j = 0; j < (SideForestLength - 1) / 2; j++) {
                    int type = (rand() % (3 - 1)) + 1;
                    if (type == 1) {
                        entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, -26.8 - (j * 2 * 6.6), -10.4, (6.6 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 2 * 6.6), randomRotation());
                        entitys[15]->printmode();

                        entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 13.2 + offset + (j * 2 * 6.6), -10.4, (6.6 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 2 * 6.6), randomRotation());
                        entitys[15]->printmode();
                    }
                    else {
                        entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, -26.8 - (j * 2 * 6.6), -10.4, (6.6 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 2 * 6.6), randomRotation());
                        entitys[16]->printmode();

                        entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 13.2 + offset + (j * 2 * 6.6), -10.4, (6.6 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 2 * 6.6), randomRotation());
                        entitys[16]->printmode();
                    }
                }
            }


            //generate Grounds(Town road)
            for (int i = 0; i < roadLength; i++) {
                for (int j = 0; j < 1; j++) {
                    entitys[0]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, 0.0 + (j * 6.6), 0.0);
                    entitys[0]->printmode();
                }
            }
            for (int i = 0; i < roadLength; i++) {
                entitys[1]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, -6.6, 0.0);
                entitys[1]->printmode();
                entitys[1]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, 6.6, 0.0);
                entitys[1]->printmode();
            }
            for (int i = 0; i < roadLength; i++) {
                entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, -13.2, 0.0);
                entitys[2]->printmode();
                entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, 13.2, 0.0);
                entitys[2]->printmode();
            }
            for (int i = 0; i < roadLength; i++) {
                for (int j = 0; j < roadWidth; j++) {
                    entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, -19.8 - (j * 6.6), 0.0);
                    entitys[3]->printmode();
                    entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, 19.8 + (j * 6.6), 0.0);
                    entitys[3]->printmode();
                }
            }

            //generate houses
            {
                int previsousHouseType = -1;
                int previsous2HouseType = -1;
                int previsous3HouseType = -1;
                for (int i = 0; i < roadLength / 2; i++) {
                    bool notSame = false;
                    int houseType = -1;
                    while (!notSame) {
                        houseType = (rand() % (10 - 1)) + 1;
                        if (houseType != previsousHouseType && houseType != previsous2HouseType && houseType != previsous3HouseType) {
                            previsous3HouseType = previsous2HouseType;
                            previsous2HouseType = previsousHouseType;
                            previsousHouseType = houseType;
                            notSame = true;
                        }
                    }
                    //generate random from type 1-9
                    switch (houseType) {
                    case 1: {
                        entitys[4]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11.2, 3.15);
                        entitys[4]->checkPosition(cameraPosition);
                        entitys[4]->printmode();
                        break;
                    }
                    case 2: {
                        entitys[5]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11.67, 1.57);
                        entitys[5]->checkPosition(cameraPosition);
                        entitys[5]->printmode();
                        break;
                    }
                    case 3: {
                        entitys[6]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -12.8, 3.15);
                        entitys[6]->checkPosition(cameraPosition);
                        entitys[6]->printmode();

                        entitys[22]->Setmode(worldMatrixLocation, mat4(1.0f), 7.0, -2.0 + (6.6 * 2 * i), -10.0, -8.6, 3.15);
                        entitys[22]->checkPosition(cameraPosition);
                        entitys[22]->printmode();
                        //well
                        break;
                    }
                    case 4: {
                        entitys[7]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11, 1.57);
                        entitys[7]->checkPosition(cameraPosition); entitys[6]->checkPosition(cameraPosition);
                        entitys[7]->printmode();
                        break;
                    }
                    case 5: {
                        entitys[8]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11.7, 1.57);
                        entitys[8]->checkPosition(cameraPosition);
                        entitys[8]->printmode();
                        break;
                    }
                    case 6: {
                        entitys[9]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -13.0, 0.0);
                        entitys[9]->checkPosition(cameraPosition);
                        entitys[9]->printmode();
                        break;
                    }
                    case 7: {
                        entitys[10]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 4.0 + (6.6 * 2 * i), -10.0, -12.85, 0.0);
                        entitys[10]->checkPosition(cameraPosition);
                        entitys[10]->printmode();
                        break;
                    }
                    case 8: {
                        entitys[11]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -14.8, 0.0);
                        entitys[11]->checkPosition(cameraPosition);
                        entitys[11]->printmode();

                        entitys[23]->Setmode(worldMatrixLocation, mat4(1.0f), 7.0, -3.7 + (6.6 * 2 * i), -10.0, -14.6, 3.15);
                        entitys[23]->checkPosition(cameraPosition);
                        entitys[23]->printmode();
                        break;
                    }
                    case 9: {
                        entitys[12]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11.9, 4.72);
                        entitys[12]->checkPosition(cameraPosition);
                        entitys[12]->printmode();
                        break;
                    }


                    }
                }


                //generate road lamps
                for (int i = 0; i < roadLength / 4; i++) {
                    entitys[13]->Setmode(worldMatrixLocation, mat4(1.0f), 1.7, 0.0 + (i * 4 * 6.6), -10.0, 10.8, 4.72);
                    entitys[13]->printmode();
                }
                //generate walls
                for (int i = 0; i < roadLength / 5; i++) {
                    entitys[14]->Setmode(worldMatrixLocation, mat4(1.0f), 7.0, 7.0 + (i * 5 * 6.6), -10.0, 12.4, 4.72);
                    entitys[14]->printmode();
                }

                //generate large trees
                for (int i = 0; i < roadLength / 2; i++) {
                    int type = (rand() % (3 - 1)) + 1;
                    if (type == 1) {
                        entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, 27.4, randomRotation());
                        entitys[15]->printmode();
                    }
                    else {
                        entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, 27.4, randomRotation());
                        entitys[16]->printmode();
                    }
                }
                int extraTreeNumbers = (roadWidth - 2) / 2;
                if (extraTreeNumbers >= 1) {
                    for (int i = 0; i < roadLength / 2; i++) {
                        for (int j = 0; j < extraTreeNumbers; j++) {
                            int type = (rand() % (3 - 1)) + 1;
                            if (type == 1) {
                                entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, 40.6 + (j * 2 * 6.6), randomRotation());
                                entitys[15]->printmode();
                            }
                            else {
                                entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, 40.6 + (j * 2 * 6.6), randomRotation());
                                entitys[16]->printmode();
                            }
                            int type2 = (rand() % (3 - 1)) + 1;
                            if (type2 == 1) {
                                entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, -25.6 - (j * 2 * 6.6), randomRotation());
                                entitys[15]->printmode();
                            }
                            else {
                                entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, -25.6 - (j * 2 * 6.6), randomRotation());
                                entitys[16]->printmode();
                            }
                        }
                    }
                }

                //generate small trees
                for (int i = 0; i < roadLength / 1; i++) {
                    int type = (rand() % (3 - 1)) + 1;
                    int treeposition = (rand() % (3 - 1)) + 1;
                    if (type == 1) {
                        entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 14.0, -3.3 + (i * 1 * 6.6), -10.4, (treeposition == 1 ? 20.4 : 15.4), randomRotation());
                        entitys[15]->printmode();
                    }
                    else {
                        entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 14.0, -3.3 + (i * 1 * 6.6), -10.4, (treeposition == 1 ? 20.4 : 15.4), randomRotation());
                        entitys[16]->printmode();
                    }
                }


                //generate items(on the road)
                int previsousItemType = -1;
                int previsous2ItemType = -1;
                for (int i = 0; i < roadLength / 3; i++) {
                    //Avoid duplicate items
                    bool notSame = false;
                    int ItemType = -1;
                    while (!notSame) {
                        ItemType = (rand() % (6 - 1)) + 1;
                        if (ItemType != previsousItemType && ItemType != previsous2ItemType) {
                            previsous2ItemType = previsousItemType;
                            previsousItemType = ItemType;
                            notSame = true;
                        }
                    }
                    switch (ItemType) {
                    case 1: {
                        entitys[17]->Setmode(worldMatrixLocation, mat4(1.0f), 0.7, 7.0 + (i * 3 * 6.6), -10.0, -5.4, 1.55);
                        entitys[17]->printmode();
                        break;
                    }
                    case 2: {
                        entitys[18]->Setmode(worldMatrixLocation, mat4(1.0f), 7.0, 7.0 + (i * 3 * 6.6), -10.0, -5.4, randomRotation());
                        entitys[18]->printmode();
                        break;
                    }
                    case 3: {
                        entitys[19]->Setmode(worldMatrixLocation, mat4(1.0f), 5.0, 7.0 + (i * 3 * 6.6), -10.0, -6.4, randomRotation());
                        entitys[19]->printmode();
                        break;
                    }
                    case 4: {
                        entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 5.0, 7.0 + (i * 3 * 6.6), -10.0, -6.4, randomRotation());
                        entitys[20]->printmode();
                        break;
                    }
                    case 5: {
                        entitys[21]->Setmode(worldMatrixLocation, mat4(1.0f), 5.0, 7.0 + (i * 3 * 6.6), -10.0, -6.4, randomRotation());
                        entitys[21]->printmode();
                        break;
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


                srand(seed);
                //set the seed


                float offset = 6.6 * (roadLength);
                //genereate the start & end of the town
                for (int i = 0; i < 3; i++) {
                    entitys[1]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -6.6, -10.0, -6.6 + (i * 6.6), 0.0);
                    entitys[1]->printmodeshadow();
                    entitys[1]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + offset, -10.0, -6.6 + (i * 6.6), 0.0);
                    entitys[1]->printmodeshadow();
                }
                for (int i = 0; i < 1 + (roadWidth); i++) {
                    entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -6.6, -10.0, 13.2 + (i * 6.6), 0.0);
                    entitys[2]->printmodeshadow();
                    entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -6.6, -10.0, -13.2 - (i * 6.6), 0.0);
                    entitys[2]->printmodeshadow();

                    entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 15.0, -10.6, -10.0, -13.2 - (i * 6.6), randomRotation());
                    entitys[20]->printmodeshadow();
                    entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 15.0, -10.6, -10.0, 13.2 + (i * 6.6), randomRotation());
                    entitys[20]->printmodeshadow();

                    entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + offset, -10.0, 13.2 + (i * 6.6), 0.0);
                    entitys[2]->printmodeshadow();
                    entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + offset, -10.0, -13.2 - (i * 6.6), 0.0);
                    entitys[2]->printmodeshadow();

                    entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 15.0, -3.3 + offset, -10.0, -13.2 - (i * 6.6), randomRotation());
                    entitys[20]->printmodeshadow();
                    entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 15.0, -3.3 + offset, -10.0, 13.2 + (i * 6.6), randomRotation());
                    entitys[20]->printmodeshadow();
                }
                int totalRoadWidth = 5 + 2 * (roadWidth);
                for (int i = 0; i < totalRoadWidth; i++) {

                    entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -13.2, -10.0, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 6.6), 0.0);
                    entitys[3]->printmodeshadow();

                    entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 6.6 + offset, -10.0, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 6.6), 0.0);
                    entitys[3]->printmodeshadow();
                }
                for (int i = 0; i < totalRoadWidth; i++) {

                    for (int j = 0; j < SideForestLength - 1; j++) {
                        entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, -19.8 - (j * 6.6), -10.0, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 6.6), 0.0);
                        entitys[3]->printmodeshadow();
                        entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 13.2 + offset + (j * 6.6), -10.0, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 6.6), 0.0);
                        entitys[3]->printmodeshadow();
                    }
                }
                //generate forest
                for (int i = 0; i < totalRoadWidth / 1; i++) {
                    int type = (rand() % (3 - 1)) + 1;
                    int treeposition = (rand() % (3 - 1)) + 1;
                    if (type == 1) {
                        entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 16.0, -14.2, -10.4, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 1 * 6.6), randomRotation());
                        entitys[15]->printmodeshadow();

                        entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 16.0, 2.0 + offset, -10.4, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 1 * 6.6), randomRotation());
                        entitys[15]->printmodeshadow();
                    }
                    else {
                        entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 16.0, -14.2, -10.4, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 1 * 6.6), randomRotation());
                        entitys[16]->printmodeshadow();

                        entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 16.0, 2.0 + offset, -10.4, (0 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 1 * 6.6), randomRotation());
                        entitys[16]->printmodeshadow();
                    }
                }
                for (int i = 0; i < totalRoadWidth / 2; i++) {
                    for (int j = 0; j < (SideForestLength - 1) / 2; j++) {
                        int type = (rand() % (3 - 1)) + 1;
                        if (type == 1) {
                            entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, -26.8 - (j * 2 * 6.6), -10.4, (6.6 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 2 * 6.6), randomRotation());
                            entitys[15]->printmodeshadow();

                            entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 13.2 + offset + (j * 2 * 6.6), -10.4, (6.6 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 2 * 6.6), randomRotation());
                            entitys[15]->printmodeshadow();
                        }
                        else {
                            entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, -26.8 - (j * 2 * 6.6), -10.4, (6.6 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 2 * 6.6), randomRotation());
                            entitys[16]->printmodeshadow();

                            entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 13.2 + offset + (j * 2 * 6.6), -10.4, (6.6 - (6.6 * (totalRoadWidth - 1)) / 2) + (i * 2 * 6.6), randomRotation());
                            entitys[16]->printmodeshadow();
                        }
                    }
                }


                //generate Grounds(Town road)
                for (int i = 0; i < roadLength; i++) {
                    for (int j = 0; j < 1; j++) {
                        entitys[0]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, 0.0 + (j * 6.6), 0.0);
                        entitys[0]->printmodeshadow();
                    }
                }
                for (int i = 0; i < roadLength; i++) {
                    entitys[1]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, -6.6, 0.0);
                    entitys[1]->printmodeshadow();
                    entitys[1]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, 6.6, 0.0);
                    entitys[1]->printmodeshadow();
                }
                for (int i = 0; i < roadLength; i++) {
                    entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, -13.2, 0.0);
                    entitys[2]->printmodeshadow();
                    entitys[2]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, 13.2, 0.0);
                    entitys[2]->printmodeshadow();
                }
                for (int i = 0; i < roadLength; i++) {
                    for (int j = 0; j < roadWidth; j++) {
                        entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, -19.8 - (j * 6.6), 0.0);
                        entitys[3]->printmodeshadow();
                        entitys[3]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (i * 6.6), -10.0, 19.8 + (j * 6.6), 0.0);
                        entitys[3]->printmodeshadow();
                    }
                }

                //generate houses
                {
                    int previsousHouseType = -1;
                    int previsous2HouseType = -1;
                    int previsous3HouseType = -1;
                    for (int i = 0; i < roadLength / 2; i++) {
                        bool notSame = false;
                        int houseType = -1;
                        while (!notSame) {
                            houseType = (rand() % (10 - 1)) + 1;
                            if (houseType != previsousHouseType && houseType != previsous2HouseType && houseType != previsous3HouseType) {
                                previsous3HouseType = previsous2HouseType;
                                previsous2HouseType = previsousHouseType;
                                previsousHouseType = houseType;
                                notSame = true;
                            }
                        }
                        //generate random from type 1-9
                        switch (houseType) {
                        case 1: {
                            entitys[4]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11.2, 3.15);
                            entitys[4]->printmodeshadow();
                            break;
                        }
                        case 2: {
                            entitys[5]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11.67, 1.57);
                            entitys[5]->printmodeshadow();
                            break;
                        }
                        case 3: {
                            entitys[6]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -12.8, 3.15);
                            entitys[6]->printmodeshadow();

                            entitys[22]->Setmode(worldMatrixLocation, mat4(1.0f), 7.0, -2.0 + (6.6 * 2 * i), -10.0, -8.6, 3.15);
                            entitys[22]->printmodeshadow();
                            //well
                            break;
                        }
                        case 4: {
                            entitys[7]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11, 1.57);
                            entitys[7]->printmodeshadow();
                            break;
                        }
                        case 5: {
                            entitys[8]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11.7, 1.57);
                            entitys[8]->printmodeshadow();
                            break;
                        }
                        case 6: {
                            entitys[9]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -13.0, 0.0);
                            entitys[9]->printmodeshadow();
                            break;
                        }
                        case 7: {
                            entitys[10]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 4.0 + (6.6 * 2 * i), -10.0, -12.85, 0.0);
                            entitys[10]->printmodeshadow();
                            break;
                        }
                        case 8: {
                            entitys[11]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -14.8, 0.0);
                            entitys[11]->printmodeshadow();

                            entitys[23]->Setmode(worldMatrixLocation, mat4(1.0f), 7.0, -3.7 + (6.6 * 2 * i), -10.0, -14.6, 3.15);
                            entitys[23]->printmodeshadow();
                            break;
                        }
                        case 9: {
                            entitys[12]->Setmode(worldMatrixLocation, mat4(1.0f), 1.0, 0.0 + (6.6 * 2 * i), -10.0, -11.9, 4.72);
                            entitys[12]->printmodeshadow();
                            break;
                        }


                        }
                    }


                    //generate road lamps
                    for (int i = 0; i < roadLength / 4; i++) {
                        entitys[13]->Setmode(worldMatrixLocation, mat4(1.0f), 1.7, 0.0 + (i * 4 * 6.6), -10.0, 10.8, 4.72);
                        entitys[13]->printmodeshadow();
                    }
                    //generate walls
                    for (int i = 0; i < roadLength / 5; i++) {
                        entitys[14]->Setmode(worldMatrixLocation, mat4(1.0f), 7.0, 7.0 + (i * 5 * 6.6), -10.0, 12.4, 4.72);
                        entitys[14]->printmodeshadow();
                    }

                    //generate large trees
                    for (int i = 0; i < roadLength / 2; i++) {
                        int type = (rand() % (3 - 1)) + 1;
                        if (type == 1) {
                            entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, 27.4, randomRotation());
                            entitys[15]->printmodeshadow();
                        }
                        else {
                            entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, 27.4, randomRotation());
                            entitys[16]->printmodeshadow();
                        }
                    }
                    int extraTreeNumbers = (roadWidth - 2) / 2;
                    if (extraTreeNumbers >= 1) {
                        for (int i = 0; i < roadLength / 2; i++) {
                            for (int j = 0; j < extraTreeNumbers; j++) {
                                int type = (rand() % (3 - 1)) + 1;
                                if (type == 1) {
                                    entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, 40.6 + (j * 2 * 6.6), randomRotation());
                                    entitys[15]->printmodeshadow();
                                }
                                else {
                                    entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, 40.6 + (j * 2 * 6.6), randomRotation());
                                    entitys[16]->printmodeshadow();
                                }
                                int type2 = (rand() % (3 - 1)) + 1;
                                if (type2 == 1) {
                                    entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, -25.6 - (j * 2 * 6.6), randomRotation());
                                    entitys[15]->printmodeshadow();
                                }
                                else {
                                    entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 28.0, 0.0 + (i * 2 * 6.6), -10.4, -25.6 - (j * 2 * 6.6), randomRotation());
                                    entitys[16]->printmodeshadow();
                                }
                            }
                        }
                    }

                    //generate small trees
                    for (int i = 0; i < roadLength / 1; i++) {
                        int type = (rand() % (3 - 1)) + 1;
                        int treeposition = (rand() % (3 - 1)) + 1;
                        if (type == 1) {
                            entitys[15]->Setmode(worldMatrixLocation, mat4(1.0f), 14.0, -3.3 + (i * 1 * 6.6), -10.4, (treeposition == 1 ? 20.4 : 15.4), randomRotation());
                            entitys[15]->printmodeshadow();
                        }
                        else {
                            entitys[16]->Setmode(worldMatrixLocation, mat4(1.0f), 14.0, -3.3 + (i * 1 * 6.6), -10.4, (treeposition == 1 ? 20.4 : 15.4), randomRotation());
                            entitys[16]->printmodeshadow();
                        }
                    }


                    //generate items(on the road)
                    int previsousItemType = -1;
                    int previsous2ItemType = -1;
                    for (int i = 0; i < roadLength / 3; i++) {
                        //Avoid duplicate items
                        bool notSame = false;
                        int ItemType = -1;
                        while (!notSame) {
                            ItemType = (rand() % (6 - 1)) + 1;
                            if (ItemType != previsousItemType && ItemType != previsous2ItemType) {
                                previsous2ItemType = previsousItemType;
                                previsousItemType = ItemType;
                                notSame = true;
                            }
                        }
                        switch (ItemType) {
                        case 1: {
                            entitys[17]->Setmode(worldMatrixLocation, mat4(1.0f), 0.7, 7.0 + (i * 3 * 6.6), -10.0, -5.4, 1.55);
                            entitys[17]->printmodeshadow();
                            break;
                        }
                        case 2: {
                            entitys[18]->Setmode(worldMatrixLocation, mat4(1.0f), 7.0, 7.0 + (i * 3 * 6.6), -10.0, -5.4, randomRotation());
                            entitys[18]->printmodeshadow();
                            break;
                        }
                        case 3: {
                            entitys[19]->Setmode(worldMatrixLocation, mat4(1.0f), 5.0, 7.0 + (i * 3 * 6.6), -10.0, -6.4, randomRotation());
                            entitys[19]->printmodeshadow();
                            break;
                        }
                        case 4: {
                            entitys[20]->Setmode(worldMatrixLocation, mat4(1.0f), 5.0, 7.0 + (i * 3 * 6.6), -10.0, -6.4, randomRotation());
                            entitys[20]->printmodeshadow();
                            break;
                        }
                        case 5: {
                            entitys[21]->Setmode(worldMatrixLocation, mat4(1.0f), 5.0, 7.0 + (i * 3 * 6.6), -10.0, -6.4, randomRotation());
                            entitys[21]->printmodeshadow();
                            break;
                        }

                        }
                    }
                }

            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
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


        //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //    mode_angle -= 1.1f * dt;
        //}
        //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //    mode_angle += 1.1f * dt;
        //}
        //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //    mode_X += 4.4f * dt;
        //    cameraPosition += vec3(0.4f, 0.0f, 0.0f) * dt;
        //}
        //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //    mode_X -= 4.4f * dt;
        //    cameraPosition -= vec3(0.4f, 0.0f, 0.0f) * dt;
        //}
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
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPosition += cameraLookAt * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPosition -= cameraLookAt * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPosition += cameraSideVector * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPosition -= cameraSideVector * dt * currentCameraSpeed;
        }
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;




    }

    glfwTerminate();

    return 0;
}