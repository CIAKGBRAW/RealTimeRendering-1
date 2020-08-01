////
////#include <Windows.h>
////#include<stdio.h>
////#include<gl/glew.h>
////#include<gl/GL.h>
////
////#include"vmath.h"
////
////#define WIN_WIDTH 800
////#define WIN_HEIGHT 600
////
////#pragma comment(lib,"glew32.lib")
////#pragma comment(lib,"opengl32.lib")
////
////using namespace vmath;
////
////enum
////{
////	VDG_ATTRIBUTE_VERTEX = 0,
////	VDG_ATTRIBUTE_COLOR,
////	VDG_ATTRIBUTE_NORMAL,
////	VDG_ATTRIBUTE_TEXTURE0,
////};
////
//////Prototype of WndProc() declared Globally
////LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
////
////
//////Global variable declarations
////FILE *gpFile = NULL;
////
////GLfloat gAnglePyramid = 0.0f;
////
////HWND ghwnd = NULL;
////HDC ghdc = NULL;
////HGLRC ghrc = NULL;
////
////
////DWORD dwStyle;
////WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
////
////bool gbActiveWindow = false;
////bool gbEscapeKeyIsPressed = false;
////bool gbFullScreen = false;
////
////GLuint gVertexShaderObject;
////GLuint gFragmentShaderObject;
////GLuint gShaderProgramObject;
////
////GLuint gVao_Pyramid;
////GLuint gVbo_Position_Pyramid;
////GLuint gVbo_Color_Pyramid;
////GLuint gVbo_Normal_Pyramid;
////
////
////GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;
////
////GLuint L_KeyPressed_uniform;
////GLuint L_KeyPressed_uniformRed;
////
////GLuint La_uniform;
////GLuint Ld_uniform;
////GLuint Ls_uniform;
////GLuint light_position_uniform;
////GLuint light_position_uniformRed;
////
////GLuint Ka_uniform;
////GLuint Kd_uniform;
////GLuint Ks_uniform;
////GLuint material_shininess_uniform;
////
////GLuint La_uniformRed;
////GLuint Ld_uniformRed;
////GLuint Ls_uniformRed;
////
////GLuint Ka_uniformRed;
////GLuint Kd_uniformRed;
////GLuint Ks_uniformRed;
////GLuint material_shininess_uniformRed;
////
////mat4 gPerspectiveProjectionMatrix;
////
////
////GLfloat light_Ambient[5] = { 0.0f,0.0f,0.0f,0.0f };
////GLfloat light_Diffuse[5] = { 1.0f,0.0f,0.0f,1.0f };
////GLfloat light_Specular[5] = { 1.0f,0.0f,0.0f,1.0f };
////GLfloat light_position[5] = { 2.0f,1.0f,1.0f,0.0f };
////
////GLfloat light_AmbientL1[5] = { 0.0f,0.0f,0.0f,0.0f };
////GLfloat light_DiffuseL1[5] = { 0.0f,0.0f,1.0f,1.0f };
////GLfloat light_SpecularL1[5] = { 0.0f,0.0f,1.0f,1.0f };
////GLfloat light_positionL1[5] = { -2.0f,1.0f,1.0f,1.0f };
////
////GLfloat material_ambient[5] = { 0.0f,0.0f,0.0f,0.0f };
////GLfloat material_diffuse[5] = { 1.0f,1.0f,1.0f,1.0f };
////GLfloat material_specular[5] = { 1.0f,1.0f,1.0f,1.0f };
////GLfloat material_shininess = 50.0f;
////
////bool gbLight;
////bool gbLight2;
////
//////main()
////int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
////{
////	//function prototype
////	void display();
////	void initialize();
////	void uninitialize(void);
////	void update();
////
////	//variable declaration
////	WNDCLASSEX wndclass;
////	HWND hwnd;
////	MSG msg;
////	TCHAR szClassName[] = TEXT("OpenGLPP");
////	bool bDone = false;
////
////	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
////	{
////		MessageBox(NULL, TEXT("Log File Can not be Created\Exitting.."), TEXT("ERROR"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
////		exit(0);
////	}
////	else
////	{
////		fprintf(gpFile, "Log file is Successfully Openend \n");
////	}
////
////	//code
////	//initializing member of WNDCLASS
////	wndclass.cbSize = sizeof(WNDCLASSEX);
////	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
////	wndclass.cbClsExtra = 0;
////	wndclass.cbWndExtra = 0;
////	wndclass.hInstance = hInstance;
////	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
////	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
////	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
////	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
////	wndclass.lpfnWndProc = WndProc;
////	wndclass.lpszClassName = szClassName;
////	wndclass.lpszMenuName = NULL;
////
////	//Registering Class
////	RegisterClassEx(&wndclass);
////
////	//CreateWindow
////	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
////		szClassName,
////		TEXT("OpenGL Programmable PipeLine Using Native Windowing: First ortho Trianle Window Shree Ganeshaya Namaha"),
////		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
////		0,
////		0,
////		WIN_WIDTH,
////		WIN_HEIGHT,
////		NULL,
////		NULL,
////		hInstance,
////		NULL
////		);
////
////	ghwnd = hwnd;
////
////	//intitialize()
////	initialize();
////
////	ShowWindow(hwnd, SW_SHOW);
////	SetForegroundWindow(hwnd);
////	SetFocus(hwnd);
////
////	//Message Loop
////	while (bDone == false)
////	{
////		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
////		{
////			if (msg.message == WM_QUIT)
////				bDone = true;
////			else
////			{
////				TranslateMessage(&msg);
////				DispatchMessage(&msg);
////			}
////		}
////		else
////		{
////			//display();
////			if (gbActiveWindow == true)
////			{
////				update();
////				display();
////
////				if (gbEscapeKeyIsPressed == true)
////					bDone = true;
////			}
////		}
////	}
////
////	uninitialize();
////	return((int)msg.wParam);
////
////}
////
//////WndProc()
////LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
////{
////	//function prototypes
////	//	void display();
////	void resize(int, int);
////	void ToggleFullScreen(void);
////	void uninitialize(void);
////
////	static bool gbIsLKeyPressed = false;
////
////	//code
////	switch (iMsg)
////	{
////	case WM_ACTIVATE:
////		if (HIWORD(wParam) == 0)
////			gbActiveWindow = true;
////		else
////			gbActiveWindow = false;
////		break;
////	case WM_PAINT:
////		//		display();
////		break;
////
////	case WM_SIZE:
////		resize(LOWORD(lParam), HIWORD(lParam));
////		break;
////
////	case WM_KEYDOWN:
////		switch (wParam)
////		{
////		case VK_ESCAPE:
////			gbEscapeKeyIsPressed = true;
////			break;
////
////		case 0x46:			//for 'f' or 'F'
////			if (gbFullScreen == false)
////			{
////				ToggleFullScreen();
////				gbFullScreen = true;
////			}
////			else
////			{
////				ToggleFullScreen();
////				gbFullScreen = false;
////			}
////			break;
////		case 0x4c:		//for l or L key (Light)
////			if (gbIsLKeyPressed == false)
////			{
////				gbLight = true;
////				gbLight2 = true;
////				gbIsLKeyPressed = true;
////			}
////			else
////			{
////				gbLight = false;
////				gbLight2 = false;
////				gbIsLKeyPressed = false;
////			}
////			break;
////		default:
////			break;
////		}
////		break;
////
////
////	case WM_LBUTTONDOWN:
////		break;
////
////	case WM_DESTROY:
////		PostQuitMessage(0);
////		break;
////
////	default:
////		break;
////	}
////	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
////
////}
////
////
////void ToggleFullScreen(void)
////{
////	//variable declarations
////	MONITORINFO mi;
////
////	//code
////	if (gbFullScreen == false)
////	{
////		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
////		if (dwStyle & WS_OVERLAPPEDWINDOW)
////		{
////			mi = { sizeof(MONITORINFO) };
////			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
////			{
////				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
////				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left,
////					mi.rcMonitor.top,
////					mi.rcMonitor.right - mi.rcMonitor.left,
////					mi.rcMonitor.bottom - mi.rcMonitor.top,
////					SWP_NOZORDER | SWP_FRAMECHANGED);
////			}
////
////		}
////		ShowCursor(FALSE);
////	}
////	else
////	{
////		//code
////		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
////		SetWindowPlacement(ghwnd, &wpPrev);
////		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
////		ShowCursor(TRUE);
////	}
////}
////
////void initialize()
////{
////	//function prototype
////	void uninitialize(void);
////	void resize(int, int);
////
////	//variable declaration
////	PIXELFORMATDESCRIPTOR pfd;
////	int iPixelFormatIndex;
////
////	//code
////	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
////
////	//Initialization code
////	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
////	pfd.nVersion = 1;
////	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
////	pfd.iPixelType = PFD_TYPE_RGBA;
////	pfd.cColorBits = 8;
////	pfd.cRedBits = 8;
////	pfd.cGreenBits = 8;
////	pfd.cBlueBits = 8;
////	pfd.cAlphaBits = 8;
////
////	ghdc = GetDC(ghwnd);
////
////	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
////	if (iPixelFormatIndex == 0)
////	{
////		ReleaseDC(ghwnd, ghdc);
////		ghdc = NULL;
////	}
////	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
////	{
////		ReleaseDC(ghwnd, ghdc);
////		ghdc = NULL;
////	}
////
////	ghrc = wglCreateContext(ghdc);
////	if (ghrc == NULL)
////	{
////		ReleaseDC(ghwnd, ghdc);
////		ghdc = NULL;
////	}
////
////	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
////	{
////		wglDeleteContext(ghrc);
////		ghrc = NULL;
////		ReleaseDC(ghwnd, ghdc);
////		ghdc = NULL;
////	}
////
////	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
////	GLenum glew_error = glewInit();
////	if (glew_error != GLEW_OK)
////	{
////		wglDeleteContext(ghrc);
////		ghrc = NULL;
////		ReleaseDC(ghwnd, ghdc);
////		ghdc = NULL;
////	}
////
////	//******** VERTEX SHADER ******
////	//create shader
////	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
////
////	//provide source code to shader
////	const GLchar *vertexShaderSourceCode =
////		"#version 430 core"	\
////		"\n" \
////		"in vec4 vPosition;" \
////		"in vec3 vNormal;" \
////		"uniform mat4 u_model_matrix;" \
////		"uniform mat4 u_view_matrix;" \
////		"uniform mat4 u_projection_matrix;" \
////		"uniform int u_lighting_enabledBlue;" \
////		"uniform int u_lighting_enabledRed;" \
////		"uniform vec3 u_LaBlue;" \
////		"uniform vec3 u_LdBlue;" \
////		"uniform vec3 u_LsBlue;" \
////
////		"uniform vec3 u_LaRed;" \
////		"uniform vec3 u_LsRed;" \
////		"uniform vec3 u_LdRed;" \
////
////		"uniform vec4 u_light_positionBlue;" \
////		"uniform vec4 u_light_positionRed;" \
////		"uniform vec3 u_KaBlue;" \
////		"uniform vec3 u_KdBlue;" \
////		"uniform vec3 u_KsBlue;" \
////
////		"uniform vec3 u_KaRed;" \
////		"uniform vec3 u_KdRed;" \
////		"uniform vec3 u_KsRed;" \
////		"uniform float u_material_shininess;" \
////		"uniform float u_material_shininessRed;" \
////
////		"out vec3 phong_ads_colorBlue;" \
////		"out vec3 phong_ads_colorRed;" \
////		"void main(void)" \
////		"{"
////		"vec4 eye_coordinates= u_view_matrix * u_model_matrix *vPosition;" \
////		"vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
////		"vec3 viewer_vector = normalize(-eye_coordinates.xyz);" \
////		"if(u_lighting_enabledRed == 1)" \
////		"{" \
////		"vec3 light_directionRed = normalize(vec3(u_light_positionRed) - eye_coordinates.xyz);"\
////		"float tn_dot_ld = max(dot(transformed_normals,light_directionRed),0.0);" \
////		"vec3 ambientRed = u_LaRed * u_KaRed;" \
////		"vec3 diffuseRed = u_LdRed * u_KdRed * tn_dot_ld;" \
////		"vec3 reflection_vector = reflect(-light_directionRed,transformed_normals);"\
////		"vec3 specularRed =  u_LsRed * u_KsRed * pow(max(dot(reflection_vector,viewer_vector),0.0),u_material_shininessRed);" \
////		"phong_ads_colorRed = ambientRed + diffuseRed + specularRed;" \
////
////		"vec3 light_directionBlue = normalize(vec3(u_light_positionBlue) - eye_coordinates.xyz);"\
////		"float tn_dot_ldBlue = max(dot(transformed_normals,light_directionBlue),0.0);" \
////		"vec3 ambientBlue = u_LaBlue * u_KaBlue;" \
////		"vec3 diffuseBlue = u_LdBlue * u_KdBlue * tn_dot_ldBlue;" \
////		"vec3 reflection_vectorBlue = reflect(-light_directionBlue,transformed_normals);"\
////		"vec3 specularBlue = u_LsBlue * u_KsBlue * pow(max(dot(reflection_vectorBlue,viewer_vector),0.0),u_material_shininess);" \
////		"phong_ads_colorBlue = ambientBlue + diffuseBlue + specularBlue;" \
////
////		"}" \
////	
////		"gl_Position =u_projection_matrix *  u_view_matrix * u_model_matrix * vPosition;" \
////		"}";
////
////
////	glShaderSource(gVertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);
////
////	//compile shader
////	glCompileShader(gVertexShaderObject);
////	GLint iInfoLength = 0;
////	GLint iShaderCompiledStatus = 0;
////	char *szInfoLog = NULL;
////
////	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
////	if (iShaderCompiledStatus == GL_FALSE)
////	{
////		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLength);
////		if (iInfoLength > 0)
////		{
////			szInfoLog = (char *)malloc(iInfoLength);
////			if (szInfoLog != NULL)
////			{
////				GLsizei written;
////				glGetShaderInfoLog(gVertexShaderObject, iInfoLength, &written, szInfoLog);
////				fprintf(gpFile, "Vertex Shader Compilation Log:%s\n", szInfoLog);
////				free(szInfoLog);
////				uninitialize();
////				exit(0);
////			}
////		}
////	}
////
////	//**********FRAGMENT SHADER*********
////	//create shader
////	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
////
////	//provide source code to shader
////	const GLchar *fragmentShaderSourceCode =
////		"#version 430 core" \
////		"\n" \
////		"in vec3 phong_ads_colorBlue;" \
////		"in vec3 phong_ads_colorRed;" \
////		"out vec4 FragColorBlue;" \
////		"out vec4 FragColorRed;" \
////		"uniform int u_lighting_enabledBlue;" \
////		"uniform int u_lighting_enabledRed;" \
////		"void main(void)" \
////		"{" \
////		"if(u_lighting_enabledRed == 1)" \
////		"{" \
////		"FragColorRed = vec4(phong_ads_colorRed,1.0);" \
////		"FragColorBlue = vec4(phong_ads_colorBlue,1.0);" \
////
////		"}" \
////		"FragColorBlue = vec4(phong_ads_colorBlue,1.0);" \
////		"}";
////
////	glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
////
////	//compile shader
////	glCompileShader(gFragmentShaderObject);
////	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
////	if (iShaderCompiledStatus == GL_FALSE)
////	{
////		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iShaderCompiledStatus);
////		if (iInfoLength > 0)
////		{
////			szInfoLog = (char *)malloc(iInfoLength);
////			if (szInfoLog != NULL)
////			{
////				GLsizei written;
////				glGetShaderInfoLog(gFragmentShaderObject, iInfoLength, &written, szInfoLog);
////				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
////				free(szInfoLog);
////				uninitialize();
////				exit(0);
////			}
////		}
////	}
////
////	//********** SHADER PROGRAM *********
////	//create
////	gShaderProgramObject = glCreateProgram();
////
////	//attach vertex shader to shader program
////	glAttachShader(gShaderProgramObject, gVertexShaderObject);
////
////	//attach fragment shader to shader program
////	glAttachShader(gShaderProgramObject, gFragmentShaderObject);
////
////	//pre-link binding of shader program object with vertex position attribute
////	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_VERTEX, "vPosition");
////
////	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_NORMAL, "vNormal");
////
////	//Link Shader
////	glLinkProgram(gShaderProgramObject);
////	GLint iShaderProgramLinkStatus = 0;
////	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
////	if (iShaderProgramLinkStatus == GL_FALSE)
////	{
////		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLength);
////		if (iInfoLength > 0)
////		{
////			szInfoLog = (char *)malloc(iInfoLength);
////			if (szInfoLog != NULL)
////			{
////				GLsizei written;
////				glGetProgramInfoLog(gShaderProgramObject, iInfoLength, &written, szInfoLog);
////				fprintf(gpFile, "Shader Program LinK Log %s\n");
////				free(szInfoLog);
////				uninitialize();
////				exit(0);
////			}
////		}
////	}
////
////
////	//get MVP uniform locaion
////	model_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_model_matrix");
////	view_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_view_matrix");
////	projection_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");
////	//L/I Key is pressed or not
////	L_KeyPressed_uniform = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabledBlue");
////
//////	L_KeyPressed_uniform2 = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabled2");
////
////	//ambient color intensity of LIGHT
////	La_uniform = glGetUniformLocation(gShaderProgramObject, "u_LaBlue");
////	//diffuse color intensity of LIGHT
////	Ld_uniform = glGetUniformLocation(gShaderProgramObject, "u_LdBlue");
////	//specular color intensity of LIGHT
////	Ls_uniform = glGetUniformLocation(gShaderProgramObject, "u_LsBlue");
////	//position of light
////	light_position_uniform = glGetUniformLocation(gShaderProgramObject, "u_light_positionBlue");
////
////	//ambient reflective color intensity of MATERIAL
////	Ka_uniform = glGetUniformLocation(gShaderProgramObject, "u_KaBlue");
////	//diffuse reflective color intensity of MATERIAL
////	Kd_uniform = glGetUniformLocation(gShaderProgramObject, "u_KdBlue");
////	//specular reflective color intensity of MATERIAL
////	Ks_uniform = glGetUniformLocation(gShaderProgramObject, "u_KsBlue");
////
////	//shininess of material (value is conventionally between 1 to 200)
////	material_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material_shininess");
////
////	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	La_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LaRed");
////
////	Ls_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LsRed");
////
////	Ld_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LdRed");
////
////	Ka_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KaRed");
////
////	Ks_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KsRed");
////
////	Kd_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KdRed");
////
////	light_position_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_light_positionRed");
////
////	material_shininess_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_material_shininessRed");
////
////	L_KeyPressed_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabledRed");
////	//**** vertices, colors, shader attribs, vbo, vao initializations ****//
////
////	const GLfloat pyramidVertices[] =
////	{
////		//FRONT FACE
////		0.0f, 1.0f, 0.0f,
////		-1.0f, -1.0f, 1.0f,
////		1.0f, -1.0f, 1.0f,
////
////		//RIGHT FACE
////		0.0f, 1.0f, 0.0f,
////		1.0f, -1.0f, 1.0f,
////		1.0f, -1.0f, -1.0f,
////
////		//LEFT FACE
////		0.0f, 1.0f, 0.0f,
////		-1.0f, -1.0f, 1.0f,
////		-1.0f, -1.0f, -1.0f,
////
////		//BACK FACE
////		0.0f, 1.0f, 0.0f,
////		-1.0f, -1.0f, -1.0f,
////		1.0f, -1.0f, -1.0f
////	};
////
////	const GLfloat pyramidNormals[] =
////	{
////		//FRONT FACE
////		0.0f,0.447214f,0.894427f,
////
////		//RIGHT FACE
////		0.894427f, 0.447214f, 0.0f,
////
////		//LEFT FACE
////		-0.894427f, 0.447214f, 0.0f,
////
////		//BACK FACE
////		0.0f, 0.447214f, -0.894427f
////	};
////	
////
////
////	//Start of VAO Pyramid
////	glGenVertexArrays(1, &gVao_Pyramid);
////	glBindVertexArray(gVao_Pyramid);
////	//start of VboPositio Pyramid 
////	glGenBuffers(1, &gVbo_Position_Pyramid);
////	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Position_Pyramid);
////	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
////	glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);
////	glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);
////
////	glBindBuffer(GL_ARRAY_BUFFER, 0);
////	//fininshing/completion of above gVboPosition buffer state
////
////	//VBO For colors
////	//start of vboColor Pyramid 
////	glGenBuffers(1, &gVbo_Normal_Pyramid);
////	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Normal_Pyramid);
////	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidNormals), pyramidNormals, GL_STATIC_DRAW);
////	glVertexAttribPointer(VDG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
////	glEnableVertexAttribArray(VDG_ATTRIBUTE_NORMAL);
////
////	//glBindBuffer(GL_ARRAY_BUFFER, 0);	//fininshing/completion of above gVboColor buffer state
////
////	glBindVertexArray(0);		
////
////	glShadeModel(GL_FLAT);
////	glClearDepth(1.0f);
////	glEnable(GL_DEPTH_TEST);
////	glDepthFunc(GL_LEQUAL);
////	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
////	glEnable(GL_CULL_FACE);
////	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
////	gbLight = false;
////
////	gPerspectiveProjectionMatrix = mat4::identity();
////
////	resize(WIN_WIDTH, WIN_HEIGHT);
////
////}
////
////void display(void)
////{
////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////
////	//start using OpenGL program Object
////	glUseProgram(gShaderProgramObject);
////	if (gbLight == true)
////	{		//set u_lighting_enabled uniform
////		glUniform1i(L_KeyPressed_uniformRed, 1);
////
////		//setting lights properties
////		glUniform3fv(La_uniform, 1, light_AmbientL1);
////		glUniform3fv(Ld_uniform, 1, light_DiffuseL1);
////		glUniform3fv(Ls_uniform, 1, light_SpecularL1);
////		glUniform4fv(light_position_uniform, 1, light_positionL1);
////
////		glUniform3fv(Ka_uniform, 1, material_ambient);
////		glUniform3fv(Kd_uniform, 1, material_diffuse);
////		glUniform3fv(Ks_uniform, 1, material_specular);
////		glUniform1f(material_shininess_uniform, material_shininess);
////
////		glUniform3fv(La_uniformRed, 1, light_Ambient);
////		glUniform3fv(Ld_uniformRed, 1, light_Diffuse);
////		glUniform3fv(Ls_uniformRed, 1, light_Specular);
////		glUniform4fv(light_position_uniformRed, 1, light_position);
////
////		glUniform3fv(Ka_uniformRed, 1, material_ambient);
////		glUniform3fv(Kd_uniformRed, 1, material_diffuse);
////		glUniform3fv(Ks_uniformRed, 1, material_specular);
////		glUniform1f(material_shininess_uniformRed, material_shininess);
////
////		//glUniform3fv(La_uniform, 1, light_Ambient);
////		//glUniform3fv(Ld_uniform, 1, light_Diffuse);
////		//glUniform3fv(Ls_uniform, 1, light_Specular);
////		//GLfloat light_position[5] = { 2.0f,1.0f,1.0f,0.0f };
////
////		//glUniform4fv(light_position_uniform, 1, light_position);
////
////		//////setting material's properties
////
////		//glUniform3fv(Ka_uniform, 1, material_ambient);
////		//glUniform3fv(Kd_uniform, 1, material_diffuse);
////		//glUniform3fv(Ks_uniform, 1, material_specular);
////		//glUniform1f(material_shininess_uniform, material_shininess);
////	//	glUniform1i(L_KeyPressed_uniform2, 1);
////
////		//glUniform1i(L_KeyPressed_uniform, 1);
////		//glUniform3fv(La_uniform, 1, light_Ambient);
////		//glUniform3fv(Ld_uniform, 1, light_Diffuse);
////		//glUniform3fv(Ls_uniform, 1, light_Specular);
////		//glUniform4fv(light_position_uniform, 1, light_position);
////
////		//glUniform3fv(Ka_uniform, 1, material_ambient);
////		//glUniform3fv(Kd_uniform, 1, material_diffuse);
////		//glUniform3fv(Ks_uniform, 1, material_specular);
////		//glUniform1f(material_shininess_uniform1, material_shininess);
////	}
////	//glUseProgram(0);
////	//glUseProgram(gShaderProgramObject);
////
////	 glUseProgram(0);
////
////	//else
////	//{
////	//	//set 'u_lighting_enabled' uniform
////	//	glUniform1i(L_KeyPressed_uniform, 0);
////	//}
////	//OpenL Drawing 
////	//set modelview & modelviewprojection matrices to identity 
////	glUseProgram(gShaderProgramObject);
////
////	mat4 modelMatrix = mat4::identity();
////	mat4 viewMatrix = mat4::identity();
////	mat4 projectionMatrix = mat4::identity();
////	mat4 rotationmatrix = mat4::identity();
////
////	modelMatrix = translate(0.0f, 0.0f, -6.0f);
//////	rotationmatrix = vmath::rotate(gAnglePyramid, 0.0f, 1.0f, 0.0f);
////
////	modelMatrix = modelMatrix * rotationmatrix;
////
////	//multipley the modelview and orthographic matrix to get modelviewprojection matrix
////	projectionMatrix = gPerspectiveProjectionMatrix * modelMatrix;	//ORDER IS IMPORTANT
////																				//pass above modelviewprojection matrix to the vertex shader in 'u_mvp_matrix' shader variable
////																				//whose position value we already calculated in initWithFrame() by using glGetUniformLocation()
////
////	glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
////	glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
////	glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);
////
////
////	//*** bind vao ****
////	glBindVertexArray(gVao_Pyramid);
////
////	// ****  draw either by glDrawTriangles() or glDrawarrays() or glDrawElements()
////	glDrawArrays(GL_TRIANGLES, 0, 12);			//(each with its x,y,z ) vertices in triangleVertices array
////
////												//**** unbind vaoTriangle ****
////	glBindVertexArray(0);
////													//stop using OpenGL program Object
////	glUseProgram(0);
////
////	SwapBuffers(ghdc);
////
////}
////
////void resize(int width, int height)
////{
////	//code
////	if (height == 0)
////		height = 1;
////	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
////
////	gPerspectiveProjectionMatrix = perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
////}
////
////void uninitialize(void)
////{
////	//UNINITIALIZATION CODE
////	if (gbFullScreen == true)
////	{
////		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
////		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
////		SetWindowPlacement(ghwnd, &wpPrev);
////		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
////		ShowCursor(TRUE);
////	}
////
////	//destroy vao
////	if (gVao_Pyramid)
////	{
////		glDeleteVertexArrays(1, &gVao_Pyramid);
////		gVao_Pyramid = 0;
////	}
////
////	//destroy vbo
////	if (gVbo_Position_Pyramid)
////	{
////		glDeleteVertexArrays(1, &gVbo_Position_Pyramid);
////		gVbo_Position_Pyramid = 0;
////	}
////
////	if (gVbo_Normal_Pyramid)
////	{
////		glDeleteVertexArrays(1, &gVbo_Normal_Pyramid);
////		gVbo_Normal_Pyramid = 0;
////	}
////
////	//detach vertex shader from shader program object
////	glDetachShader(gShaderProgramObject, gVertexShaderObject);
////
////	//detach fragment shader from shader program object
////	glDetachShader(gShaderProgramObject, gFragmentShaderObject);
////
////	//delete vertex shader object 
////	glDeleteShader(gVertexShaderObject);
////	gVertexShaderObject = 0;
////
////	//delete fragment shader object
////	glDeleteShader(gFragmentShaderObject);
////	gFragmentShaderObject = 0;
////
////	//unlink shader program
////	glUseProgram(0);
////
////	wglMakeCurrent(NULL, NULL);
////
////	wglDeleteContext(ghrc);
////	ghrc = NULL;
////
////	ReleaseDC(ghwnd, ghdc);
////	ghdc = NULL;
////
////	DestroyWindow(ghwnd);
////	ghwnd = NULL;
////
////}
////
////
////
////void update()
////{
////	gAnglePyramid = gAnglePyramid + 0.1f;
////	if (gAnglePyramid >= 360.0f)
////		gAnglePyramid = 0.0f;
////
////}
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////





