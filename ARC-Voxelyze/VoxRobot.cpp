#include "VoxRobot.h"
#define USE_OPEN_GL

using namespace std;

extern const float voxTimeStep = 1.0e-6;
float voxelTimeStep;
//taira
int stairsWidth; // x 幅 voxel数
int stairsLength; // y 奥行 voxel数
int stairsHeight; // z 1段ごとの高さ voxel数
int stairsSteps; // 段数
float stairsYoung; // ヤング率[Pa] 
int stairsTread; // ふみ面 voxel数

int ringOuterRadius; //外半径
int ringInnerRadius; //内半径
float ringYoung; // ヤング率[Pa] 
int ringThickness; // 厚み
float ringPoissons;

extern char filename[100];
extern ofstream os;

int clowLinks; // クローラーライクなものの長さ

float moment_x;
float moment_y;
float moment_z;
float force_x;
float force_y;
float force_z;
double voxelsize;

VoxRobot::VoxRobot()
{
	////cout << "Called constructor :: VoxRobot()" << endl;
	//timeStep = 0;
	//initVoxelyze();
	//initVoxels();
	//logOutPut();

	//voxelTimeStep = Vx.recommendedTimeStep();
	//int i = log10(voxelTimeStep);
	//int j = (-1)*i - 3;
	//setgSkipStep((int)pow(10, (double)j));

}

VoxRobot::~VoxRobot()
{
    cout << "Called destructor :: VoxRobot()" << endl;

}


void VoxRobot::initVoxRobot()
{
	//cout << "Called constructor :: VoxRobot()" << endl;
	timeStep = 0;
	initVoxelyze();
	initVoxels();
	logOutPut();

	voxelTimeStep = Vx.recommendedTimeStep();
	int i = log10(voxelTimeStep);
	int j = (-1)*i - 3;
	setgSkipStep((int)pow(10, (double)j));

}

void VoxRobot::setProperty(float pmudulus=1.0e+6f)
{


	//float CTE = 2.0e-2;
	//float cdamp = 5.0e-1;
	//float idamp = 1.0f;
	//float gdamp = 0.0f;

	//float kfric = 0.5;
	//float sfric = 1.0;
	//float modulus = 1.0e+6;
	//float density = 1.0e+3;
	//float poissons = 0.35;


    float CTE        = 2.0e-2;
    float cdamp      = 5.0e-1;
	cdamp = 0.8f;
    float idamp      = 1.0f;
    float gdamp      = 0.0f;
	//gdamp = 0.1f;

    float kfric      = 0.5;
	kfric = 1.0;
	float sfric = 1.0;
	sfric = 1.36;
    //float modulus    = 1.0e+6;
    float density    = 1.0e+3;
	density = 0.9e+3;
    float poissons   = 0.3;
	poissons = 0.35;

    CVX_Material* matRed    = createMaterial(ringYoung, density, ringPoissons, 255, 0, 0, 128,  CTE, kfric, sfric, cdamp, idamp, gdamp);
    CVX_Material* matGreen  = createMaterial(stairsYoung, density, poissons, 0, 255, 0, 128, 1.0e-5, kfric, sfric, cdamp, idamp, gdamp);
    CVX_Material* matBlue   = createMaterial(ringYoung, density, poissons, 0, 0, 255, 128, CTE,  kfric, sfric, cdamp, idamp, gdamp);

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

	voxelsize = 0.005;
	Vx = CVoxelyze(voxelsize);
    //CVoxelyze Vx(0.005);     // 1.0e-2[m] voxels
    //Vx.setGravity(1.0);

    //setProperty();
}


