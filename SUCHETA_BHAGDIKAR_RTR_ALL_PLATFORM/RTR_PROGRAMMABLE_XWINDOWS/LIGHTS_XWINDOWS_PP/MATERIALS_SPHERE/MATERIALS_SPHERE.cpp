#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>


#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>

#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<vector>
#include"vmath.h"
#include"Sphere.h"


using namespace vmath;

#define WIN_WIDHT 1920
#define WIN_HEIGHT 1080



#define NUMBER_OF_SPHERES 24

Display *gpDisplay =NULL;
XVisualInfo *gpXVisualInfo=NULL;
Colormap gColormap;
Window gWindow;
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *,GLXFBConfig,GLXContext,Bool,const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB=NULL;
GLXFBConfig gGLXFBConfig;
GLXContext gGLXContext;


enum
{

	VDG_ATTRIBUTE_VERTEX=0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0
};


bool gbFullscreen = false;
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

mat4 modelMatrix = mat4::identity();
mat4 viewMatrix = mat4::identity();
GLuint L_KeyPressed_uniform;

GLuint La_uniform;
GLuint Ld_uniform;
GLuint Ls_uniform;
GLuint light_position_uniform;

GLuint Ka_uniform;
GLuint Kd_uniform;
GLuint Ks_uniform;
GLuint material_shininess_uniform;

mat4 gPerspectiveProjection;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.0f;

GLfloat myArrX[4];
GLfloat myArrY[4];
GLfloat myArrZ[4];

bool gbRotateX = false;
bool gbRotateY = false;
bool gbRotateZ = false;


GLfloat angleRadianX = 0.0;
GLfloat angleRadianY = 0.0;
GLfloat angleRadianZ = 0.0;

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;

GLfloat lightPositionX[] = { 100.0f,0.0f,0.0f,1.0f };
GLfloat lightPositionY[] = { 0.0f,100.0f,0.0f,1.0f };
GLfloat lightPositionZ[] = { 0.0f,0.0f,100.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,0.0f,1.0f };
using std::vector;

std::vector <vector<GLfloat> > g_Spheres;
std::vector<GLfloat> materialSpheres;

bool gbLight;

FILE *gpFile = NULL;

