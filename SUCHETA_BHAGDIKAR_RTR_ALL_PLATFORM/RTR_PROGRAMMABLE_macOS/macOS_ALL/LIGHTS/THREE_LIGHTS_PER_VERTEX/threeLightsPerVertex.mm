#import <Foundation/Foundation.h>
#import<Cocoa/Cocoa.h>
#import<QuartzCore/CVDisplayLink.h>
#import<OpenGL/gl3.h>
#import<OpenGL/gl3ext.h>
#include<math.h>
#import "vmath.h"
#import "Sphere.h"

enum
{
	VDG_ATTRIBUTE_VERTEX = 0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0,
};



CVReturn MyDisplayLinkCallback(CVDisplayLinkRef,const CVTimeStamp*,const CVTimeStamp *,CVOptionFlags,CVOptionFlags * ,void *);

FILE *gpFile = NULL;

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end


@interface GLView : NSOpenGLView
@end







int main(int argc, const char * argv[])
{
	NSAutoreleasePool *pPool=[[NSAutoreleasePool alloc]init];

	NSApp=[NSApplication sharedApplication];

	[NSApp setDelegate:[[AppDelegate alloc]init]];

	[NSApp run];

	[pPool release];

	return(0);
}

@implementation AppDelegate
{
	@private
		NSWindow *window;
		GLView *glView;
}




-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	NSBundle *mainBundle=[NSBundle mainBundle];
	NSString *appDirName = [mainBundle bundlePath];
	NSString *parentDirPath=[appDirName stringByDeletingLastPathComponent];
	NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/Log.txt",parentDirPath];
	const char *pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];
	gpFile = fopen(pszLogFileNameWithPath,"w");
	if(gpFile==NULL)
	{

        printf("Can not Create Log File\n");
		[self release];
		[NSApp terminate:self];
	}
  fprintf(gpFile, "Program Is Started Successfully\n");
    
	NSRect win_rect;
	win_rect = NSMakeRect(0.0,0.0,800.0,600.0);

	window=[[NSWindow alloc]initWithContentRect:win_rect
									styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
										backing:NSBackingStoreBuffered
											defer:NO];

	[window setTitle:@"macOS Three Lights Per Vertex OpenGL Window"];
	[window center];

	glView = [[GLView alloc]initWithFrame:win_rect];

	[window setContentView:glView];
	[window setDelegate:self];
	[window makeKeyAndOrderFront:self];

}




-(void) applicationWillTerminate:(NSNotification *)aNotification
{
	fprintf(gpFile,"Program is terminated successfully\n");
	if(gpFile)
	{
		fclose(gpFile);
		gpFile=NULL;
	}
}



-(void)windowWillClose:(NSNotification *)aNotification
{
	[NSApp terminate:self];
}


-(void)dealloc
{

	[glView release];

	[window release];

	[super dealloc];
}

@end





