#include <iostream>
#include <GL/glew.h>
#include <3dgl/3dgl.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

// Include GLM core features
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#pragma comment (lib, "glew32.lib")

using namespace std;
using namespace _3dgl;
using namespace glm;



// GLSL Program
C3dglProgram program;


// 3D models
C3dglModel table;
C3dglModel vase;
C3dglModel heart;
C3dglModel lamp;

float pyramidRotationAngle = 0.0f;


// The View Matrix
mat4 matrixView;

// Camera & navigation
float maxspeed = 4.f;	// camera max speed
float accel = 4.f;		// camera acceleration
vec3 _acc(0), _vel(0);	// camera acceleration and velocity vectors
float _fov = 60.f;		// field of view (zoom)


//Vertext buffers for pyramid
float vertices[] = {
	-4, 0, -4,  4, 0, -4,  0, 7, 0,
	-4, 0, 4,   4, 0, 4,   0, 7, 0,
	-4, 0, -4, -4, 0, 4,   0, 7, 0,
	4, 0, -4,   4, 0, 4,   0, 7, 0
};
float normals[] = {
	0, 4, -7,  0, 4, -7,  0, 4, -7,
	0, 4, 7,   0, 4, 7,   0, 4, 7,
	-7, 4, 0,  -7, 4, 0,  -7, 4, 0,
	7, 4, 0,   7, 4, 0,   7, 4, 0
};
unsigned indices[] = {
	0, 1, 2,  3, 4, 5,  6, 7, 8,  9, 10, 11, 12, 13, 14, 13, 14, 15
};


//buffers names
unsigned vertexBuffer = 0;
unsigned normalBuffer = 0;
unsigned indexBuffer = 0;



