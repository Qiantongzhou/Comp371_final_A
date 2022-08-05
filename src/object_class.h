#pragma once


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
#include<math.h>

using namespace glm;
using namespace std;
enum Printtype
{
    EBO,
    VAO
};
float world_angleX = 0.0f;
float world_angleY = 0.0f;
vec3 axis_rotate_worldY = vec3(0.0f, 1.0f, 0.0f);
vec3 axis_rotate_worldX = vec3(1.0f, 0.0f, 0.0f);
vec3 worldcenter = vec3(0.0f, 0.0f, 0.0f);
class mode1 {
public:

    float mode_X = 0.0f;
    float mode_Y = 0.0f;

    float mode_scale = 1.0f;
    float mode_angle = 0.0f;

    GLuint modeid = 0;
    GLuint texture = 0;
    int vertices;
    mat4 mode;
    GLuint worldMatrixLocation=0;
    Printtype type=EBO;

    vec3 centre = vec3((0.0f + mode_X) * mode_scale, 0.0f * mode_scale, (0.0f + mode_Y) * mode_scale);
    vec3 axis_rotate = vec3(0.0f, 1.0f, 0.0f);
    vec3 location = vec3(0.0f, 0.0f, 0.0f);
    vec3 shape = vec3(1.0f, 1.0f, 1.0f);
    mode1(Printtype type,string Path, GLuint texture,float shape_X,float shape_Y,float shape_Z) {
        this->shape = vec3(shape_X, shape_Y, shape_Z);
        this->modeid = modeid;
        this->texture = texture;
        this->type = type;
        modeid = setupModelEBO(Path, vertices);

    }

    mat4 Setmode(GLuint worldMatrixLocation, mat4 mode, float mode_scale, float mode_X, float mode_Y,float mode_Z, float mode_angle) {

        // cout << "printboard: " << glfwGetTime() << endl;
        centre = vec3((0.0f + mode_X), 0.0f+mode_Y , (0.0f + mode_Z) );
        location = vec3((0.0f + mode_X) , 0.0f+mode_Y , (0.0f + mode_Z) );
        

        mode = mode * rota(mode_angle, centre, axis_rotate) * translate(mat4(1.0f), location) *
            scale(mat4(1.0f), shape*mode_scale);
        this->worldMatrixLocation = worldMatrixLocation;
        this->mode = mode;
        
        return mode;
    }

    void printmode() {
        if (type == Printtype::EBO) {
             printEBOmode();
        }
        if (type == Printtype::VAO) {
             printVAOmode();
        }
    }

    void printEBOmode() {
       
        glBindVertexArray(modeid);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mode[0][0]);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, 0);

    }
    void printVAOmode() {
        glBindVertexArray(modeid);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mode[0][0]);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, vertices);
    }
    void changeProgram(GLuint worldMatrixLocation) {
        this->worldMatrixLocation = worldMatrixLocation;
    }
    void printmodeshadow() {
        if (type == Printtype::EBO) {
            printEBOmodeshadow();
        }
        if (type == Printtype::VAO) {
            printVAOmodeshadow();
        }
    }

    void printEBOmodeshadow() {
        glBindVertexArray(modeid);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mode[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, 0);
    }
    void printVAOmodeshadow() {
        glBindVertexArray(modeid);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mode[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLES, 0, vertices);
    }
};

class cube1 :public mode1 {
public:
    //a,b,c,d,e
    mat4 cube;
    mat4 cuberam;
    float movelocal = -.5f;

    //bit map example
    int letter[5][5] = { {0,0,1,0,0},
                         {0,0,1,0,0},
                         {0,0,1,0,0},
                         {0,0,1,0,0},
                         {1,1,1,1,1} };
    void Pcube1(GLuint worldMatrixLocation, mat4 mode) {
        //  cout << "print cube" << endl;

        vec3 shape = vec3(.02f, 1.0f, .1f);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                location = vec3(-.2f + j * 0.042f + movelocal, 1.8f + i * 2.0f, 0.4f);

                cube = mode * translate(mat4(1.0f), location) *
                    scale(mat4(1.0f), shape) * rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
                if (letter[i][j] == 1) {
                    cube = cube * rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
                    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &cube[0][0]);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }


            }
        }



    }

};