#include <Windows.h>
#include<stdio.h>
#include<gl/glew.h>
#include<gl/GL.h>

#include"vmath.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")

using namespace vmath;

enum
{
	VDG_ATTRIBUTE_VERTEX = 0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0,
};

//Prototype of WndProc() declared Globally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//Global variable declarations
FILE *gpFile = NULL;

GLfloat gAnglePyramid = 0.0f;

HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;


DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullScreen = false;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint gVao_Pyramid;
GLuint gVbo_Position_Pyramid;
GLuint gVbo_Color_Pyramid;
GLuint gVbo_Normal_Pyramid;


GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;

GLuint L_KeyPressed_uniform;
GLuint L_KeyPressed_uniformRed;

GLuint La_uniform;
GLuint Ld_uniform;
GLuint Ls_uniform;
GLuint light_position_uniform;
GLuint light_position_uniformRed;

GLuint Ka_uniform;
GLuint Kd_uniform;
GLuint Ks_uniform;
GLuint material_shininess_uniform;

GLuint La_uniformRed;
GLuint Ld_uniformRed;
GLuint Ls_uniformRed;

GLuint Ka_uniformRed;
GLuint Kd_uniformRed;
GLuint Ks_uniformRed;
GLuint material_shininess_uniformRed;

mat4 gPerspectiveProjectionMatrix;


