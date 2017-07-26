#include "VoxGraphics.h"


using namespace std;

// Global variables : to avoid multiple definition
int   gTimeStep;
int   gSkipStep;
bool  gCamRot;                  // Camera rotation
float gCamAng;                  // Camera angle
float gCamSpeed;                // Camera speed
Vec3D <float> gCamPos;          // Camera position
Vec3D <float> gCamForward;      // Camera forward direction
Vec3D <float> gCamRight;		// Camera right direction
int   gMode;                  // Orthographic: gMode = 0, Perspective: gMode = 1
int   gGridSize;


VoxRobot* voxRobot;

int mouse_l = 0;
int mouse_m = 0;
int mouse_r = 0;

int mpos[2];  // mpos[0]=x座標, mpos[1]=y座標

double trans[3] = { 0.0, 0.0, 0.0 };
double theta[3] = { 0.0, 0.0, 0.0 };
double angle[3] = { 0.0, 0.0, 0.0 };

bool spaceKey;
bool keyup;
bool keydown;
bool keyleft;
bool keyright;


GLCapture glCapture;

GLfloat lightPosition[4] = { 0.25f, 1.0f, 0.25f, 0.0f };
GLfloat lightDiffuse[3] = { 1.0f, 1.0f, 1.0f };
GLfloat lightAmbient[3] = { 0.25f, 0.25f, 0.25f };
GLfloat lightSpecular[3] = { 1.0f, 1.0f, 1.0f };

GLfloat diffuse[3] = { 1.0f, 1.0f, 0.0f };
GLfloat ambient[3] = { 0.25f, 0.25f, 0.25f };
GLfloat specular[3] = { 1.0f, 1.0f, 1.0f };
GLfloat shininess[1] = { 32.0f };

int GLframe = 0; //フレーム数
int GLtimenow = 0;//経過時間
int GLtimebase = 0;//計測開始時間

int windowWidth = 1000;
int windowHeight = 750;


extern char filename[100];
extern ofstream os;
extern float voxelTimeStep;

float colorScale;
float extColorScale;

VoxGraphics::VoxGraphics() {
	voxRobot = new VoxRobot();
	cout << "new VoxRobot()" << endl;
}

void VoxGraphics::initVoxGraphics(int argc, char *argv[])
{
    //cout << "Called constructor :: VoxGraphics" << endl;

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("ARC-Voxelyze");
    glutIdleFunc(idleGL);
    glutDisplayFunc(paintGL);
	//glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	//glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

    gTimeStep = 0;
    //gSkipStep = 1.0e3;
	//setgSkipStep(1.0e3);
    gCamRot = true;
    gCamAng = 90.0f;
    gCamSpeed = 1.0f;
    gCamPos = Vec3D<float>{0.0f, 0.18f, 0.0f}; // Camera position
	//gCamPos = Vec3D<float>{ 0.0f, 0.02f, 0.0f }; // Camera position
    gCamForward = Vec3D<float>(0.0f, 0.0f, 0.0f) - gCamPos;	// Camera pose
    gCamRight = Vec3D<float>(0.0f, 1.0f, 0.0f);	// Camera right direction
    gMode = 0;
    gGridSize = 10;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	char buf[100];
	strcpy(buf, filename);
	strcat(buf, ".avi");
	glCapture.setWriteFile(buf, CV_FOURCC('X', 'V', 'I', 'D'), 30.0f, cv::Size(windowWidth, windowHeight));
	//引数で細かく設定
	//glCapture.setWriteFile("output2.avi",CV_FOURCC('M','J','P','G'),60.0f,cv::Size(320,240));

	//拡大縮小時の補間方法の設定（デフォルトはバイリニア補間）
	glCapture.setInterpolation(cv::INTER_AREA);		//ピクセル領域の関係を利用したリサンプリングに変更

    initGL();
    initVx();
	cout << "initVoxGraphics()" << endl;

    glutMainLoop();
}


