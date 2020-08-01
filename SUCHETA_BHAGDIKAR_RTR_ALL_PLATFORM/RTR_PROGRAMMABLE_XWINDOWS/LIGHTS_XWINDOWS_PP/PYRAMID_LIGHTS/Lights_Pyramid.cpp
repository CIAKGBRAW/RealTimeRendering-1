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
#include<SOIL/SOIL.h>
#include"vmath.h"


using namespace vmath;

#define WIN_WIDHT 800
#define WIN_HEIGHT 600

FILE *gpFile =NULL;


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

GLuint gVao_Pyramid;
GLuint gVbo_Position_Pyramid;
GLuint gVbo_Color_Pyramid;
GLuint gVbo_Normal_Pyramid;


GLuint L_KeyPressed_uniform;
GLuint L_KeyPressed_uniformRed;
GLuint gMVPUniform;
mat4 gPerspectiveProjection;


GLuint gLKeyPressedUniform;
GLuint model_matrix_uniform, view_matrix_uniform, gProjectionMatrixUniform;
GLuint gLdUniform, gKdUniform, gLightPositonUniform;


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
GLfloat gAngle = 0.0f;



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

bool gbAnimate;
bool gbLight;

int main()
{

	void CreateWindow();
	void ToggleFullscreen();
	void initialize();
	void resize(int,int);
	void display();
	void uninitialize();
	void update();

	
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
	static bool gbIsLKeyPressed = false;
	static bool gbIsAKeyPressed = false;



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
						case XK_A:
						case XK_a:
						if (gbIsAKeyPressed == false)
						{
							gbAnimate = true;
							gbIsAKeyPressed = true;
						}
						else
						{
							gbAnimate = false;
							gbIsAKeyPressed = false;
						}
						break;
						case XK_L:
						case XK_l:
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
			
		display();
		update();

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

	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_NORMAL, "vNormal");

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
	gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");
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


	//gvao cube
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
//end vao square

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);	
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
//	glEnable(GL_CULL_FACE);
	gbAnimate = false;
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
	else
	{
		glUniform1i(gLKeyPressedUniform, 0);
	}
	//OpenGL Drawing
	mat4 modelmatrix = mat4::identity();
	mat4 modelViewMatrix = mat4::identity();
	mat4 rotationMatrix=mat4::identity();
	mat4 viewMatrix = mat4::identity();

	modelmatrix = translate(0.0f, 0.0f, -6.0f);

	rotationMatrix =  vmath::rotate(gAngle, 0.0f, 1.0f, 0.0f);

	modelViewMatrix = modelmatrix * rotationMatrix;

	glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelViewMatrix);
	glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjection);

	glBindVertexArray(gVao_Pyramid);

	// ****  draw either by glDrawTriangles() or glDrawarrays() or glDrawElements()
	glDrawArrays(GL_TRIANGLES, 0, 12);			//(each with its x,y,z ) vertices in triangleVertices array

												//**** unbind vaoTriangle ****
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

	
	if(gVao_Pyramid)
	{
		glDeleteVertexArrays(1,&gVao_Pyramid);
		gVao_Pyramid = 0;
	}
	if(gVbo_Position_Pyramid)
	{
		glDeleteBuffers(1,&gVbo_Position_Pyramid);
		gVbo_Position_Pyramid = 0;
	}
	
	if(gVbo_Normal_Pyramid)
	{
		glDeleteBuffers(1,&gVbo_Normal_Pyramid);
		gVbo_Normal_Pyramid = 0;
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
   gAngle = gAngle + 2.0f;
   if(gAngle > 360.0f)
   	gAngle = 0.0f;

}









