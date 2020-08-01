#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "Sphere.h"
#import "GLESView.h"
#import "vmath.h"

enum
{
    VDG_ATTRIBUTE_VERTEX = 0,
    VDG_ATTRIBUTE_COLOR,
    VDG_ATTRIBUTE_NORMAL,
    VDG_ATTRIBUTE_TEXTURE0,
};



@implementation GLESView
{
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    int singleTap;
    
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
    
    GLfloat light_ambient2[4];
    GLfloat lightDiffuse2[4];
    GLfloat lightSpecular2[4];
    GLfloat lightPosition2[4];
    
    GLfloat material_ambient2[4];
    GLfloat material_diffuse2[4];
    GLfloat material_specular2[4];
    GLfloat material_shininess2;
    bool gbLight2;
    
    
    bool gbLKeyIsPressed;
    
    int numVertices;
    int numElemets;
    
    id displayLink;
    NSInteger animationFrameInterval;
    BOOL isAnimating;
    
    Sphere sphere;
    
    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;
    
    GLuint vertexShaderObject2;
    GLuint fragmentShaderObject2;
    GLuint shaderProgramObject2;
    
    vmath::mat4 perspectiveProjectionMatrix;
    
}



-(id)initWithFrame:(CGRect)frame;
{
    self = [super initWithFrame:frame];
    if(self)
    {
        
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)super.layer;
        
        eaglLayer.opaque=YES;
        eaglLayer.drawableProperties=[NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE],
                                      kEAGLDrawablePropertyRetainedBacking,kEAGLColorFormatRGBA8,kEAGLDrawablePropertyColorFormat,nil];
        
        eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if(eaglContext == nil)
        {
            [self release];
            return(nil);
        }
        
        [EAGLContext setCurrentContext:eaglContext];
        
        glGenFramebuffers(1,&defaultFramebuffer);
        glGenRenderbuffers(1,&colorRenderbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER,colorRenderbuffer);
        
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,colorRenderbuffer);
        
        GLint backingWidth;
        GLint backingHeight;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH,&backingWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT,&backingHeight);
        
        
        glGenRenderbuffers(1,&depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER,depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,backingWidth,backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthRenderbuffer);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Failed to create complete Framebuffer object %x",glCheckFramebufferStatus(GL_FRAMEBUFFER));
            glDeleteFramebuffers(1,&defaultFramebuffer);
            glDeleteRenderbuffers(1,&colorRenderbuffer);
            glDeleteRenderbuffers(1,&depthRenderbuffer);
            
            return(nil);
        }
        
        
        printf("Renderer :%s |GL VERSION :%s | GL VERSION : %s\n",glGetString(GL_RENDERER),glGetString(GL_VERSION),glGetString(GL_SHADING_LANGUAGE_VERSION));
        
        isAnimating=NO;
        animationFrameInterval = 60;
        
/*        //vertex shader
        vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
        //provide source code to shader
        const GLchar * vertexShaderShaderCode =
        "#version 300 es" \
        "\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_projection_matrix;" \
        "uniform vec4 u_light_position;" \
        "out vec3 transformed_normals;" \
        "out vec3 light_direction;" \
        "out vec3 viewer_vector;" \
        "void main(void)" \
        "{" \
        "vec4 eye_coordinates = u_view_matrix*u_model_matrix*vPosition;" \
        "transformed_normals = mat3(u_view_matrix * u_model_matrix) * vNormal;" \
        "light_direction = vec3(u_light_position) - eye_coordinates.xyz;" \
        "viewer_vector = -eye_coordinates.xyz;" \
        "gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
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
                    printf("Fragment Shader Compilation Log:%s\n",szInfoLog);
                    free(szInfoLog);
                    [self release];
                    
                }
            }
        }
        
        //*****FRAGMENT SHADER****
        fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        
        //provide source code to shader
        const GLchar *fragmentShaderSourceCode =
        "#version 300 es" \
        "\n" \
        "precision highp float;"\
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
        "uniform int u_lighting_enabled;"\
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
                    printf("Fragment Shader Compilation Log:%s\n",szInfoLog);
                    free(szInfoLog);
                    [self release];
                    
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
                    printf("Shader Program Link Log : %s\n",szInfoLog);
                    free(szInfoLog);
                    [self release];
                    
                }
            }
        }
        
        
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
 
 
 
 */
        
        
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
        //    glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        
        perspectiveProjectionMatrix = vmath::mat4::identity();
        
        
        UITapGestureRecognizer *singleTapGestureRecognizer=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        UITapGestureRecognizer *doubleTapGestureRecognizer=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        UISwipeGestureRecognizer *swipeGestureRecognizer=[[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
    }
    return(self);
}