VoxGraphics::~VoxGraphics()
{
    //cout << "Called destructor :: VoxGraphics" << endl;
}

void VoxGraphics::initGL()
{
    //glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    gCamForward.Normalize();
    gCamRight.Normalize();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    static GLfloat lightPos[4] = {10.0f, 10.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0 , GL_POSITION , lightPos);
}


void VoxGraphics::initVx()
{
    //voxRobot =  new VoxRobot();
	voxRobot->initVoxRobot();
}

void idleGL(){

    glutPostRedisplay();
}

void paintGL(){

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glViewport(0, windowHeight / 3, windowWidth/2, windowHeight / 3 * 2);

    setupCamera();

	if (!spaceKey) {
		voxUpdatePhysX();
	}

    glPushMatrix();
        float halfVoxSize = voxRobot->Vx.voxelSize()/2.0f;
        glTranslatef(0, 0, -halfVoxSize);
        drawGrid(gGridSize);
    glPopMatrix();

    drawVoxels(0);

	std::stringstream ss;
	ss << "Simulation Time[s]: " << scientific << voxelTimeStep;
	DrawString(ss.str(), windowWidth / 2, windowHeight / 3 * 2, 0, 20);


	dispExternalX(windowWidth / 2, windowHeight / 3 * 2, windowWidth / 4, windowHeight / 3);
	dispExternalY(windowWidth / 2, windowHeight / 3, windowWidth / 4, windowHeight / 3);
	dispExternalZ(windowWidth / 2, 0, windowWidth / 4, windowHeight / 3);
	
	dispInnerX(windowWidth / 4 * 3, windowHeight / 3 * 2, windowWidth / 4, windowHeight / 3);
	dispInnerY(windowWidth / 4 * 3, windowHeight / 3 , windowWidth / 4, windowHeight / 3);
	dispInnerZ(windowWidth / 4 * 3, 0, windowWidth / 4, windowHeight / 3);

	dispInfo(0, 0, windowWidth / 2, windowHeight / 3);

    //glFlush();
	if (!spaceKey) {
		gTimeStep++;
	}

	/*char buf[100];
	strcpy(buf, filename);
	strcat(buf, ".log");

	ofstream os(buf, ios::app);
	*/
	//os << "Simulation Time: " << scientific << voxelTimeStep << endl;
	//os.close();

	glutSwapBuffers();

	glFinish();

	glCapture.write();
}

void dispExternalX(int x, int y, int w, int h) {

	glViewport(x, y, w, h);

	setupCamera();

	glPushMatrix();
	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	glTranslatef(0, 0, -halfVoxSize);
	drawGrid(gGridSize);
	glPopMatrix();

	drawVoxels(4);


	DrawString("External Force:x", w, h, 0, 20);
	std::stringstream ss;
	ss << "Scale[N]: " << scientific << extColorScale;
	DrawString(ss.str(), w, h, 0, 20 + 16);

}


void dispExternalY(int x, int y, int w, int h) {

	glViewport(x, y, w, h);
	setupCamera();

	glPushMatrix();
	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	glTranslatef(0, 0, -halfVoxSize);
	drawGrid(gGridSize);
	glPopMatrix();

	drawVoxels(5);


	DrawString("External Force:y", w, h, 0, 20);
	std::stringstream ss;
	ss << "Scale[N]: " << scientific << extColorScale;
	DrawString(ss.str(), w, h, 0, 20 + 16);

}

void dispExternalZ(int x, int y, int w, int h) {

	glViewport(x, y, w, h);
	setupCamera();

	glPushMatrix();
	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	glTranslatef(0, 0, -halfVoxSize);
	drawGrid(gGridSize);
	glPopMatrix();

	drawVoxels(6);


	DrawString("External Force:z", w, h, 0, 20);
	std::stringstream ss;
	ss << "Scale[N]: " << scientific << extColorScale;
	DrawString(ss.str(), w, h, 0, 20 + 16);

}