bool init()
{

	// rendering states
	glEnable(GL_DEPTH_TEST);	// depth test is necessary for most 3D scenes
	glEnable(GL_NORMALIZE);		// normalization is needed by AssImp library models
	glShadeModel(GL_SMOOTH);	// smooth shading mode is the default one; try GL_FLAT here!
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// this is the default one; try GL_LINE!


	// Initialise Shaders
	C3dglShader vertexShader;
	C3dglShader fragmentShader;
    
	if (!vertexShader.create(GL_VERTEX_SHADER)) return false;
    if (!vertexShader.loadFromFile("shaders/basic.vert")) return false;
    if (!vertexShader.compile()) return false;

    if (!fragmentShader.create(GL_FRAGMENT_SHADER)) return false;
	if (!fragmentShader.loadFromFile("shaders/basic.frag")) return false;
	if (!fragmentShader.compile()) return false;

    if (!program.create()) return false;
	if (!program.attach(vertexShader)) return false;
	if (!program.attach(fragmentShader)) return false;
	if (!program.link()) return false;
	if (!program.use(true)) return false;

	// glut additional setup
	glutSetVertexAttribCoord3(program.getAttribLocation("aVertex"));
	glutSetVertexAttribNormal(program.getAttribLocation("aNormal"));

   // prepare vertex data
   glGenBuffers(1, &vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // prepare normal data
   glGenBuffers(1, &normalBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);


	// prepare indices array
    glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	// load your 3D models here!
	if (!table.load("models\\table.obj")) return false;
	if (!vase.load("models\\vase.obj")) return false;
	if (!heart.load("models\\heart.obj")) return false;
	if (!lamp.load("models\\lamp.obj")) return false;



	// Initialise the View Matrix (initial position of the camera)
	matrixView = rotate(mat4(1), radians(12.f), vec3(1, 0, 0));
	matrixView *= lookAt(
		vec3(0.0, 7.0, 10.0),
		vec3(0.0, 5.0, 0.0),
		vec3(0.0, 1.0, 0.0));

	// setup the screen background colour
	glClearColor(0.18f, 0.25f, 0.22f, 1.0f);   // deep grey background

	cout << endl;
	cout << "Use:" << endl;
	cout << "  WASD or arrow key to navigate" << endl;
	cout << "  QE or PgUp/Dn to move the camera up and down" << endl;
	cout << "  Shift to speed up your movement" << endl;
	cout << "  Drag the mouse to look around" << endl;
	cout << endl;

	return true;
}





void renderScene(mat4& matrixView, float time, float deltaTime)
{

	
	mat4 m;
	
	// setup materials table 
	program.sendUniform("material", vec3(0.878f, 0.686f, 0.627f));
	// table
	m = matrixView;
	m = rotate(m, radians(245.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.005f, 0.005f, 0.005f));
	table.render(1, m);

	// setup materials chairs 
	program.sendUniform("material", vec3(0.878f, 0.686f, 0.627f));
	//chairs
	m = matrixView;
	m = translate(m, vec3(0.0f, 0.0f, 0.0f));
	m = rotate(m, radians(245.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.005f, 0.005f, 0.005f));
	table.render(0, m);

	m = matrixView;
	m = translate(m, vec3(0.0f, 0.0f, 0.0f));
	m = rotate(m, radians(-300.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.005f, 0.005f, 0.005f));
	table.render(0, m);

	m = matrixView;
	m = translate(m, vec3(0.0f, 0.0f, 0.0f));
	m = rotate(m, radians(-390.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.005f, 0.005f, 0.005f));
	table.render(0, m);

	m = matrixView;
	m = translate(m, vec3(0.0f, 0.0f, 0.0f));
	m = rotate(m, radians(-210.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.005f, 0.005f, 0.005f));
	table.render(0, m);

	// setup materials vase 
	program.sendUniform("material", vec3(1.0f, 0.302f, 0.427f));
	//vase
	m = matrixView;
	m = translate(m, vec3(-0.3f, 3.8f, 0.0f));
	m = rotate(m, radians(25.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.13f, 0.13f, 0.13f));
	vase.render(m);

	
	
	// setup materials heart 
	program.sendUniform("material", vec3(0.969f, 0.839f, 0.878f));
	//heart
	m = matrixView;
	m = translate(m, vec3(-1.5f, 4.5f, 1.0f));
	m = rotate(m, radians(pyramidRotationAngle), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.05f, 0.05f, 0.05f));
	heart.render(m);


	// setup materials lamps 
	program.sendUniform("material", vec3(0.980f, 0.980f, 0.980f));
	//lamps
	m = matrixView;
	m = translate(m, vec3(-2.8f, 3.8f, 1.5f));
	m = rotate(m, radians(30.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.02f, 0.02f, 0.02f));
	lamp.render(m);

	m = matrixView;
	m = translate(m, vec3(2.0f, 3.8f, -1.5f));
	m = rotate(m, radians(60.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.02f, 0.02f, 0.02f));
	lamp.render(m);



	// setup materials teapot 
	program.sendUniform("material", vec3(1, 0, 0));
	// teapot
	m = matrixView;
	m = translate(m, vec3(0.5f, 4.2f, -1.0f));
	m = rotate(m, radians(25.0f), vec3(0.0f, 1.0f, 0.0f));
	m = scale(m, vec3(0.3f, 0.3f, 0.3f));

	// the GLUT objects require the Model View Matrix setup
	program.sendUniform("matrixModelView", m);
	glutSolidTeapot(2.0);

	// setup materials pyramid 
	program.sendUniform("material", vec3(0.788f, 0.094f, 0.290f));
    //Pyramid 
	m = matrixView;
	m = translate(m, vec3(-1.5f, 4.5f, 1.0f));
	m = rotate(m, radians(pyramidRotationAngle), vec3(0.0f, 1.0f, 0.0f));
	m = rotate(m, radians(180.0f), vec3(1.0f, 0.0f, 0.0f));  
	m = scale(m, vec3(0.1f, 0.1f, 0.1f));
	program.sendUniform("matrixModelView", m);

	// Get Attribute Locations
	GLuint attribVertex = program.getAttribLocation("aVertex");
	GLuint attribNormal = program.getAttribLocation("aNormal");

	// Enable vertex attribute arrays
	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribNormal);

	// Bind (activate) the vertex buffer and set the pointer to it
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Bind (activate) the normal buffer and set the pointer to it
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw triangles � using index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	// Disable arrays
	glDisableVertexAttribArray(attribVertex);
	glDisableVertexAttribArray(attribNormal);

}

void onRender()
{
	// these variables control time & animation
	static float prev = 0;
	float time = glutGet(GLUT_ELAPSED_TIME) * 0.001f;	// time since start in seconds
	float deltaTime = time - prev;						// time since last frame
	prev = time;										// framerate is 1/deltaTime

	// clear screen and buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setup the View Matrix (camera)
	_vel = clamp(_vel + _acc * deltaTime, -vec3(maxspeed), vec3(maxspeed));
	float pitch = getPitch(matrixView);
	matrixView = rotate(translate(rotate(mat4(1),
		pitch, vec3(1, 0, 0)),	// switch the pitch off
		_vel * deltaTime),		// animate camera motion (controlled by WASD keys)
		-pitch, vec3(1, 0, 0))	// switch the pitch on
		* matrixView;

	//rotation of pyramid
	pyramidRotationAngle += 20.0f * deltaTime;
	if (pyramidRotationAngle > 360.0f) {
		pyramidRotationAngle -= 360.0f;  
	}

	// render the scene objects
	renderScene(matrixView, time, deltaTime);

	// essential for double-buffering technique
	glutSwapBuffers();

	// proceed the animation
	glutPostRedisplay();


}

// called before window opened or resized - to setup the Projection Matrix
void onReshape(int w, int h)
{
	float ratio = w * 1.0f / h;      // we hope that h is not zero
	glViewport(0, 0, w, h);
	mat4 matrixProjection = perspective(radians(_fov), ratio, 0.02f, 1000.f);

	// Setup the Projection Matrix
	program.sendUniform("matrixProjection", matrixProjection);
}

