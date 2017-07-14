#include "exec.h"

using namespace std;
char filename[100];
ofstream os;

int main(int argc, char *argv[])
{

	//taira
	extern int stairsWidth; // x 幅 voxel数
	extern int stairsLength; // y 奥行 voxel数
	extern int stairsHeight; // z 1段ごとの高さ voxel数
	extern int stairsSteps; // 段数
	extern float stairsYoung; // ヤング率[Pa] 
	extern int stairsTread; // ふみ面 voxel数
	extern float ringYoung; // ヤング率[Pa] 
	extern int ringOuterRadius; // 外半径 
	extern int ringInnerRadius; // 内半径
	extern int ringThickness; // 厚み
	extern float ringPoissons; // ポアソン比

	extern int clowLinks;

	extern float moment_x;
	extern float moment_y;
	extern float moment_z;
	extern float force_x;
	extern float force_y;
	extern float force_z;

	extern float colorScale; // 
	extern float extColorScale; // 


	time_t t;
	struct tm *timeptr, result;

	setlocale(LC_ALL, "/QSYS.LIB/EN_US.LOCALE");
	t = time(NULL);
	timeptr = localtime(&t);
	strftime(filename, sizeof(filename), "%Y%m%d%H%M", timeptr);
	
	char buf[100];
	strcpy(buf, filename);
	strcat(buf, ".log");

	os = ofstream(buf, ios::app);

	VoxGraphics *voxgrahics = new VoxGraphics();

	//cout << "Stairs Young's Modulus[Pa]:";
	//cin >> stairsYoung;
	//cout << "             Width(bottom):";
	//cin >> stairsWidth;
	//cout << "            Length(bottom):";
	//cin >> stairsLength;
	//cout << "         Height(each step):";
	//cin >> stairsHeight;
	//cout << "           Number of Steps:";
	//cin >> stairsSteps;
	//cout << "               Tread Depth:";
	//cin >> stairsTread;
	//cout << "Ring   Young's Modulus[Pa]:";
	//cin >> ringYoung;
	//cout << "              Outer Radius:";
	//cin >> ringOuterRadius;
	//cout << "              Inner Radius:";
	//cin >> ringInnerRadius;
	//cout << "                 Thickness:";
	//cin >> ringThickness;
	//cout << "            Poissons Ratio:";
	//cin >> ringPoissons;
	//cout << "             Links(bottom):";
	//cin >> clowLinks;

	//os << "Stairs Young's Modulus[Pa]:";
	//os << stairsYoung << endl;
	//os << "             Width(bottom):";
	//os << stairsWidth << endl;
	//os << "            Length(bottom):";
	//os << stairsLength << endl;
	//os << "         Height(each step):";
	//os << stairsHeight << endl;
	//os << "           Number of Steps:";
	//os << stairsSteps << endl;
	//os << "               Tread Depth:";
	//os << stairsTread << endl;
	//os << "Ring   Young's Modulus[Pa]:";
	//os << ringYoung << endl;
	//os << "              Outer Radius:";
	//os << ringOuterRadius << endl;
	//os << "              Inner Radius:";
	//os << ringInnerRadius << endl;
	//os << "                 Thickness:";
	//os << ringThickness << endl;
	//os << "            Poissons Ratio:";
	//os << ringPoissons << endl;
	//os << "             Links(bottom):";
	//os << clowLinks << endl;


	//stairsYoung = 1.0e6;
	extern VoxRobot* voxRobot;
	cout << "RED Moment xyz:";
	cin >> moment_x >> moment_y >> moment_z;
	cout << "RED Force xyz:";
	cin >> force_x >> force_y >> force_z;
	voxRobot->redMoment = Vec3D<float>(moment_x, moment_y, moment_z);
	voxRobot->redForce = Vec3D<float>(force_x, force_y, force_z);
	double externalF = force_x;
	double external = moment_x;
	external = (moment_y > external) ? moment_y : external;
	external = (moment_z > external) ? moment_z : external;
	externalF = (force_x > externalF) ? force_x : externalF;
	externalF = (force_y > externalF) ? force_y : externalF;
	externalF = (force_z > externalF) ? force_z : externalF;

	os << "RED Moment xyz:";
	os << scientific << moment_x << "," << moment_y << "," << moment_z << endl;
	os << "RED Force xyz:";
	os << scientific << force_x << "," << force_y << "," << force_z << endl;

	cout << "GREEN Moment xyz:";
	cin >> moment_x >> moment_y >> moment_z;
	cout << "GREEN Force xyz:";
	cin >> force_x >> force_y >> force_z;
	voxRobot->greenMoment = Vec3D<float>(moment_x, moment_y, moment_z);
	voxRobot->greenForce = Vec3D<float>(force_x, force_y, force_z);

	external = (moment_x > external) ? moment_x : external;
	external = (moment_y > external) ? moment_y : external;
	external = (moment_z > external) ? moment_z : external;
	externalF = (force_x > externalF) ? force_x : externalF;
	externalF = (force_y > externalF) ? force_y : externalF;
	externalF = (force_z > externalF) ? force_z : externalF;

	os << "GREEN Moment xyz:";
	os << scientific << moment_x << "," << moment_y << "," << moment_z << endl;
	os << "GREEN Force xyz:";
	os << scientific << force_x << "," << force_y << "," << force_z << endl;


	cout << "BLUE Moment xyz:";
	cin >> moment_x >> moment_y >> moment_z;
	cout << "BLUE Force xyz:";
	cin >> force_x >> force_y >> force_z;
	voxRobot->blueMoment = Vec3D<float>(moment_x, moment_y, moment_z);
	voxRobot->blueForce = Vec3D<float>(force_x, force_y, force_z);

	external = (moment_x > external) ? moment_x : external;
	external = (moment_y > external) ? moment_y : external;
	external = (moment_z > external) ? moment_z : external;
	externalF = (force_x > externalF) ? force_x : externalF;
	externalF = (force_y > externalF) ? force_y : externalF;
	externalF = (force_z > externalF) ? force_z : externalF;

	os << "BLUE Moment xyz:";
	os << scientific << moment_x << "," << moment_y << "," << moment_z << endl;
	os << "BLUE Force xyz:";
	os << scientific << force_x << "," << force_y << "," << force_z << endl;

	cout << "MAGENTA Moment xyz:";
	cin >> moment_x >> moment_y >> moment_z;
	cout << "MAGENTA Force xyz:";
	cin >> force_x >> force_y >> force_z;
	voxRobot->magentaMoment = Vec3D<float>(moment_x, moment_y, moment_z);
	voxRobot->magentaForce = Vec3D<float>(force_x, force_y, force_z);

	external = (moment_x > external) ? moment_x : external;
	external = (moment_y > external) ? moment_y : external;
	external = (moment_z > external) ? moment_z : external;
	externalF = (force_x > externalF) ? force_x : externalF;
	externalF = (force_y > externalF) ? force_y : externalF;
	externalF = (force_z > externalF) ? force_z : externalF;

	os << "MAGENTA Moment xyz:";
	os << scientific << moment_x << "," << moment_y << "," << moment_z << endl;
	os << "MAGENTA Force xyz:";
	os << scientific << force_x << "," << force_y << "," << force_z << endl;

	cout << "YELLOW/CYAN Moment xyz:";
	cin >> moment_x >> moment_y >> moment_z;
	cout << "YELLOW/CYAN Force xyz:";
	cin >> force_x >> force_y >> force_z;
	voxRobot->yellowMoment = Vec3D<float>(moment_x, moment_y, moment_z);
	voxRobot->yellowForce = Vec3D<float>(force_x, force_y, force_z);

	external = (moment_x > external) ? moment_x : external;
	external = (moment_y > external) ? moment_y : external;
	external = (moment_z > external) ? moment_z : external;
	externalF = (force_x > externalF) ? force_x : externalF;
	externalF = (force_y > externalF) ? force_y : externalF;
	externalF = (force_z > externalF) ? force_z : externalF;

	os << "YELLOW/CYAN Moment xyz:";
	os << scientific << moment_x << "," << moment_y << "," << moment_z << endl;
	os << "YELLOW/CYAN Force xyz:";
	os << scientific << force_x << "," << force_y << "," << force_z << endl;

	external = (external > externalF) ? external : externalF;
	colorScale = pow(10, (int)log10(external) + 1);
	if (externalF == 0) {
		externalF = 1;
	}
	extColorScale = pow(10, (int)log10(externalF)-1);


	os << "Color Scale Max Value: ";
	os << scientific << colorScale << endl;
	os << "External Color Scale Max Value: ";
	os << scientific << extColorScale << endl;


	voxgrahics->initVoxGraphics(argc, argv);

	os.close();


	return 0;
}