GLfloat light_Ambient[5] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light_Diffuse[5] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light_Specular[5] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light_position[5] = { 2.0f,1.0f,1.0f,0.0f };

GLfloat light_AmbientL1[5] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light_DiffuseL1[5] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat light_SpecularL1[5] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat light_positionL1[5] = { -2.0f,1.0f,1.0f,1.0f };

GLfloat material_ambient[5] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat material_diffuse[5] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[5] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;

bool gbLight;
bool gbLight2;

//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//function prototype
	void display();
	void initialize();
	void uninitialize(void);
	void update();

	//variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("OpenGLPP");
	bool bDone = false;

	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log File Can not be Created\Exitting.."), TEXT("ERROR"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log file is Successfully Openend \n");
	}

	//code
	//initializing member of WNDCLASS
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

	//Registering Class
	RegisterClassEx(&wndclass);

	//CreateWindow
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szClassName,
		TEXT("OpenGL Programmable PipeLine Using Native Windowing: First ortho Trianle Window Shree Ganeshaya Namaha"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		0,
		0,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ghwnd = hwnd;

	//intitialize()
	initialize();

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//Message Loop
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
			//display();
			if (gbActiveWindow == true)
			{
				update();
				display();

				if (gbEscapeKeyIsPressed == true)
					bDone = true;
			}
		}
	}

	uninitialize();
	return((int)msg.wParam);

}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function prototypes
	//	void display();
	void resize(int, int);
	void ToggleFullScreen(void);
	void uninitialize(void);

	static bool gbIsLKeyPressed = false;
	static bool gIsRKeyPressed = false;
	//code
	switch (iMsg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			gbActiveWindow = true;
		else
			gbActiveWindow = false;
		break;
	case WM_PAINT:
		//		display();
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = true;
			break;

		case 0x46:			//for 'f' or 'F'
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
				gbLight= false;
				gbIsLKeyPressed = false;
			}
			break;
			default:
			break;
		}
		break;


	case WM_LBUTTONDOWN:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}


