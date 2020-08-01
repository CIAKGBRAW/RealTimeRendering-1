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

	[window setTitle:@"macOS Ortho Triangle OpenGL Window"];
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

		GLuint vao;
		GLuint vbo;
		GLuint mvpUniform;

		vmath::mat4 orthographicProjectionMatrix;
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
    // OpenGL Info
    fprintf(gpFile, "OpenGL Version  : %s\n",glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL Version    : %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    [[self openGLContext]makeCurrentContext];
    
    GLint swapInt=1;
    [[self openGLContext]setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    // *** VERTEX SHADER ***
    // create shader
    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    
    // provide source code to shader
    const GLchar *vertexShaderSourceCode =
    "#version 410" \
    "\n" \
    "in vec4 vPosition;" \
    "uniform mat4 u_mvp_matrix;" \
    "void main(void)" \
    "{" \
    "gl_Position = u_mvp_matrix * vPosition;" \
    "}";
    glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);
    
    // compile shader
    glCompileShader(vertexShaderObject);
    GLint iInfoLogLength = 0;
    GLint iShaderCompiledStatus = 0;
    char *szInfoLog = NULL;
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
    if (iShaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (char *)malloc(iInfoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObject, iInfoLogLength, &written, szInfoLog);
                fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    
    // *** FRAGMENT SHADER ***
    // re-initialize
    iInfoLogLength = 0;
    iShaderCompiledStatus = 0;
    szInfoLog = NULL;
    
    // create shader
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    
    // provide source code to shader
    const GLchar *fragmentShaderSourceCode =
    "#version 410" \
    "\n" \
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "FragColor = vec4(1.0, 1.0, 1.0, 1.0);" \
    "}";
    glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);
    
    // compile shader
    glCompileShader(fragmentShaderObject);
    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
    if (iShaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (char *)malloc(iInfoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragmentShaderObject, iInfoLogLength, &written, szInfoLog);
                fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    
    // *** SHADER PROGRAM ***
    // create
    shaderProgramObject = glCreateProgram();
    
    // attach vertex shader to shader program
    glAttachShader(shaderProgramObject, vertexShaderObject);
    
    // attach fragment shader to shader program
    glAttachShader(shaderProgramObject, fragmentShaderObject);
    
    // pre-link binding of shader program object with vertex shader position attribute
    glBindAttribLocation(shaderProgramObject, VDG_ATTRIBUTE_VERTEX, "vPosition");
    
    // link shader
    glLinkProgram(shaderProgramObject);
    GLint iShaderProgramLinkStatus = 0;
    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
    if (iShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength>0)
        {
            szInfoLog = (char *)malloc(iInfoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject, iInfoLogLength, &written, szInfoLog);
                fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    
    // get MVP uniform location
    mvpUniform = glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
    
    // *** vertices, colors, shader attribs, vbo, vao initializations ***
    const GLfloat triangleVertices[] =
    { 0.0f, 50.0f, 0.0f, // apex
        -50.0f, -50.0f, 0.0f, // left-bottom
        50.0f, -50.0f, 0.0f // right-bottom
    };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(VDG_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glEnableVertexAttribArray(VDG_ATTRIBUTE_VERTEX);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glClearDepth(1.0f);
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    // depth test to do
    glDepthFunc(GL_LEQUAL);
    // We will always cull back faces for better performance
    glEnable(GL_CULL_FACE);
    
    // set background color
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // blue
    
    // set projection matrix to identity matrix
    orthographicProjectionMatrix = vmath::mat4::identity();
    
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink,&MyDisplayLinkCallback,self);
    CGLContextObj cglContext=(CGLContextObj)[[self openGLContext]CGLContextObj];
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
		//ORTHO CODE
    if (width <= height)
        orthographicProjectionMatrix = vmath::ortho(-100.0f, 100.0f, (-100.0f * (height / width)), (100.0f * (height / width)), -100.0f, 100.0f);
    else
        orthographicProjectionMatrix = vmath::ortho((-100.0f * (width / height)), (100.0f * (width / height)), -100.0f, 100.0f, -100.0f, 100.0f);
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
    
    vmath::mat4 modelViewMatrix = vmath::mat4::identity();
    vmath::mat4 modelViewProjectionMatrix = vmath::mat4::identity();
    
   
    modelViewProjectionMatrix = orthographicProjectionMatrix * modelViewMatrix; // ORDER IS IMPORTANT
    
      glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    
    
    glBindVertexArray(vao);
    
   
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 (each with its x,y,z ) vertices in triangleVertices array
    
    
    glBindVertexArray(0);
    
   
    glUseProgram(0);
	CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
	CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
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
    if(vao)
	{
		glDeleteVertexArrays(1,&vao);
		vao=0;
	}

	if(vbo)
	{
		glDeleteBuffers(1,&vbo);
		vbo=0;
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































































