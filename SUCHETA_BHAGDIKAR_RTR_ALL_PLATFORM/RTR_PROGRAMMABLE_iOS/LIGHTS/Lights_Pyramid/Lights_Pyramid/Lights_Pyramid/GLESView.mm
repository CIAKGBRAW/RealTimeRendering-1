#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

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
    
    GLuint vao_Pyramid;
    GLuint vbo_Position_Pyramid;
    GLuint vbo_Normal_Pyramid;
    
    GLuint vao_Cube;
    GLuint vbo_Position_Cube;
    GLuint vbo_Color_Cube;
    
    GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;
    
    
    GLuint mvpUniform;
    id displayLink;
    NSInteger animationFrameInterval;
    BOOL isAnimating;
    
    GLuint La_uniformBlue;
    GLuint Ld_uniformBlue;
    GLuint Ls_uniformBlue;
    GLuint light_position_uniformBlue;
    GLuint material_shininess_uniform;

    GLuint Ka_uniformBlue;
    GLuint Kd_uniformBlue;
    GLuint Ks_uniformBlue;
    
    GLuint La_uniformRed;
    GLuint Ld_uniformRed;
    GLuint Ls_uniformRed;
    GLuint light_position_uniformRed;
    GLuint material_shininess_uniformRed;

    GLuint Ka_uniformRed;
    GLuint Kd_uniformRed;
    GLuint Ks_uniformRed;
    
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
    
    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;
    
    int singleTap;
     GLuint L_KeyPressed_uniform;
    
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
        
        //vertex shader
        vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
        const GLchar *vertexShaderSourceCode =
        "#version 300 es" \
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
        "{" \
        "vec4 eye_coordinates= u_view_matrix * u_model_matrix *vPosition;" \
        "vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
        "vec3 viewer_vector = normalize(-eye_coordinates.xyz);" \
      
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
                    printf("Vertex Shader Co	mpilation Log:%s\n",szInfoLog);
                    free(szInfoLog);
                    [self release];
                    
                }
            }
        }
        
        
        
        //FRAGMENT SHADER
        iInfoLogLength    =0;
        iShaderCompiledStatus = 0;
        szInfoLog = NULL;
        
        fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        
        const GLchar *fragmentShaderSourceCode =
        "#version 300 es" \
        "\n" \
        "precision highp float;"\
        "in vec3 phong_ads_colorBlue;" \
        "in vec3 phong_ads_colorRed;" \
        "out vec4 light;" \
        " vec4 FragColorRed;" \
        " vec4 FragColorBlue;" \
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
        glBindAttribLocation(shaderProgramObject,VDG_ATTRIBUTE_COLOR,"vNormal");
        
        
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
                    printf("Shader Program Link Log : %s\n",szInfoLog);
                    free(szInfoLog);
                    [self release];
                    
                }
            }
        }
        
        
        mvpUniform = glGetUniformLocation(shaderProgramObject,"u_mvp_matrix");
        model_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_model_matrix");
        view_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_view_matrix");
        projection_matrix_uniform = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
        
        La_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_LaBlue");
        //diffuse color intensity of LIGHT
        Ld_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_LdBlue");
        //specular color intensity of LIGHT
        Ls_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_LsBlue");
        //position of light
        light_position_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_light_positionBlue");
        
        //ambient reflective color intensity of MATERIAL
        Ks_uniformBlue = glGetUniformLocation(shaderProgramObject, "u_KaBlue");
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
        
        L_KeyPressed_uniform = glGetUniformLocation(shaderProgramObject, "u_lighting_enabledRed");
        
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
        
        
        const GLfloat normalVerticesPyramid[]=
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(normalVerticesPyramid), normalVerticesPyramid, GL_STATIC_DRAW);
        glVertexAttribPointer(VDG_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(VDG_ATTRIBUTE_COLOR);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindVertexArray(0);
      
        glBindVertexArray(0);
        
        
        //    glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
      //  glEnable(GL_CULL_FACE);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        
        perspectiveProjectionMatrix = vmath::mat4::identity();
        
        
        
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


+(Class)layerClass
{
    return([CAEAGLLayer class]);
}

-(void)drawView:(id)sender
{
    static float gAngleTri = 0.0f;
 
    [EAGLContext setCurrentContext:eaglContext];
    
    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebuffer);
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    if(singleTap == 1)
    {
        
      //  glUniform1i(L_KeyPressed_uniform, 1);
        
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
    
       glUseProgram(shaderProgramObject);
    
    vmath::mat4 modelMatrix = vmath::mat4::identity();
    vmath::mat4 viewMatrix = vmath::mat4::identity();
    vmath::mat4 projectionMatrix = vmath::mat4::identity();
    vmath::mat4 rotationmatrix = vmath::mat4::identity();
    
    modelMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
    rotationmatrix = vmath::rotate(gAngleTri, 0.0f, 1.0f, 0.0f);
    
    
    
    modelMatrix = modelMatrix * rotationmatrix;
    
    //multipley the modelview and orthographic matrix to get modelviewprojection matrix
    projectionMatrix = perspectiveProjectionMatrix * modelMatrix;    //ORDER IS IMPORTANT
    //pass above modelviewprojection matrix to the vertex shader in 'u_mvp_matrix' shader variable
    //whose position value we already calculated in initWithFrame() by using glGetUniformLocation()
    
    glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, perspectiveProjectionMatrix);
    
    
    glBindVertexArray(vao_Pyramid);
    
    glDrawArrays(GL_TRIANGLES, 0, 12);
    
    glBindVertexArray(0);
    glBindRenderbuffer(GL_RENDERBUFFER,colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
     gAngleTri = gAngleTri + 1.0f;
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
    if(singleTap >= 2)
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
    
    if(vao_Pyramid)
    {
        glDeleteVertexArrays(1,&vao_Pyramid);
        vao_Pyramid=0;
    }
    
    if(vao_Cube)
    {
        glDeleteVertexArrays(1,&vao_Cube);
        vao_Cube = 0;
    }
    
    if(vbo_Color_Cube)
    {
        
        glDeleteBuffers(1,&vbo_Color_Cube);
        vbo_Color_Cube = 0;
    }
    
    if(vbo_Position_Cube)
    {
        
        glDeleteBuffers(1,&vbo_Position_Cube);
        vbo_Position_Cube = 0;
    }
    
    
    if(vbo_Position_Pyramid)
    {
        
        glDeleteBuffers(1,&vbo_Position_Pyramid);
        vbo_Position_Pyramid = 0;
    }
    
    if(vbo_Normal_Pyramid)
    {
        
        glDeleteBuffers(1,&vbo_Normal_Pyramid);
        vbo_Normal_Pyramid = 0;
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














































