void VoxRobot::initVoxels()
{
	std::vector<ActVoxel>().swap(redVoxelList);
	redVoxelList.reserve(500);

    std::vector<ActVoxel>().swap(greenVoxelList);
    greenVoxelList.reserve(500);

	std::vector<ActVoxel>().swap(blueVoxelList);
	blueVoxelList.reserve(500);

	std::vector<ActVoxel>().swap(magentaVoxelList);
	magentaVoxelList.reserve(500);

	std::vector<ActVoxel>().swap(yellowVoxelList);
	yellowVoxelList.reserve(500);

	std::vector<ActVoxel>().swap(cyanVoxelList);
	cyanVoxelList.reserve(500);


	//if (clowLinks != 0) {
	//	makeVoxels();
	//}
	//else {
	//	makeRing();
	//}
	//if(stairsWidth != 0){
	//	makeStairs();
	//}

	Vx.loadJSON("input.xvl.json");
	Vx.setGravity(1.0);
	cout <<"-----------------" <<  Vx.voxelCount() << endl;
	for (int i = 0; i < Vx.voxelCount(); i++) {
		CVX_Voxel* n = Vx.voxel(i);
		if (strcmp((*(n->material())).name(), "RED") == 0) {
			ActVoxel actVoxel;
			actVoxel.voxel = n;
			redVoxelList.push_back(actVoxel);
		}
		else if (strcmp((*(n->material())).name(), "GREEN") == 0) {
			ActVoxel actVoxel;
			actVoxel.voxel = n;
			greenVoxelList.push_back(actVoxel);
		
		}
		else if (strcmp((*(n->material())).name(), "BLUE") == 0) {
			ActVoxel actVoxel;
			actVoxel.voxel = n;
			blueVoxelList.push_back(actVoxel);

		}
		else if (strcmp((*(n->material())).name(), "MAGENTA") == 0) {
			ActVoxel actVoxel;
			actVoxel.voxel = n;
			magentaVoxelList.push_back(actVoxel);
		}
		else if (strcmp((*(n->material())).name(), "YELLOW") == 0) {
			ActVoxel actVoxel;
			actVoxel.voxel = n;
			yellowVoxelList.push_back(actVoxel);
		}
		else {
			ActVoxel actVoxel;
			actVoxel.voxel = n;
			cyanVoxelList.push_back(actVoxel);
		}
	}

	os << "RED     Voxel Count: " << redVoxelList.size() << endl;
	os << "GREEN   Voxel Count: " << greenVoxelList.size() << endl;
	os << "BLUE    Voxel Count: " << blueVoxelList.size() << endl;
	os << "MAGENTA Voxel Count: " << magentaVoxelList.size() << endl;
	os << "YELLOW  Voxel Count: " << yellowVoxelList.size() << endl;
	os << "CYAN    Voxel Count: " << cyanVoxelList.size() << endl;

	//Vx.saveJSON("output.xvl.json");
	//iniExternal();

}

void VoxRobot::makeVoxels(){

	const int rad = ringOuterRadius;
	const int rad2 = rad * rad;
	const int tx2 = rad;

	const int posX = 0;
	const int posY = ringThickness / 2 * (-1);

	const int innerRad = ringInnerRadius;
	const int innerRad2 = innerRad * innerRad;

	for (int x3 = posY; x3 < ringThickness + posY; x3++) {
		for (int x1 = -rad; x1 <= rad; x1++) {
			for (int x2 = -rad; x2 <= rad; x2++) {
				double eq1 = pow(x1, 2.0) + pow(x2, 2.0);
				if (eq1 < rad2 && eq1 >= innerRad2) {
					int z1 = x2 + rad - 1;
					int top = (ringOuterRadius - 1 )*2;

					//if(x1 != 0) {
						int offset;
						offset = 0;
						//if(x1 < 0){
						//	offset = -clowLinks;
						//}
						//else {
						//	offset = clowLinks;
						//}

						if ((int)abs((double)x1) != rad-1 || ((int)abs((double)x1) == rad-1 && ((int)abs(x2 % 2)) == 1)) {
							if ((int)abs((double)x2) != rad - 1 || ((int)abs((double)x2) == rad - 1 && ((int)abs(x1 % 2)) == 1)) {
								ActVoxel actVoxel;
								actVoxel.voxel = Vx.setVoxel(matRed, x1 + offset, x3, x2 + rad - 1);
								actVoxel.index = Vec3D<int>(x1 + offset, x3, x2 + rad - 1);
								greenVoxelList.push_back(actVoxel);	
							}
						}
					//}
					//else {
					//	
					//	for (int k = -clowLinks; k <= clowLinks; k++) {
					//		if((int)abs((double)x2) != rad-1 || ((int)abs((double)x2) == rad-1 && ((int)abs(k))%2 == 1)){
					//			ActVoxel actVoxel2;
					//			actVoxel2.voxel = Vx.setVoxel(matRed, k, x3, x2 + rad - 1);
					//			actVoxel2.index = Vec3D<int>(k, x3, x2 + rad - 1);
					//			actVoxelList.push_back(actVoxel2);
					//		}
					//	}
					//}
				}
			}
		}
	}
}