@implementation GLView
{
	@private
		CVDisplayLinkRef displayLink;

		GLuint vertexShaderObject;
		GLuint fragmentShaderObject;
		GLuint shaderProgramObject;
    
       float sphere_vertices[1146];
       float sphere_normals[1146];
       float sphere_textures[764];
       short sphere_elements[2280];
    
        GLuint gVao_Sphere;
        GLuint gVbo_Sphere_positions;
        GLuint gVbo_Sphere_normal;
        GLuint gVbo_Sphere_element;
    
        GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;
    
        GLuint L_KeyPressed_uniform;
    
        GLfloat angleRed;
        GLfloat myArrRed[4];
     GLfloat angleRadianRed;
        GLuint La_uniform;
        GLuint Ld_uniform;
        GLuint Ls_uniform;
        GLuint light_position_uniform;
    
        GLuint Ka_uniform;
        GLuint Kd_uniform;
        GLuint Ks_uniform;
    
        GLfloat angleGreen;
        GLfloat myArrGreen[4];
     GLfloat angleRadianGreen;
        GLuint La_uniformGreen;
        GLuint Ld_uniformGreen;
        GLuint Ls_uniformGreen;
        GLuint light_position_uniformGreen;
    
        GLuint Ka_uniformGreen;
        GLuint Kd_uniformGreen;
        GLuint Ks_uniformGreen;
    
    
    
        GLfloat angleBlue;
        GLfloat myArrBlue[4];
        GLfloat angleRadian;
        GLuint La_uniformBlue;
        GLuint Ld_uniformBlue;
        GLuint Ls_uniformBlue;
        GLuint light_position_uniformBlue;
    
        GLuint Ka_uniformBlue;
        GLuint Kd_uniformBlue;
        GLuint Ks_uniformBlue;
    
        GLuint material_shininess_uniform;

   
    //0.0f,0.0f,1.0f};
     GLfloat light_ambient[4];
    GLfloat lightDiffuse[4];
    GLfloat lightSpecular[4];
    GLfloat lightPosition[4];
    
    GLfloat light_ambient2[4];
    GLfloat lightDiffuse2[4];
    GLfloat lightSpecular2[4];
    GLfloat lightPosition2[4];
    
    GLfloat light_ambient3[4];
    GLfloat lightDiffuse3[4];
    GLfloat lightSpecular3[4];
    GLfloat lightPosition3[4];
    
    GLfloat material_ambient[4];
    GLfloat material_diffuse[4];
    GLfloat material_specular[4];
     GLfloat material_shininess;
    bool gbLight;
    
    bool gbLKeyIsPressed;
    
    int numVertices;
    int numElemets;
    
		GLuint vao;
		GLuint vbo;
		GLuint mvpUniform;

    Sphere sphere;
  
		vmath::mat4 perspectiveProjectionMatrix;
}

-(id)initWithFrame:(NSRect)frame;
{
	self = [super initWithFrame:frame];

	if(self)
	{
		[[self window]setContentView:self];

		NSOpenGLPixelFormatAttribute attrs[] = 
		{
			NSOpenGLPFAOpenGLProfile,
			NSOpenGLProfileVersion4_1Core,
			NSOpenGLPFAScreenMask,CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
			NSOpenGLPFANoRecovery,
			NSOpenGLPFAAccelerated,
			NSOpenGLPFAColorSize,24,
			NSOpenGLPFADepthSize,24,
			NSOpenGLPFAAlphaSize,24,
			NSOpenGLPFADoubleBuffer,
			0};

			NSOpenGLPixelFormat *pixelFormat=[[[NSOpenGLPixelFormat alloc]initWithAttributes:attrs]autorelease];

			if(pixelFormat==nil)
			{
					fprintf(gpFile,"No valid OpenGL Pixel Format\n");
					[self release];
					[NSApp terminate:self];
			}
		   NSOpenGLContext *glContext=[[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil]autorelease];
        
			[self setPixelFormat:pixelFormat];
			[self setOpenGLContext:glContext];
	}
	return(self);
}


-(CVReturn)getFrameForTime:(const CVTimeStamp *)pOutputTime
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

	[self drawView];

	[pool release];
	return(kCVReturnSuccess);
}

