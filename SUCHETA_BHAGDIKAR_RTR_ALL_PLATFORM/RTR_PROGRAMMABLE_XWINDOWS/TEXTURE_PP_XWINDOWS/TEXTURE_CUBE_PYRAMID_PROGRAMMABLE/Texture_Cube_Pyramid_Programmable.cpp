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
GLuint gVbo_Texture_Pyramid;

GLuint gVao_Cube;
GLuint gVbo_Position_Cube;
GLuint gVbo_Texture_Cube;

GLuint gMVPUniform;
mat4 gPerspectiveProjection;

GLuint gTexture_sampler_uniform;
GLuint gTexture_stone;
GLuint gTexture_kundali;

GLfloat gAnglePyramid = 0.0f;
GLfloat gAngleCube = 0.0f;

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
	int LoadGLTextures(GLuint*,const char*);
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
	"in vec2 vTexture0_Coord;" \
	"out vec2 out_texture0_coord;" \
	"uniform mat4 u_mvp_matrix;" \
	"void main(void)" \
	"{" \
		"gl_Position = u_mvp_matrix * vPosition;" \
		"out_texture0_coord = vTexture0_Coord;" \
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
	"in vec2 out_texture0_coord;" \
	"out vec4 FragColor;" \
	"uniform sampler2D u_texture0_sampler;" \
	"void main(void)" \
	"{" \
	"FragColor = texture(u_texture0_sampler,out_texture0_coord);" \
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

	glBindAttribLocation(gShaderProgramObject,VDG_ATTRIBUTE_TEXTURE0,"vTexture0_Coord");

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
	
	gMVPUniform = glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");
	
	gTexture_sampler_uniform = glGetUniformLocation(gShaderProgramObject,"u_texture0_sampler");

	//vertices, color,textures
	const GLfloat pyramidVertices[]=
	{
		/*//FRONT FACE
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		//RIGHT FACE
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,		

		//LEFT FACE
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,

		//BACK FACE
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f*/
		//FRONT
		0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f, -1.0f,1.0f,

		//right
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		//back
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f,-1.0f,
		-1.0f, -1.0f,-1.0f,

		//left

		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f,-1.0f,
		-1.0f, -1.0f,1.0f



	};
	
	const GLfloat pyramidTexcoords[]=
	{
		/*0.5, 1.0, // front-top
		0.0, 0.0, // front-left
		1.0, 0.0, // front-right
			  
		0.5, 1.0, // right-top
		1.0, 0.0, // right-left
		0.0, 0.0, // right-right
				  				  
		0.5, 1.0, // left-top
		0.0, 0.0, // left-left
		1.0, 0.0, // left-right

		0.5, 1.0, // back-top
		1.0, 0.0, // back-left
		0.0, 0.0, // back-right*/

			//FRONT
			0.5f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			//RIGHT
			0.5f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			//BACK 
			0.5f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			//LEFT
			0.5f, 1.0f,
			0.0f,0.0f,
			1.0f,0.0f

	};

	const GLfloat cubeVertices[]=
	{

		//FRONT FACE
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		
		//RIGHT FACE
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,	

		//BACK FACE
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,1.0f,-1.0f,
		1.0f,1.0f,-1.0f, 

		//LEFT FACE
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		
		//TOP FACE
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		//BOTTOM FACE
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		
	};
	
	const GLfloat cubeTexcoords[]=
	{
		//FRONT
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f,0.0f,
		0.0f,0.0f,

		//RIGHT
		0.0f, 0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f, 0.0f,

		//BACK
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
	
		//LEFT
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		//TOP
		1.0f,1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		//BOTTOM
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	//gVao pyramid

	glGenVertexArrays(1, &gVao_Pyramid);
	glBindVertexArray(gVao_Pyramid);
	//start vbo position pyramid
	glGenBuffers(1,&gVbo_Position_Pyramid);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_Position_Pyramid);		
	glBufferData(GL_ARRAY_BUFFER,sizeof(pyramidVertices),pyramidVertices,GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);	

	glBindBuffer(GL_ARRAY_BUFFER,0);	
	//end vbo position pyramid

	glGenBuffers(1,&gVbo_Texture_Pyramid);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_Texture_Pyramid);		//start vbo color triangle
	glBufferData(GL_ARRAY_BUFFER,sizeof(pyramidTexcoords),pyramidTexcoords,GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_TEXTURE0,2,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER,0);	//end vbo triangle color		
	glBindVertexArray(0);			//end vao triangle



	//gvao cube
	glGenVertexArrays(1,&gVao_Cube);
	glBindVertexArray(gVao_Cube);

	glGenBuffers(1,&gVbo_Position_Cube);			//start vbo cube position
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_Position_Cube);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);			//end vbo cube position

	//vbo texture cube
	glGenBuffers(1,&gVbo_Texture_Cube);	
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_Texture_Cube);		//start vbo color triangle
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeTexcoords),cubeTexcoords,GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_TEXTURE0,2,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER,0);	//end vbo triangle color		
	
	glBindVertexArray(0);				//end vao square

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);	
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_CULL_FACE);
	
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	LoadGLTextures(&gTexture_stone,"Stone.bmp");
	LoadGLTextures(&gTexture_kundali,"Vijay_Kundali.bmp");

	gPerspectiveProjection = mat4::identity();
	resize(WIN_WIDHT,WIN_HEIGHT);

}		