void VoxRobot::makeRing() {

	const int rad = ringOuterRadius;
	const int rad2 = rad * rad;
	const int tx2 = rad;

	const int posX = 0;
	const int posY = ringThickness / 2 * (-1);

	const int innerRad = ringInnerRadius;
	const int innerRad2 = innerRad * innerRad;

	for (int x3 = posY; x3 < ringThickness + posY; x3++) {
		for (int x1 = -rad; x1 <= rad; x1++) {
			for (int x2 = -rad; x2 <= rad; x2++) {
				double eq1 = pow(x1, 2.0) + pow(x2, 2.0);
				if (eq1 < rad2 && eq1 >= innerRad2) {
					ActVoxel actVoxel;
					actVoxel.voxel = Vx.setVoxel(matRed, x1, x3, x2 + rad - 1);
					actVoxel.index = Vec3D<int>(x1, x3, x2 + rad - 1);
					greenVoxelList.push_back(actVoxel);
				}
			}
		}
	}
}

void VoxRobot::makeStairs() {

	const int intX = stairsWidth;
	const int intY = stairsLength;
	const int intZ = stairsHeight;
	const int intSteps = stairsSteps;

	const int posX = 10;
	const int posY = stairsLength/2*(-1);

	int x;

	for (int k = 0; k < intSteps; k++) {
		x = posX + k*stairsTread;
		if (x > intX + posX) {
			break;
		}

		for (int l = k*intZ; l < (k+1)*intZ; l++) {
			for (int i = x; i < intX + posX; i++) {
				for (int j = posY; j < intY + posY; j++) {
					ActVoxel actVoxel;
					actVoxel.voxel = Vx.setVoxel(matGreen, i, j, l);
					actVoxel.index = Vec3D<int>(i, j, l);
					redVoxelList.push_back(actVoxel);
				}
			}
		}
	}
}




