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
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    //change before run===============================================================
    string localdirectory = path;
    //=================================================================================

    string shaderPathPrefix = localdirectory + "assets/shaders/";

    GLuint shaderShadow = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");

    GLuint texturedShaderProgram = loadSHADER(shaderPathPrefix + "texturev.glsl", shaderPathPrefix + "texture.glsl");


    //Setup models
    //string heraclesPath = "assets/models/sphereb.obj";
    string yeziPath = localdirectory + "assets/models/cube.obj";
    string treePath = localdirectory + "assets/models/allGrass_001.obj";

    int yeziVertices;
    int treeVertices;
    GLuint yeziEBO = setupModelEBO(yeziPath, yeziVertices);
    GLuint treeEBO = setupModelEBO(treePath, treeVertices);

    GLuint wood = loadTexture(localdirectory + "assets/texture/wood1.png");
    GLuint leaves = loadTexture(localdirectory + "assets/texture/leaf4.png");
    GLuint grass = loadTexture(localdirectory + "assets/texture/grass.png");
    GLuint groundTexture = loadTexture(localdirectory + "assets/texture/ground.png");


    GLuint texturedCubeVAO = createTexturedCubeVertexArrayObject();


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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINE);
    // The two calls below tell the texture sampler inside the shader how it should deal with texture coordinates outside of the [0, 1] range. Here we decide to just tile the image.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
    
    mode1* yezi = new mode1(EBO, yeziEBO, leaves, yeziVertices, 1.0, 1.0, 1.0);

    mode1* tree = new mode1(EBO, treeEBO, grass, treeVertices, 1.0, 1.0, 1.0);

    yezi->Setmode(worldMatrixLocation, mat4(1.0f), 0.0, 1.0, 0.0, 1.0);
    entitys.push_back(yezi);

    tree->Setmode(worldMatrixLocation, mat4(1.0f), 0.5, 0.0, 0.0, 1.0);
    entitys.push_back(tree);




    //==============================================================camera==============================================//
    // Camera parameters for view transform
    vec3 cameraPosition(6.0f, 18.0f, 42.0f);
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
    vec3 lightPosition;
    vec3 lightFocus(0.0, 0.0, 0.0);


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
            ab = 0.4f;
            di = 0.3f;
            spec = 0.2f;
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
            glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //vector print
            for (mode1* i : entitys) {
                i->printmode();
            }

            glActiveTexture(GL_TEXTURE0 + 1);
            glBindVertexArray(texturedCubeVAO);
            //ground
            glBindTexture(GL_TEXTURE_2D, groundTexture);
            mat4 groundMatrix;
            float groundX = -50.0f;
            float groundZ = -50.0f;
            for (int i = 0; i <= 10; i++) {
                for (int j = 0; j <= 10; j++) {
                    groundMatrix = translate(mat4(1.0f), vec3(groundX, -1.0f, groundZ)) *
                        scale(mat4(1.0f), vec3(5.0f, 0.05f, 5.0f));
                    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundMatrix[0][0]);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    groundZ += 10.0f;

                }
                groundX += 10.0f;
                groundZ = -50.0f;
            }


            //leaf
            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix = translate(mat4(1.0f), vec3(0.0f, 9.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix1 = translate(mat4(leafMatrix), vec3(2.0f, 0.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix1[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix2 = translate(mat4(leafMatrix), vec3(-2.0f, 0.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix2[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix3 = translate(mat4(leafMatrix), vec3(-2.0f, 0.0f, 2.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix3[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix4 = translate(mat4(leafMatrix), vec3(2.0f, 0.0f, 2.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix4[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            mat4 leafMatrix5 = translate(mat4(leafMatrix), vec3(-2.0f, 0.0f, -2.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix5[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix6 = translate(mat4(leafMatrix), vec3(2.0f, 0.0f, -2.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix6[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix7 = translate(mat4(leafMatrix), vec3(0.0f, 0.0f, 2.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix7[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix8 = translate(mat4(leafMatrix), vec3(0.0f, 0.0f, -2.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix8[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            mat4 leafMatrix9 = translate(mat4(leafMatrix), vec3(1.0f, 2.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix9[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix10 = translate(mat4(leafMatrix), vec3(-1.0f, 2.0f, -1.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix10[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix11 = translate(mat4(leafMatrix), vec3(1.0f, 2.0f, -1.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix11[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix12 = translate(mat4(leafMatrix), vec3(-1.0f, 2.0f, 1.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix12[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix14 = translate(mat4(leafMatrix), vec3(0.0f, 4.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix14[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            //wood
            glBindTexture(GL_TEXTURE_2D, wood);
            mat4 woodMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.6f, 1.6f, 1.6f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &woodMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, wood);
            mat4 woodMatrix1 = translate(mat4(woodMatrix), vec3(0.0f, 2.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &woodMatrix1[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindTexture(GL_TEXTURE_2D, wood);
            mat4 woodMatrix2 = translate(mat4(woodMatrix), vec3(0.0f, 4.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &woodMatrix2[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

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

            glBindVertexArray(texturedCubeVAO);
            glBindTexture(GL_TEXTURE_2D, groundTexture);
            mat4 groundMatrix = translate(mat4(1.0f), vec3(0.0f, -1.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(50.0f, 0.2f, 50.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundMatrix[0][0]);
            SetUniformMat4(shaderShadow, "model_matrix", groundMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);



            glBindTexture(GL_TEXTURE_2D, leaves);
            mat4 leafMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(12.0f, 12.0f, 12.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leafMatrix[0][0]);
            SetUniformMat4(shaderShadow, "model_matrix", leafMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 36);


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

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPosition += cameraSideVector * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPosition -= cameraSideVector * dt * currentCameraSpeed;
        }
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;




    }

    glfwTerminate();

    return 0;
}
