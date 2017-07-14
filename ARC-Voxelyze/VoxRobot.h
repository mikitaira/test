#ifndef __VOXROBOT_H__
#define __VOXROBOT_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <Voxelyze.h>
#include <VX_MeshRender.h>
#include "VoxGraphics.h"

enum ENVIRONMENT {LAND, WATER, AIR};

struct ActVoxel {
	CVX_Voxel* voxel;
	CVX_Material* material;
	Vec3D<int> index;
};

class VoxRobot
{
    public:
    VoxRobot();
    ~VoxRobot();

	void initVoxRobot();
    void initVoxelyze();
    void initVoxels();
    void recallRobot();
	void iniExternal();
    void setProperty(float pmudulus);
    void makeVoxels();
	//taira
	void makeRing();
	void makeStairs();
	void logOutPut();

    CVX_Material* createMaterial(float pa, float dens, float poissons,
                                 int red, int green, int blue, int alpha,
                                 float cte, float kfric, float sfric, float cdamp, float idamp, float gdamp);

    void setVx(CVoxelyze Vx);
    CVoxelyze getVx();

    void setMeshRender(CVX_MeshRender* meshRender);
    CVX_MeshRender* getMeshRender();

    void setMaterial(CVX_Material* vx_mat);
    CVX_Material* getMaterial();

    void setDefaultDynamicRedMaterial(CVX_Material* material);
    void setDefaultDynamicGreenMaterial(CVX_Material* material);
    void setDefaultDynamicBlueMaterial(CVX_Material* material);

    public:
    ENVIRONMENT env;

    int timeStep;

    CVoxelyze Vx;
    CVX_MeshRender* meshRender;
    CVX_Material* vx_mat;

    //std::vector<ActVoxel>  actVoxelList;
	//std::vector<ActVoxel>  staticVoxelList;
	std::vector<ActVoxel>  redVoxelList;
	std::vector<ActVoxel>  greenVoxelList;
	std::vector<ActVoxel>  blueVoxelList;
	std::vector<ActVoxel>  magentaVoxelList;
	std::vector<ActVoxel>  yellowVoxelList;
	std::vector<ActVoxel>  cyanVoxelList;

	Vec3D<float> redMoment;
	Vec3D<float> greenMoment;
	Vec3D<float> blueMoment;
	Vec3D<float> magentaMoment;
	Vec3D<float> yellowMoment;
	//Vec3D<float> cyanMoment;

	Vec3D<float> redForce;
	Vec3D<float> greenForce;
	Vec3D<float> blueForce;
	Vec3D<float> magentaForce;
	Vec3D<float> yellowForce;
	//Vec3D<float> cyanForce;


    CVX_Material*   matRed;
    CVX_Material*   matGreen;
    CVX_Material*   matBlue;

};

#endif // VOXROBOT_H