void VoxRobot::recallRobot()
{

    if(timeStep==0){
        Vx.setGravity(1);
        Vx.enableFloor(true);
        Vx.enableCollisions(true);
		voxelTimeStep = 0;
    }

	////-------------------------------------------------------------------
	//// リング　初期張力+モーメント？
	////-------------------------------------------------------------------
	//if (timeStep < 25000) {
	//	float newTimeStep = Vx.recommendedTimeStep();
	//	voxelTimeStep += newTimeStep;
	//	Vx.doTimeStep(newTimeStep);
	//	timeStep++;
	//	//cout << timeStep << endl;
	//	return;
	//}

	//=======================================
	// RED
	//=======================================
	bool moment = redMoment != Vec3D<float>(0, 0, 0);
	bool force = redForce != Vec3D<float>(0, 0, 0);
	if (moment || force) {
		Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
		Vec3D<double> min = redVoxelList.begin()->voxel->position();
		Vec3D<double> max = redVoxelList.begin()->voxel->position();
		for (auto &n : redVoxelList) {
			centroid += n.voxel->position();
			min = min.Min(n.voxel->position());
			max = max.Max(n.voxel->position());

		}
		centroid /= redVoxelList.size();
		Vec3D<float> pppredForce = redForce.Scale(Vec3D<float>(1, 1, 1));
		Vec3D<float> npnredForce = redForce.Scale(Vec3D<float>(-1, 1, -1));
		Vec3D<float> nppredForce = redForce.Scale(Vec3D<float>(-1, 1, 1));
		Vec3D<float> ppnredForce = redForce.Scale(Vec3D<float>(1, 1, -1));

		for (auto &n : redVoxelList) {
			CVX_Voxel* voxel = n.voxel;

			if (moment) {

				//cout << "redMoment()" << redMoment.getX() << redMoment.getY() << redMoment.getZ() << endl;
				voxel->external()->setMoment(redMoment);
			}

			if (force) {

				//-------------------------------------------------------------------
				// 楕円
				//-------------------------------------------------------------------
				//パターン1張力のみ
				//std::vector<Vec3D<int>> fpSymmetry;
				//fpSymmetry.clear();
				//if (voxel->position().getZ() < centroid.z/* && voxel->position().getX() > centroid.x*/) {
				//	voxel->external()->setForce(npnredForce);
				//}
				//else if (voxel->position().getZ() > centroid.z/* && voxel->position().getX() > centroid.x*/) {
				//	voxel->external()->setForce(pppredForce);
				//}
				//else {
				//	voxel->external()->setForce(Vec3D<float>(0, 0, 0));
				//}


				////パターン2モーメント+張力
				//std::vector<Vec3D<int>> fpSymmetry;
				//fpSymmetry.clear();
				//if (voxel->position().x < min.x + Vx.voxelSize() * 2) {
				//	voxel->external()->setForce(npnredForce);
				//}
				//else if (voxel->position().x > max.x - Vx.voxelSize() * 2) {
				//	voxel->external()->setForce(ppnredForce);
				//}
				//else {
				//	voxel->external()->setForce(Vec3D<float>(0, 0, 0));
				//}

				//-------------------------------------------------------------------
				// リング　初期張力+モーメント？
				//-------------------------------------------------------------------


			}
		}
	}



	//=======================================
	// GREEN
	//=======================================
	moment = greenMoment != Vec3D<float>(0, 0, 0);
	force = greenForce != Vec3D<float>(0, 0, 0);
	if (moment || force) {
		Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
		for (auto &n : greenVoxelList) {
			centroid += n.voxel->position();

		}
		centroid /= greenVoxelList.size();

		for (auto &n : greenVoxelList) {
			CVX_Voxel* voxel = n.voxel;

			if (moment) {
				voxel->external()->setMoment(greenMoment);
			}

			if (force) {
				voxel->external()->setForce(greenForce);
			}
		}
	}
	
	//=======================================
	// BLUE 
	//=======================================
	moment = blueMoment != Vec3D<float>(0, 0, 0);
	force = blueForce != Vec3D<float>(0, 0, 0);
	if (moment || force) {
		Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
		for (auto &n : blueVoxelList) {
			centroid += n.voxel->position();

		}
		centroid /= blueVoxelList.size();

		for (auto &n : blueVoxelList) {
			CVX_Voxel* voxel = n.voxel;

			if (moment) {
				voxel->external()->setMoment(blueMoment);
			}

			if (force) {
				voxel->external()->setForce(blueForce);
			}
		}
	}

	//=======================================
	// MAGENTA 
	//=======================================
	moment = magentaMoment != Vec3D<float>(0, 0, 0);
	force = magentaForce != Vec3D<float>(0, 0, 0);
	if (moment || force) {
		Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
		for (auto &n : magentaVoxelList) {
			centroid += n.voxel->position();

		}
		centroid /= magentaVoxelList.size();

		for (auto &n : magentaVoxelList) {
			CVX_Voxel* voxel = n.voxel;

			if (moment) {
				voxel->external()->setMoment(blueMoment);
			}

			if (force) {
				voxel->external()->setForce(blueForce);
			}
		}
	}

	//=======================================
	// YELLOW/CYAN
	//=======================================
	moment = yellowMoment != Vec3D<float>(0, 0, 0);
	force = yellowForce != Vec3D<float>(0, 0, 0);
	if (moment || force) {
		Vec3D<float> pyellowForce = yellowForce.Scale(Vec3D<float>(-1, 1, 1));

		Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
		Vec3D<double> min = yellowVoxelList.begin()->voxel->position();
		for (auto &n : yellowVoxelList) {
			centroid += n.voxel->position();
			min = min.Min(n.voxel->position());

		}
		for (auto &n : cyanVoxelList) {
			centroid += n.voxel->position();
			min = min.Min(n.voxel->position());

		}
		centroid /= (yellowVoxelList.size() + cyanVoxelList.size());

		for (auto &n : yellowVoxelList) {
			CVX_Voxel* voxel = n.voxel;

			if (moment) {
				voxel->external()->setMoment(yellowMoment);
			}

			if (force) {
				if (voxel->position().getX() <= min.getX() + Vx.voxelSize() * 2) {
					voxel->external()->setForce(pyellowForce);
				}
				else {
					voxel->external()->setForce(Vec3D<float>(0, 0, 0));
				}
			}
		}
		for (auto &n : cyanVoxelList) {
			CVX_Voxel* voxel = n.voxel;

			if (moment) {
				voxel->external()->setMoment(yellowMoment);
			}

			if (force) {
				if (voxel->position().getX() <= min.getX() + Vx.voxelSize() * 2) {
					voxel->external()->setForce(pyellowForce);
				}
				else {
					voxel->external()->setForce(Vec3D<float>(0, 0, 0));
				}
			}
		}
	}


	//std::vector<Vec3D<int>> fpSymmetry;
	//fpSymmetry.clear();
	//std::vector<Vec3D<int>> fpSymmetry2;
	//fpSymmetry2.clear();
	//std::vector<Vec3D<int>> fpSymmetry3;
	//fpSymmetry3.clear();

	//for (auto &n : greenVoxelList) {
	//	CVX_Voxel* voxel = n.voxel;

	//	// add tension
	//	//if (voxel->position().getX() > centroid.x + (ringInnerRadius)* voxelsize) {

	//	//	// 床に接触したvoxelの対角線上のvoxcelを取得
	//	//	int x = voxel->indexX();
	//	//	int y = voxel->indexY();
	//	//	int z = voxel->indexZ();

	//	//	fpSymmetry.push_back(Vec3D<int>(x, y, z));
	//	//}else{
	//		if (voxel->position().getZ() < centroid.z) {

	//			// 床に接触したvoxelの対角線上のvoxcelを取得
	//			int x = voxel->indexX();
	//			int y = voxel->indexY();
	//			int z = voxel->indexZ();

	//			fpSymmetry3.push_back(Vec3D<int>(x, y, z));

	//		}
	//		else {
	//			//voxel->external()->setForce(Vec3D<float>(0, 0, 0));
	//		}
	//	//}

	//}

	//for (auto &n : greenVoxelList) {
	//	CVX_Voxel* voxel = n.voxel;
	//	int x1 = voxel->indexX();
	//	int y1 = voxel->indexY();
	//	int z1 = voxel->indexZ();

	//	int x2 = -1 * x1;
	//	int y2 = y1;
	//	int z2 = ringOuterRadius - 1 + ringOuterRadius - 1 - z1;


	//	if (force_x != 0 || force_y != 0 || force_z != 0) {
	//		voxel->external()->setForce(Vec3D<float>(force_x, force_y, force_z));
	//	}

	//	//for (auto &m : fpSymmetry3) {
	//	//	if (x1 == m.getX() && z1 == m.getZ() && y1 == m.getY()) {
	//	//		voxel->external()->setForce(Vec3D<float>(-force_x, force_y, -force_z));
	//	//	}

	//	//	if (x2 == m.getX() && z2 == m.getZ() && y2 == m.getY()) {
	//	//		voxel->external()->setForce(Vec3D<float>(force_x, force_y, force_z));
	//	//	}
	//	//}

	//	//for (auto &m : fpSymmetry) {
	//	//	if (x1 == m.getX() && z1 == m.getZ() && y1 == m.getY()) {
	//	//		voxel->external()->setForce(Vec3D<float>(force_x * 2, force_y, -force_z));
	//	//	}

	//	//	if (x2 == m.getX() && z2 == m.getZ() && y2 == m.getY()) {
	//	//		voxel->external()->setForce(Vec3D<float>(-force_x * 2, force_y, force_z));
	//	//	}
	//	//}

	//	//for (auto &m : fpSymmetry2) {
	//	//	if (x == m.getX() && z == m.getZ() && y == m.getY()) {
	//	//		voxel->external()->setForce(Vec3D<float>(-force_x * 4, force_y, force_z));
	//	//	}
	//	//}

	//}

	////if(timeStep > 80000){
	////	for (auto &n : staticVoxelList) {
	////		if(n.voxel->indexX() == 0){
	////			os << "x=" << n.voxel->position().getX() << ", y=" << n.voxel->position().getY() << ", z=" << n.voxel->position().getZ() << endl;
	////		}
	////	}
	////}

	float newTimeStep = Vx.recommendedTimeStep();
	voxelTimeStep += newTimeStep;
	Vx.doTimeStep(newTimeStep);
    timeStep ++;
	//cout << timeStep << endl;

}