//only override drawRect
/*- (void) drawRect:(CGRect)rect
 {
 
 }
 */





-(void) perVertex
{
    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    
    //vertex shader
    //provide source code to shader
    const GLchar * vertexShaderShaderCode =
    "#version 300 es" \
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
    "uniform float u_material_shininess;" \
    "out vec3 phong_ads_color;" \
    "void main(void)" \
    "{"
    "if(u_lighting_enabled == 1)" \
    "{" \
    "vec4 eye_coordinates= u_view_matrix * u_model_matrix *vPosition;" \
    "vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
    "vec3 light_direction = normalize(vec3(u_light_position) - eye_coordinates.xyz);"\
    "float tn_dot_ld = max(dot(transformed_normals,light_direction),0.0);" \
    "vec3 ambient = u_La * u_Ka;" \
    "vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
    "vec3 reflection_vector = reflect(-light_direction,transformed_normals);"\
    "vec3 viewer_vector = normalize(-eye_coordinates.xyz);" \
    "vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector,viewer_vector),0.0),u_material_shininess);" \
    "phong_ads_color = ambient + diffuse + specular;" \
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
                printf("Vertex Shader Compilation Log:%s\n", szInfoLog);
                free(szInfoLog);
                [self release];
               
            }
        }
    }
    
    //*****FRAGMENT SHADER****
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    
    //provide source code to shader
    const GLchar *fragmentShaderSourceCode =
    "#version 300 es" \
    "\n" \
    "precision highp float;"\
    "in vec3 phong_ads_color;" \
    "out vec4 FragColor;" \
    "void main(void)" \
    "{" \
    "FragColor = vec4(phong_ads_color,1.0);" \
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
                printf("Fragment Shader Compilation Log:%s\n",szInfoLog);
                free(szInfoLog);
                [self release];
             
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
                printf("Shader Program Link Log : %s\n",szInfoLog);
                free(szInfoLog);
                [self release];
            }
        }
    }
    
    printf("After Shader Link\n");
    
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
    
    
    
    
}





