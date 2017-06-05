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

enum ENVIRONMENT {LAND, WATER, AIR};

struct ActVoxel{
    CVX_Voxel* voxel;
    CVX_Material* material;
    Vec3D<int> index;
};

class VoxRobot
{
    public:
    VoxRobot();
    ~VoxRobot();

    void initVoxelyze();
    void initVoxels();
    void recallRobot();
    void setProperty();
    void makeVoxels();

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

    std::vector<ActVoxel>  actVoxelList;

    CVX_Material*   matRed;
    CVX_Material*   matGreen;
    CVX_Material*   matBlue;
};

#endif // VOXROBOT_H
