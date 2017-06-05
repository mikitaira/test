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

VoxGraphics::VoxGraphics(int argc, char *argv[])
{
    cout << "Called constructor :: VoxGraphics" << endl;

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("ARC-Voxelyze");
    glutIdleFunc(idleGL);
    glutDisplayFunc(paintGL);

    gTimeStep = 0;
    gSkipStep = 1.0e3;
    gCamRot = true;
    gCamAng = 90.0f;
    gCamSpeed = 1.0f;
    gCamPos = Vec3D<float>{0.0f, 0.02f, 0.0f}; // Camera position
    gCamForward = Vec3D<float>(0.0f, 0.0f, 0.0f) - gCamPos;	// Camera pose
    gCamRight = Vec3D<float>(0.0f, 1.0f, 0.0f);	// Camera right direction
    gMode = 0;
    gGridSize = 10;

    initGL();
    initVx();

    glutMainLoop();
}

VoxGraphics::~VoxGraphics()
{
    cout << "Called destructor :: VoxGraphics" << endl;
}

void VoxGraphics::initGL()
{
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

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
    voxRobot =  new VoxRobot();
}

void idleGL(){
    glutPostRedisplay();
}

void paintGL(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    setupCamera();
    voxUpdatePhysX();

    glPushMatrix();
        float halfVoxSize = voxRobot->Vx.voxelSize()/2.0f;
        glTranslatef(0, 0, -halfVoxSize);
        drawGrid(gGridSize);
    glPopMatrix();

    drawVoxels();

    glFinish();
    //glFlush();
    gTimeStep++;
}

void voxUpdatePhysX()
{
    cout << "Elapsed time: " << gTimeStep * 1.0e-6 << "[s]" << endl;
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

        Vec3D<double> centroid = Vec3D<double>(0,0,0);
        for(int i=0; i<voxRobot->actVoxelList.size(); i++)
        {
            centroid += voxRobot->actVoxelList[i].voxel->position();
        }
        centroid /= voxRobot->actVoxelList.size();
        gluLookAt(gCamPos.x+centroid.x,gCamPos.y+centroid.y, gCamPos.z+centroid.z,
                  centroid.x, centroid.y, centroid.z,
                  0.0f, 0.0f, 1.0f);
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

void drawVoxels()
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

    // quadColors
     std::vector<float> quadColors;
     quadColors.clear();
     voxRobot->getMeshRender()->getQuadColors(quadColors);

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
    for (int i=0; i<qCount; i++) {
        glNormal3d(quadNormals[i*3], quadNormals[i*3+1], quadNormals[i*3+2]);
        glColor3d(quadColors[i*3], quadColors[i*3+1], quadColors[i*3+2]);
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
