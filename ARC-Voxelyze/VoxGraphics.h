#ifndef __VOXGRAPHICS_H__
#define __VOXGRAPHICS_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <random>

#include "Voxelyze.h"
#include "VoxRobot.h"

void idleGL();
void paintGL();
void setupCamera();
void voxUpdatePhysX();
void drawVoxels();
void drawCube(Vec3D<float> vertex[], int face[][4]);
void drawGrid(int gridSize);


struct MaterialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat emission[4];
    GLfloat shininess;
};



class VoxGraphics
{
    public:
        VoxGraphics(int argc, char *argv[]);
        ~VoxGraphics();
        void initGL();
        void initVx();
};

#endif // VOXGRAPHICS_H