void ToggleFullScreen(void)
{
	//variable declarations
	MONITORINFO mi;

	//code
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
		//code
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

void initialize()
{
	//function prototype
	void uninitialize(void);
	void resize(int, int);

	//variable declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	//Initialization code
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

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//******** VERTEX SHADER ******
	//create shader
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to shader
	const GLchar *vertexShaderSourceCode =
		"#version 430 core"	\
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform int u_lighting_enabledBlue;" \
		"uniform int u_lighting_enabledRed;" \
		"uniform vec3 u_LaBlue;" \
		"uniform vec3 u_LdBlue;" \
		"uniform vec3 u_LsBlue;" \

		"uniform vec3 u_LaRed;" \
		"uniform vec3 u_LsRed;" \
		"uniform vec3 u_LdRed;" \

		"uniform vec4 u_light_positionBlue;" \
		"uniform vec4 u_light_positionRed;" \
		"uniform vec3 u_KaBlue;" \
		"uniform vec3 u_KdBlue;" \
		"uniform vec3 u_KsBlue;" \

		"uniform vec3 u_KaRed;" \
		"uniform vec3 u_KdRed;" \
		"uniform vec3 u_KsRed;" \
		"uniform float u_material_shininess;" \
		"uniform float u_material_shininessRed;" \

		"out vec3 phong_ads_colorBlue;" \
		"out vec3 phong_ads_colorRed;" \
		"void main(void)" \
		"{"
		"vec4 eye_coordinates= u_view_matrix * u_model_matrix *vPosition;" \
		"vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
		"vec3 viewer_vector = normalize(-eye_coordinates.xyz);" \
		"if(u_lighting_enabledRed == 1)" \
		"{" \
		"vec3 light_directionRed = normalize(vec3(u_light_positionRed) - eye_coordinates.xyz);"\
		"float tn_dot_ld = max(dot(transformed_normals,light_directionRed),0.0);" \
		"vec3 ambientRed = u_LaRed * u_KaRed;" \
		"vec3 diffuseRed = u_LdRed * u_KdRed * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-light_directionRed,transformed_normals);"\
		"vec3 specularRed =  u_LsRed * u_KsRed * pow(max(dot(reflection_vector,viewer_vector),0.0),u_material_shininessRed);" \
		"phong_ads_colorRed = ambientRed + diffuseRed + specularRed;" \
		
		"vec3 light_directionBlue = normalize(vec3(u_light_positionBlue) - eye_coordinates.xyz);"\
		"float tn_dot_ldBlue = max(dot(transformed_normals,light_directionBlue),0.0);" \
		"vec3 ambientBlue = u_LaBlue * u_KaBlue;" \
		"vec3 diffuseBlue = u_LdBlue * u_KdBlue * tn_dot_ldBlue;" \
		"vec3 reflection_vectorBlue = reflect(-light_directionBlue,transformed_normals);"\
		"vec3 specularBlue = u_LsBlue * u_KsBlue * pow(max(dot(reflection_vectorBlue,viewer_vector),0.0),u_material_shininess);" \
		"phong_ads_colorBlue = ambientBlue + diffuseBlue + specularBlue;" \
		"}" \

		"gl_Position =u_projection_matrix *  u_view_matrix * u_model_matrix * vPosition;" \
		"}";


	glShaderSource(gVertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject);
	GLint iInfoLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;

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

	//**********FRAGMENT SHADER*********
	//create shader
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to shader
	const GLchar *fragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"in vec3 phong_ads_colorBlue;" \
		"in vec3 phong_ads_colorRed;" \
		"out vec4 light;" \
		" vec4 FragColorRed;" \
		" vec4 FragColorBlue;" \
		"uniform int u_lighting_enabledBlue;" \
		"uniform int u_lighting_enabledRed;" \
		"void main(void)" \
		"{" \
		"if(u_lighting_enabledRed == 1)" \
		"{" \
		"FragColorRed = vec4 (phong_ads_colorRed,1.0);" \
		"FragColorBlue = vec4(phong_ads_colorBlue,1.0);" \
		"light = FragColorRed + FragColorBlue ;" \
		"}" \
		"else"\
		"{" \
		"light=vec4(1,1,1,1); "\
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
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//********** SHADER PROGRAM *********
	//create
	gShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	//pre-link binding of shader program object with vertex position attribute
	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_VERTEX, "vPosition");

	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_NORMAL, "vNormal");

	//Link Shader
	glLinkProgram(gShaderProgramObject);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program LinK Log %s\n");
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	//get MVP uniform locaion
	model_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_model_matrix");
	view_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_view_matrix");
	projection_matrix_uniform = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");
	//L/I Key is pressed or not