// Handle WASDQE keys
void onKeyDown(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'w': _acc.z = accel; break;
	case 's': _acc.z = -accel; break;
	case 'a': _acc.x = accel; break;
	case 'd': _acc.x = -accel; break;
	case 'e': _acc.y = accel; break;
	case 'q': _acc.y = -accel; break;
	}
}

// Handle WASDQE keys (key up)
void onKeyUp(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'w':
	case 's': _acc.z = _vel.z = 0; break;
	case 'a':
	case 'd': _acc.x = _vel.x = 0; break;
	case 'q':
	case 'e': _acc.y = _vel.y = 0; break;
	}
}

// Handle arrow keys and Alt+F4
void onSpecDown(int key, int x, int y)
{
	maxspeed = glutGetModifiers() & GLUT_ACTIVE_SHIFT ? 20.f : 4.f;
	switch (key)
	{
	case GLUT_KEY_F4:		if ((glutGetModifiers() & GLUT_ACTIVE_ALT) != 0) exit(0); break;
	case GLUT_KEY_UP:		onKeyDown('w', x, y); break;
	case GLUT_KEY_DOWN:		onKeyDown('s', x, y); break;
	case GLUT_KEY_LEFT:		onKeyDown('a', x, y); break;
	case GLUT_KEY_RIGHT:	onKeyDown('d', x, y); break;
	case GLUT_KEY_PAGE_UP:	onKeyDown('q', x, y); break;
	case GLUT_KEY_PAGE_DOWN:onKeyDown('e', x, y); break;
	case GLUT_KEY_F11:		glutFullScreenToggle();
	}
}

// Handle arrow keys (key up)
void onSpecUp(int key, int x, int y)
{
	maxspeed = glutGetModifiers() & GLUT_ACTIVE_SHIFT ? 20.f : 4.f;
	switch (key)
	{
	case GLUT_KEY_UP:		onKeyUp('w', x, y); break;
	case GLUT_KEY_DOWN:		onKeyUp('s', x, y); break;
	case GLUT_KEY_LEFT:		onKeyUp('a', x, y); break;
	case GLUT_KEY_RIGHT:	onKeyUp('d', x, y); break;
	case GLUT_KEY_PAGE_UP:	onKeyUp('q', x, y); break;
	case GLUT_KEY_PAGE_DOWN:onKeyUp('e', x, y); break;
	}
}

// Handle mouse click
void onMouse(int button, int state, int x, int y)
{
	glutSetCursor(state == GLUT_DOWN ? GLUT_CURSOR_CROSSHAIR : GLUT_CURSOR_INHERIT);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	if (button == 1)
	{
		_fov = 60.0f;
		onReshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
}

// handle mouse move
void onMotion(int x, int y)
{
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	// find delta (change to) pan & pitch
	float deltaYaw = 0.005f * (x - glutGet(GLUT_WINDOW_WIDTH) / 2);
	float deltaPitch = 0.005f * (y - glutGet(GLUT_WINDOW_HEIGHT) / 2);

	if (abs(deltaYaw) > 0.3f || abs(deltaPitch) > 0.3f)
		return;	// avoid warping side-effects

	// View = Pitch * DeltaPitch * DeltaYaw * Pitch^-1 * View;
	constexpr float maxPitch = radians(80.f);
	float pitch = getPitch(matrixView);
	float newPitch = glm::clamp(pitch + deltaPitch, -maxPitch, maxPitch);
	matrixView = rotate(rotate(rotate(mat4(1.f),
		newPitch, vec3(1.f, 0.f, 0.f)),
		deltaYaw, vec3(0.f, 1.f, 0.f)), 
		-pitch, vec3(1.f, 0.f, 0.f)) 
		* matrixView;
}

void onMouseWheel(int button, int dir, int x, int y)
{
	_fov = glm::clamp(_fov - dir * 5.f, 5.0f, 175.f);
	onReshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

int main(int argc, char **argv)
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("3DGL Scene: First Example");

	// init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		C3dglLogger::log("GLEW Error {}", (const char*)glewGetErrorString(err));
		return 0;
	}
	C3dglLogger::log("Using GLEW {}", (const char*)glewGetString(GLEW_VERSION));

	// register callbacks
	glutDisplayFunc(onRender);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecUp);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
	glutMouseWheelFunc(onMouseWheel);

	C3dglLogger::log("Vendor: {}", (const char *)glGetString(GL_VENDOR));
	C3dglLogger::log("Renderer: {}", (const char *)glGetString(GL_RENDERER));
	C3dglLogger::log("Version: {}", (const char*)glGetString(GL_VERSION));
	C3dglLogger::log("");

	// init light and everything � not a GLUT or callback function!
	if (!init())
	{
		C3dglLogger::log("Application failed to initialise\r\n");
		return 0;
	}

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}