int LoadGLTextures(GLuint *texture,const char * path)
{
	fprintf(gpFile, "In LoadGLTextures..\n");

	
	unsigned char* imageData = NULL;
	GLint width, height;
	int iStatus = 0;	
	glGenTextures(1,texture);
	imageData = SOIL_load_image(path,&width,&height,0,SOIL_LOAD_RGB);
		fprintf(gpFile,"%s\n",path);
		fprintf(gpFile,"%d\n",width);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glBindTexture(GL_TEXTURE_2D,*texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,(void*)imageData);
		fprintf(gpFile,"%s\n",path);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(imageData);


	fprintf(gpFile, "Exiting LoadGLTextures..\n");

	return(iStatus);
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	//start using opengl program object
	glUseProgram(gShaderProgramObject);

	mat4  modelViewMatrix;
	mat4  rotationMatrix;
	mat4 modelViewProjectionMatrix;

	modelViewMatrix = mat4::identity();	
	modelViewMatrix = translate(-1.5f,0.0f,-6.0f);
	rotationMatrix = rotate(gAnglePyramid,0.0f,1.0f,0.0f);
	modelViewMatrix = modelViewMatrix * rotationMatrix;

	modelViewProjectionMatrix = gPerspectiveProjection * modelViewMatrix;

	glUniformMatrix4fv(gMVPUniform,1,GL_FALSE,modelViewProjectionMatrix);

	//bind with pyramid ttexture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,gTexture_stone);
	glUniform1i(gTexture_sampler_uniform,0);

	//bind vertex array
	glBindVertexArray(gVao_Pyramid);
	glDrawArrays(GL_TRIANGLES,0,12);
	glBindVertexArray(0);
	
	//CUBE CODE
	modelViewMatrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	
	
	modelViewMatrix =translate(1.5f,0.0f,-6.0f);
	rotationMatrix = rotate(gAngleCube,gAngleCube,gAngleCube);
	modelViewMatrix = modelViewMatrix  *rotationMatrix;

	modelViewProjectionMatrix = gPerspectiveProjection *modelViewMatrix;
	glUniformMatrix4fv(gMVPUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,gTexture_kundali);
	glUniform1i(gTexture_sampler_uniform,0);

	glBindVertexArray(gVao_Cube);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

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
	
	if(gVbo_Texture_Pyramid)
	{
		glDeleteBuffers(1,&gVbo_Texture_Pyramid);
		gVbo_Texture_Pyramid = 0;
	}

	if(gVao_Cube)
	{
		glDeleteVertexArrays(1,&gVao_Cube);
		gVao_Cube = 0;
	}
	if(gVbo_Position_Cube)
	{
		glDeleteBuffers(1,&gVbo_Position_Cube);
		gVbo_Position_Cube = 0;
	}
	
	if(gVbo_Texture_Cube)
	{
		glDeleteBuffers(1,&gVbo_Texture_Cube);
		gVbo_Texture_Cube = 0;
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
   gAnglePyramid = gAnglePyramid + 2.0f;
   if(gAnglePyramid > 360.0f)
   	gAnglePyramid = 0.0f;

  gAngleCube = gAngleCube + 2.0f;
  if(gAngleCube > 360.0f)
	gAngleCube = 0.0f;
	
}









