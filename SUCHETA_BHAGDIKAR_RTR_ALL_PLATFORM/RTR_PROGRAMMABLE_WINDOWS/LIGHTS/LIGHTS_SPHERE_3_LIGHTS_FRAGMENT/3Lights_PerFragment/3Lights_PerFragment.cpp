

#include<windows.h>
#include<stdio.h>
#include<gl/glew.h>
#include<gl/GL.h>

#include"vmath.h"
#include"Sphere.h"

using namespace vmath;

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define M_PI 3.1415926535

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

#pragma comment(lib,"Sphere.lib")


//Prototype of WndProc() declared gloabally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

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

GLfloat angleRadian = 0.0;
GLfloat angleRadianGreen = 0.0;
GLfloat angleRadianBlue = 0.0;


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


GLuint La_uniformGreen;
GLuint Ld_uniformGreen;
GLuint Ls_uniformGreen;
GLuint light_position_uniformGreen;
GLfloat angleGreen = 0.0f;

GLuint Ka_uniformGreen;
GLuint Kd_uniformGreen;
GLuint Ks_uniformGreen;

GLuint La_uniformRed;
GLuint Ld_uniformRed;
GLuint Ls_uniformRed;
GLuint light_position_uniformRed;
GLfloat angleRed = 0.0f;

GLuint Ka_uniformRed;
GLuint Kd_uniformRed;
GLuint Ks_uniformRed;

mat4 gPerspectiveProjectionMatrix;
GLfloat myrotationArrBlue[4];
GLfloat myrotationArrGreen[4];
GLfloat myrotationArrRed[4];

