#include<windows.h>
#include<stdio.h>
#include<gl/glew.h>
#include<gl/GL.h>
#include<vector>

#include"vmath.h"
#include"Sphere.h"

using namespace vmath;
using namespace std;

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080
#define M_PI 3.1415926535

#define NUMBER_OF_SPHERES 24

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

#pragma comment(lib,"Sphere.lib")


//Prototype of WndProc() declared gloabally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

GLfloat angleRadianX = 0.0;
GLfloat angleRadianY = 0.0;
GLfloat angleRadianZ = 0.0;



bool gbRotateX = false;
bool gbRotateY = false;
bool gbRotateZ = false;

//OpenGL Drawing
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

enum
{
	VDG_ATTRIBUTE_VERTEX = 0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0,
};

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint gNumElements;
GLuint gNumVertices;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint gVao_Sphere;
GLuint gVbo_Sphere_positions;
GLuint gVbo_Sphere_normal;
GLuint gVbo_Sphere_element;

GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;

GLuint L_KeyPressed_uniform;

GLuint La_uniform;
GLuint Ld_uniform;
GLuint Ls_uniform;
GLuint light_position_uniform;

GLuint Ka_uniform;
GLuint Kd_uniform;
GLuint Ks_uniform;
GLuint material_shininess_uniform;

mat4 gPerspectiveProjectionMatrix;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.0f;

