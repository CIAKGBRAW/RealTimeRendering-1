#import <Foundation/Foundation.h>
#import<Cocoa/Cocoa.h>
#import<QuartzCore/CVDisplayLink.h>
#import<OpenGL/gl3.h>
#import<OpenGL/gl3ext.h>
#import "vmath.h"


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

	[window setTitle:@"macOS Perspective Triangle OpenGL Window"];
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

		GLuint vao_Pyramid;
		GLuint vbo_Position_Pyramid;
		GLuint vbo_Normal_Pyramid;

		GLuint L_KeyPressed_uniform;
    
        GLuint L_KeyPressed_uniformRed;
    GLuint material_shininess_uniform;

        GLuint La_uniformBlue;
        GLuint Ld_uniformBlue;
        GLuint Ls_uniformBlue;
        GLuint light_position_uniformBlue;
        GLuint light_position_uniformRed;
    
        GLuint Ka_uniformBlue;
        GLuint Kd_uniformBlue;
        GLuint Ks_uniformBlue;
        GLuint material_shininess_uniformRed;
    
        GLuint La_uniformRed;
        GLuint Ld_uniformRed;
        GLuint Ls_uniformRed;
    
        GLuint Ka_uniformRed;
        GLuint Kd_uniformRed;
        GLuint Ks_uniformRed;
    GLuint material_shininess_uniformBlue;
    
    
        GLfloat light_ambient[4];
        GLfloat lightDiffuse[4];
        GLfloat lightSpecular[4];
        GLfloat lightPosition[4];
    
        GLfloat light_ambient2[4];
        GLfloat lightDiffuse2[4];
        GLfloat lightSpecular2[4];
        GLfloat lightPosition2[4];
    
    
        GLfloat material_ambient[4];
        GLfloat material_diffuse[4];
        GLfloat material_specular[4];
        GLfloat material_shininess;
    bool gbLight;
    
    GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;
    
   
    
			
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
	fprintf(gpFile,"OpenGL Version: %s\n",glGetString(GL_VERSION));
	fprintf(gpFile,"OpenGL Version: %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

	[[self openGLContext]makeCurrentContext];

	GLint swapInt = 1;
	[[self openGLContext]setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

	//create shader
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//vertex shader
		const GLchar *vertexShaderSourceCode =
		"#version 410" \
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
    

	 glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);


			//compile shader
		glCompileShader(vertexShaderObject);
		GLint iInfoLogLength  = 0;
		GLint iShaderCompiledStatus = 0;
		char * szInfoLog=NULL;
		glGetShaderiv(vertexShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
		if(iShaderCompiledStatus == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
			if(iInfoLogLength > 0)
			{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if(szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject,iInfoLogLength,&written,szInfoLog);
				fprintf(gpFile,"Vertex Shader Compilation Log:%s\n",szInfoLog);
				free(szInfoLog);
				[self release];
				[NSApp terminate:self];
			}
		}
	}



	//FRAGMENT SHADER
	iInfoLogLength	=0;
	iShaderCompiledStatus = 0;
	szInfoLog = NULL;

	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar *fragmentShaderSourceCode = 
		"#version 410" \
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

	glShaderSource(fragmentShaderObject,1,(const GLchar**)&fragmentShaderSourceCode,NULL);

	//compile shader
	glCompileShader(fragmentShaderObject);
	glGetShaderiv(fragmentShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
	if(iShaderCompiledStatus == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
			if(iInfoLogLength > 0)
			{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if(szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject,iInfoLogLength,&written,szInfoLog);
				fprintf(gpFile,"Vertex Shader Compilation Log:%s\n",szInfoLog);
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
        glGetProgramiv(shaderProgramObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
        if(iInfoLogLength >0)
        {
            szInfoLog = (char *)malloc(iInfoLogLength);
            if(szInfoLog != NULL)
            {
                
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject,iInfoLogLength,&written,szInfoLog);
                fprintf(gpFile, "Shader Program Link Log : %s\n",szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    
    
    model_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_model_matrix");
    view_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_view_matrix");
    projection_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
    //L/I Key is pressed or not
       //ambient color intensity of LIGHT
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
    //specular reflective color intensity of MATERIAL
    Ks_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_KsBlue");
    
    //shininess of material (value is conventionally between 1 to 200)
    material_shininess_uniform = glGetUniformLocation(shaderProgramObject, "u_material_shininess");
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    La_uniformRed = glGetUniformLocation(shaderProgramObject, "u_LaRed");
    
    Ls_uniformRed = glGetUniformLocation(shaderProgramObject, "u_LsRed");
    
    Ld_uniformRed = glGetUniformLocation(shaderProgramObject, "u_LdRed");
    
    Ka_uniformRed = glGetUniformLocation(shaderProgramObject, "u_KaRed");
    
    Ks_uniformRed = glGetUniformLocation(shaderProgramObject, "u_KsRed");
    
    Kd_uniformRed = glGetUniformLocation(shaderProgramObject, "u_KdRed");
    
    light_position_uniformRed = glGetUniformLocation(shaderProgramObject, "u_light_positionRed");
    
    material_shininess_uniformRed = glGetUniformLocation(shaderProgramObject, "u_material_shininessRed");
    
    L_KeyPressed_uniformRed = glGetUniformLocation(shaderProgramObject, "u_lighting_enabledRed");

    const GLfloat pyramidVertices[] =
    { 0.0f, 1.0f, 0.0f,
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

		
	const GLfloat colorVerticesPyramid[]=
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




    
    glGenVertexArrays(1, &vao_Pyramid);
    glBindVertexArray(vao_Pyramid);
    
    glGenBuffers(1, &vbo_Position_Pyramid);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Pyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);    
    glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);    
    glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	//vbo color triangle
    
    glGenBuffers(1, &vbo_Normal_Pyramid);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_Normal_Pyramid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorVerticesPyramid), colorVerticesPyramid, GL_STATIC_DRAW);    
    glVertexAttribPointer(VDG_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);    
    glEnableVertexAttribArray(VDG_ATTRIBUTE_COLOR);    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

    
    
    light_ambient[0] = 0.0f;
    light_ambient[1] = 0.0f;
    light_ambient[2] = 0.0f;
    light_ambient[3] = 1.0f;
    
    lightDiffuse[0] = 1.0f;
    lightDiffuse[1] = 0.0f;
    lightDiffuse[2] = 0.0f;
    lightDiffuse[3] = 0.0f;
    
    lightPosition[0] = 2.0f;
    lightPosition[1] = 1.0f;
    lightPosition[2] = 1.0f;
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
    lightDiffuse2[1] = 0.0f;
    lightDiffuse2[2] = 1.0f;
    lightDiffuse2[3] = 0.0f;
    
    lightPosition2[0] = -2.0f;
    lightPosition2[1] = 0.0f;
    lightPosition2[2] = 0.0f;
    lightPosition2[3] = 1.0f;
    
    lightSpecular2[0] = 0.0f;
    lightSpecular2[1] = 0.0f;
    lightSpecular2[2] = 1.0f;
    lightSpecular2[3] = 0.0f;
    
    
    
    
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

	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	CVDisplayLinkSetOutputCallback(displayLink,&MyDisplayLinkCallback,self);
	CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
    CGLPixelFormatObj cglPixelFormat=(CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink,cglContext,cglPixelFormat);
	CVDisplayLinkStart(displayLink);
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
    
    static float gAnglePyramid =0.0f;
    static float gAngleCube = 0.0f;
    
    [[self openGLContext]makeCurrentContext];
    
	CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);
	
	glUseProgram(shaderProgramObject);

    if (gbLight == true)
    {
        
        //set u_lighting_enabled uniform
        glUniform3fv(La_uniformBlue, 1, light_ambient2);
        glUniform3fv(Ld_uniformBlue, 1, lightDiffuse2);
        glUniform3fv(Ls_uniformBlue, 1, lightSpecular2);
        glUniform4fv(light_position_uniformBlue, 1, lightPosition2);
        
        glUniform3fv(Ka_uniformBlue, 1, material_ambient);
        glUniform3fv(Kd_uniformBlue, 1, material_diffuse);
        glUniform3fv(Ks_uniformBlue, 1, material_specular);
        glUniform1f(material_shininess_uniform, material_shininess);
        
        glUniform1i(L_KeyPressed_uniform, 1);
        
        glUniform3fv(La_uniformRed, 1, light_ambient);
        glUniform3fv(Ld_uniformRed, 1, lightDiffuse);
        glUniform3fv(Ls_uniformRed, 1, lightSpecular);
        glUniform4fv(light_position_uniformRed, 1, lightPosition);
        
        glUniform3fv(Ka_uniformRed, 1, material_ambient);
        glUniform3fv(Kd_uniformRed, 1, material_diffuse);
        glUniform3fv(Ks_uniformRed, 1, material_specular);
        glUniform1f(material_shininess_uniformRed, material_shininess);

    }
    else
    {
        //set 'u_lighting_enabled' uniform
        glUniform1i(L_KeyPressed_uniform, 0);
    }
    vmath::mat4 modelViewMatrix = vmath::mat4::identity();
    vmath::mat4 modelViewProjectionMatrix = vmath::mat4::identity();
	vmath::mat4 rotationmatrix = vmath::mat4::identity();
	vmath::mat4 scalematrix = vmath::mat4::identity();
    
	modelViewMatrix = vmath::translate(-1.5f,0.0f,-6.0f);
	rotationmatrix = vmath::rotate(gAnglePyramid,0.0f,1.0f,0.0f);
	scalematrix = vmath::scale(0.75f,0.75f,0.75f);
	
	modelViewMatrix = modelViewMatrix * rotationmatrix;
	modelViewMatrix = modelViewMatrix * scalematrix;
    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix; 

   
    
	glBindVertexArray(vao_Pyramid);

	glDrawArrays(GL_TRIANGLES, 0, 12);			
													
	glBindVertexArray(0);


    glUseProgram(0);
	CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
	CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    
    gAnglePyramid = gAnglePyramid + 1.0f;
    gAngleCube = gAngleCube + 1.0f;
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
    if(vao_Pyramid)
	{
		glDeleteVertexArrays(1,&vao_Pyramid);
		vao_Pyramid=0;
	}


	if(vbo_Position_Pyramid)
	{
		glDeleteBuffers(1,&vbo_Position_Pyramid);
		vbo_Position_Pyramid=0;
	}

	if(vbo_Normal_Pyramid)
	{
		glDeleteBuffers(1,&vbo_Normal_Pyramid);
		vbo_Normal_Pyramid=0;
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































































