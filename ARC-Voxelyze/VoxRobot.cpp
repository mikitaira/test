#include "VoxRobot.h"
#define USE_OPEN_GL

using namespace std;

extern const float voxTimeStep = 1.0e-6;

VoxRobot::VoxRobot()
{
    cout << "Called constructor :: VoxRobot()" << endl;
    timeStep = 0;
    initVoxelyze();
    initVoxels();
}

VoxRobot::~VoxRobot()
{
    cout << "Called destructor :: VoxRobot()" << endl;

}

void VoxRobot::setProperty()
{
    float CTE        = 2.0e-2;
    float cdamp      = 5.0e-1;
    float idamp      = 1.0f;
    float gdamp      = 0.0f;

    float kfric      = 0.5;
    float sfric      = 1.0;
    float modulus    = 1.0e+6;
    float density    = 1.0e+3;
    float poissons   = 0.35;

    CVX_Material* matRed    = createMaterial(modulus, density, poissons, 255, 0, 0, 128,  CTE, kfric, sfric, cdamp, idamp, gdamp);
    CVX_Material* matGreen  = createMaterial(modulus, density, poissons, 0, 255, 0, 128, -CTE, kfric, sfric, cdamp, idamp, gdamp);
    CVX_Material* matBlue   = createMaterial(modulus, density, poissons, 0, 0, 255, 128,   0,  kfric, sfric, cdamp, idamp, gdamp);

    setDefaultDynamicRedMaterial(matRed);
    setDefaultDynamicGreenMaterial(matGreen);
    setDefaultDynamicBlueMaterial(matBlue);

}
CVX_Material* VoxRobot::createMaterial(
        float pa, float dens, float poissons,
        int red, int green, int blue, int alpha,
        float cte, float kfric, float sfric, float cdamp, float idamp, float gdamp)
{
    CVX_Material* mat;
    mat = Vx.addMaterial(pa, dens);
    mat->setColor(red,green,blue,alpha);
    mat->setCte(cte);
    mat->setKineticFriction(kfric);
    mat->setStaticFriction(sfric);
    mat->setCollisionDamping(cdamp);
    mat->setInternalDamping(idamp);
    mat->setGlobalDamping(gdamp);
    mat->setPoissonsRatio(poissons);
    return mat;
}

void VoxRobot::initVoxelyze()
{
    Vx.clear();

    CVoxelyze Vx(0.005);     // 1.0e-2[m] voxels
    Vx.setGravity(1.0);
    setProperty();
}


void VoxRobot::initVoxels()
{
    std::vector<ActVoxel>().swap(actVoxelList);
    actVoxelList.reserve(1000);
    makeVoxels();
}

void VoxRobot::makeVoxels(){
    //    this->Vx.setVoxel(pMaterial1, 0, 0, 0);
    const int rad  = 10;
    const int rad2 = rad * rad;
    const int tx2 = rad;
    for(int x1 = -rad; x1 <= rad; x1++ ){
        for(int x2 = -rad+tx2; x2 <= rad+tx2; x2++){
            double eq1 = pow(x1, 2.0) + pow(x2-tx2, 2.0);
            if(eq1 < rad2 /*&& eq1 >= rad2 / 4*/){
                ActVoxel actVoxel;
                actVoxel.voxel =  Vx.setVoxel(matRed, x1, 0, x2-1);
                actVoxel.index =  Vec3D<int>(x1, 0, x2-1);
                actVoxelList.push_back(actVoxel);
            }
//            else if(eq1 < rad2 / 4){
//                ActVoxel actVoxel;
//                actVoxel.voxel =  Vx.setVoxel(matGreen, x1, 0, x2-1);
//                actVoxel.index =  Vec3D<int>(x1, 0, x2-1);
//                actVoxelList.push_back(actVoxel);
//            }
        }
    }
}



void VoxRobot::recallRobot()
{

    if(timeStep==0){
        Vx.setGravity(1);
        Vx.enableFloor(true);
        Vx.enableCollisions(true);
        if(Vx.recommendedTimeStep()<voxTimeStep){
            cout << "Error: voxTimeStep is too large" << endl;
            exit(1);
        }
    }


    for(auto &n: actVoxelList){
        CVX_Voxel* voxel = n.voxel;
        voxel->external()->setMoment(0, 4.0e-8, 0);
    }
    Vx.doTimeStep(voxTimeStep);
    timeStep ++;
}

void VoxRobot::setVx(CVoxelyze Vx)
{
    (this)->Vx = (Vx);
}

CVoxelyze VoxRobot::getVx()
{
    return this->Vx;
}

void VoxRobot::setMeshRender(CVX_MeshRender* meshRender)
{
    this->meshRender = meshRender;
}

CVX_MeshRender* VoxRobot::getMeshRender()
{
    return this->meshRender;
}

void VoxRobot::setMaterial(CVX_Material* vx_mat)
{
    this->vx_mat = vx_mat;
}

void VoxRobot::setDefaultDynamicRedMaterial(CVX_Material* material)
{
    matRed = material;
}
void VoxRobot::setDefaultDynamicGreenMaterial(CVX_Material* material)
{
    matGreen = material;
}
void VoxRobot::setDefaultDynamicBlueMaterial(CVX_Material* material)
{
    matBlue = material;
}