GLfloat angleBlue;

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat lightAmbientL1[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuseL1[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat lightSpecularL1[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat lightPositionL1[] = { 100.0f,100.0f,100.0f,100.0f };

GLfloat lightAmbientL2[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuseL2[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat lightSpecularL2[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat lightPositionL2[] = { 100.0f,100.0f,100.0f,0.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;
bool gbLight;
bool gbFLight;

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
	static bool gbIsFKeyPressed = false;
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
		"uniform vec4 u_light_positionGreen;" \
		"uniform vec4 u_light_positionRed;" \
		"uniform vec4 u_light_position;" \

		"uniform int u_lighting_enabled;" \
		"out vec3 transformed_normals;" \
		"out vec3 light_directionBlue;" \
		"out vec3 light_directionRed;" \
		"out vec3 light_directionGreen;" \

		"out vec3 viewer_vector;" \
		"void main(void)" \
		"{" \
		"if(u_lighting_enabled == 1)" \
		"{" \
		"vec4 eye_coordinates = u_view_matrix*u_model_matrix*vPosition;" \
		"transformed_normals = mat3(u_view_matrix * u_model_matrix) * vNormal;" \
		"light_directionGreen = vec3(u_light_positionGreen) - eye_coordinates.xyz;" \
		"light_directionRed = vec3(u_light_positionRed) - eye_coordinates.xyz;" \
		"light_directionBlue = vec3(u_light_position) - eye_coordinates.xyz;" \

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
		"in vec3 light_directionBlue;" \
		"in vec3 light_directionRed;" \
		"in vec3 light_directionGreen;" \

		"in vec3 viewer_vector;" \
		"out vec4 FragColor;"  \
		"vec4 FragColorBlue;" \
		"vec4 FragColorGreen;" \
		"vec4 FragColorRed;" \

		"uniform vec3 u_La;" \
		"uniform vec3 u_Ld;" \
		"uniform vec3 u_Ls;" \
		"uniform vec3 u_Ka;" \
		"uniform vec3 u_Kd;" \
		"uniform vec3 u_Ks;" \

		"uniform vec3 u_LaGreen;" \
		"uniform vec3 u_LdGreen;" \
		"uniform vec3 u_LsGreen;" \
		"uniform vec3 u_KaGreen;" \
		"uniform vec3 u_KdGreen;" \
		"uniform vec3 u_KsGreen;" \

		"uniform vec3 u_LaRed;" \
		"uniform vec3 u_LdRed;" \
		"uniform vec3 u_LsRed;" \
		"uniform vec3 u_KaRed;" \
		"uniform vec3 u_KdRed;" \
		"uniform vec3 u_KsRed;" \


		"uniform float u_material_shininess;" \
		"uniform int u_lighting_enabled;" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"vec3 phong_ads_colorGreen;" \
		"vec3 phong_ads_colorRed;" \

		"if(u_lighting_enabled==1)"\
		"{" \
		"vec3 normalized_transformed_normals=normalize(transformed_normals);" \
		"vec3 normalized_viewer_vector=normalize(viewer_vector);" \

		"vec3 normalized_light_directionBlue=normalize(light_directionBlue);" \
		"vec3 ambient = u_La * u_Ka;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_directionBlue),0.0);" \
		"vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_directionBlue,normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector,normalized_viewer_vector),0.0),u_material_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;"  \
		"FragColorBlue =vec4(phong_ads_color,1.0);" \

		"vec3 normalized_light_directionRed=normalize(light_directionRed);" \
		"vec3 ambientRed = u_LaRed * u_KaRed;" \
		"float tn_dot_ldRed = max(dot(normalized_transformed_normals, normalized_light_directionRed),0.0);" \
		"vec3 diffuseRed = u_LdRed * u_KdRed * tn_dot_ldRed;" \
		"vec3 reflection_vectorRed = reflect(-normalized_light_directionRed,normalized_transformed_normals);" \
		"vec3 specularRed = u_LsRed * u_KsRed * pow(max(dot(reflection_vectorRed,normalized_viewer_vector),0.0),u_material_shininess);" \
		"phong_ads_colorRed = ambientRed + diffuseRed+specularRed;" \
		"FragColorRed =vec4(phong_ads_colorRed,1.0);" \

		"vec3 normalized_light_directionGreen=normalize(light_directionGreen);" \
		"vec3 ambientGreen = u_LaGreen * u_KaGreen;" \
		"float tn_dot_ldGreen = max(dot(normalized_transformed_normals, normalized_light_directionGreen),0.0);" \
		"vec3 diffuseGreen = u_LdGreen * u_KdGreen * tn_dot_ldGreen;" \
		"vec3 reflection_vectorGreen = reflect(-normalized_light_directionGreen,normalized_transformed_normals);" \
		"vec3 specularGreen = u_LsGreen * u_KsGreen * pow(max(dot(reflection_vectorGreen,normalized_viewer_vector),0.0),u_material_shininess);" \
		"phong_ads_colorGreen = ambientGreen + diffuseGreen+specularGreen;" \
		"FragColorGreen =vec4(phong_ads_colorGreen,1.0);" \

		"FragColor = FragColorBlue + FragColorGreen + FragColorRed;" \

		"}" \
		"else" \
		"{" \
		"FragColor = vec4(1.0f,1.0f,1.0f,1.0);" \
		"}" \
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


	////////////////////////////////////////////////////////////////////////////////////////////////


	La_uniformGreen = glGetUniformLocation(gShaderProgramObject, "u_LaGreen");
	//diffuse color intensity of LIGHT
	Ld_uniformGreen = glGetUniformLocation(gShaderProgramObject, "u_LdGreen");
	//specular color intensity of LIGHT
	Ls_uniformGreen = glGetUniformLocation(gShaderProgramObject, "u_LsGreen");
	//position of light
	light_position_uniformGreen = glGetUniformLocation(gShaderProgramObject, "u_light_positionGreen");

	//ambient reflective color intensity of MATERIAL
	Ka_uniformGreen = glGetUniformLocation(gShaderProgramObject, "u_KaGreen");
	//diffuse reflective color intensity of MATERIAL
	Kd_uniformGreen = glGetUniformLocation(gShaderProgramObject, "u_KdGreen");
	//specular reflective color intensity of MATERIAL
	Ks_uniformGreen = glGetUniformLocation(gShaderProgramObject, "u_KsGreen");
	//shininess of material (value is conventionally between 1 to 200)

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	La_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LaRed");
	//diffuse color intensity of LIGHT
	Ld_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LdRed");
	//specular color intensity of LIGHT
	Ls_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LsRed");
	//position of light
	light_position_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_light_positionRed");
	//ambient reflective color intensity of MATERIAL
	Ka_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KaRed");
	//diffuse reflective color intensity of MATERIAL
	Kd_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KdRed");
	//specular reflective color intensity of MATERIAL
	Ks_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KsRed");
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

	//start using OpenGL program object
	glUseProgram(gShaderProgramObject);


	if (gbLight == true)
	{

		//set u_lighting_enabled uniform
		glUniform1i(L_KeyPressed_uniform, 1);

		////setting lights properties
		glUniform3fv(La_uniform, 1, lightAmbient);
		glUniform3fv(Ld_uniform, 1, lightDiffuse);
		glUniform3fv(Ls_uniform, 1, lightSpecular);
		glUniform4fv(light_position_uniform, 1, myrotationArrBlue);

		//setting material's properties
		glUniform3fv(Ka_uniform, 1, material_ambient);
		glUniform3fv(Kd_uniform, 1, material_diffuse);
		glUniform3fv(Ks_uniform, 1, material_specular);
		glUniform1f(material_shininess_uniform, material_shininess);

		glUniform3fv(La_uniformGreen, 1, lightAmbientL1);
		glUniform3fv(Ld_uniformGreen, 1, lightDiffuseL1);
		glUniform3fv(Ls_uniformGreen, 1, lightSpecularL1);
		glUniform4fv(light_position_uniformGreen, 1, myrotationArrGreen);

		//setting material's properties
		glUniform3fv(Ka_uniformGreen, 1, material_ambient);
		glUniform3fv(Kd_uniformGreen, 1, material_diffuse);
		glUniform3fv(Ks_uniformGreen, 1, material_specular);
		glUniform1f(material_shininess_uniform, material_shininess);

		glUniform3fv(La_uniformRed, 1, lightAmbientL2);
		glUniform3fv(Ld_uniformRed, 1, lightDiffuseL2);
		glUniform3fv(Ls_uniformRed, 1, lightSpecularL2);
		glUniform4fv(light_position_uniformRed, 1, myrotationArrRed);

		//setting material's properties
		glUniform3fv(Ka_uniformRed, 1, material_ambient);
		glUniform3fv(Kd_uniformRed, 1, material_diffuse);
		glUniform3fv(Ks_uniformRed, 1, material_specular);
		glUniform1f(material_shininess_uniform, material_shininess);
	}
	else
	{
		//set 'u_lighting_enabled' uniform
		glUniform1i(L_KeyPressed_uniform, 0);
	}


	//OpenGL Drawing
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 rotationMatrixBlue = mat4::identity();
	mat4 rotationMatrixGreen = mat4::identity();

	myrotationArrRed[1] = lightPositionL2[0] * (sin(angleRadian));
	myrotationArrRed[2] = lightPositionL2[0] * (cos(angleRadian));

	myrotationArrGreen[0] = lightPositionL1[1] * (sin(angleRadianGreen));
	myrotationArrGreen[2] = lightPositionL1[1] * (cos(angleRadianGreen));

	myrotationArrBlue[0] = lightPosition[2] * (sin(angleRadianBlue));
	myrotationArrBlue[1] = lightPosition[2] * (cos(angleRadianBlue));

	//apply z axis translation to go deep into the screen by -2.0
	//so that triangle with same fullscreen coordinated but due to above translation will look small

	modelMatrix = translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	SwapBuffers(ghdc);
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	gPerspectiveProjectionMatrix = perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
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
	angleBlue = angleBlue + 0.1f;
	if (angleBlue > 360.0f)
		angleBlue = 0.0f;

	angleGreen = angleGreen + 0.1f;
	if (angleGreen > 360.0f)
		angleGreen = 0.0f;

	angleRed = angleRed + 0.1;
	if (angleRed > 360.0f)
		angleRed = 0.0f;

	angleRadian = angleRed * (M_PI / 180.0);
	angleRadianGreen = angleGreen * (M_PI / 180.0);
	angleRadianBlue = angleBlue * (M_PI / 180.0);
}