void dispInnerX(int x, int y, int w, int h) {

	glViewport(x, y, w, h);

	setupCamera();

	glPushMatrix();
		float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
		glTranslatef(0, 0, -halfVoxSize);
		drawGrid(gGridSize);
	glPopMatrix();

	drawVoxels(1);

	DrawString("Internal Force:x", w, h, 0, 20);
	std::stringstream ss;
	ss << "Scale[N]: " << scientific << colorScale;
	DrawString(ss.str(), w, h, 0, 20+16);
}


void dispInnerY(int x, int y, int w, int h) {

	glViewport(x, y, w, h);
	setupCamera();

	glPushMatrix();
	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	glTranslatef(0, 0, -halfVoxSize);
	drawGrid(gGridSize);
	glPopMatrix();

	drawVoxels(2);

	DrawString("Internal Force:y", w, h, 0, 20);
	std::stringstream ss;
	ss << "Scale[N]: " << scientific << colorScale;
	DrawString(ss.str(), w, h, 0, 20 + 16);
}

void dispInnerZ(int x, int y, int w, int h) {

	glViewport(x, y, w, h);
	setupCamera();

	glPushMatrix();
	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	glTranslatef(0, 0, -halfVoxSize);
	drawGrid(gGridSize);
	glPopMatrix();

	drawVoxels(3);

	DrawString("Internal Force:z", w, h, 0, 20);
	std::stringstream ss;
	ss << "Scale[N]: " << scientific << colorScale;
	DrawString(ss.str(), w, h, 0, 20 + 16);

}



void dispInfo(int x, int y, int w, int h) {

	glViewport(x, y, w, h);

	extern float moment_x;
	extern float moment_y;
	extern float moment_z;
	extern float force_x;
	extern float force_y;
	extern float force_z;
	extern float colorScale;
	extern float ringPoissons;
	extern double voxelsize;
	extern float ringYoung;

	std::vector<string> vstr;
	vstr.clear();
	std::stringstream ss;
	//ss << "           Voxel Size[m]: " << voxelsize;
	//vstr.push_back(ss.str());
	//ss.str("");
	//ss.clear(stringstream::goodbit);
	//ss << "     Young's modulus[Pa]: " << scientific << ringYoung;
	//vstr.push_back(ss.str());
	//ss.str("");
	//ss.clear(stringstream::goodbit);
	//ss << "         Poissonss Ratio: " << ringPoissons;
	//vstr.push_back(ss.str());
	//ss.str("");
	//ss.clear(stringstream::goodbit);
	

	//ss << "      -------------------------------------";
	//vstr.push_back(ss.str());
	//ss.str("");
	//ss.clear(stringstream::goodbit);
	ss << "RED   -------------------------------------";
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "Moment[Nm]: " << scientific << voxRobot->redMoment.getX() << "," << voxRobot->redMoment.getY() << "," << voxRobot->redMoment.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "  Force[N]: " << scientific << voxRobot->redForce.getX() << "," << voxRobot->redForce.getY() << "," << voxRobot->redForce.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "GREEN -------------------------------------";
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "Moment[Nm]: " << scientific << voxRobot->greenMoment.getX() << "," << voxRobot->greenMoment.getY() << "," << voxRobot->greenMoment.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "  Force[N]: " << scientific << voxRobot->greenForce.getX() << "," << voxRobot->greenForce.getY() << "," << voxRobot->greenForce.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "BLUE  -------------------------------------";
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "Moment[Nm]: " << scientific << voxRobot->blueMoment.getX() << "," << voxRobot->blueMoment.getY() << "," << voxRobot->blueMoment.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "  Force[N]: " << scientific << voxRobot->blueForce.getX() << "," << voxRobot->blueForce.getY() << "," << voxRobot->blueForce.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "MAGENTA ----------------------------------";
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "Moment[Nm]: " << scientific << voxRobot->magentaMoment.getX() << "," << voxRobot->magentaMoment.getY() << "," << voxRobot->magentaMoment.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "  Force[N]: " << scientific << voxRobot->magentaForce.getX() << "," << voxRobot->magentaForce.getY() << "," << voxRobot->magentaForce.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "YELLOW/CYAN -----------------------------";
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "Moment[Nm]: " << scientific << voxRobot->yellowMoment.getX() << "," << voxRobot->yellowMoment.getY() << "," << voxRobot->yellowMoment.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);
	ss << "  Force[N]: " << scientific << voxRobot->yellowForce.getX() << "," << voxRobot->yellowForce.getY() << "," << voxRobot->yellowForce.getZ();
	vstr.push_back(ss.str());
	ss.str("");
	ss.clear(stringstream::goodbit);

	//ss << "      -------------------------------------";
	//vstr.push_back(ss.str());
	//ss.str("");
	//ss.clear(stringstream::goodbit);


	//ss << "Color Scale Max Value[N]: " << scientific << colorScale;
	//vstr.push_back(ss.str());
	//ss.str("");
	//ss.clear(stringstream::goodbit);
	//os << "Simulation Time: " << scientific << voxelTimeStep << endl;

	for (int i = 0; i < vstr.size(); i++) {
		DrawString(vstr[i], w, h, 0, 20 + (i * 16));
	}
}