-(void)prepareOpenGL
{
// void dealloc();
    fprintf(gpFile,"OpenGL Version: %s\n",glGetString(GL_VERSION));
	fprintf(gpFile,"OpenGL Version: %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

	[[self openGLContext]makeCurrentContext];

	GLint swapInt = 1;
	[[self openGLContext]setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

	//create shader
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//vertex shader
    //provide source code to shader
    const GLchar * vertexShaderShaderCode =
    "#version 410" \
    "\n" \
    "in vec4 vPosition;" \
    "in vec3 vNormal;" \
    "uniform mat4 u_model_matrix;" \
    "uniform mat4 u_view_matrix;" \
    "uniform mat4 u_projection_matrix;" \
    "uniform int u_lighting_enabled;" \
    "uniform vec3 u_La;" \
    "uniform vec3 u_Ld;" \
    "uniform vec3 u_Ls;" \
    "uniform vec4 u_light_position;" \
    "uniform vec3 u_Ka;" \
    "uniform vec3 u_Kd;" \
    "uniform vec3 u_Ks;" \
    
    
    "uniform vec3 u_LaGreen;" \
    "uniform vec3 u_LdGreen;" \
    "uniform vec3 u_LsGreen;" \
    "uniform vec4 u_light_positionGreen;" \
    "uniform vec3 u_KaGreen;" \
    "uniform vec3 u_KdGreen;" \
    "uniform vec3 u_KsGreen;" \
    
    "uniform vec3 u_LaBlue;" \
    "uniform vec3 u_LdBlue;" \
    "uniform vec3 u_LsBlue;" \
    "uniform vec4 u_light_positionBlue;" \
    "uniform vec3 u_KaBlue;" \
    "uniform vec3 u_KdBlue;" \
    "uniform vec3 u_KsBlue;" \
    
    "uniform float u_material_shininess;" \
    "out vec3 phong_ads_color;" \
    "out vec3 phong_ads_colorGreen;" \
    "out vec3 phong_ads_colorBlue;" \
    "void main(void)" \
    "{"
    "if(u_lighting_enabled == 1)" \
    "{" \
    "vec4 eye_coordinates= u_view_matrix * u_model_matrix *vPosition;" \
    "vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
    "vec3 viewer_vector = normalize(-eye_coordinates.xyz);" \
    
    "vec3 light_direction = normalize(vec3(u_light_position) - eye_coordinates.xyz);"\
    "float tn_dot_ld = max(dot(transformed_normals,light_direction),0.0);" \
    "vec3 ambient = u_La * u_Ka;" \
    "vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
    "vec3 reflection_vector = reflect(-light_direction,transformed_normals);"\
    "vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector,viewer_vector),0.0),u_material_shininess);" \
    "phong_ads_color = ambient + diffuse + specular;" \
    
    "vec3 light_directionGreen = normalize(vec3(u_light_positionGreen) - eye_coordinates.xyz);"\
    "float tn_dot_ldGreen = max(dot(transformed_normals,light_directionGreen),0.0);" \
    "vec3 ambientGreen = u_LaGreen * u_KaGreen;" \
    "vec3 diffuseGreen = u_LdGreen * u_KdGreen * tn_dot_ldGreen;" \
    "vec3 reflection_vectorGreen = reflect(-light_directionGreen,transformed_normals);"\
    "vec3 specularGreen = u_LsGreen * u_KsGreen * pow(max(dot(reflection_vectorGreen,viewer_vector),0.0),u_material_shininess);" \
    "phong_ads_colorGreen = ambientGreen + diffuseGreen + specularGreen;" \
    
    "vec3 light_directionBlue = normalize(vec3(u_light_positionBlue) - eye_coordinates.xyz);"\
    "float tn_dot_ldBlue = max(dot(transformed_normals,light_directionBlue),0.0);" \
    "vec3 ambientBlue = u_LaBlue * u_KaBlue;" \
    "vec3 diffuseBlue = u_LdBlue * u_KdBlue * tn_dot_ldBlue;" \
    "vec3 reflection_vectorBlue = reflect(-light_directionBlue,transformed_normals);"\
    "vec3 specularBlue = u_LsBlue * u_KsBlue * pow(max(dot(reflection_vectorGreen,viewer_vector),0.0),u_material_shininess);" \
    "phong_ads_colorBlue = ambientBlue + diffuseBlue + specularBlue;" \

    
    "}" \
    "else" \
    "{" \
    "phong_ads_color = vec3(1.0,1.0,1.0);" \
    "}" \
    "gl_Position =u_projection_matrix *  u_view_matrix * u_model_matrix * vPosition;" \
    "}";
    
    
    glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderShaderCode, NULL);
    
    //compile shader
    glCompileShader(vertexShaderObject);
    
    GLint iInfoLength = 0;
    GLint iShaderCompiledStatus = 0;
    char * szInfoLog = NULL;
    
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
    if (iShaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLength);
        if (iInfoLength > 0)
        {
            szInfoLog = (char *)malloc(iInfoLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObject, iInfoLength, &written, szInfoLog);
                fprintf(gpFile, "Vertex Shader Compilation Log:%s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    
    //*****FRAGMENT SHADER****
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    
    //provide source code to shader
    const GLchar *fragmentShaderSourceCode =
   "#version 410" \
    "\n" \
    "in vec3 phong_ads_color;" \
    "in vec3 phong_ads_colorGreen;" \
    "in vec3 phong_ads_colorBlue;"\
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "FragColor = vec4(phong_ads_color,1.0)+ vec4(phong_ads_colorGreen,1.0) + vec4(phong_ads_colorBlue,1.0);" \
    "}";
    
	glShaderSource(fragmentShaderObject,1,(const GLchar**)&fragmentShaderSourceCode,NULL);

	//compile shader
	glCompileShader(fragmentShaderObject);
	glGetShaderiv(fragmentShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
	if(iShaderCompiledStatus == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject,GL_INFO_LOG_LENGTH,&iInfoLength);
			if(iInfoLength > 0)
			{
			szInfoLog = (char *)malloc(iInfoLength);
			if(szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject,iInfoLength,&written,szInfoLog);
				fprintf(gpFile,"Fragment Shader Compilation Log:%s\n",szInfoLog);
				free(szInfoLog);
				[self release];
				[NSApp terminate:self];
			}
		}
	}
    
    
    //// SHADER PROGRAM
    shaderProgramObject = glCreateProgram();
    
    glAttachShader(shaderProgramObject,vertexShaderObject);
    
    glAttachShader(shaderProgramObject,fragmentShaderObject);
    
    glBindAttribLocation(shaderProgramObject,VDG_ATTRIBUTE_VERTEX,"vPosition");
  
      glBindAttribLocation(shaderProgramObject,VDG_ATTRIBUTE_NORMAL,"vNormal");
    
    glLinkProgram(shaderProgramObject);
    GLint iShaderProgramLinkStatus = 0;
    glGetProgramiv(shaderProgramObject,GL_LINK_STATUS,&iShaderProgramLinkStatus);
    if(iShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject,GL_INFO_LOG_LENGTH,&iInfoLength);
        if(iInfoLength >0)
        {
            szInfoLog = (char *)malloc(iInfoLength);
            if(szInfoLog != NULL)
            {
                
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject,iInfoLength,&written,szInfoLog);
                fprintf(gpFile, "Shader Program Link Log : %s\n",szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    
    fprintf(gpFile,"After Shader Link\n");
    
    model_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_model_matrix");
     view_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_view_matrix");
    projection_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
    
    //L/I Key is pressed or not
    L_KeyPressed_uniform = glGetUniformLocation(shaderProgramObject, "u_lighting_enabled");
    
    //ambient color intensity of LIGHT
    La_uniform = glGetUniformLocation(shaderProgramObject, "u_La");
    //diffuse color intensity of LIGHT
    Ld_uniform = glGetUniformLocation(shaderProgramObject, "u_Ld");
    //specular color intensity of LIGHT
    Ls_uniform = glGetUniformLocation(shaderProgramObject, "u_Ls");
    //position of light
    light_position_uniform = glGetUniformLocation(shaderProgramObject, "u_light_position");
    
    //ambient reflective color intensity of MATERIAL
    Ka_uniform = glGetUniformLocation(shaderProgramObject, "u_Ka");
    //diffuse reflective color intensity of MATERIAL
    Kd_uniform = glGetUniformLocation(shaderProgramObject, "u_Kd");
    
    //specular reflective color intensity of MATERIALc
    Ks_uniform = glGetUniformLocation(shaderProgramObject, "u_Ks");
    
    
    //Green
    
    La_uniformGreen = glGetUniformLocation(shaderProgramObject, "u_LaGreen");
    //diffuse color intensity of LIGHT
    Ld_uniformGreen = glGetUniformLocation(shaderProgramObject, "u_LdGreen");
    //specular color intensity of LIGHT
    Ls_uniformGreen = glGetUniformLocation(shaderProgramObject, "u_LsGreen");
    //position of light
    light_position_uniformGreen = glGetUniformLocation(shaderProgramObject, "u_light_positionGreen");
    
    //ambient reflective color intensity of MATERIAL
    Ka_uniformGreen = glGetUniformLocation(shaderProgramObject, "u_KaGreen");
    //diffuse reflective color intensity of MATERIAL
    Kd_uniformGreen = glGetUniformLocation(shaderProgramObject, "u_KdGreen");
    //specular reflective color intensity of MATERIALc
    Ks_uniformGreen = glGetUniformLocation(shaderProgramObject, "u_KsGreen");
    
    //Blue
    
    
    La_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_LaBlue");
    //diffuse color intensity of LIGHT
    Ld_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_LdBlue");
    //specular color intensity of LIGHT
    Ls_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_LsBlue");
    //position of light
    light_position_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_light_positionBlue");
    
    //ambient reflective color intensity of MATERIAL
    Ka_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_KaBlue");
    //diffuse reflective color intensity of MATERIAL
    Kd_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_KdBlue");
    
    //specular reflective color intensity of MATERIALc
    Ks_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_KsBlue");
    
    
    
    
    //shininess of material (value is conventionally between 1 to 200)
    material_shininess_uniform = glGetUniformLocation(shaderProgramObject, "u_material_shininess");
   
    sphere.getSphereVertexData(sphere_vertices,sphere_normals,sphere_textures,sphere_elements);
    numVertices = sphere.getNumberOfSphereVertices();
    numElemets = sphere.getNumberOfSphereElements();	
    
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
    fprintf(gpFile,"After VAO and VBO");
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    light_ambient[0] = 0.0f;
    light_ambient[1] = 0.0f;
    light_ambient[2] = 0.0f;
    light_ambient[3] = 1.0f;
    
    lightDiffuse[0] = 1.0f;
    lightDiffuse[1] = 0.0f;
    lightDiffuse[2] = 0.0f;
    lightDiffuse[3] = 0.0f;
    
    lightPosition[0] = 0.0f;
    lightPosition[1] = 100.0f;
    lightPosition[2] = 100.0f;
    lightPosition[3] = 1.0f;
    
    lightSpecular[0] = 1.0f;
    lightSpecular[1] = 0.0f;
    lightSpecular[2] = 0.0f;
    lightSpecular[3] = 0.0f;
    
    light_ambient2[0] = 0.0f;
    light_ambient2[1] = 0.0f;
    light_ambient2[2] = 0.0f;
    light_ambient2[3] = 1.0f;
    
    lightDiffuse2[0] = 0.0f;
    lightDiffuse2[1] = 1.0f;
    lightDiffuse2[2] = 0.0f;
    lightDiffuse2[3] = 0.0f;
    
    lightPosition2[0] = 100.0f;
    lightPosition2[1] = 0.0f;
    lightPosition2[2] = 100.0f;
    lightPosition2[3] = 1.0f;
    
    lightSpecular2[0] = 0.0f;
    lightSpecular2[1] = 1.0f;
    lightSpecular2[2] = 0.0f;
    lightSpecular2[3] = 0.0f;
    
    
    light_ambient3[0] = 0.0f;
    light_ambient3[1] = 0.0f;
    light_ambient3[2] = 0.0f;
    light_ambient3[3] = 1.0f;
    
    lightDiffuse3[0] = 0.0f;
    lightDiffuse3[1] = 0.0f;
    lightDiffuse3[2] = 1.0f;
    lightDiffuse3[3] = 0.0f;
    
    lightPosition3[0] = 100.0f;
    lightPosition3[1] = 100.0f;
    lightPosition3[2] = 0.0f;
    lightPosition3[3] = 1.0f;
    
    lightSpecular3[0] = 0.0f;
    lightSpecular3[1] = 0.0f;
    lightSpecular3[2] = 1.0f;
    lightSpecular3[3] = 0.0f;
    
    
    material_ambient[0] = 0.0f;
    material_ambient[1] = 0.0f;
    material_ambient[2] = 0.0f;
    material_ambient[3] = 1.0f;
    
    material_diffuse[0] = 1.0f;
    material_diffuse[1] = 1.0f;
    material_diffuse[2] = 1.0f;
    material_diffuse[3] = 1.0f;
    
    
    material_specular[0] = 1.0f;
    material_specular[1] = 1.0f;
    material_specular[2] = 1.0f;
    material_specular[3] = 1.0f;
    
    material_shininess = 50.0f;

	perspectiveProjectionMatrix = vmath::mat4::identity();

    fprintf(gpFile,"After VAO and VBO2");
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink,&MyDisplayLinkCallback,self);
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
    CGLPixelFormatObj cglPixelFormat=(CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink,cglContext,cglPixelFormat);
    CVDisplayLinkStart(displayLink);
    fprintf(gpFile,"Before init exit VAO and VBO");
}



-(void)reshape	
{

    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    NSRect rect = [self bounds];
    
    GLfloat width = rect.size.width;
    GLfloat height = rect.size.height;
    
    if(height == 0)
        height = 1;
    
    glViewport(0,0,(GLsizei)width,(GLsizei)height);
    //PERSPECTIVE CODE
    
    perspectiveProjectionMatrix = vmath::perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}


-(void)drawRect:(NSRect)dirtyRect
{
	[self drawView];
}


-(void)drawView
{
    
 
    
 
	[[self openGLContext]makeCurrentContext];

	CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramObject);
    
    
    if (gbLight == true)
    {  myArrRed[1] =     100 * (cos ( 2*3.1415926535 * angleRadianRed ));
        myArrRed[2] =   100 * (sin ( 2*3.1415926535 * angleRadianRed ));
        
        myArrGreen[0] = 100 * (cos ( 2*3.1415926535 * angleRadianGreen ));
        myArrGreen[2] = 100 * (sin (2 * 3.1415 * angleRadianGreen));
        
        myArrBlue[0] =     100 * (cos ( 2*3.1415926535 * angleRadian ));
        myArrBlue[1] =     100 * (sin ( 2*3.1415926535 * angleRadian ));
        
        //set u_lighting_enabled uniform
        glUniform1i(L_KeyPressed_uniform, 1);
        
        //setting lights properties
       glUniform3fv(La_uniform, 1, light_ambient);
        glUniform3fv(Ld_uniform, 1, lightDiffuse);
        glUniform3fv(Ls_uniform, 1, lightSpecular);
        glUniform4fv(light_position_uniform, 1, myArrRed);
        
        //setting material's properties
        glUniform3fv(Ka_uniform, 1, material_ambient);
        glUniform3fv(Kd_uniform, 1, material_diffuse);
        glUniform3fv(Ks_uniform, 1, material_specular);
  
        
        //green
        glUniform3fv(La_uniformGreen, 1, light_ambient2);
        glUniform3fv(Ld_uniformGreen, 1, lightDiffuse2);
        glUniform3fv(Ls_uniformGreen, 1, lightSpecular2);
        glUniform4fv(light_position_uniformGreen, 1, myArrGreen);
        
        //setting material's properties
        glUniform3fv(Ka_uniformGreen, 1, material_ambient);
        glUniform3fv(Kd_uniformGreen, 1, material_diffuse);
        glUniform3fv(Ks_uniformGreen, 1, material_specular);
        //blue
        glUniform3fv(La_uniformBlue, 1, light_ambient3);
        glUniform3fv(Ld_uniformBlue, 1, lightDiffuse3);
        glUniform3fv(Ls_uniformBlue, 1, lightSpecular3);
        glUniform4fv(light_position_uniformBlue, 1, myArrBlue);
        
        //setting material's properties
        glUniform3fv(Ka_uniformBlue, 1, material_ambient);
        glUniform3fv(Kd_uniformBlue, 1, material_diffuse);
        glUniform3fv(Ks_uniformBlue, 1, material_specular);
        
        glUniform1f(material_shininess_uniform, material_shininess);
    }
    else
    {
        //set 'u_lighting_enabled' uniform
        glUniform1i(L_KeyPressed_uniform, 0);
    }
    
    
    vmath::mat4 modelMatrix = vmath::mat4::identity();
    vmath::mat4 viewMatrix = vmath::mat4::identity();
    
    //apply z axis translation to go deep into the screen by -2.0
    //so that triangle with same fullscreen coordinated but due to above translation will look small
    
    modelMatrix = vmath::translate(0.0f, 0.0f, -2.5f);
    
    glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, perspectiveProjectionMatrix);
    
    glBindVertexArray(gVao_Sphere);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
    glDrawElements(GL_TRIANGLES, numElemets, GL_UNSIGNED_SHORT, 0);
    
    glBindVertexArray(0);
    glUseProgram(0);
   
	CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
	CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    
    
    
    angleBlue = angleBlue + 0.3f;
    if (angleBlue > 360.0f)
        angleBlue = 0.0f;
    angleRadian = angleBlue * (3.1415926535 / 180.0);

    
    angleRed = angleRed + 0.3f;
    if (angleRed > 360.0f)
        angleRed = 0.0f;
    angleRadianRed = angleRed * (3.1415926535 / 180.0);
    
    angleGreen = angleGreen + 0.3f;
    if (angleGreen > 360.0f)
        angleGreen = 0.0f;
    angleRadianGreen = angleGreen * (3.1415926535 / 180.0);
    
}



-(BOOL)acceptsFirstResponder
{
[[self window]makeFirstResponder:self];
return(YES);

}


-(void)keyDown:(NSEvent *)theEvent
{
    // code
  
    int key=(int)[[theEvent characters]characterAtIndex:0];
    switch(key)
    {
        case 27: // Esc key
            [ self release];
            [NSApp terminate:self];
            break;
        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self];
            break;
        //case 0x4C:
        case 'L':
        case 'l':
            if(gbLKeyIsPressed == false)
            {
                gbLight = true;
                gbLKeyIsPressed = true;
            }
            else
            {
                gbLight = false;
                gbLKeyIsPressed = false;
            }
            break;
        default:
            break;
    }
}