-(void) perFragment
{
    vertexShaderObject2 = glCreateShader(GL_VERTEX_SHADER);
    
    //vertex shader
    //provide source code to shader
    const GLchar * vertexShaderShaderCode2 =
    "#version 300 es" \
    "\n" \
    "in vec4 vPosition;" \
    "in vec3 vNormal;" \
    "uniform mat4 u_model_matrix2;" \
    "uniform mat4 u_view_matrix2;" \
    "uniform mat4 u_projection_matrix2;" \
    "uniform vec4 u_light_position2;" \
    //"uniform int u_lighting_enabled2;" \
    
    "out vec3 transformed_normals2;" \
    "out vec3 light_direction2;" \
    "out vec3 viewer_vector2;" \
    "void main(void)" \
    "{" \
    "vec4 eye_coordinates = u_view_matrix2*u_model_matrix2*vPosition;" \
    "transformed_normals2 = mat3(u_view_matrix2 * u_model_matrix2) * vNormal;" \
    "light_direction2 = vec3(u_light_position2) - eye_coordinates.xyz;" \
    "viewer_vector2 = -eye_coordinates.xyz;" \
  
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
                printf("Vertex Shader Compilation Log2:%s\n", szInfoLog);
                free(szInfoLog);
                [self release];
              
            }
        }
    }
    
    //*****FRAGMENT SHADER****
    fragmentShaderObject2 = glCreateShader(GL_FRAGMENT_SHADER);
    
    //provide source code to shader
    const GLchar *fragmentShaderSourceCode2 =
    "#version 300 es" \
    "\n" \
    "precision highp float;"\
    "in vec3 transformed_normals2;" \
    "in vec3 light_direction2;" \
    "in vec3 viewer_vector2;" \
    "out vec4 FragColor2;" \
    "uniform vec3 u_La2;" \
    "uniform vec3 u_Ld2;" \
    "uniform vec3 u_Ls2;" \
    "uniform vec3 u_Ka2;" \
    "uniform vec3 u_Kd2;" \
    "uniform vec3 u_Ks2;" \
    "uniform float u_material_shininess2;" \
    "uniform int u_lighting_enabled2;" \
    
    "void main(void)" \
    "{" \
    "vec3 phong_ads_color;" \
    "if(u_lighting_enabled2==1)"\
    "{" \
    "vec3 normalized_transformed_normals=normalize(transformed_normals2);" \
    "vec3 normalized_light_direction=normalize(light_direction2);" \
    "vec3 normalized_viewer_vector=normalize(viewer_vector2);" \
    "vec3 ambient = u_La2 * u_Ka2;" \
    "float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction),0.0);" \
    "vec3 diffuse = u_Ld2 * u_Kd2 * tn_dot_ld;" \
    "vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normals);" \
    "vec3 specular = u_Ls2 * u_Ks2 * pow(max(dot(reflection_vector,normalized_viewer_vector),0.0),u_material_shininess2);" \
    "phong_ads_color = ambient + diffuse + specular;" \
    "}" \
    "else" \
    "{" \
    
    "phong_ads_color = vec3(1.0f,1.0f,1.0f);" \
    "}" \
    "FragColor2 = vec4(phong_ads_color,1.0);" \
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
                printf("Fragment Shader Compilation Log:%s\n",szInfoLog);
                free(szInfoLog);
                [self release];
               
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
                printf("Shader Program Link Log : %s\n",szInfoLog);
                free(szInfoLog);
                [self release];
               
            }
        }
    }
    
    printf("After Shader Link\n");
    
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
    
    //shininess of material (value is conventionally between 1 to 200)
    material_shininess_uniform2 = glGetUniformLocation(shaderProgramObject, "u_material_shininess2");
    
}



+(Class)layerClass
{
    return([CAEAGLLayer class]);
}

-(void)drawView:(id)sender
{
    
    light_ambient[0] = 0.0f;
    light_ambient[1] = 0.0f;
    light_ambient[2] = 0.0f;
    light_ambient[3] = 1.0f;
    
    lightDiffuse[0] = 1.0f;
    lightDiffuse[1] = 1.0f;
    lightDiffuse[2] = 1.0f;
    lightDiffuse[3] = 1.0f;
    
    lightPosition[0] = 100.0f;
    lightPosition[1] = 100.0f;
    lightPosition[2] = 100.0f;
    lightPosition[3] = 1.0f;
    
    lightSpecular[0] = 1.0f;
    lightSpecular[1] = 1.0f;
    lightSpecular[2] = 1.0f;
    lightSpecular[3] = 1.0f;
    
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
    lightDiffuse2[1] = 1.0f;
    lightDiffuse2[2] = 1.0f;
    lightDiffuse2[3] = 1.0f;
    
    lightPosition2[0] = 100.0f;
    lightPosition2[1] = 100.0f;
    lightPosition2[2] = 100.0f;
    lightPosition2[3] = 1.0f;
    
    lightSpecular2[0] = 1.0f;
    lightSpecular2[1] = 1.0f;
    lightSpecular2[2] = 1.0f;
    lightSpecular2[3] = 1.0f;
    
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
    
    [EAGLContext setCurrentContext:eaglContext];
    
    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebuffer);
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    glUseProgram(shaderProgramObject);
    
    vmath::mat4 modelMatrix = vmath::mat4::identity();
    vmath::mat4 viewMatrix = vmath::mat4::identity();
    
    modelMatrix = vmath::translate(0.0f, 0.0f, -2.5f);
    
    
    glBindVertexArray(gVao_Sphere);
    [self perVertex];
    if(singleTap == 1)
    {
        glUniform1i(L_KeyPressed_uniform, 1);
        
        //setting lights properties
        glUniform3fv(La_uniform, 1, light_ambient);
        glUniform3fv(Ld_uniform, 1, lightDiffuse);
        glUniform3fv(Ls_uniform, 1, lightSpecular);
        glUniform4fv(light_position_uniform, 1, lightPosition);
        
        //setting material's properties
        glUniform3fv(Ka_uniform, 1, material_ambient);
        glUniform3fv(Kd_uniform, 1, material_diffuse);
        glUniform3fv(Ks_uniform, 1, material_specular);
        glUniform1f(material_shininess_uniform, material_shininess);
    }
    if (singleTap == 2)
    {
        modelMatrix = vmath::mat4::identity();
        viewMatrix = vmath::mat4::identity();
        
        
        
        glUniform1i(L_KeyPressed_uniform2, 1);
        
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
        modelMatrix = vmath::translate(0.0f, 0.0f, -1.5f);
        glUniformMatrix4fv(model_matrix_uniform2, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(view_matrix_uniform2, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projection_matrix_uniform2, 1, GL_FALSE, perspectiveProjectionMatrix);
        [self perFragment];
    }
    
    
    
    
    modelMatrix = vmath::translate(0.0f, 0.0f, -1.5f);
    
    glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, perspectiveProjectionMatrix);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
    glDrawElements(GL_TRIANGLES, numElemets, GL_UNSIGNED_SHORT, 0);
    
    glBindVertexArray(0);
    glUseProgram(0);
    
    glBindRenderbuffer(GL_RENDERBUFFER,colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}