void voxUpdatePhysX()
{
    //cout << "Elapsed time: " << gTimeStep * 1.0e-6 << "[s]" << endl;
    for(int i=0; i<gSkipStep; i++)
    {
        voxRobot->recallRobot();
    }
}

void setupCamera(){

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if(gMode=0) glOrtho(-0.02, +0.02, -0.02, +0.02, -0.01, 0.1);
        else        gluPerspective(gCamAng, 1.0, 0.001f, 10.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

		CVoxelyze* Vx = &voxRobot->Vx;
        Vec3D<double> centroid = Vec3D<double>(0,0,0);
		for (auto n : *(Vx->voxelList())) {
			centroid += n->position();
		}
        centroid /= (double)Vx->voxelCount();

		if (keyup) {

			gluLookAt(gCamPos.x + centroid.x, gCamPos.y + centroid.y, 0.2f,
				centroid.x, centroid.y, centroid.z,
				0.0f, 0.0f, 1.0f);
		

		}
		else if (keydown) {

			gluLookAt(gCamPos.x + centroid.x, gCamPos.y + centroid.y, -0.005f,
				centroid.x, centroid.y, centroid.z,
				0.0f, 0.0f, 1.0f);

		}
		else if (keyleft) {


			gluLookAt(gCamPos.x + centroid.x + 0.15f, gCamPos.y + centroid.y - 0.1f,  gCamPos.z + centroid.z,
				centroid.x, centroid.y, centroid.z,
				0.0f, 0.0f, 1.0f);

		}
		else if (keyright) {

			gluLookAt(gCamPos.x + centroid.x - 0.15f, gCamPos.y + centroid.y - 0.2f, gCamPos.z + centroid.z,
				centroid.x, centroid.y, centroid.z,
				0.0f, 0.0f, 1.0f);


		}
		else {

			//cout << "x:" + to_string((float)gCamPos.x + (float)centroid.x) << endl;
			//cout << "y:" + to_string((float)gCamPos.y + (float)centroid.y) << endl;
			//cout << "z:" + to_string((float)gCamPos.z + (float)centroid.z) << endl;

			gluLookAt(gCamPos.x + centroid.x, gCamPos.y + centroid.y, gCamPos.z + centroid.z,
				centroid.x, centroid.y, centroid.z,
				0.0f, 0.0f, 1.0f);

		}

//        gluLookAt(gCamPos.x,gCamPos.y, gCamPos.z,
//                  centroid.x, centroid.y, centroid.z,
//                  0.0f, 0.0f, 1.0f);
}

void drawGrid(int gridSize)
{
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_POLYGON_SMOOTH);
    int numGrid  = 0;
    float gGridScale = voxRobot->Vx.voxelSize() * 10.0;
    for(int i=-gridSize; i<=gridSize; i++){
        for(int j=-gridSize; j<=gridSize; j++){
            if(numGrid%2 == 0)	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
            if(numGrid%2 == 1)	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

            float x0 = gGridScale *  i;
            float y0 = gGridScale *  j;
            float x1 = gGridScale * (i + 1);
            float y1 = gGridScale * (j + 1);
            glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(x0, y0, 0.0f);
            glVertex3f(x1, y0, 0.0f);
            glVertex3f(x1, y1, 0.0f);
            glVertex3f(x0, y1, 0.0f);
            glEnd();
            numGrid ++;
        }
    }

    glEnable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void drawVoxels(int axis)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_POLYGON_SMOOTH);

    CVX_MeshRender voxMeshRender(&voxRobot->Vx);
    voxRobot->setMeshRender(&voxMeshRender);

    // quads
    std::vector<int> quads;
    quads.clear();
    voxRobot->getMeshRender()->getQuads(quads);
    int qCount =  quads.size()/4;

	
	std::vector<int> quadVoxIndices;
	quadVoxIndices.clear();
	voxRobot->getMeshRender()->getQuadVoxIndices(quadVoxIndices);

    // quadColors
     std::vector<float> quadColors;
     quadColors.clear();
     voxRobot->getMeshRender()->getQuadColors(quadColors);

	 std::vector<float> quadExtColors;
	 std::vector<float> quadInnColors;
	 if (axis != 0 && axis > 3) {
		 quadExtColors.clear();
		 setColorExtForce(&voxRobot->Vx, qCount, quadVoxIndices, quadExtColors, axis);
	 }

	 if (axis != 0 && axis <= 3) {
		 quadInnColors.clear();
		 setColorInnForce(&voxRobot->Vx, qCount, quadVoxIndices, quadInnColors, axis);
	 }

    // quadNormals
    std::vector<float> quadNormals;
    quadNormals.clear();
    voxRobot->getMeshRender()->getQuadNormals(quadNormals);

    // vertices
    std::vector<float> vertices;
    vertices.clear();
    voxRobot->getMeshRender()->getVertices(vertices);

    // lines
    std::vector<int> lines;
    lines.clear();
    voxRobot->getMeshRender()->getLines(lines);
    int lCount = lines.size()/2;

    glLineWidth(1.0);
    glColor3f((GLfloat)0.0,(GLfloat)0.0,(GLfloat)0.0);
    glBegin(GL_LINES);
   // glColor3d(0, 0, 0); //black lines...

    for (int i=0; i<lCount; i++) {
        glVertex3d(vertices[3*lines[2*i]], vertices[3*lines[2*i]+1], vertices[3*lines[2*i]+2]);
        glVertex3d(vertices[3*lines[2*i+1]], vertices[3*lines[2*i+1]+1], vertices[3*lines[2*i+1]+2]);
    }
    glEnd();

    glColor3f((GLfloat)1.0,(GLfloat)0.0,(GLfloat)0.0);

	std::vector<float> dispColors;
	dispColors.clear();
	if (axis != 0 && axis <= 3) {
		copy(quadInnColors.begin(), quadInnColors.end(), back_inserter(dispColors));
	}
	else if(axis == 0){
		copy(quadColors.begin(), quadColors.end(), back_inserter(dispColors));
	}
	else {
		copy(quadExtColors.begin(), quadExtColors.end(), back_inserter(dispColors));
	}

    for (int i=0; i<qCount; i++) {
        glNormal3d(quadNormals[i*3], quadNormals[i*3+1], quadNormals[i*3+2]);
        glColor3d(dispColors[i*3], dispColors[i*3+1], dispColors[i*3+2]);
        //glLoadName(quadVoxIndices[i]); //to enable picking

        glBegin(GL_TRIANGLES);
        glVertex3d(vertices[3*quads[4*i]],   vertices[3*quads[4*i]+1],   vertices[3*quads[4*i]+2]);
        glVertex3d(vertices[3*quads[4*i+1]], vertices[3*quads[4*i+1]+1], vertices[3*quads[4*i+1]+2]);
        glVertex3d(vertices[3*quads[4*i+2]], vertices[3*quads[4*i+2]+1], vertices[3*quads[4*i+2]+2]);

        glVertex3d(vertices[3*quads[4*i+2]], vertices[3*quads[4*i+2]+1], vertices[3*quads[4*i+2]+2]);
        glVertex3d(vertices[3*quads[4*i+3]], vertices[3*quads[4*i+3]+1], vertices[3*quads[4*i+3]+2]);
        glVertex3d(vertices[3*quads[4*i]],   vertices[3*quads[4*i]+1],   vertices[3*quads[4*i]+2]);
        glEnd();
    }

    glEnable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

