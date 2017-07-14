#ifndef __VOXGRAPHICS_H__
#define __VOXGRAPHICS_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <gl/freeglut.h>
#include <random>


#include <Voxelyze.h>
#include "VoxRobot.h"
#include "GLCapture.hpp"


#ifdef _WIN64

#ifdef _DEBUG

#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_calib3d320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_core320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_features2d320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_flann320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_highgui320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_imgcodecs320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_imgproc320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_ml320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_objdetect320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_photo320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_shape320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_stitching320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_superres320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_video320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_videoio320d.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_videostab320d.lib")

#else											  
//Releaseモードの場合

#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_calib3d320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_core320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_features2d320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_flann320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_highgui320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_imgcodecs320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_imgproc320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_ml320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_objdetect320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_photo320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_shape320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_stitching320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_superres320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_video320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_videoio320.lib")
#pragma comment(lib,"C:\\opencv320\\build64\\install\\x64\\vc14\\lib\\opencv_videostab320.lib")

#endif

#else
#ifdef _DEBUG
//Debugモードの場合

#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_calib3d320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_core320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_features2d320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_flann320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_highgui320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_imgcodecs320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_imgproc320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_ml320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_objdetect320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_photo320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_shape320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_stitching320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_superres320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_video320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_videoio320d.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_videostab320d.lib")

#else											  
//Releaseモードの場合

#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_calib3d320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_core320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_features2d320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_flann320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_highgui320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_imgcodecs320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_imgproc320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_ml320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_objdetect320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_photo320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_shape320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_stitching320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_superres320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_video320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_videoio320.lib")
#pragma comment(lib,"C:\\opencv320\\build\\install\\x86\\vc14\\lib\\opencv_videostab320.lib")

#endif
#endif

void idleGL();
void paintGL();
void setupCamera();
void voxUpdatePhysX();
void drawVoxels(int axis);
void drawCube(Vec3D<float> vertex[], int face[][4]);
void drawGrid(int gridSize);
void mouse(int button, int state, int x, int y);
void key(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseWheel(int WheelNumber, int Direction, int MouseX, int MouseY);
void setgSkipStep(int skipstep);
void special(int key, int x, int y);
void specialUp(int key, int x, int y);
void setColorExtForce(CVoxelyze* voxelyzeInstance, int quadCount, std::vector<int>  &quadVoxIndices, std::vector<float>  &array, int coloraxis);
void setColorInnForce(CVoxelyze* voxelyzeInstance, int quadCount, std::vector<int>  &quadVoxIndices, std::vector<float>  &array, int coloraxis);
float getColorScale(float value, float scale);
void DrawString(std::string str, int w, int h, int x0, int y0);

void dispInnerX(int x, int y, int w, int h);
void dispInnerY(int x, int y, int w, int h);
void dispInnerZ(int x, int y, int w, int h);


void dispExternalX(int x, int y, int w, int h);
void dispExternalY(int x, int y, int w, int h);
void dispExternalZ(int x, int y, int w, int h);

void dispInfo(int x, int y, int w, int h);

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
		//VoxGraphics(int argc, char *argv[]);
        VoxGraphics();
        ~VoxGraphics();
		void initVoxGraphics(int argc, char *argv[]);
        void initGL();
        void initVx();

};


#endif // VOXGRAPHICS_H