void VoxRobot::iniExternal()
{

	//=======================================
	// RED
	//=======================================
	bool moment = redMoment != Vec3D<float>(0, 0, 0);
	bool force = redForce != Vec3D<float>(0, 0, 0);
	if (moment || force) {
		Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
		Vec3D<double> min = redVoxelList.begin()->voxel->position();
		Vec3D<double> max = redVoxelList.begin()->voxel->position();
		for (auto &n : redVoxelList) {
			centroid += n.voxel->position();
			min = min.Min(n.voxel->position());
			max = max.Max(n.voxel->position());

		}
		centroid /= redVoxelList.size();
		Vec3D<float> pppredForce = redForce.Scale(Vec3D<float>(1, 1, 1));
		Vec3D<float> npnredForce = redForce.Scale(Vec3D<float>(-1, 1, -1));
		Vec3D<float> nppredForce = redForce.Scale(Vec3D<float>(-1, 1, 1));
		Vec3D<float> ppnredForce = redForce.Scale(Vec3D<float>(1, 1, -1));

		for (auto &n : redVoxelList) {
			CVX_Voxel* voxel = n.voxel;

			//if (moment) {

			//	//cout << "redMoment()" << redMoment.getX() << redMoment.getY() << redMoment.getZ() << endl;
			//	voxel->external()->setMoment(redMoment);
			//}

			if (force) {

				//-------------------------------------------------------------------
				// リング　初期張力+モーメント？
				//-------------------------------------------------------------------
				std::vector<Vec3D<int>> fpSymmetry;
				fpSymmetry.clear();
				if (voxel->position().x < min.x + Vx.voxelSize() * 3) {
					voxel->external()->setForce(npnredForce);
				}
				else if (voxel->position().x > max.x - Vx.voxelSize() * 3) {
					voxel->external()->setForce(ppnredForce);
				}
				else {
					//voxel->external()->setForce(Vec3D<float>(0, 0, 0));
				}



			}
		}
	}

}

void VoxRobot::logOutPut() {

	os << "voxelSize: " << Vx.voxelSize() << endl;
	os << "gravityAcceleration: " << Vx.gravity() << endl;
	os << "voxelCount: " << Vx.voxelCount() << endl;
	for (int i = 0; i < Vx.materialCount(); i++) {
		CVX_Material* m = Vx.material(i);

		os << "----------------------------------------------------------" << endl;
		os << "name: " << m->name() << endl;
		os << "youngsModulus: " << scientific << m->youngsModulus() << endl;
		os << "density: " << m->density() << endl;
		os << "red: " << m->red() << endl;
		os << "green: " << m->green() << endl;
		os << "blue: " << m->blue() << endl;
		os << "alpha: " << m->alpha() << endl;
		os << "poissonsRatio: " << scientific << m->poissonsRatio() << endl;
		os << "CTE: " << scientific << m->cte() << endl;
		os << "staticFriction: " << m->staticFriction() << endl;
		os << "kineticFriction: " << m->kineticFriction() << endl;
		os << "collisionDamping: " << m->collisionDamping() << endl;
		os << "internalDamping: " << m->internalDamping() << endl;
		os << "globalDamping: " << m->globalDamping() << endl;
	}

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