/*
void VoxGraphics::saveImage(){

    unsigned char* buffer =  (unsigned char*)calloc(400*400*4, sizeof(unsigned char));
    glReadBuffer((GL_FRONT));
    glReadPixels(0,0,400,400,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
    std::ostringstream oss;
    oss << "../image/im" << timeStep << ".rgba";
    std::ofstream ofs(oss.str());
    ofs.write(reinterpret_cast<const char*>(buffer), 400*400*4);
    ofs.close();

    std::ostringstream output;
    output << "../image/im" << timeStep << ".png";
    std::stringstream command;
    command << "convert -size " << "400x400" << " -depth 8 " << oss.str() << " " << output.str();
    system(command.str().c_str());
    std::remove(oss.str().c_str());
}
*/


void mouse(int button, int state, int x, int y)
{
	//cout << "mouse event" << endl;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			mpos[0] = x;
			mpos[1] = y;
			mouse_l = 1;
		}
		if (state == GLUT_UP) {
			mouse_l = 0;
		}
		break;
	default:
		break;
	}
}

//void motion(int x, int y)
//{
//	cout << "motion event" << endl;
//
//	if (mouse_l == 1) {
//		theta[0] = (double)(y - mpos[1]) / 5.0;
//		theta[1] = (double)(x - mpos[0]) / 5.0;
//	}
//	if (mouse_l == 1 || mouse_m == 1 || mouse_r == 1) {
//		mpos[0] = x;  // ボタンを押しているときの マウスのx座標
//		mpos[1] = y;  // ボタンを押しているときの マウスのy座標
//		angle[0] += theta[0];
//		angle[1] += theta[1];
//		//glutPostRedisplay();
//	}
//}


