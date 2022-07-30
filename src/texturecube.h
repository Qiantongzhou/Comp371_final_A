#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include<fstream>
#include<sstream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

using namespace glm;
using namespace std;

GLuint loadTexture(string filename)
{
    // Step1 Create and bind textures
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    assert(textureId != 0);


    glBindTexture(GL_TEXTURE_2D, textureId);
    const char* file = filename.c_str();
    // Step2 Set filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Step3 Load Textures with dimension data
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
        return 0;
    }

    // Step4 Upload the texture to the PU
    GLenum format = 0;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
        0, format, GL_UNSIGNED_BYTE, data);

    // Step5 Free resources
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}


int createTexturedCubeVertexArrayObject() 
{
    // Cube model w/ position and texture 
    float vertexArray[] = {
        // Position           Normal               color                      Texture
       -1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f   ,   0.0f, 0.0f,
        1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f,                                                
        1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 0.0f,
       -1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 1.0f,                                                  
       -1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 0.0f,
        1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 1.0f,                                                  
        1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 1.0f,
       -1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 1.0f,
       -1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 0.0f,                                                  
       -1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    1.0f, 0.0f,
       -1.0f,  1.0f, -1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 1.0f,                                                  
       -1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 1.0f,
       -1.0f, -1.0f,  1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 0.0f,
       -1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    1.0f, 0.0f,                                                  
        1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,   1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   0.0f, 1.0f,
        1.0f,  1.0f, -1.0f,   1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 1.0f,                                                  
        1.0f, -1.0f, -1.0f,   1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   0.0f, 1.0f,
        1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f,
        1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   0.0f, 0.0f,                                                
       -1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 1.0f,
        1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f,                                                 
        1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f,
       -1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 0.0f,
       -1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 1.0f,                                                 
       -1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 1.0f,
        1.0f,  1.0f , 1.0f,   0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 1.0f,                                                 
        1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f   ,   1.0f, 0.0f,
       -1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 1.0f,
       -1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f ,    0.0f, 0.0f,
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                            // attribute 0 matches aPos in Vertex Shader
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        11 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)0                      // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,                            // attribute 1 matches aNormal in Vertex Shader
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        11 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)(3 * sizeof(float))    // array buffer offset
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(3,                            // attribute 2 matches aTexCoords in Vertex Shader
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        11 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)(6 * sizeof(float))    // array buffer offset
    );
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(2,                            // attribute 2 matches aTexCoords in Vertex Shader
        2,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        11 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)(9 * sizeof(float))    // array buffer offset
    );
    glEnableVertexAttribArray(2);
 
    return vertexArrayObject;
}
int createcoloredCubeVertexArrayObject()
{
    // Cube model w/ position and texture 
    float vertexArray[] = {
        // Position           Normal                Texture
       -1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,  .0f, 1.0f, 1.0f   ,  0.0f, 0.0f,
        1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,  .0f, 1.0f, 1.0f   ,  1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,  .0f, 1.0f, 1.0f   ,  1.0f, 0.0f,
                                                           
        1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,  .0f, 1.0f, 1.0f   ,  1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,   0.0f,  0.0f, -1.0f,  .0f, 1.0f, 1.0f  ,   0.0f, 0.0f,
       -1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,  .0f, 1.0f, 1.0f  ,   0.0f, 1.0f,

       -1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f  ,   0.0f, 0.0f,
        1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f   ,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f   ,  1.0f, 1.0f,

        1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f   ,  1.0f, 1.0f,
       -1.0f,  1.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f  ,   0.0f, 1.0f,
       -1.0f, -1.0f,  1.0f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f  ,   0.0f, 0.0f,

       -1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, .0f, .0f  ,   1.0f, 0.0f,
       -1.0f,  1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, .0f, .0f  ,   1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, .0f, .0f  ,   0.0f, 1.0f,

       -1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, .0f, .0f  ,   0.0f, 1.0f,
       -1.0f, -1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, .0f, .0f  ,   0.0f, 0.0f,
       -1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, .0f, .0f  ,   1.0f, 0.0f,

        1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f   ,  1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f   ,  0.0f, 1.0f,
        1.0f,  1.0f, -1.0f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f   ,  1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f   ,  0.0f, 1.0f,
        1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f   ,  1.0f, 0.0f,
        1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f   ,  0.0f, 0.0f,

       -1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1.0f  ,   0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1.0f   ,  1.0f, 1.0f,
        1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1.0f   ,  1.0f, 0.0f,

        1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1.0f   ,  1.0f, 0.0f,
       -1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1.0f  ,   0.0f, 0.0f,
       -1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1.0f  ,   0.0f, 1.0f,

       -1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 0.0f  ,   0.0f, 1.0f,
        1.0f,  1.0f , 1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 0.0f   ,  1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 0.0f   ,  1.0f, 1.0f,

        1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 0.0f   ,  1.0f, 0.0f,
       -1.0f,  1.0f, -1.0f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 0.0f  ,   0.0f, 1.0f,
       -1.0f,  1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f, 0.0f ,    0.0f, 0.0f,
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                            // attribute 0 matches aPos in Vertex Shader
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        11 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)0                      // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,                            // attribute 1 matches aNormal in Vertex Shader
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        11 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)(3 * sizeof(float))    // array buffer offset
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(3,                            // attribute 2 matches aTexCoords in Vertex Shader
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        11 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)(6 * sizeof(float))    // array buffer offset
    );
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(2,                            // attribute 2 matches aTexCoords in Vertex Shader
        2,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        11 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)(9 * sizeof(float))    // array buffer offset
    );
    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}
void setLightMatrix(int shaderProgram, mat4 lightMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "lightMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lightMatrix[0][0]);
}