//	glBindFragDataLocation(gShaderProgramObject, 1, "FragColorBlue");
//	L_KeyPressed_uniform = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabledBlue");
//	L_KeyPressed_uniform = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabledBlue");

	//L_KeyPressed_uniform = glGetFragDataLocation(gShaderProgramObject, "FragColorBlue");

	//	L_KeyPressed_uniform2 = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabled2");

	//ambient color intensity of LIGHT
	La_uniform = glGetUniformLocation(gShaderProgramObject, "u_LaBlue");
	//diffuse color intensity of LIGHT
	Ld_uniform = glGetUniformLocation(gShaderProgramObject, "u_LdBlue");
	//specular color intensity of LIGHT
	Ls_uniform = glGetUniformLocation(gShaderProgramObject, "u_LsBlue");
	//position of light
	light_position_uniform = glGetUniformLocation(gShaderProgramObject, "u_light_positionBlue");

	//ambient reflective color intensity of MATERIAL
	Ka_uniform = glGetUniformLocation(gShaderProgramObject, "u_KaBlue");
	//diffuse reflective color intensity of MATERIAL
	Kd_uniform = glGetUniformLocation(gShaderProgramObject, "u_KdBlue");
	//specular reflective color intensity of MATERIAL
	Ks_uniform = glGetUniformLocation(gShaderProgramObject, "u_KsBlue");

	//shininess of material (value is conventionally between 1 to 200)
	material_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material_shininess");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	La_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LaRed");

	Ls_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LsRed");

	Ld_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_LdRed");

	Ka_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KaRed");

	Ks_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KsRed");

	Kd_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_KdRed");

	light_position_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_light_positionRed");

	material_shininess_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_material_shininessRed");

	L_KeyPressed_uniformRed = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabledRed");