void mouseWheel(int WheelNumber, int Direction, int x, int y)
{
	//cout << "mouseWheel event" << endl;
	mpos[0] = mpos[0] + Direction*1.0e2;
	mpos[1] = mpos[1] + Direction*1.0e2;
}


void key(unsigned char key, int x, int y) {
	switch (key) {
		case 0x20:
			spaceKey = true;
			break;
		default:
			break;
	}

	
}

void special(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_UP:
			keyup = true;
			break;
		case GLUT_KEY_RIGHT:
			keyright = true;
			break;
		case GLUT_KEY_DOWN:
			keydown = true;
			break;
		case GLUT_KEY_LEFT:
			keyleft = true;
			break;
		default:
			break;
	}


}


void keyUp(unsigned char key, int x, int y) {

	spaceKey = false;
	//keyup = false;
	//keydown = false;
	//keyright = false;
	//keyleft = false;
	mpos[0] = 250;
	mpos[1] = 250;

}

void specialUp(int key, int x, int y) {

	//spaceKey = false;
	keyup = false;
	keydown = false;
	keyright = false;
	keyleft = false;

}

void setgSkipStep(int gskipstep) {
	gSkipStep = gskipstep;


	//char buf[100];
	//strcpy(buf, filename);
	//strcat(buf, ".log");

	//ofstream os(buf, ios::app);

	cout << "Simulation SkipStep:";
	cout << gSkipStep << endl;

	//os << "Simulation SkipStep:";
	//os << gSkipStep << endl;

	//os.close();
}