int main()
{

	void CreateWindow();
	void ToggleFullscreen();
	void initialize();
	void resize(int,int);
	void display();
	void uninitialize();
	void update();

	static bool gbIsLKeyPressed = false;
	static bool gbIsAKeyPressed = false;

	gpFile = fopen("Log.txt","w");
	if(gpFile == NULL)
	{
		printf("Log File cannot be created.Exitting..\n");
		exit(0);
	}
	else
	{
		fprintf(gpFile,"Log File is successfully Opened\n");
	}

	CreateWindow();

	initialize();
		
	XEvent event;
	KeySym keySym;
	int winWidth=WIN_WIDHT ;
	int winHeight=WIN_HEIGHT;
	bool bDone=false;



	while(bDone==false)
	{
		while(XPending(gpDisplay))
		{
			XNextEvent(gpDisplay,&event);
			switch(event.type)
			{
				case MapNotify:
					break;
				case KeyPress:	
					keySym=XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);
					switch(keySym)
					{
						case XK_Escape:
						bDone =true;
						break;

						case XK_f:
						case XK_F:
						  if(gbFullscreen ==false)
						  {
							ToggleFullscreen();
							gbFullscreen=true;
						  }
						  else
						  {
							ToggleFullscreen();
							gbFullscreen=false;
						  }
						 break;
						case XK_L:
						case XK_l:		//for l or L key (Light)
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

					        case XK_X:
						case XK_x:
							if (gbRotateX == false)
							{
								gbRotateX = true;
								gbRotateY = false;
								gbRotateZ = false;
							}
						break;
 						case XK_Y:
						case XK_y:
							if (gbRotateY == false)
							{
								gbRotateY = true;
								gbRotateX = false;
								gbRotateZ = false;
							}
						break;
						case XK_Z:
						case XK_z:
							if (gbRotateZ == false)
							{
								gbRotateX = false;
								gbRotateY = false;
								gbRotateZ = true;
	
							}

						break;
					}		
					break;

			       case ButtonPress:	
				switch(event.xbutton.button)
				{
					case 1:		//Left Button
						break;

					case 2:		//Middle Button
						break;	
					case 3:		//Right Button
						break;
					default:		//
						break;
				}		
				break;
				case MotionNotify:	
				  break;
				case ConfigureNotify:
				  winWidth=event.xconfigure.width;
				  winHeight=event.xconfigure.height;
			          resize(winWidth,winHeight);
				  break;

				case Expose:
				break;
				case DestroyNotify:
				break;
				case 33:
					bDone=true;
					break;
				default:
					break;
			 
			}

		}
		update();	
		display();
	}	

	uninitialize();
	return(0);
}





 void CreateWindow()
 {
	fprintf(gpFile,"In Create window.\n");
	void uninitialize();
	

	
	XSetWindowAttributes winAttribs;
	GLXFBConfig *pGLXFBConfigs=NULL;
	GLXFBConfig bestGLXFBConfig;
	XVisualInfo *pTempXVisualInfo=NULL;
	int iNumFBConfigs=0;
	int styleMask;
	int i;
	
	static int frameBufferAttributes[]={
		GLX_X_RENDERABLE,True,
		GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE,GLX_TRUE_COLOR,
		GLX_RED_SIZE,8,
		GLX_GREEN_SIZE,8,
		GLX_BLUE_SIZE,8,
		GLX_ALPHA_SIZE,8,
		GLX_DEPTH_SIZE,24,
		GLX_STENCIL_SIZE,8,
		GLX_DOUBLEBUFFER,True,
				
		None}; 
	
	//code
	gpDisplay=XOpenDisplay(NULL);
	if(gpDisplay==NULL)
	{
		printf("ERROR : Unable to obtain X Display.\n");
		uninitialize();
		exit(1);
	}
	
	
	pGLXFBConfigs=glXChooseFBConfig(gpDisplay,DefaultScreen(gpDisplay),frameBufferAttributes,&iNumFBConfigs);
	if(pGLXFBConfigs==NULL)
	{
		printf( "Failed to get valid Framebuffer Config. Exitting Now ...\n");
		uninitialize();
		exit(1);
	}
	printf("%d Matching FB Configs Found.\n",iNumFBConfigs);
	
	
	int bestFramebufferconfig=-1,worstFramebufferConfig=-1,bestNumberOfSamples=-1,worstNumberOfSamples=999;
	for(i=0;i<iNumFBConfigs;i++)
	{
		pTempXVisualInfo=glXGetVisualFromFBConfig(gpDisplay,pGLXFBConfigs[i]);
		if(pTempXVisualInfo)
		{
			int sampleBuffer,samples;
			glXGetFBConfigAttrib(gpDisplay,pGLXFBConfigs[i],GLX_SAMPLE_BUFFERS,&sampleBuffer);
			glXGetFBConfigAttrib(gpDisplay,pGLXFBConfigs[i],GLX_SAMPLES,&samples);
			printf("Matching Framebuffer Config=%d : Visual ID=0x%lu : SAMPLE_BUFFERS=%d : SAMPLES=%d\n",i,pTempXVisualInfo->visualid,sampleBuffer,samples);
			if(bestFramebufferconfig < 0 || sampleBuffer && samples > bestNumberOfSamples)
			{
				bestFramebufferconfig=i;
				bestNumberOfSamples=samples;
			}
			if( worstFramebufferConfig < 0 || !sampleBuffer || samples < worstNumberOfSamples)
			{
				worstFramebufferConfig=i;
			    worstNumberOfSamples=samples;
			}
		}
	  XFree(pTempXVisualInfo);
	
}

	bestGLXFBConfig = pGLXFBConfigs[bestFramebufferconfig];
	
	gGLXFBConfig=bestGLXFBConfig;
	
	XFree(pGLXFBConfigs);
	
	gpXVisualInfo=glXGetVisualFromFBConfig(gpDisplay,bestGLXFBConfig);
	printf("Choosen visual ID=0x%lu\n",gpXVisualInfo->visualid );

	
	
	winAttribs.border_pixel=0;
	winAttribs.background_pixmap=0;
	winAttribs.colormap=XCreateColormap(gpDisplay,
		

	RootWindow(gpDisplay,gpXVisualInfo->screen), 
		gpXVisualInfo->visual,
		AllocNone); 

										
	winAttribs.event_mask=StructureNotifyMask | KeyPressMask | ButtonPressMask |
						  ExposureMask | VisibilityChangeMask | PointerMotionMask;
	
	styleMask=CWBorderPixel | CWEventMask | CWColormap;
	 gColormap=winAttribs.colormap;										           
	
	gWindow=XCreateWindow(gpDisplay,
						  RootWindow(gpDisplay,gpXVisualInfo->screen),
						  0,
						  0,
						  WIN_WIDHT,
						  WIN_HEIGHT,
						  0, 
						  gpXVisualInfo->depth,          
						  InputOutput, 
						  gpXVisualInfo->visual,
						  styleMask,
						  &winAttribs);
	if(!gWindow)
	{
		printf("Failure In Window Creation.\n");
		uninitialize();
		exit(1);
	}
	
	XStoreName(gpDisplay,gWindow,"OpenGL Window");
	
	Atom windowManagerDelete=XInternAtom(gpDisplay,"WM_WINDOW_DELETE",True);
	XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);
	
	XMapWindow(gpDisplay,gWindow);

 }




