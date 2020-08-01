#import <Foundation/Foundation.h>
#import<Cocoa/Cocoa.h>
#import<QuartzCore/CVDisplayLink.h>
#import<OpenGL/gl3.h>
#import<OpenGL/gl3ext.h>
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

	[window setTitle:@"macOS Three Lights Toggle OpenGL Window"];
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

		GLuint vertexShaderObject2;
		GLuint fragmentShaderObject2;
		GLuint shaderProgramObject2;
    
       float sphere_vertices[1146];
       float sphere_normals[1146];
       float sphere_textures[764];
       short sphere_elements[2280];
    
        GLuint gVao_Sphere;
        GLuint gVbo_Sphere_positions;
        GLuint gVbo_Sphere_normal;
        GLuint gVbo_Sphere_element;
    
        GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;
        GLuint model_matrix_uniform2, view_matrix_uniform2, projection_matrix_uniform2;

        GLfloat angleRed;
        GLfloat myArrRed[4];
        GLfloat angleRadianRed;
        GLfloat angleGreen;
        GLfloat myArrGreen[4];
        GLfloat angleRadianGreen;
        GLfloat angleBlue;
        GLfloat myArrBlue[4];
        GLfloat angleRadian;
    
        GLuint L_KeyPressed_uniform;
        GLuint L_KeyPressed_uniform2;

        GLuint La_uniform;
        GLuint Ld_uniform;
        GLuint Ls_uniform;
        GLuint light_position_uniform;
    
        GLuint Ka_uniform;
        GLuint Kd_uniform;
        GLuint Ks_uniform;
        GLuint material_shininess_uniform;

		
        GLuint La_uniform2;
        GLuint Ld_uniform2;
        GLuint Ls_uniform2;
        GLuint light_position_uniform2;
    
        GLuint Ka_uniform2;
        GLuint Kd_uniform2;
        GLuint Ks_uniform2;
        GLuint material_shininess_uniform2;
   
    //0.0f,0.0f,1.0f};
     GLfloat light_ambient[4];    
    GLfloat lightDiffuse[4];
    GLfloat lightSpecular[4];
    GLfloat lightPosition[4];
    
    GLfloat material_ambient[4];
    GLfloat material_diffuse[4];
    GLfloat material_specular[4];
     GLfloat material_shininess;
    bool gbLight;
    
    bool gbLKeyIsPressed;

	GLfloat light_ambient2[4];    
    GLfloat lightDiffuse2[4];
    GLfloat lightSpecular2[4];
    GLfloat lightPosition2[4];
    
    GLfloat material_ambient2[4];
    GLfloat material_diffuse2[4];
    GLfloat material_specular2[4];
    GLfloat material_shininess2;
    
    GLfloat light_ambient3[4];
    GLfloat lightDiffuse3[4];
    GLfloat lightSpecular3[4];
    GLfloat lightPosition3[4];
    
    GLuint La_uniform3;
    GLuint Ld_uniform3;
    GLuint Ls_uniform3;
    GLuint light_position_uniform3;
    
    GLuint Ka_uniform3;
    GLuint Kd_uniform3;
    GLuint Ks_uniform3;
    GLuint material_shininess_uniform3;
    
    GLfloat light_ambient4[4];
    GLfloat lightDiffuse4[4];
    GLfloat lightSpecular4[4];
    GLfloat lightPosition4[4];
    
    GLuint La_uniform4;
    GLuint Ld_uniform4;
    GLuint Ls_uniform4;
    GLuint light_position_uniform4;
    
    GLuint Ka_uniform4;
    GLuint Kd_uniform4;
    GLuint Ks_uniform4;
    GLuint material_shininess_uniform4;
    
    GLfloat light_ambient5[4];
    GLfloat lightDiffuse5[4];
    GLfloat lightSpecular5[4];
    GLfloat lightPosition5[4];
    
    GLuint La_uniform5;
    GLuint Ld_uniform5;
    GLuint Ls_uniform5;
    GLuint light_position_uniform5;
    
    GLuint Ka_uniform5;
    GLuint Kd_uniform5;
    GLuint Ks_uniform5;
    GLuint material_shininess_uniform5;
    
    GLfloat light_ambient6[4];
    GLfloat lightDiffuse6[4];
    GLfloat lightSpecular6[4];
    GLfloat lightPosition6[4];
    
    GLuint La_uniform6;
    GLuint Ld_uniform6;
    GLuint Ls_uniform6;
    GLuint light_position_uniform6;
    
    GLuint Ka_uniform6;
    GLuint Kd_uniform6;
    GLuint Ks_uniform6;
    GLuint material_shininess_uniform6;
    
    bool gbLight2;
    
    bool gbLKeyIsPressed2;
  
    
    int numVertices;
    int numElemets;
    
		GLuint vao;
		GLuint vbo;
		GLuint mvpUniform;

		GLuint vao2;
		GLuint vbo2;
		GLuint mvpUniform2;

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
    
    lightPosition[0] = 100.0f;
    lightPosition[1] = 100.0f;
    lightPosition[2] = 100.0f;
    lightPosition[3] = 1.0f;
    
    lightSpecular[0] = 1.0f;
    lightSpecular[1] = 0.0f;
    lightSpecular[2] = 0.0f;
    lightSpecular[3] = 0.0f;
    
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
    
    
    
    
    
    
    light_ambient2[0] = 0.0f;
    light_ambient2[1] = 0.0f;
    light_ambient2[2] = 0.0f;
    light_ambient2[3] = 1.0f;
    
    lightDiffuse2[0] = 1.0f;
    lightDiffuse2[1] = 0.0f;
    lightDiffuse2[2] = 0.0f;
    lightDiffuse2[3] = 0.0f;
    
    lightPosition2[0] = 100.0f;
    lightPosition2[1] = 100.0f;
    lightPosition2[2] = 100.0f;
    lightPosition2[3] = 1.0f;
    
    lightSpecular2[0] = 1.0f;
    lightSpecular2[1] = 0.0f;
    lightSpecular2[2] = 0.0f;
    lightSpecular2[3] = 0.0f;
    
    material_ambient2[0] = 0.0f;
    material_ambient2[1] = 0.0f;
    material_ambient2[2] = 0.0f;
    material_ambient2[3] = 1.0f;
    
    material_diffuse2[0] = 1.0f;
    material_diffuse2[1] = 1.0f;
    material_diffuse2[2] = 1.0f;
    material_diffuse2[3] = 1.0f;
    
    material_specular2[0] = 1.0f;
    material_specular2[1] = 1.0f;
    material_specular2[2] = 1.0f;
    material_specular2[3] = 1.0f;
    
    material_shininess2 = 50.0f;
    
    
    
    light_ambient3[0] = 0.0f;
    light_ambient3[1] = 0.0f;
    light_ambient3[2] = 0.0f;
    light_ambient3[3] = 1.0f;
    
    lightDiffuse3[0] = 0.0f;
    lightDiffuse3[1] = 1.0f;
    lightDiffuse3[2] = 0.0f;
    lightDiffuse3[3] = 0.0f;
    
    lightPosition3[0] = 100.0f;
    lightPosition3[1] = 0.0f;
    lightPosition3[2] = 100.0f;
    lightPosition3[3] = 1.0f;
    
    lightSpecular3[0] = 0.0f;
    lightSpecular3[1] = 1.0f;
    lightSpecular3[2] = 0.0f;
    lightSpecular3[3] = 0.0f;
    
    
    light_ambient4[0] = 0.0f;
    light_ambient4[1] = 0.0f;
    light_ambient4[2] = 0.0f;
    light_ambient4[3] = 1.0f;
    
    lightDiffuse4[0] = 0.0f;
    lightDiffuse4[1] = 1.0f;
    lightDiffuse4[2] = 0.0f;
    lightDiffuse4[3] = 0.0f;
    
    lightPosition4[0] = 100.0f;
    lightPosition4[1] = 0.0f;
    lightPosition4[2] = 100.0f;
    lightPosition4[3] = 1.0f;
    
    lightSpecular4[0] = 0.0f;
    lightSpecular4[1] = 1.0f;
    lightSpecular4[2] = 0.0f;
    lightSpecular4[3] = 0.0f;
    
    
    light_ambient5[0] = 0.0f;
    light_ambient5[1] = 0.0f;
    light_ambient5[2] = 0.0f;
    light_ambient5[3] = 1.0f;
    
    lightDiffuse5[0] = 0.0f;
    lightDiffuse5[1] = 0.0f;
    lightDiffuse5[2] = 1.0f;
    lightDiffuse5[3] = 0.0f;
    
    lightPosition5[0] = 100.0f;
    lightPosition5[1] = 100.0f;
    lightPosition5[2] = 0.0f;
    lightPosition5[3] = 1.0f;
    
    lightSpecular5[0] = 0.0f;
    lightSpecular5[1] = 0.0f;
    lightSpecular5[2] = 1.0f;
    lightSpecular5[3] = 0.0f;
    
    
    light_ambient6[0] = 0.0f;
    light_ambient6[1] = 0.0f;
    light_ambient6[2] = 0.0f;
    light_ambient6[3] = 1.0f;
    
    lightDiffuse6[0] = 0.0f;
    lightDiffuse6[1] = 0.0f;
    lightDiffuse6[2] = 1.0f;
    lightDiffuse6[3] = 0.0f;
    
    lightPosition6[0] = 100.0f;
    lightPosition6[1] = 100.0f;
    lightPosition6[2] = 0.0f;
    lightPosition6[3] = 1.0f;
    
    lightSpecular6[0] = 0.0f;
    lightSpecular6[1] = 0.0f;
    lightSpecular6[2] = 1.0f;
    lightSpecular6[3] = 0.0f;
    

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