void setColorExtForce(CVoxelyze* voxelyzeInstance, int quadCount, std::vector<int>  &quadVoxIndices, std::vector<float>  &array, int coloraxis)
{

	//char buf[100];
	//strcpy(buf, filename);
	//strcat(buf, ".log");

	//ofstream os(buf, ios::app);

	array.resize(quadCount * 3);

	for (int i = 0; i < quadCount; i++) {
		CVX_Voxel* pV = voxelyzeInstance->voxel(quadVoxIndices[i]);
		pV->external();
		Vec3D<float> extF = pV->externalForce();
		//os << "extF.x = " << extF.x << ", y= " << extF.y << ", z= " << extF.z << endl;

		switch (coloraxis) {
		case 4: //x軸

			if (extF.x > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(extF.x, extColorScale);
				array[i * 3 + 2] = getColorScale(extF.x, extColorScale);
			}
			else if (extF.x == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(extF.x, extColorScale);
				array[i * 3 + 1] = getColorScale(extF.x, extColorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;

		case 5: //y軸
			if (extF.y > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(extF.y, extColorScale);
				array[i * 3 + 2] = getColorScale(extF.y, extColorScale);
			}
			else if (extF.y == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(extF.y, extColorScale);
				array[i * 3 + 1] = getColorScale(extF.y, extColorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;
		default:
			if (extF.z > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(extF.z, extColorScale);
				array[i * 3 + 2] = getColorScale(extF.z, extColorScale);
			}
			else if (extF.z == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(extF.z, extColorScale);
				array[i * 3 + 1] = getColorScale(extF.z, extColorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;
		}
	}
}

void setColorInnForce(CVoxelyze* voxelyzeInstance, int quadCount, std::vector<int>  &quadVoxIndices, std::vector<float>  &array, int coloraxis)
{


	array.resize(quadCount * 3);

	for (int i = 0; i < quadCount; i++) {
		CVX_Voxel* pV = voxelyzeInstance->voxel(quadVoxIndices[i]);
		Vec3D<float> innF = pV->force();
		//os << "innF.x = " << innF.x << ", y= " << innF.y << ", z= " << innF.z << endl;

		switch (coloraxis) {
		case 1: //x軸

			if (innF.x > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(innF.x, colorScale);
				array[i * 3 + 2] = getColorScale(innF.x, colorScale);
			}
			else if (innF.x == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(innF.x, colorScale);
				array[i * 3 + 1] = getColorScale(innF.x, colorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;

		case 2: //y軸
			if (innF.y > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(innF.y, colorScale);
				array[i * 3 + 2] = getColorScale(innF.y, colorScale);
			}
			else if (innF.y == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(innF.y, colorScale);
				array[i * 3 + 1] = getColorScale(innF.y, colorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;
		default:
			if (innF.z > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(innF.z, colorScale);
				array[i * 3 + 2] = getColorScale(innF.z, colorScale);
			}
			else if (innF.z == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(innF.z, colorScale);
				array[i * 3 + 1] = getColorScale(innF.z, colorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;
		}
	}

}

float getColorScale(float value, float scale) {

	float absValue = (float)abs((double)value);

	if (absValue >= scale) return 0;

	float rate;

	rate = 1 - absValue / scale;

	return rate;

}


/*!
* 文字列描画
* @param[in] str 文字列
* @param[in] w,h ウィンドウサイズ
* @param[in] x0,y0 文字列の位置(左上原点のスクリーン座標系,文字列の左下がこの位置になる)
*/
void DrawString(string str, int w, int h, int x0, int y0)
{
	glColor3d(1.0, 1.0, 1.0);

	glDisable(GL_LIGHTING);
	// 平行投影にする
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 画面上にテキスト描画
	glRasterPos2f(x0, y0);
	int size = (int)str.size();
	for (int i = 0; i < size; ++i) {
		char ic = str[i];
		//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ic);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}