void ToggleFullscreen()
{
	 
    fprintf(gpFile,"Entering in  ToggleFullScreen Function\n");
    Atom wm_state;
  
    Atom fullscreen;

    XEvent xev={0};

   //code
   
  wm_state=XInternAtom(gpDisplay,"_NET_WM_STATE",False);
  
  memset(&xev,0,sizeof(xev));



   xev.type=ClientMessage;
  
   xev.xclient.window=gWindow;
   
   xev.xclient.message_type=wm_state;
  
   xev.xclient.format=32;
 
   xev.xclient.data.l[0]=gbFullscreen ? 0 : 1;

  
     fullscreen=XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",False);
 
     xev.xclient.data.l[1]=fullscreen;

     XSendEvent(gpDisplay,
	
		RootWindow(gpDisplay,gpXVisualInfo->screen),

		False,
	
		StructureNotifyMask,
	
		&xev);

	fprintf(gpFile,"Exiting from  ToggleFullScreen Function\n");
}

void initialize()
{

	void uninitialize();	
	void resize(int,int);
	GLint num;
	
glXCreateContextAttribsARB =(glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");


	GLint attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB,3,
		GLX_CONTEXT_MINOR_VERSION_ARB,0,
		GLX_CONTEXT_PROFILE_MASK_ARB,
	GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0};

	gGLXContext = glXCreateContextAttribsARB(gpDisplay,gGLXFBConfig,0,True,attribs);
	if(!gGLXContext)
	{	
		GLint attribs[] ={
		GLX_CONTEXT_MAJOR_VERSION_ARB,1,
		GLX_CONTEXT_MINOR_VERSION_ARB,0,
		0};
		printf("Failed to create GLX 4.5 context.Hence using old style GLX Context\n");
		gGLXContext = glXCreateContextAttribsARB(gpDisplay,gGLXFBConfig,0,True,attribs);
	}
	else
	{
		printf("OpenGL Context 4.5 Is Created\n");
	}
	
	if(!glXIsDirect(gpDisplay,gGLXContext))
	{
		printf("Indirect GLX Rendering Context Obtained\n");
	}
	else
	{
		printf("Direct GLX Rendering Context Obtained\n");
	}

	
	
	glXMakeCurrent(gpDisplay,gWindow,gGLXContext);
	
	GLenum glew_error = glewInit();

	if(glew_error != GLEW_OK)
	{
		glXMakeCurrent(gpDisplay,0,0);
	}
	
	//************VERTEX SHADERS ***********
	//create Shader
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	//provide source code to shader
	const GLchar* vertexShaderSourceCode =
	"#version 130" \
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
	glShaderSource(gVertexShaderObject,1,(const char**) &vertexShaderSourceCode,NULL);
	
	//Compile shader
	glCompileShader(gVertexShaderObject);
	GLint iInfoLength = 0;
	GLint iShaderCompiledStatus = 0;
	char * szInfoLog = NULL;
	
	glGetShaderiv(gVertexShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
	if(iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject,GL_INFO_LOG_LENGTH,&iInfoLength);
		if(iInfoLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLength);
			if(szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject,iInfoLength,&written,szInfoLog);
				fprintf(gpFile,"Vertex Shader Compilation Log:%s\n",szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	// ***** FRAGMENT SHADER ***
	//create shader
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSourceCode = 
	"#version 130" \
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
		"vec3 phong_ads_color;" \
		"if(u_lighting_enabled==1)"\
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

	
	glShaderSource(gFragmentShaderObject,1,(const GLchar**)&fragmentShaderSourceCode,NULL);
	glCompileShader(gFragmentShaderObject);

	glGetShaderiv(gFragmentShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
	if(iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject,GL_INFO_LOG_LENGTH,&iInfoLength);
		if(iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if(szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject,iInfoLength,&written,szInfoLog);
				fprintf(gpFile,"Fragment Shader Compilation Log%s\n",szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}

	
	}
	//*************** SHADER PROGRAM ******
	//create shader program
	gShaderProgramObject = glCreateProgram();
	
	//attach vertex shader to shader program
	glAttachShader(gShaderProgramObject,gVertexShaderObject);
	
	//attach fragment shader to shader program
	glAttachShader(gShaderProgramObject,gFragmentShaderObject);
		
	//Pre-linking Binding of shader
	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_VERTEX, "vPosition");

	glBindAttribLocation(gShaderProgramObject,VDG_ATTRIBUTE_NORMAL,"vNormal");

	//Link Shader
	glLinkProgram(gShaderProgramObject);

	GLint iShaderProgramLinkStatus = 0;
//	char * szInfoLog = NULL;
	glGetProgramiv(gShaderProgramObject,GL_LINK_STATUS,&iShaderProgramLinkStatus);
	if(iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject,GL_INFO_LOG_LENGTH,&iInfoLength);
		if(iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if(szInfoLog != NULL)
			{
			GLsizei written;
			glGetProgramInfoLog(gShaderProgramObject,iInfoLength,&written,szInfoLog);
			fprintf(gpFile,"Shader Program Link Log%s\n",szInfoLog);
			free(szInfoLog);
			uninitialize();
			exit(0);
			}
		}
	}
	
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
	glClearDepth(1.0f);	
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_CULL_FACE);
	gbLight = false;
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	gPerspectiveProjection = mat4::identity();
	resize(WIN_WIDHT,WIN_HEIGHT);

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

	for (int i = 0; i < NUMBER_OF_SPHERES; i++)
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
			glViewport(0, 0, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);

		break;
		case 2:
			glViewport((GLsizei)WIN_WIDHT / 10, 0, (GLsizei)WIN_WIDHT / 6, (GLsizei)WIN_HEIGHT/6);
		break;
		case 3:
			glViewport((GLsizei)WIN_WIDHT / 5, 0, (GLsizei)WIN_WIDHT / 6, (GLsizei)WIN_HEIGHT / 6);
		break;
		case 4:
			glViewport((GLsizei)WIN_WIDHT/3.3 , 0, (GLsizei)WIN_WIDHT / 6, (GLsizei)WIN_HEIGHT / 6);
		break;
		case 5:
			glViewport((GLsizei)WIN_WIDHT/2.4, 0, (GLsizei)WIN_WIDHT / 6, (GLsizei)WIN_HEIGHT / 6);
			break;
		case 6:
			glViewport((GLsizei)WIN_WIDHT/1.9, 0, (GLsizei)WIN_WIDHT / 6, (GLsizei)WIN_HEIGHT / 6);
			break;

		case 7:
			glViewport(0, (GLsizei)WIN_HEIGHT / 5.5, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 8:
			glViewport((GLsizei)WIN_WIDHT / 10, (GLsizei)WIN_HEIGHT / 5.5, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 9:
			glViewport((GLsizei)WIN_WIDHT / 5, (GLsizei)WIN_HEIGHT / 5.5, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 10:
			glViewport((GLsizei)WIN_WIDHT/3.3, (GLsizei)WIN_HEIGHT / 5.5, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 11:
			glViewport((GLsizei)WIN_WIDHT/2.4, (GLsizei)WIN_HEIGHT / 5.5, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);

			break;
		case 12:
			glViewport((GLsizei)WIN_WIDHT/1.9, (GLsizei)WIN_HEIGHT / 5.5, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;

		case 13:
			glViewport(0, (GLsizei)WIN_HEIGHT / 2.7, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 14:
			glViewport((GLsizei)WIN_WIDHT / 10, (GLsizei)WIN_HEIGHT / 2.7, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 15:
			glViewport((GLsizei)WIN_WIDHT / 5, (GLsizei)WIN_HEIGHT / 2.7, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 16:
			glViewport((GLsizei)WIN_WIDHT/3.3, (GLsizei)WIN_HEIGHT / 2.7, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 17:
			glViewport((GLsizei)WIN_WIDHT/2.4, (GLsizei)WIN_HEIGHT / 2.7, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 18:
			glViewport((GLsizei)WIN_WIDHT/1.9, (GLsizei)WIN_HEIGHT / 2.7, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 19:
			glViewport(0, (GLsizei)WIN_HEIGHT / 1.8, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 20:
			glViewport((GLsizei)WIN_WIDHT / 10, (GLsizei)WIN_HEIGHT / 1.8, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 21:
			glViewport((GLsizei)WIN_WIDHT / 5, (GLsizei)WIN_HEIGHT / 1.8, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 22:
			glViewport((GLsizei)WIN_WIDHT/3.3, (GLsizei)WIN_HEIGHT / 1.8, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 23:
			glViewport((GLsizei)WIN_WIDHT/2.4, (GLsizei)WIN_HEIGHT / 1.8, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			break;
		case 24:
			glViewport((GLsizei)WIN_WIDHT/1.9, (GLsizei)WIN_HEIGHT / 1.8, (GLsizei)WIN_WIDHT/6, (GLsizei)WIN_HEIGHT/6);
			
			break;

		}
		drawSpheres();
	}

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
	glXSwapBuffers(gpDisplay,gWindow);
}

void resize(int width, int height)
{
	fprintf(gpFile,"Entering in  resize Function\n");
	//code
	if(height==0)
		height=1;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);		

	gPerspectiveProjection = perspective(58.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}




void uninitialize()
{	
	fprintf(gpFile,"Entering in  uinitialize Function\n");
	GLXContext currentGLXContext;
	currentGLXContext=glXGetCurrentContext();

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
	glDetachShader(gShaderProgramObject,gVertexShaderObject);
	
	glDetachShader(gShaderProgramObject,gFragmentShaderObject);


	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;


	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;
	
	glUseProgram(0);

	if(currentGLXContext != NULL && currentGLXContext==gGLXContext)
	{
		glXMakeCurrent(gpDisplay,0,0);
	}

	if(gGLXContext)
	{
		glXDestroyContext(gpDisplay,gGLXContext);
	}
	
	if(gWindow)
	{
		XDestroyWindow(gpDisplay,gWindow);
	}
		
	if(gColormap)
	{
		XFreeColormap(gpDisplay,gColormap);
	}
		
	if(gpXVisualInfo)
	{
		free(gpXVisualInfo);
		gpXVisualInfo=NULL;
	}
	if(gpDisplay)
	{
	XCloseDisplay(gpDisplay);
	gpDisplay=NULL;
	}

	if(gpFile)
	{
		fprintf(gpFile,"Exiting from  uinitialize Function\n");
		fclose(gpFile);
		gpFile=NULL;
	}
	
}	






void drawSpheres()
{
	glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, gPerspectiveProjection);
	modelMatrix = translate(0.0f, 0.0f, -1.5f);

	glBindVertexArray(gVao_Sphere);

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
	materialSpheres.push_back(0.01f);
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
