-(void)layoutSubviews
{
    GLint width;
    GLint height;
    
    glBindRenderbuffer(GL_RENDERBUFFER,colorRenderbuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH,&width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT,&height);
    
    glGenRenderbuffers(1,&depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,width,height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthRenderbuffer);
    
    glViewport(0,0,width,height);
    
    //glOrtho(left,right,bottom,top,near,far);
    GLfloat fwidth = (GLfloat)width;
    GLfloat fheight = (GLfloat)height;
    
    perspectiveProjectionMatrix = vmath::perspective(45.0f,fwidth/fheight,0.1f,100.0f);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Failed To Create Complete Framebuffer Object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
    
    [self drawView:nil];
}

-(void)startAnimation
{
    if(!isAnimating)
    {
        displayLink = [NSClassFromString(@"CADisplayLink")displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink setPreferredFramesPerSecond:animationFrameInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        isAnimating=YES;
        
    }
}



-(void)stopAnimation
{
    if(isAnimating)
    {
        [displayLink invalidate];
        displayLink=nil;
        
        isAnimating=NO;
    }
}



-(BOOL)acceptsFirstResponder
{
    return(YES);
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

-(void)onSingleTap:(UITapGestureRecognizer *)gr
{
    singleTap = singleTap + 1;
    if(singleTap >= 3)
        singleTap = 0;
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    
}

-(void)onSwipe:(UISwipeGestureRecognizer *)gr
{
    // code
    [self release];
    exit(0);
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gr
{
    
}


-(void)dealloc
{
    
    
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
    
    glDetachShader(shaderProgramObject, vertexShaderObject);
    glDetachShader(shaderProgramObject, fragmentShaderObject);
    
    glDeleteShader(vertexShaderObject);
    vertexShaderObject = 0;
    glDeleteShader(fragmentShaderObject);
    fragmentShaderObject = 0;
    
    glDeleteProgram(shaderProgramObject);
    shaderProgramObject = 0;
    
    if(depthRenderbuffer)
    {
        glDeleteRenderbuffers(1,&depthRenderbuffer);
        depthRenderbuffer=0;
    }
    
    if(colorRenderbuffer)
    {
        glDeleteRenderbuffers(1,&colorRenderbuffer);
        colorRenderbuffer=0;
    }
    
    if(defaultFramebuffer)
    {
        glDeleteFramebuffers(1,&defaultFramebuffer);
        defaultFramebuffer=0;
    }
    
    if([EAGLContext currentContext]==eaglContext)
    {
        [EAGLContext setCurrentContext:nil];
    }
    [eaglContext release];
    eaglContext=nil;
    
    [super dealloc];
}

@end














































































