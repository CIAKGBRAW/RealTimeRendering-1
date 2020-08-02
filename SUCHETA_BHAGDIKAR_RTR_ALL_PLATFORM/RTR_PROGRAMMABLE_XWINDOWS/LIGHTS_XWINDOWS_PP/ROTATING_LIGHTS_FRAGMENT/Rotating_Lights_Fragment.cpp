#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<math.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>

#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<SOIL/SOIL.h>
#include"vmath.h"
#include"Sphere.h"


using namespace vmath;

#define WIN_WIDHT 800
#define WIN_HEIGHT 600


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


bool gbFullScreen = false;
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

GLuint La_uniformGreen;
GLuint Ld_uniformGreen;
GLuint Ls_uniformGreen;
GLuint light_position_uniformGreen;
GLfloat angleGreen;
GLfloat myArrGreen[4];
GLuint Ka_uniformGreen;
GLuint Kd_uniformGreen;
GLuint Ks_uniformGreen;


GLuint La_uniformRed;
GLuint Ld_uniformRed;
GLuint Ls_uniformRed;
GLuint light_position_uniformRed;
GLfloat angleRed;
GLfloat myArrRed[4];
GLuint Ka_uniformRed;
GLuint Kd_uniformRed;
GLuint Ks_uniformRed;


GLuint La_uniform;
GLuint Ld_uniform;
GLuint Ls_uniform;
GLuint light_position_uniform;
GLfloat angleBlue;
GLfloat myArrBlue[4];
GLuint Ka_uniform;
GLuint Kd_uniform;
GLuint Ks_uniform;

GLuint L_KeyPressed_uniform;

GLuint material_shininess_uniform;

mat4 gPerspectiveProjection;

GLfloat angleRadian = 0.0;
GLfloat angleRadianGreen = 0.0;
GLfloat angleRadianBlue = 0.0;

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat lightAmbientRed[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuseRed[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat lightSpecularRed[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat lightPositionRed[] = { 100.0,0.0f,0.0f,1.0f };

GLfloat lightAmbientGreen[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuseGreen[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat lightSpecularGreen[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat lightPositionGreen[] = { 0.0f,100.0f,0.0f,1.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;
bool gbLight;
bool gbFLight;


FILE *gpFile = NULL;

int main()
{

	void CreateWindow();
	void ToggleFullScreen();
	void initialize();
	void resize(int,int);
	void display();
	void uninitialize();
	void update();

	static bool gbIsLKeyPressed = false;
	static bool gbIsAKeyPressed = false;
	static bool gbIsFKeyPressed = false;

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
						case XK_Q:
						case XK_q:
						bDone =true;
						break;
						case XK_Escape:
		
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

						case XK_f:
						case XK_F:
						if (gbIsFKeyPressed ==false)
						{
							gbFLight = true;
							gbIsFKeyPressed = true;
						}
						else
						{
							gbFLight = false;
							gbIsFKeyPressed = false;
							 
						}
						break;
						case XK_L:
						case XK_l:		//for l or L key (Light)
						if (gbIsLKeyPressed == false)
						{
							fprintf(gpFile,"L key Pressed for vertex\n");
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




void ToggleFullScreen()
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
 
   xev.xclient.data.l[0]=gbFullScreen ? 0 : 1;

  
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
//	F_KeyPressed_uniform = glGetUniformLocation(gShaderProgramObject, "u_fraglighting_enabled");

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

	////////////////////////////////////

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
	//start using opengl program object
	glUseProgram(gShaderProgramObject);
	
		if (gbLight == true)
	{

		//set u_lighting_enabled uniform
		glUniform1i(L_KeyPressed_uniform, 1);

	//	setting lights properties
		glUniform3fv(La_uniformGreen, 1, lightAmbientGreen);
		glUniform3fv(Ld_uniformGreen, 1, lightDiffuseGreen);
		glUniform3fv(Ls_uniformGreen, 1, lightSpecularGreen);
		glUniform4fv(light_position_uniformGreen, 1, myArrGreen);

		//setting material's properties
		glUniform3fv(Ka_uniformGreen, 1, material_ambient);
		glUniform3fv(Kd_uniformGreen, 1, material_diffuse);
		glUniform3fv(Ks_uniformGreen, 1, material_specular);
		glUniform1f(material_shininess_uniform, material_shininess);

		glUniform3fv(La_uniformRed, 1, lightAmbientRed);
		glUniform3fv(Ld_uniformRed, 1, lightDiffuseRed);
		glUniform3fv(Ls_uniformRed, 1, lightSpecularRed);
		glUniform4fv(light_position_uniformRed, 1, myArrRed);

		//setting material's properties
		glUniform3fv(Ka_uniformRed, 1, material_ambient);
		glUniform3fv(Kd_uniformRed, 1, material_diffuse);
		glUniform3fv(Ks_uniformRed, 1, material_specular);
		glUniform1f(material_shininess_uniform, material_shininess);

		glUniform3fv(La_uniform, 1, lightAmbient);
		glUniform3fv(Ld_uniform, 1, lightDiffuse);
		glUniform3fv(Ls_uniform, 1, lightSpecular);
		glUniform4fv(light_position_uniform, 1, myArrBlue);

		//setting material's properties
		glUniform3fv(Ka_uniform, 1, material_ambient);
		glUniform3fv(Kd_uniform, 1, material_diffuse);
		glUniform3fv(Ks_uniform, 1, material_specular);
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

//	myArrRed[0] = lightPositionRed[0] * cos(angleRadian);
	myArrRed[1] = lightPositionRed[0] * (sin(angleRadian));
	myArrRed[2] = lightPositionRed[0] *(cos(angleRadian));

	myArrGreen[0] = lightPositionGreen[1] * (sin(angleRadianGreen));
	myArrGreen[2] = lightPositionGreen[1] * (cos(angleRadianGreen));

	myArrBlue[0] = lightPosition[2] * (sin(angleRadianBlue));
	myArrBlue[1] = lightPosition[2] * (cos(angleRadianBlue));



	
	//apply z axis translation to go deep into the screen by -2.0
	//so that triangle with same fullscreen coordinated but due to above translation will look small

	modelMatrix = translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, gPerspectiveProjection);

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

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

	gPerspectiveProjection = perspective(45.0f,(GLfloat)width / height,0.1f,100.0f);
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








void update()
{
	angleBlue = angleBlue + 0.5f;
	if (angleBlue > 360.0f)
		angleBlue = 0.0f;

	angleRed = angleRed + 0.5f;
	if (angleRed > 360.0f)
		angleRed = 0.0f;
	angleGreen = angleGreen + 0.5f;
	if (angleGreen > 360.0f)
		angleGreen = 0.0f;

	angleRadian = angleRed * (M_PI / 180.0);
	angleRadianGreen = angleGreen * (M_PI / 180.0);
	angleRadianBlue = angleBlue * (M_PI / 180.0);
}