-(void) perVertex
{
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
    "out vec3 phong_ads_colorGreen;"\
    "out vec3 phong_ads_colorBlue;"\
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
    "vec3 specularBlue = u_LsBlue * u_KsBlue * pow(max(dot(reflection_vectorBlue,viewer_vector),0.0),u_material_shininess);" \
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
    "in vec3 phong_ads_colorGreen;"\
    "in vec3 phong_ads_colorBlue;"\
    "out vec4 FragColor;" \
     "vec4 FragColor1;"\
    "vec4 FragColor2;"\
     "vec4 FragColor3;"\
    "void main(void)" \
    "{" \
    "FragColor1 = vec4(phong_ads_color,1.0);" \
    "FragColor2 = vec4(phong_ads_colorGreen,1.0);" \
    "FragColor3 = vec4(phong_ads_colorBlue,1.0);" \
    "FragColor = FragColor1 + FragColor2 + FragColor3;"\
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
    
    //shininess of material (value is conventionally between 1 to 200)
    material_shininess_uniform = glGetUniformLocation(shaderProgramObject, "u_material_shininess");
    
    ///////////////////////////////////////Green
    La_uniform3 = glGetUniformLocation(shaderProgramObject, "u_LaGreen");
    //diffuse color intensity of LIGHT
    Ld_uniform3 = glGetUniformLocation(shaderProgramObject, "u_LdGreen");
    //specular color intensity of LIGHT
    Ls_uniform3 = glGetUniformLocation(shaderProgramObject, "u_LsGreen");
    //position of light
    light_position_uniform3 = glGetUniformLocation(shaderProgramObject, "u_light_positionGreen");
    
    //ambient reflective color intensity of MATERIAL
    Ka_uniform3 = glGetUniformLocation(shaderProgramObject, "u_KaGreen");
    //diffuse reflective color intensity of MATERIAL
    Kd_uniform3 = glGetUniformLocation(shaderProgramObject, "u_KdGreen");
    
    //specular reflective color intensity of MATERIALc
    Ks_uniform3 = glGetUniformLocation(shaderProgramObject, "u_KsGreen");
    
    
    ///////////////////////////////////////Blue
    La_uniform5 = glGetUniformLocation(shaderProgramObject, "u_LaBlue");
    //diffuse color intensity of LIGHT
    Ld_uniform5 = glGetUniformLocation(shaderProgramObject, "u_LdBlue");
    //specular color intensity of LIGHT
    Ls_uniform5 = glGetUniformLocation(shaderProgramObject, "u_LsBlue");
    //position of light
    light_position_uniform5 = glGetUniformLocation(shaderProgramObject, "u_light_positionBlue");
    
    //ambient reflective color intensity of MATERIAL
    Ka_uniform5 = glGetUniformLocation(shaderProgramObject, "u_KaBlue");
    //diffuse reflective color intensity of MATERIAL
    Kd_uniform5 = glGetUniformLocation(shaderProgramObject, "u_KdBlue");
    
    //specular reflective color intensity of MATERIALc
    Ks_uniform5 = glGetUniformLocation(shaderProgramObject, "u_KsBlue");
}
-(void) perFragment
{
    vertexShaderObject2 = glCreateShader(GL_VERTEX_SHADER);
    
    //vertex shader
    //provide source code to shader
    const GLchar * vertexShaderShaderCode2 =
    "#version 410" \
    "\n" \
    "in vec4 vPosition;" \
    "in vec3 vNormal;" \
    "uniform mat4 u_model_matrix2;" \
    "uniform mat4 u_view_matrix2;" \
    "uniform mat4 u_projection_matrix2;" \
    "uniform vec4 u_light_position2;" \
    "uniform vec4 u_light_positionGreen;" \
    "uniform vec4 u_light_positionBlue;" \
    "uniform int u_lighting_enabled2;" \
    "out vec3 transformed_normals2;" \
    "out vec3 light_direction2;" \
    "out vec3 light_directionGreen;" \
     "out vec3 light_directionBlue;" \
    "out vec3 viewer_vector2;" \
    "void main(void)" \
    "{" \
    "if(u_lighting_enabled2 == 1)" \
    "{" \
    "vec4 eye_coordinates = u_view_matrix2*u_model_matrix2*vPosition;" \
    "transformed_normals2 = mat3(u_view_matrix2 * u_model_matrix2) * vNormal;" \
    "light_direction2 = vec3(u_light_position2) - eye_coordinates.xyz;" \
     "light_directionGreen = vec3(u_light_positionGreen) - eye_coordinates.xyz;" \
    "light_directionBlue = vec3(u_light_positionBlue) - eye_coordinates.xyz;" \
    "viewer_vector2 = -eye_coordinates.xyz;" \
    "}" \
    "gl_Position = u_projection_matrix2 * u_view_matrix2 * u_model_matrix2 * vPosition;" \
    "}";
    
    glShaderSource(vertexShaderObject2, 1, (const GLchar**)&vertexShaderShaderCode2, NULL);
    
    //compile shader
    glCompileShader(vertexShaderObject2);
    
  GLint  iInfoLength = 0;
  GLint  iShaderCompiledStatus = 0;
    char *szInfoLog = NULL;
    
    glGetShaderiv(vertexShaderObject2, GL_COMPILE_STATUS, &iShaderCompiledStatus);
    if (iShaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject2, GL_INFO_LOG_LENGTH, &iInfoLength);
        if (iInfoLength > 0)
        {
            szInfoLog = (char *)malloc(iInfoLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObject2, iInfoLength, &written, szInfoLog);
                fprintf(gpFile, "Vertex Shader Compilation Log2:%s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    
    //*****FRAGMENT SHADER****
    fragmentShaderObject2 = glCreateShader(GL_FRAGMENT_SHADER);
    
    //provide source code to shader
    const GLchar *fragmentShaderSourceCode2 =
    "#version 410" \
    "\n" \
    "in vec3 transformed_normals2;" \
    "in vec3 light_direction2;" \
     "in vec3 light_directionGreen;" \
     "in vec3 light_directionBlue;" \
    "in vec3 viewer_vector2;" \
    "out vec4 FragColor;" \
    "uniform vec3 u_La2;" \
    "uniform vec3 u_Ld2;" \
    "uniform vec3 u_Ls2;" \
    "uniform vec3 u_Ka2;" \
    "uniform vec3 u_Kd2;" \
    "uniform vec3 u_Ks2;" \
    
    "uniform vec3 u_LaGreen;" \
    "uniform vec3 u_LdGreen;" \
    "uniform vec3 u_LsGreen;" \
    "uniform vec3 u_KaGreen;" \
    "uniform vec3 u_KdGreen;" \
    "uniform vec3 u_KsGreen;" \
    
    "uniform vec3 u_LaBlue;" \
    "uniform vec3 u_LdBlue;" \
    "uniform vec3 u_LsBlue;" \
    "uniform vec3 u_KaBlue;" \
    "uniform vec3 u_KdBlue;" \
    "uniform vec3 u_KsBlue;" \
    "uniform float u_material_shininess2;" \
    "uniform int u_lighting_enabled2;" \
    "vec4 FragColorGreen;" \
    "vec4 FragColorRed;" \
    "vec4 FragColorBlue;" \
    "void main(void)" \
    "{" \
    "vec3 phong_ads_color;" \
    "vec3 phong_ads_colorGreen;" \
    "vec3 phong_ads_colorBlue;"\
    "if(u_lighting_enabled2==1)"\
    "{" \
    "vec3 normalized_transformed_normals=normalize(transformed_normals2);" \
    "vec3 normalized_viewer_vector=normalize(viewer_vector2);" \
    
     "vec3 normalized_light_direction=normalize(light_direction2);" \
    "vec3 ambient = u_La2 * u_Ka2;" \
    "float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction),0.0);" \
    "vec3 diffuse = u_Ld2 * u_Kd2 * tn_dot_ld;" \
    "vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normals);" \
    "vec3 specular = u_Ls2 * u_Ks2 * pow(max(dot(reflection_vector,normalized_viewer_vector),0.0),u_material_shininess2);" \
    "phong_ads_color = ambient + diffuse + specular;" \
    "FragColorRed = vec4(phong_ads_color,1.0);"\
    
    "vec3 normalized_light_directionGreen=normalize(light_directionGreen);" \
    "vec3 ambientGreen = u_LaGreen * u_KaGreen;" \
    "float tn_dot_ldGreen = max(dot(normalized_transformed_normals, normalized_light_directionGreen),0.0);" \
    "vec3 diffuseGreen = u_LdGreen * u_KdGreen * tn_dot_ldGreen;" \
    "vec3 reflection_vectorGreen = reflect(-normalized_light_directionGreen,normalized_transformed_normals);" \
    "vec3 specularGreen = u_LsGreen * u_KsGreen * pow(max(dot(reflection_vectorGreen,normalized_viewer_vector),0.0),u_material_shininess2);" \
    "phong_ads_colorGreen = ambientGreen + diffuseGreen + specularGreen;" \
    "FragColorGreen = vec4(phong_ads_colorGreen,1.0);"\
    
    
    "vec3 normalized_light_directionBlue=normalize(light_directionBlue);" \
    "vec3 ambientBlue = u_LaBlue * u_KaBlue;" \
    "float tn_dot_ldBlue = max(dot(normalized_transformed_normals, normalized_light_directionBlue),0.0);" \
    "vec3 diffuseBlue= u_LdBlue * u_KdBlue * tn_dot_ldBlue;" \
    "vec3 reflection_vectorBlue= reflect(-normalized_light_directionBlue,normalized_transformed_normals);" \
    "vec3 specularBlue= u_LsBlue* u_KsBlue* pow(max(dot(reflection_vectorBlue,normalized_viewer_vector),0.0),u_material_shininess2);" \
    "phong_ads_colorBlue = ambientBlue + diffuseBlue + specularBlue;" \
    "FragColorBlue= vec4(phong_ads_colorBlue,1.0);"\
    "FragColor = FragColorRed + FragColorGreen + FragColorBlue;"\
    "}" \
    "else"\
    "{" \
    "FragColor = vec4(1.0,1.0,1.0,1.0);" \
    "}"\
    "}";

    glShaderSource(fragmentShaderObject2,1,(const GLchar**)&fragmentShaderSourceCode2,NULL);
    
    //compile shader
    glCompileShader(fragmentShaderObject2);
    glGetShaderiv(fragmentShaderObject2,GL_COMPILE_STATUS,&iShaderCompiledStatus);
    if(iShaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject2,GL_INFO_LOG_LENGTH,&iInfoLength);
        if(iInfoLength > 0)
        {
            szInfoLog = (char *)malloc(iInfoLength);
            if(szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragmentShaderObject2,iInfoLength,&written,szInfoLog);
                fprintf(gpFile,"Fragment Shader Compilation Log:%s\n",szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    
    
    //// SHADER PROGRAM
    shaderProgramObject = glCreateProgram();
    
        glAttachShader(shaderProgramObject,vertexShaderObject2);
    
       glAttachShader(shaderProgramObject,fragmentShaderObject2);
    
    glBindAttribLocation(shaderProgramObject,VDG_ATTRIBUTE_VERTEX,"vPosition");
    
    glBindAttribLocation(shaderProgramObject,VDG_ATTRIBUTE_NORMAL,"vNormal");
    
    glLinkProgram(shaderProgramObject);
 GLint   iShaderProgramLinkStatus = 0;
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
    
    model_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_model_matrix2");
    view_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_view_matrix2");
    projection_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_projection_matrix2");
    
    //L/I Key is pressed or not
    L_KeyPressed_uniform2 = glGetUniformLocation(shaderProgramObject, "u_lighting_enabled2");
    
    //ambient color intensity of LIGHT
    La_uniform2 = glGetUniformLocation(shaderProgramObject, "u_La2");
    //diffuse color intensity of LIGHT
    Ld_uniform2 = glGetUniformLocation(shaderProgramObject, "u_Ld2");
    //specular color intensity of LIGHT
    Ls_uniform2 = glGetUniformLocation(shaderProgramObject, "u_Ls2");
    //position of light
    light_position_uniform2 = glGetUniformLocation(shaderProgramObject, "u_light_position2");
    
    //ambient reflective color intensity of MATERIAL
    Ka_uniform2 = glGetUniformLocation(shaderProgramObject, "u_Ka2");
    //diffuse reflective color intensity of MATERIAL
    Kd_uniform2 = glGetUniformLocation(shaderProgramObject, "u_Kd2");
    
    //specular reflective color intensity of MATERIALc
    Ks_uniform2 = glGetUniformLocation(shaderProgramObject, "u_Ks2");
    
    
    
    ///////////////////////////////////////Green
    La_uniform4 = glGetUniformLocation(shaderProgramObject, "u_LaGreen");
    //diffuse color intensity of LIGHT
    Ld_uniform4 = glGetUniformLocation(shaderProgramObject, "u_LdGreen");
    //specular color intensity of LIGHT
    Ls_uniform4 = glGetUniformLocation(shaderProgramObject, "u_LsGreen");
    //position of light
    light_position_uniform4 = glGetUniformLocation(shaderProgramObject, "u_light_positionGreen");
    
    //ambient reflective color intensity of MATERIAL
    Ka_uniform4 = glGetUniformLocation(shaderProgramObject, "u_KaGreen");
    //diffuse reflective color intensity of MATERIAL
    Kd_uniform4 = glGetUniformLocation(shaderProgramObject, "u_KdGreen");
    
    //specular reflective color intensity of MATERIALc
    Ks_uniform4 = glGetUniformLocation(shaderProgramObject, "u_KsGreen");
    
    
    ///////////////////////////////////////Blue
    La_uniform6 = glGetUniformLocation(shaderProgramObject, "u_LaBlue");
    //diffuse color intensity of LIGHT
    Ld_uniform6 = glGetUniformLocation(shaderProgramObject, "u_LdBlue");
    //specular color intensity of LIGHT
    Ls_uniform6 = glGetUniformLocation(shaderProgramObject, "u_LsBlue");
    //position of light
    light_position_uniform6 = glGetUniformLocation(shaderProgramObject, "u_light_positionBlue");
    
    //ambient reflective color intensity of MATERIAL
    Ka_uniform6 = glGetUniformLocation(shaderProgramObject, "u_KaBlue");
    //diffuse reflective color intensity of MATERIAL
    Kd_uniform6 = glGetUniformLocation(shaderProgramObject, "u_KdBlue");
    
    //specular reflective color intensity of MATERIALc
    Ks_uniform6 = glGetUniformLocation(shaderProgramObject, "u_KsBlue");
    

    //shininess of material (value is conventionally between 1 to 200)
    material_shininess_uniform2 = glGetUniformLocation(shaderProgramObject, "u_material_shininess2");
    
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
    vmath::mat4 modelMatrix = vmath::mat4::identity();
    vmath::mat4 viewMatrix = vmath::mat4::identity();
     [self perVertex];
    if (gbLight == true)
    {
        myArrRed[1] =     100 * (cos ( 2*3.1415926535 * angleRadianRed ));
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
        glUniform1f(material_shininess_uniform, material_shininess);
        
        glUniform3fv(La_uniform3, 1, light_ambient3);
        glUniform3fv(Ld_uniform3, 1, lightDiffuse3);
        glUniform3fv(Ls_uniform3, 1, lightSpecular3);
        glUniform4fv(light_position_uniform3, 1, myArrGreen);
        
        //setting material's properties
        glUniform3fv(Ka_uniform3, 1, material_ambient);
        glUniform3fv(Kd_uniform3, 1, material_diffuse);
        glUniform3fv(Ks_uniform3, 1, material_specular);
        glUniform1f(material_shininess_uniform, material_shininess);
        
        glUniform3fv(La_uniform5, 1, light_ambient5);
        glUniform3fv(Ld_uniform5, 1, lightDiffuse5);
        glUniform3fv(Ls_uniform5, 1, lightSpecular5);
        glUniform4fv(light_position_uniform5, 1, myArrBlue);
        
        //setting material's properties
        glUniform3fv(Ka_uniform5, 1, material_ambient);
        glUniform3fv(Kd_uniform5, 1, material_diffuse);
        glUniform3fv(Ks_uniform5, 1, material_specular);
        glUniform1f(material_shininess_uniform, material_shininess);
        
      //  [self perVertex];
    }
  
    
   if (gbLight2 == true)
    {
        modelMatrix = vmath::mat4::identity();
      viewMatrix = vmath::mat4::identity();
        
        myArrRed[1] =     100 * (cos ( 2*3.1415926535 * angleRadianRed ));
        myArrRed[2] =   100 * (sin ( 2*3.1415926535 * angleRadianRed ));
        
        myArrGreen[0] = 100 * (cos ( 2*3.1415926535 * angleRadianGreen ));
        myArrGreen[2] = 100 * (sin (2 * 3.1415 * angleRadianGreen));
        
        myArrBlue[0] =     100 * (cos ( 2*3.1415926535 * angleRadian ));
        myArrBlue[1] =     100 * (sin ( 2*3.1415926535 * angleRadian ));
    
        glUniform1i(L_KeyPressed_uniform2, 1);
        
        //setting lights properties
        glUniform3fv(La_uniform2, 1, light_ambient2);
        glUniform3fv(Ld_uniform2, 1, lightDiffuse2);
        glUniform3fv(Ls_uniform2, 1, lightSpecular2);
        glUniform4fv(light_position_uniform2, 1, myArrRed);
        
        //setting material's properties
        glUniform3fv(Ka_uniform2, 1, material_ambient2);
        glUniform3fv(Kd_uniform2, 1, material_diffuse2);
        glUniform3fv(Ks_uniform2, 1, material_specular2);
        glUniform1f(material_shininess_uniform2, material_shininess2);
        
        glUniform3fv(La_uniform4, 1, light_ambient4);
        glUniform3fv(Ld_uniform4, 1, lightDiffuse4);
        glUniform3fv(Ls_uniform4, 1, lightSpecular4);
        glUniform4fv(light_position_uniform4, 1, myArrGreen);
        
        //setting material's properties
        glUniform3fv(Ka_uniform4, 1, material_ambient2);
        glUniform3fv(Kd_uniform4, 1, material_diffuse2);
        glUniform3fv(Ks_uniform4, 1, material_specular2);
        glUniform1f(material_shininess_uniform4, material_shininess2);
        
        glUniform3fv(La_uniform6, 1, light_ambient6);
        glUniform3fv(Ld_uniform6, 1, lightDiffuse6);
        glUniform3fv(Ls_uniform6, 1, lightSpecular6);
        glUniform4fv(light_position_uniform6, 1, myArrBlue);
        
        //setting material's properties
        glUniform3fv(Ka_uniform6, 1, material_ambient2);
        glUniform3fv(Kd_uniform6, 1, material_diffuse2);
        glUniform3fv(Ks_uniform6, 1, material_specular2);
        glUniform1f(material_shininess_uniform4, material_shininess2);
        
        
        glUniformMatrix4fv(model_matrix_uniform2, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(view_matrix_uniform2, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projection_matrix_uniform2, 1, GL_FALSE, perspectiveProjectionMatrix);
         [self perFragment];
    }
   
    


    //OpenGL Drawing
   
    
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





	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*


	  glUseProgram(shaderProgramObject2);
    
    if (gbLight2 == true)
    {
        
        //set u_lighting_enabled uniform
        glUniform1i(L_KeyPressed_uniform2, 1);
//
        //setting lights properties
        glUniform3fv(La_uniform2, 1, light_ambient2);
        glUniform3fv(Ld_uniform2, 1, lightDiffuse2);
        glUniform3fv(Ls_uniform2, 1, lightSpecular2);
        glUniform4fv(light_position_uniform2, 1, lightPosition2);
        
        //setting material's properties
        glUniform3fv(Ka_uniform2, 1, material_ambient2);
        glUniform3fv(Kd_uniform2, 1, material_diffuse2);
        glUniform3fv(Ks_uniform2, 1, material_specular2);
        glUniform1f(material_shininess_uniform2, material_shininess2);
    }
    else
    {
        //set 'u_lighting_enabled' uniform
        glUniform1i(L_KeyPressed_uniform2, 0);
    }
    
    
    //OpenGL Drawing
   // vmath::mat4 modelMatrix = vmath::mat4::identity();
   // vmath::mat4 viewMatrix = vmath::mat4::identity();
    

	modelMatrix = vmath::mat4::identity();
	viewMatrix = vmath::mat4::identity();
    
    modelMatrix = vmath::translate(0.0f, 0.0f, -2.5f);
    
    glUniformMatrix4fv(model_matrix_uniform2, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(view_matrix_uniform2, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projection_matrix_uniform2, 1, GL_FALSE, perspectiveProjectionMatrix);
    
    glBindVertexArray(gVao_Sphere);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
    glDrawElements(GL_TRIANGLES, numElemets, GL_UNSIGNED_SHORT, 0);
    
    glBindVertexArray(0);
    glUseProgram(0);
	*/
   
	CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
	CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    angleBlue = angleBlue - 0.3f;
    if (angleBlue > 360.0f)
        angleBlue = 0.0f;
    angleRadian = angleBlue * (3.1415926535 / 180.0);
    
    
    angleRed = angleRed - 0.3f;
    if (angleRed > 360.0f)
        angleRed = 0.0f;
    angleRadianRed = angleRed * (3.1415926535 / 180.0);
    
    angleGreen = angleGreen - 0.3f;
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
        case'q': // Esc key
        case 'Q':
            [ self release];
            [NSApp terminate:self];
            break;
        case 27:
       
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

        case 'f':
        case 'F':
			 if(gbLKeyIsPressed2 == false)
            {
                gbLight2 = true;
                gbLKeyIsPressed2 = true;
            }
            else
            {
                gbLight2 = false;
                gbLKeyIsPressed2 = false;
            }
            break;
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































