-(void)mouseDown:(NSEvent *)theEvent
{
    // code
}

-(void)mouseDragged:(NSEvent *)theEvent
{
    // code
}

-(void)rightMouseDown:(NSEvent *)theEvent
{
    // code
}

- (void) dealloc
{
    // code
    if(gVao_Sphere)
	{
		glDeleteVertexArrays(1,&gVao_Sphere);
		gVao_Sphere=0;
	}

	if(gVbo_Sphere_positions)
	{
		glDeleteBuffers(1,&gVbo_Sphere_positions);
		gVbo_Sphere_positions=0;
	}
    
    if(gVbo_Sphere_element)
    {
        glDeleteBuffers(1,&gVbo_Sphere_element);
        gVbo_Sphere_element=0;
    }
    
    if(gVbo_Sphere_normal)
    {
        glDeleteBuffers(1,&gVbo_Sphere_element);
        gVbo_Sphere_element=0;
    }


	glDetachShader(shaderProgramObject,vertexShaderObject);
	glDetachShader(shaderProgramObject,fragmentShaderObject);
	
	glDeleteShader(vertexShaderObject);
	vertexShaderObject = 0;

	glDeleteShader(fragmentShaderObject);
	fragmentShaderObject = 0;

	glDeleteProgram(shaderProgramObject);
	shaderProgramObject = 0;

	CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);

    [super dealloc];
}

@end

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink,const CVTimeStamp *pNow,const CVTimeStamp *pOutputTime,CVOptionFlags flagsIn,
                               CVOptionFlags *pFlagsOut,void *pDisplayLinkContext)
{
    CVReturn result=[(GLView *)pDisplayLinkContext getFrameForTime:pOutputTime];
    return(result);
}































