GLfloat myArrX[4];
GLfloat myArrY[4];
GLfloat myArrZ[4];

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPositionX[] = { 100.0f,0.0f,0.0f,1.0f };
GLfloat lightPositionY[] = { 0.0f,100.0f,0.0f,1.0f };
GLfloat lightPositionZ[] = { 0.0f,0.0f,100.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,0.0f,1.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;

vector <vector<float >> g_Spheres;
std::vector<GLfloat> materialSpheres;

bool gbLight;

FILE *gpFile = NULL;

//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	void display();
	void initialize();
	void uninitialize();
	void update();
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("RTROGL");
	bool bDone = false;

	fopen_s(&gpFile, "LogFile.txt", "w");
	if (gpFile == NULL)
	{
		printf("Error opening file\n");
		exit(0);
	}

	//initializing members of wndclass
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;


	//Registering class
	RegisterClassEx(&wndclass);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szClassName,
		TEXT("OpenGL Programmable PipeLine Native Windowing"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		0,
		0,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	initialize();

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//Game Loop
	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			display();
			update();
			if (gbActiveWindow == true)
			{
				if (gbEscapeKeyIsPressed == true)
					bDone = true;
			}
		}
	}

	uninitialize();
	fprintf(gpFile, "Exitting Main..\n");
	return((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	void display();
	void resize(int, int);
	void ToggleFullScreen(void);
	void uninitialize(void);

	static bool gbIsLKeyPressed = false;
	static bool gbIsAKeyPressed = false;
	//	static bool 

	fprintf(gpFile, "Entering WndProc..\n");

	switch (iMsg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			gbActiveWindow = true;
		else
			gbActiveWindow = false;
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			if (gbEscapeKeyIsPressed == false)
				gbEscapeKeyIsPressed = true;
			break;

		case 0x46:
			if (gbFullScreen == false)
			{
				ToggleFullScreen();
				gbFullScreen = true;
			}
			else
			{
				ToggleFullScreen();
				gbFullScreen = false;
			}
			break;
		case 0x4c:		//for l or L key (Light)
			if (gbIsLKeyPressed == false)
			{
				gbLight = true;
				gbIsLKeyPressed = true;
			}
			else
			{
				gbLight = false;
				gbIsLKeyPressed = false;
			}
			break;
		case 0x58:

			if (gbRotateX == false)
			{
				gbRotateX = true;
				gbRotateY = false;
				gbRotateZ = false;
//				LightPosition1[0] = 0.0f;
			}

			break;
		case 0x59:

			if (gbRotateY == false)
			{
				gbRotateY = true;
				gbRotateX = false;
				gbRotateZ = false;
//				LightPosition2[0] = 0.0f;
			}
			break;
		case 0x5A:

			if (gbRotateZ == false)
			{
				gbRotateX = false;
				gbRotateY = false;
				gbRotateZ = true;
//				LightPosition3[0] = -17.0f;

			}
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_CLOSE:
		uninitialize();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
	fprintf(gpFile, "Exitting WndProc..\n");

}

void ToggleFullScreen(void)
{
	MONITORINFO mi;

	if (gbFullScreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}



void initialize(void)
{

	void resize(int, int);
	void uninitialize();
	fprintf(gpFile, "Entering Initialize..\n");

	int LoadGLTextures(GLuint *, TCHAR[]);

	//Variable 
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;
	//GLint num;

	//code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	//initialize code
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 8;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	ghdc = GetDC(ghwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (wglMakeCurrent(ghdc, ghrc) == NULL)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//*****VERTEX SHADER*******
	//create shader 
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to shader
	const GLchar * vertexShaderShaderCode =
		"#version 430 core"	\
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec4 u_light_position;" \
		"uniform int u_lighting_enabled;" \
		"out vec3 transformed_normals;" \
		"out vec3 light_direction;" \
		"out vec3 viewer_vector;" \
		"void main(void)" \
		"{" \
		"if(u_lighting_enabled == 1)" \
		"{" \
		"vec4 eye_coordinates = u_view_matrix*u_model_matrix*vPosition;" \
		"transformed_normals = mat3(u_view_matrix * u_model_matrix) * vNormal;" \
		"light_direction = vec3(u_light_position) - eye_coordinates.xyz;" \
		"viewer_vector = -eye_coordinates.xyz;" \
		"}" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";

	glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderShaderCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject);

	GLint iInfoLength = 0;
	GLint iShaderCompiledStatus = 0;
	char * szInfoLog = NULL;

	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log:%s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	//*****FRAGMENT SHADER****
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to shader
	const GLchar *fragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"in vec3 transformed_normals;" \
		"in vec3 light_direction;" \
		"in vec3 viewer_vector;" \
		"out vec4 FragColor;" \

		"uniform vec3 u_La;" \
		"uniform vec3 u_Ld;" \
		"uniform vec3 u_Ls;" \
		"uniform vec3 u_Ka;" \
		"uniform vec3 u_Kd;" \
		"uniform vec3 u_Ks;" \
		"uniform float u_material_shininess;" \
		"uniform int u_lighting_enabled;" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;"  \
		"if(u_lighting_enabled==1)"  \
		"{" \
		"vec3 normalized_transformed_normals=normalize(transformed_normals);" \
		"vec3 normalized_light_direction=normalize(light_direction);" \
		"vec3 normalized_viewer_vector=normalize(viewer_vector);" \
		"vec3 ambient = u_La * u_Ka;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction),0.0);" \
		"vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector,normalized_viewer_vector),0.0),u_material_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0f,1.0f,1.0f);" \
		"}" \
		"FragColor = vec4(phong_ads_color,1.0);" \
		"}";

	glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gFragmentShaderObject);
	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iShaderCompiledStatus);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment shader compilation Log %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	//*****SHADER PROGRAM******
	//create shader program
	gShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_VERTEX, "vPosition");

	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_NORMAL, "vNormal");
	//link shader
	glLinkProgram(gShaderProgramObject);

	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Status %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// get uniform locations

	model_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_model_matrix");
	view_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_view_matrix");
	projection_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");

	//L/I Key is pressed or not
	L_KeyPressed_uniform = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabled");
	
	//ambient color intensity of LIGHT
	La_uniform = glGetUniformLocation(gShaderProgramObject, "u_La");
	//diffuse color intensity of LIGHT
	Ld_uniform = glGetUniformLocation(gShaderProgramObject, "u_Ld");
	//specular color intensity of LIGHT
	Ls_uniform = glGetUniformLocation(gShaderProgramObject, "u_Ls");
	//position of light
	light_position_uniform = glGetUniformLocation(gShaderProgramObject, "u_light_position");

	//ambient reflective color intensity of MATERIAL
	Ka_uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka");
	//diffuse reflective color intensity of MATERIAL
	Kd_uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd");

	//specular reflective color intensity of MATERIAL
	Ks_uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks");

	//shininess of material (value is conventionally between 1 to 200)
	material_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material_shininess");

	//vertices colors shader attribs 
	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);

	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	//vao
	glGenVertexArrays(1, &gVao_Sphere);
	glBindVertexArray(gVao_Sphere);

	//position vbo
	glGenBuffers(1, &gVbo_Sphere_positions);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Sphere_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//normal vbo
	glGenBuffers(1, &gVbo_Sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(VDG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//element vbo
	glGenBuffers(1, &gVbo_Sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	fprintf(gpFile, "After all vertices and normals..\n");

	glShadeModel(GL_SMOOTH);
	//DEPTH
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0, 0.0f);

	gPerspectiveProjectionMatrix = mat4::identity();
	gbLight = false;
	resize(WIN_WIDTH, WIN_HEIGHT);
	fprintf(gpFile, "Exitting Initialize..\n");

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	void drawSpheres();
	//start using OpenGL program object
	void drawSphere1();
	void drawSphere2();
	void drawSphere3();
	void drawSphere4();
	void drawSphere5();
	void drawSphere6();
	void drawSphere7();
	void drawSphere8();
	void drawSphere9();
	void drawSphere10();
	void drawSphere11();
	void drawSphere12();
	void drawSphere13();
	void drawSphere14();
	void drawSphere15();
	void drawSphere16();
	void drawSphere17();
	void drawSphere18();
	void drawSphere19();
	void drawSphere20();
	void drawSphere21();
	void drawSphere22();
	void drawSphere23();
	void drawSphere24();
	glUseProgram(gShaderProgramObject);
	int count = 0;
	if (gbLight == true)
	{

		//set u_lighting_enabled uniform
		glUniform1i(L_KeyPressed_uniform, 1);

		//setting lights properties
		glUniform3fv(La_uniform, 1, lightAmbient);
		glUniform3fv(Ld_uniform, 1, lightDiffuse);
		glUniform3fv(Ls_uniform, 1, lightSpecular);
		glUniform4fv(light_position_uniform, 1, lightPosition);

		//setting material's properties
	/*	glUniform3fv(Ka_uniform, 1, material_ambient);
		glUniform3fv(Kd_uniform, 1, material_diffuse);
		glUniform3fv(Ks_uniform, 1, material_specular);
		glUniform1f(material_shininess_uniform, material_shininess);*/
		if (gbRotateX == true)
		{
			//glUniform1i(L_KeyPressed_uniform, 1);

			//setting lights properties
			glUniform3fv(La_uniform, 1, lightAmbient);
			glUniform3fv(Ld_uniform, 1, lightDiffuse);
			glUniform3fv(Ls_uniform, 1, lightSpecular);
			glUniform4fv(light_position_uniform, 1, myArrX);
		}
		if (gbRotateY == true)
		{
			glUniform1i(L_KeyPressed_uniform, 1);

			//setting lights properties
			glUniform3fv(La_uniform, 1, lightAmbient);
			glUniform3fv(Ld_uniform, 1, lightDiffuse);
			glUniform3fv(Ls_uniform, 1, lightSpecular);
			glUniform4fv(light_position_uniform, 1, myArrY);
		}
		if (gbRotateZ == true)
		{
			glUniform1i(L_KeyPressed_uniform, 1);

			//setting lights properties
			glUniform3fv(La_uniform, 1, lightAmbient);
			glUniform3fv(Ld_uniform, 1, lightDiffuse);
			glUniform3fv(Ls_uniform, 1, lightSpecular);
			glUniform4fv(light_position_uniform, 1, myArrZ);
		}
	}
	else 
	{
		//set 'u_lighting_enabled' uniform
		glUniform1i(L_KeyPressed_uniform, 0);
	}


	drawSphere1();
	drawSphere2();
	drawSphere3();
	drawSphere4();
	drawSphere5();
	drawSphere6();
	drawSphere7();
	drawSphere8();
	drawSphere9();
	drawSphere10();
	drawSphere11();
	drawSphere12();
	drawSphere13();
	drawSphere14();
	drawSphere15();
	drawSphere16();
	drawSphere17();
	drawSphere18();
	drawSphere19();
	drawSphere20();
	drawSphere21();
	drawSphere22();
	drawSphere23();
	drawSphere24();

	//apply z axis translation to go deep into the screen by -2.0
	//so that triangle with same fullscreen coordinated but due to above translation will look small

	for (int i = 0; i <NUMBER_OF_SPHERES; i++)
	{
		int increment_address = 0;
		int increment = 4;
		glUniform3fv(Ka_uniform, 1, &g_Spheres[i][increment_address]);			increment_address += increment;
		glUniform3fv(Kd_uniform, 1, &g_Spheres[i][increment_address]);			increment_address += increment;
		glUniform3fv(Ks_uniform, 1, &g_Spheres[i][increment_address]);			increment_address += increment;
		glUniform1f(material_shininess_uniform,g_Spheres[i][increment_address]);							//shineness

		count++;

		switch (count)
		{
		case 1:
			glViewport(0, 0, (GLsizei)WIN_WIDTH/6, (GLsizei)WIN_HEIGHT/6);

		break;
		case 2:

			//modelMatrix = translate(-2.5f, 2.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 8, 0, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT/6);
		break;
		case 3:
			//modelMatrix = translate(-2.5f, 0.5f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 4, 0, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 4:
			//modelMatrix = translate(-2.5f, -1.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH/2.7 , 0, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 5:
			//modelMatrix = translate(-2.5f, -2.5f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH/2, 0, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 6:
			//modelMatrix = translate(-2.5f, -4.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH/1.6, 0, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;

		case 7:
			//modelMatrix = translate(-1.0f, 3.3f, -10.0f);
			glViewport(0, (GLsizei)WIN_HEIGHT / 5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 8:
			//modelMatrix = translate(-1.0f, 2.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 8, (GLsizei)WIN_HEIGHT / 5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 9:
			//modelMatrix = translate(-1.0f, 0.5f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 4, (GLsizei)WIN_HEIGHT / 5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);

			break;
		case 10:
			//modelMatrix = translate(-1.0f, -1.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 2.7, (GLsizei)WIN_HEIGHT / 5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);

			drawSpheres();
			break;
		case 11:
			////modelMatrix = translate(-1.0f, -2.5f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 2, (GLsizei)WIN_HEIGHT / 5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);

			break;
		case 12:
			//modelMatrix = translate(-1.0f, -4.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 1.6, (GLsizei)WIN_HEIGHT / 5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;

		case 13:
			//modelMatrix = translate(1.0f, 3.3f, -10.0f);
			glViewport(0, (GLsizei)WIN_HEIGHT / 2.5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 14:
			//modelMatrix = translate(1.0f, 2.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 8, (GLsizei)WIN_HEIGHT / 2.5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 15:
			//modelMatrix = translate(1.0f, 0.5f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 4, (GLsizei)WIN_HEIGHT / 2.5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 16:
			//modelMatrix = translate(1.0f, -1.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 2.7, (GLsizei)WIN_HEIGHT / 2.5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 17:
			//modelMatrix = translate(1.0f, -2.5f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 2, (GLsizei)WIN_HEIGHT / 2.5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 18:
			//modelMatrix = translate(1.0f, -4.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 1.6, (GLsizei)WIN_HEIGHT / 2.5, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 19:
			//modelMatrix = translate(2.0f, 3.3f, -10.0f);
			glViewport(0, (GLsizei)WIN_HEIGHT / 1.6, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 20:
			//modelMatrix = translate(2.0f, 2.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 8, (GLsizei)WIN_HEIGHT / 1.6, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 21:
			//modelMatrix = translate(2.0f, 0.5f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 4, (GLsizei)WIN_HEIGHT / 1.6, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 22:
			//modelMatrix = translate(2.0f, -1.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 2.7, (GLsizei)WIN_HEIGHT / 1.6, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 23:
			//modelMatrix = translate(2.0f, -2.5f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 2, (GLsizei)WIN_HEIGHT / 1.6, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 24:
			//modelMatrix = translate(2.0f, -4.0f, -10.0f);
			glViewport((GLsizei)WIN_WIDTH / 1.6, (GLsizei)WIN_HEIGHT / 1.6, (GLsizei)WIN_WIDTH / 6, (GLsizei)WIN_HEIGHT / 6);
			break;

		}  //switch
		drawSpheres();

	}   //loop

	if (gbRotateX == true)
	{
		myArrX[1] = lightPositionX[0] * (sin(angleRadianX));
		myArrX[2] = lightPositionX[0] * (cos(angleRadianX));
	}
	else if (gbRotateY == true)
	{
		myArrY[0] = lightPositionY[1] * (sin(angleRadianY));
		myArrY[2] = lightPositionY[1] * (cos(angleRadianY));
	}
	else if (gbRotateZ == true)
	{
		myArrZ[0] = lightPositionZ[2] * (sin(angleRadianZ));
		myArrZ[1] = lightPositionZ[2] * (cos(angleRadianZ));
	}
	
	glUseProgram(0);
	SwapBuffers(ghdc);
}


void drawSpheres()
{
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	modelMatrix = translate(0.0f, 0.0f, -1.5f);

	glBindVertexArray(gVao_Sphere);
	glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
}




void drawSphere1()
{
	void popBackAll();

	materialSpheres.push_back(0.0215f);
	materialSpheres.push_back(0.1745f);
	materialSpheres.push_back(0.0215f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.07568f);
	materialSpheres.push_back(0.61424f);
	materialSpheres.push_back(0.07568f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.633f);
	materialSpheres.push_back(0.727811f);
	materialSpheres.push_back(0.633);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.6f * 128);
	g_Spheres.push_back(materialSpheres);
	popBackAll();

}


void drawSphere2()
{
	void popBackAll();
	materialSpheres.push_back(0.135f);
	materialSpheres.push_back(0.2225f);
	materialSpheres.push_back(0.1575f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.54f);
	materialSpheres.push_back(0.89f);
	materialSpheres.push_back(0.63f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.316228f);
	materialSpheres.push_back(0.316228f);
	materialSpheres.push_back(0.316228f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.1f * 128);
	g_Spheres.push_back(materialSpheres);
	popBackAll();

}

void popBackAll()
{
	while (materialSpheres.begin() != materialSpheres.end())
	{
		materialSpheres.pop_back();
	}
}

void drawSphere3()
{
	void popBackAll();
	materialSpheres.push_back(0.05375f);
	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(0.06625f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.18275f);
	materialSpheres.push_back(0.17f);
	materialSpheres.push_back(0.22525f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.332741f);
	materialSpheres.push_back(0.328634f);
	materialSpheres.push_back(0.346435f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.3f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();

}

void drawSphere4()
{
	void popBackAll();
	materialSpheres.push_back(0.25f);
	materialSpheres.push_back(0.20725f);
	materialSpheres.push_back(0.20725f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(1.0f);
	materialSpheres.push_back(0.829f);
	materialSpheres.push_back(0.829f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.296648f);
	materialSpheres.push_back(0.296648f);
	materialSpheres.push_back(0.296648f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.088f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere5()
{
	void popBackAll();

	materialSpheres.push_back(0.175f);
	materialSpheres.push_back(0.01175f);
	materialSpheres.push_back(0.01175f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.61424f);
	materialSpheres.push_back(0.04136f);
	materialSpheres.push_back(0.04136f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.727811f);
	materialSpheres.push_back(0.626959f);
	materialSpheres.push_back(0.626959f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.6f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere6()
{
	void popBackAll();

	materialSpheres.push_back(0.1f);
	materialSpheres.push_back(0.182725f);
	materialSpheres.push_back(0.1745f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.396f);
	materialSpheres.push_back(0.74151f);
	materialSpheres.push_back(0.69102f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.297254f);
	materialSpheres.push_back(0.30829f);
	materialSpheres.push_back(0.306678f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.1f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();

}


//2nd col
void drawSphere7()
{
	void popBackAll();

	materialSpheres.push_back(0.329412f);
	materialSpheres.push_back(0.223529f);
	materialSpheres.push_back(0.027451f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.780392f);
	materialSpheres.push_back(0.568627f);
	materialSpheres.push_back(0.113725f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.992175f);
	materialSpheres.push_back(0.941176f);
	materialSpheres.push_back(0.807843f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.21794872f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere8()
{
	void popBackAll();

	materialSpheres.push_back(0.2125f);
	materialSpheres.push_back(0.1275f);
	materialSpheres.push_back(0.054f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.714f);
	materialSpheres.push_back(0.4284f);
	materialSpheres.push_back(0.18144f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.393548f);
	materialSpheres.push_back(0.271906f);
	materialSpheres.push_back(0.166721f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.2f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere9()
{
	void popBackAll();

	materialSpheres.push_back(0.25f);
	materialSpheres.push_back(0.25f);
	materialSpheres.push_back(0.25f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.774597f);
	materialSpheres.push_back(0.774597f);
	materialSpheres.push_back(0.774597f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.6f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere10()
{
	void popBackAll();

	materialSpheres.push_back(0.19125f);
	materialSpheres.push_back(0.0735f);
	materialSpheres.push_back(0.0225f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.7038f);
	materialSpheres.push_back(0.27048f);
	materialSpheres.push_back(0.0828f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.256777f);
	materialSpheres.push_back(0.137622f);
	materialSpheres.push_back(0.086014f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.1f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere11()
{
	void popBackAll();

	materialSpheres.push_back(0.24725f);
	materialSpheres.push_back(0.1995f);
	materialSpheres.push_back(0.0745f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.75164f);
	materialSpheres.push_back(0.60648f);
	materialSpheres.push_back(0.22648f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.628281f);
	materialSpheres.push_back(0.555802f);
	materialSpheres.push_back(0.366065f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.4f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere12()
{
	void popBackAll();

	materialSpheres.push_back(0.19225f);
	materialSpheres.push_back(0.19225f);
	materialSpheres.push_back(0.19225f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.50754f);
	materialSpheres.push_back(0.50754f);
	materialSpheres.push_back(0.50754f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.508273f);
	materialSpheres.push_back(0.508273f);
	materialSpheres.push_back(0.508273f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.4f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}



void drawSphere13()
{
	void popBackAll();

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.01f);
	materialSpheres.push_back(0.01f);
	materialSpheres.push_back(0.50754f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.50f);
	materialSpheres.push_back(0.50f);
	materialSpheres.push_back(0.50f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.25f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere14()
{
	void popBackAll();

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.1f);
	materialSpheres.push_back(0.06f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.50980392f);
	materialSpheres.push_back(0.50980392f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.50196078f);
	materialSpheres.push_back(0.50196078f);
	materialSpheres.push_back(0.50196078f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.25f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere15()
{
	void popBackAll();

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.01f);
	materialSpheres.push_back(0.35f);
	materialSpheres.push_back(0.50754f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.45f);
	materialSpheres.push_back(0.55f);
	materialSpheres.push_back(0.45f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.25f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere16()
{
	void popBackAll();

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(0.6f);
	materialSpheres.push_back(0.6f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.25f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere17()
{
	void popBackAll();

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.55f);
	materialSpheres.push_back(0.55f);
	materialSpheres.push_back(0.55f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.70f);
	materialSpheres.push_back(0.70f);
	materialSpheres.push_back(0.70f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.25f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere18()
{
	void popBackAll();

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.60f);
	materialSpheres.push_back(0.60f);
	materialSpheres.push_back(0.50f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.25f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}


void drawSphere19()
{
	void popBackAll();

	materialSpheres.push_back(0.02f);
	materialSpheres.push_back(0.02f);
	materialSpheres.push_back(0.02f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.01f);
	materialSpheres.push_back(0.01f);
	materialSpheres.push_back(0.01f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.078125f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere20()
{
	void popBackAll();

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.04f);
	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.078125f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere21()
{
	void popBackAll();

	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.04f);
	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(0.04f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.078125f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();

}

void drawSphere22()
{
	void popBackAll();

	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(0.04f);
	materialSpheres.push_back(0.04f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.078125f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();


}

void drawSphere23()
{
	void popBackAll();

	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.078125f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void drawSphere24()
{
	void popBackAll();

	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(0.05f);
	materialSpheres.push_back(0.0f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.5f);
	materialSpheres.push_back(0.4f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(0.7f);
	materialSpheres.push_back(0.04f);
	materialSpheres.push_back(1.0f);

	materialSpheres.push_back(0.078125f * 128);

	g_Spheres.push_back(materialSpheres);
	popBackAll();
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	gPerspectiveProjectionMatrix = perspective(58.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void uninitialize(void)
{

	if (gbFullScreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
	if (gVao_Sphere)
	{
		glDeleteVertexArrays(1, &gVao_Sphere);
		gVao_Sphere = 0;
	}

	if (gVbo_Sphere_positions)
	{
		glDeleteBuffers(1, &gVbo_Sphere_positions);
		gVbo_Sphere_positions = 0;
	}

	if (gVbo_Sphere_normal)
	{
		glDeleteBuffers(1, &gVbo_Sphere_normal);
		gVbo_Sphere_normal = 0;
	}

	if (gVbo_Sphere_element)
	{
		glDeleteBuffers(1, &gVbo_Sphere_element);
		gVbo_Sphere_element = 0;
	}
	//detach vertex shader from shader program object
	glDetachShader(gShaderProgramObject, gVertexShaderObject);

	//detach fragment shader from shader program object
	glDetachShader(gShaderProgramObject, gFragmentShaderObject);

	//delete vertex shader object
	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;

	//delete fragment shader object
	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	//delete shader program object
	glDeleteProgram(gShaderProgramObject);
	gShaderProgramObject = 0;

	//unlink shader program
	glUseProgram(0);


	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(ghwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(ghwnd);
	ghwnd = NULL;


}


void update()
{
	angleX = angleX + 0.4f;
	if (angleX >= 360.0f)
		angleX = 0.0f;
	angleRadianX = angleX * (M_PI / 180.0f);

	angleY = angleY + 0.4f;
	if (angleY >= 360.0f)
		angleY = 0.0f;
	angleRadianY = angleY * (M_PI / 180.0f);

	angleZ = angleZ + 0.4f;
	if (angleZ >= 360.0f)
		angleZ = 0.0f;
	angleRadianZ = angleZ * (M_PI / 180.0f);
}