//	glBindFragDataLocation(gShaderProgramObject, 1, "FragColorBlue");
//	L_KeyPressed_uniformRed = glGetFragDataLocation(gShaderProgramObject, "FragColorRed");
	//**** vertices, colors, shader attribs, vbo, vao initializations ****//

	const GLfloat pyramidVertices[] =
	{
		//FRONT FACE
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		//RIGHT FACE
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		//LEFT FACE
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,

		//BACK FACE
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f
	};

	const GLfloat pyramidNormals[] =
	{
		//FRONT FACE
		0.0f,0.447214f,0.894427f,
		0.0f,0.447214f,0.894427f,
		0.0f,0.447214f,0.894427f,

		//RIGHT FACE
		0.894427f, 0.447214f, 0.0f,
		0.894427f, 0.447214f, 0.0f,
		0.894427f, 0.447214f, 0.0f,

		//LEFT FACE
		-0.894427f, 0.447214f, 0.0f,
		-0.894427f, 0.447214f, 0.0f,
		-0.894427f, 0.447214f, 0.0f,

		//BACK FACE
		0.0f, 0.447214f, -0.894427f,
		0.0f, 0.447214f, -0.894427f,
		0.0f, 0.447214f, -0.894427f,
		
	};



	//Start of VAO Pyramid
	glGenVertexArrays(1, &gVao_Pyramid);
	glBindVertexArray(gVao_Pyramid);
	//start of VboPositio Pyramid 
	glGenBuffers(1, &gVbo_Position_Pyramid);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Position_Pyramid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//fininshing/completion of above gVboPosition buffer state

	//VBO For colors
	//start of vboColor Pyramid 
	glGenBuffers(1, &gVbo_Normal_Pyramid);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Normal_Pyramid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidNormals), pyramidNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_NORMAL);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);	//fininshing/completion of above gVboColor buffer state
	glBindVertexArray(0);

	glShadeModel(GL_FLAT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gbLight = false;
	gbLight2 = false;
	gPerspectiveProjectionMatrix = mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//start using OpenGL program Object
	glUseProgram(gShaderProgramObject);
	if (gbLight == true)
	{		//set u_lighting_enabled uniform

//		glUniform1i(L_KeyPressed_uniform, 1);

	//	setting lights properties
		glUniform3fv(La_uniform, 1, light_AmbientL1);
		glUniform3fv(Ld_uniform, 1, light_DiffuseL1);
		glUniform3fv(Ls_uniform, 1, light_SpecularL1);
		glUniform4fv(light_position_uniform, 1, light_positionL1);

		glUniform3fv(Ka_uniform, 1, material_ambient);
		glUniform3fv(Kd_uniform, 1, material_diffuse);
		glUniform3fv(Ks_uniform, 1, material_specular);
		glUniform1f(material_shininess_uniform, material_shininess);

		glUniform1i(L_KeyPressed_uniformRed, 1);

		glUniform3fv(La_uniformRed, 1, light_Ambient);
		glUniform3fv(Ld_uniformRed, 1, light_Diffuse);
		glUniform3fv(Ls_uniformRed, 1, light_Specular);
		glUniform4fv(light_position_uniformRed, 1, light_position);

		glUniform3fv(Ka_uniformRed, 1, material_ambient);
		glUniform3fv(Kd_uniformRed, 1, material_diffuse);
		glUniform3fv(Ks_uniformRed, 1, material_specular);
		glUniform1f(material_shininess_uniformRed, material_shininess);
	}

	
	//glUseProgram(0);
	//glUseProgram(gShaderProgramObject);

//	glUseProgram(0);

	//else
	//{
	//	//set 'u_lighting_enabled' uniform
	//	glUniform1i(L_KeyPressed_uniform, 0);
	//}
	//OpenL Drawing 
	//set modelview & modelviewprojection matrices to identity 
	glUseProgram(gShaderProgramObject);

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 projectionMatrix = mat4::identity();
	mat4 rotationmatrix = mat4::identity();

	modelMatrix = translate(0.0f, 0.0f, -6.0f);
	rotationmatrix = vmath::rotate(gAnglePyramid, 0.0f, 1.0f, 0.0f);



	modelMatrix = modelMatrix * rotationmatrix;

	//multipley the modelview and orthographic matrix to get modelviewprojection matrix
	projectionMatrix = gPerspectiveProjectionMatrix * modelMatrix;	//ORDER IS IMPORTANT
																	//pass above modelviewprojection matrix to the vertex shader in 'u_mvp_matrix' shader variable
																	//whose position value we already calculated in initWithFrame() by using glGetUniformLocation()

	glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);


	//*** bind vao ****
	glBindVertexArray(gVao_Pyramid);

	// ****  draw either by glDrawTriangles() or glDrawarrays() or glDrawElements()
	glDrawArrays(GL_TRIANGLES, 0, 12);			//(each with its x,y,z ) vertices in triangleVertices array

												//**** unbind vaoTriangle ****
	glBindVertexArray(0);
	//stop using OpenGL program Object
	glUseProgram(0);

	SwapBuffers(ghdc);

}

void resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gPerspectiveProjectionMatrix = perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void uninitialize(void)
{
	//UNINITIALIZATION CODE
	if (gbFullScreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}

	//destroy vao
	if (gVao_Pyramid)
	{
		glDeleteVertexArrays(1, &gVao_Pyramid);
		gVao_Pyramid = 0;
	}

	//destroy vbo
	if (gVbo_Position_Pyramid)
	{
		glDeleteVertexArrays(1, &gVbo_Position_Pyramid);
		gVbo_Position_Pyramid = 0;
	}

	if (gVbo_Normal_Pyramid)
	{
		glDeleteVertexArrays(1, &gVbo_Normal_Pyramid);
		gVbo_Normal_Pyramid = 0;
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
	gAnglePyramid = gAnglePyramid + 0.1f;
	if (gAnglePyramid >= 360.0f)
		gAnglePyramid = 0.0f;

}























































