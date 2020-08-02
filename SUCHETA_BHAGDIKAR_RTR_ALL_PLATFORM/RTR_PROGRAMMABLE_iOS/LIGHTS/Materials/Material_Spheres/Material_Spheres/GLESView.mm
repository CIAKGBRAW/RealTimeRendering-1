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
    
    GLint width;
    GLint height;
    GLfloat fwidth;
    GLfloat fheight;
    GLuint model_matrix_uniform, view_matrix_uniform, projection_matrix_uniform;
    
    GLuint L_KeyPressed_uniform;
    
    GLuint La_uniform;
    GLuint Ld_uniform;
    GLuint Ls_uniform;
    GLuint light_position_uniform;
    
    GLuint Ka_uniform;
    GLuint Kd_uniform;
    GLuint Ks_uniform;
    GLuint material_shininess_uniform;
    
    
    //0.0f,0.0f,1.0f};
    GLfloat light_ambient[4];
    GLfloat lightDiffuse[4];
    GLfloat lightSpecular[4];
    GLfloat lightPosition[4];
    
    GLfloat material_ambient[4];
    GLfloat material_diffuse[4];
    GLfloat material_specular[4];
    GLfloat material_shininess;
    
    GLfloat material_ambient2[4];
    GLfloat material_diffuse2[4];
    GLfloat material_specular2[4];
    GLfloat material_shininess2;
    
    GLfloat material_ambient3[4];
    GLfloat material_diffuse3[4];
    GLfloat material_specular3[4];
    GLfloat material_shininess3;
    
    
    GLfloat material_ambient4[4];
    GLfloat material_diffuse4[4];
    GLfloat material_specular4[4];
    GLfloat material_shininess4;
    
    GLfloat material_ambient5[4];
    GLfloat material_diffuse5[4];
    GLfloat material_specular5[4];
    GLfloat material_shininess5;
    
    GLfloat material_ambient6[4];
    GLfloat material_diffuse6[4];
    GLfloat material_specular6[4];
    GLfloat material_shininess6;
    
    GLfloat material_ambient7[4];
    GLfloat material_diffuse7[4];
    GLfloat material_specular7[4];
    GLfloat material_shininess7;
    
    GLfloat material_ambient8[4];
    GLfloat material_diffuse8[4];
    GLfloat material_specular8[4];
    GLfloat material_shininess8;
    
    GLfloat material_ambient9[4];
    GLfloat material_diffuse9[4];
    GLfloat material_specular9[4];
    GLfloat material_shininess9;
    
    GLfloat material_ambient10[4];
    GLfloat material_diffuse10[4];
    GLfloat material_specular10[4];
    GLfloat material_shininess10;
    
    GLfloat material_ambient11[4];
    GLfloat material_diffuse11[4];
    GLfloat material_specular11[4];
    GLfloat material_shininess11;
    
    GLfloat material_ambient12[4];
    GLfloat material_diffuse12[4];
    GLfloat material_specular12[4];
    GLfloat material_shininess12;
    
    
    GLfloat material_ambient13[4];
    GLfloat material_diffuse13[4];
    GLfloat material_specular13[4];
    GLfloat material_shininess13;
    
    GLfloat material_ambient14[4];
    GLfloat material_diffuse14[4];
    GLfloat material_specular14[4];
    GLfloat material_shininess14;
    
    
    GLfloat material_ambient15[4];
    GLfloat material_diffuse15[4];
    GLfloat material_specular15[4];
    GLfloat material_shininess15;
    
    GLfloat material_ambient16[4];
    GLfloat material_diffuse16[4];
    GLfloat material_specular16[4];
    GLfloat material_shininess16;
    
    GLfloat material_ambient17[4];
    GLfloat material_diffuse17[4];
    GLfloat material_specular17[4];
    GLfloat material_shininess17;
    
    GLfloat material_ambient18[4];
    GLfloat material_diffuse18[4];
    GLfloat material_specular18[4];
    GLfloat material_shininess18;
    
    GLfloat material_ambient19[4];
    GLfloat material_diffuse19[4];
    GLfloat material_specular19[4];
    GLfloat material_shininess19;
    
    GLfloat material_ambient20[4];
    GLfloat material_diffuse20[4];
    GLfloat material_specular20[4];
    GLfloat material_shininess20;
    
    GLfloat material_ambient21[4];
    GLfloat material_diffuse21[4];
    GLfloat material_specular21[4];
    GLfloat material_shininess21;
    
    GLfloat material_ambient22[4];
    GLfloat material_diffuse22[4];
    GLfloat material_specular22[4];
    GLfloat material_shininess22;
    
    GLfloat material_ambient23[4];
    GLfloat material_diffuse23[4];
    GLfloat material_specular23[4];
    GLfloat material_shininess23;
    
    GLfloat material_ambient24[4];
    GLfloat material_diffuse24[4];
    GLfloat material_specular24[4];
    GLfloat material_shininess24;
    
    GLfloat material_ambient25[4];
    GLfloat material_diffuse25[4];
    GLfloat material_specular25[4];
    GLfloat material_shininess25;
    
    
    GLfloat material_ambient26[4];
    GLfloat material_diffuse26[4];
    GLfloat material_specular26[4];
    GLfloat material_shininess26;

    GLfloat angleX;
    GLfloat myArrX[4];
    GLfloat angleRadianX;
    
    GLfloat angleY;
    GLfloat myArrY[4];
    GLfloat angleRadianY;
    
    GLfloat angleZ;
    GLfloat myArrZ[4];
    GLfloat angleRadianZ;
    bool gbLight;
    
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
        
        material_ambient[0] = 0.0215f;
        material_ambient[1] = 0.1745f;
        material_ambient[2] = 0.0215f;
        material_ambient[3] =1.0f;
        
        material_diffuse[0] = 0.07568f;
        material_diffuse[1] = 0.61424f;
        material_diffuse[2] =0.07568f;
        material_diffuse[3] = 1.0f;
        
        material_specular[0] = 0.633f;
        material_specular[1] = 0.633f;
        material_specular[2] =0.633f;
        material_specular[3] = 1.0f;
        
        material_shininess = 0.6 * 128;
        
        /////////////////////////// MATERIAL 2
        material_ambient2[0] =0.135f;
        material_ambient2[1] = 0.2225f;
        material_ambient2[2] = 0.1575f;
        material_ambient2[3] =1.0f;
        
        material_diffuse2[0] = 0.54;
        material_diffuse2[1] = 0.89;
        material_diffuse2[2] =0.63;
        material_diffuse2[3] = 1.0f;
        
        material_specular2[0] =0.316228f;
        material_specular2[1] = 0.316228f;
        material_specular2[2] =0.316228f;
        material_specular2[3] = 1.0f;
        
        material_shininess2 = 0.1 * 128;
        
        
        /////////////////////////// MATERIAL 3
        material_ambient3[0] = 0.05375;
        material_ambient3[1] = 0.05f;
        material_ambient3[2] = 0.06625f;
        material_ambient3[3] =1.0f;
        
        material_diffuse3[0] =0.18275;
        material_diffuse3[1] = 0.17;
        material_diffuse3[2] =0.22525;
        material_diffuse3[3] = 1.0f;
        
        material_specular3[0] =0.332741f;
        material_specular3[1] = 0.328634f;
        material_specular3[2] =0.346435f;
        material_specular3[3] = 1.0f;
        
        material_shininess3 = 0.3 * 128;
        
        ///////////////////////// MATERIAL 4
        material_ambient4[0] =0.25f;
        material_ambient4[1] = 0.20725f;
        material_ambient4[2] = 0.20725f;
        material_ambient4[3] =1.0f;
        
        material_diffuse4[0] = 1.0;
        material_diffuse4[1] = 0.829;
        material_diffuse4[2] =0.829;
        material_diffuse4[3] = 1.0f;
        
        material_specular4[0] =0.296648f;
        material_specular4[1] = 0.296648f;
        material_specular4[2] =0.296648f;
        material_specular4[3] = 1.0f;
        
        material_shininess4 = 0.088 * 128;
        
        //////////////////////// MATERIAL 5
        
        material_ambient5[0] =0.175f;
        material_ambient5[1] = 0.01175f;
        material_ambient5[2] = 0.01175f;
        material_ambient5[3] =1.0f;
        
        material_diffuse5[0] = 0.61424;
        material_diffuse5[1] =0.04136;
        material_diffuse5[2] =0.04136;
        material_diffuse5[3] = 1.0f;
        
        material_specular5[0] =0.727811f;
        material_specular5[1] =0.626959f;
        material_specular5[2] =0.626959f;
        material_specular5[3] = 1.0f;
        
        material_shininess5 =  0.6 * 128;
        
        ////////////////////////// MATERIAL 6
        
        material_ambient6[0] =0.1f;
        material_ambient6[1] = 0.182725f;
        material_ambient6[2] = 0.1745f;
        material_ambient6[3] =1.0f;
        
        material_diffuse6[0] = 0.396;
        material_diffuse6[1] = 0.74151;
        material_diffuse6[2] =0.69102;
        material_diffuse6[3] = 1.0f;
        
        material_specular6[0] =0.297254f;
        material_specular6[1] =0.30829f;
        material_specular6[2] =0.306678f;
        material_specular6[3] = 1.0f;
        
        material_shininess6 = 0.1 * 128;
        
        
        
        ////////////////////////// MATERIAL 7
        
        material_ambient7[0] =0.329412f;
        material_ambient7[1] = 0.223529f;
        material_ambient7[2] = 0.027451f;
        material_ambient7[3] =1.0f;
        
        material_diffuse7[0] =0.780392;
        material_diffuse7[1] = 0.568627;
        material_diffuse7[2] =0.113725;
        material_diffuse7[3] = 1.0f;
        
        material_specular7[0] =0.992175f;
        material_specular7[1] =0.941176f;
        material_specular7[2] =0.807843f;
        material_specular7[3] = 1.0f;
        
        material_shininess7 = 0.21794872 * 128;
        
        ////////////////////////// MATERIAL 8
        
        material_ambient8[0] =0.2125;
        material_ambient8[1] = 0.1275f;
        material_ambient8[2] = 0.054f;
        material_ambient8[3] =1.0f;
        
        material_diffuse8[0] = 0.714;
        material_diffuse8[1] =0.4284;
        material_diffuse8[2] =0.18144;
        material_diffuse8[3] = 1.0f;
        
        material_specular8[0] =0.393548f;
        material_specular8[1] =0.271906f;
        material_specular8[2] =0.166721f;
        material_specular8[3] = 1.0f;
        
        material_shininess8 = 0.2 * 128;
        
        ////////////////////////// MATERIAL 9
        
        material_ambient9[0] =0.25f;
        material_ambient9[1] = 0.25f;
        material_ambient9[2] = 0.25f;
        material_ambient9[3] =1.0f;
        
        material_diffuse9[0] = 0.4;
        material_diffuse9[1] = 0.4;
        material_diffuse9[2] =0.4;
        material_diffuse9[3] = 1.0f;
        
        material_specular9[0] =0.774597f;
        material_specular9[1] =0.774597f;
        material_specular9[2] =0.774597f;
        material_specular9[3] = 1.0f;
        
        material_shininess9 = 0.6 * 128;
        
        
        ////////////////////////// MATERIAL 10
        
        material_ambient10[0] =0.19125f;
        material_ambient10[1] = 0.0735f;
        material_ambient10[2] = 0.0225f;
        material_ambient10[3] =1.0f;
        
        material_diffuse10[0] = 0.7038;
        material_diffuse10[1] = 0.27048;
        material_diffuse10[2] =0.0828;
        material_diffuse10[3] = 1.0f;
        
        material_specular10[0] = 0.256777;
        material_specular10[1] =0.137622f;
        material_specular10[2] =0.086014;
        material_specular10[3] = 1.0f;
        
        material_shininess10 = 0.1 * 128;
        
        ////////////////////////// MATERIAL 11
        
        material_ambient11[0] =0.24725f;
        material_ambient11[1] = 0.199f;
        material_ambient11[2] = 0.0745f;
        material_ambient11[3] =1.0f;
        
        material_diffuse11[0] = 0.75164;
        material_diffuse11[1] = 0.60648;
        material_diffuse11[2] =0.22648;
        material_diffuse11[3] = 1.0f;
        
        material_specular11[0] =0.628281f;
        material_specular11[1] =0.555802f;
        material_specular11[2] =0.366065f;
        material_specular11[3] = 1.0f;
        
        material_shininess11 =  0.4 * 128;
        
        ////////////////////////// MATERIAL 12
        material_ambient12[0] =0.19225f;
        material_ambient12[1] = 0.19225f;
        material_ambient12[2] = 0.19225f;
        material_ambient12[3] =1.0f;
        
        material_diffuse12[0] =  0.50754;
        material_diffuse12[1] =  0.50754;
        material_diffuse12[2] = 0.50754;
        material_diffuse12[3] = 1.0f;
        
        material_specular12[0] = 0.508273f;
        material_specular12[1] = 0.508273f;
        material_specular12[2] = 0.508273f;
        material_specular12[3] = 1.0f;
        
        material_shininess12 = 0.4 * 128;
        
        ////////////////////////// MATERIAL 13
        material_ambient13[0] =0.0f;
        material_ambient13[1] = 0.0f;
        material_ambient13[2] = 0.0f;
        material_ambient13[3] =1.0f;
        
        material_diffuse13[0] =  0.01;
        material_diffuse13[1] =  0.01;
        material_diffuse13[2] = 0.01;
        material_diffuse13[3] = 1.0f;
        
        material_specular13[0] = 0.50f;
        material_specular13[1] = 0.50f;
        material_specular13[2] = 0.50f;
        material_specular13[3] = 1.0f;
        
        material_shininess13 = 0.25 * 128;
        
        
        ////////////////////////// MATERIAL 14
        
        material_ambient14[0] =0.0;
        material_ambient14[1] = 0.1f;
        material_ambient14[2] = 0.06f;
        material_ambient14[3] =1.0f;
        
        material_diffuse14[0] =  0.0;
        material_diffuse14[1] = 0.50980392;
        material_diffuse14[2] = 0.50980392;
        material_diffuse14[3] = 1.0f;
        
        material_specular14[0] = 0.50196078f;
        material_specular14[1] =0.50196078;
        material_specular14[2] = 0.50196078f;
        material_specular14[3] = 1.0f;
        
        material_shininess14 = 0.25 * 128;
        
        ////////////////////////// MATERIAL 15
        
        material_ambient15[0] =0.0f;
        material_ambient15[1] = 0.0f;
        material_ambient15[2] = 0.0f;
        material_ambient15[3] =1.0f;
        
        material_diffuse15[0] =  0.1;
        material_diffuse15[1] =  0.35;
        material_diffuse15[2] = 0.1;
        material_diffuse15[3] = 1.0f;
        
        material_specular15[0] = 0.45;
        material_specular15[1] = 0.55;
        material_specular15[2] = 0.45;
        material_specular15[3] = 1.0f;
        
        material_shininess15 =  0.25 * 128;
        
        ////////////////////////// MATERIAL 16
        
        material_ambient16[0] =0.0;
        material_ambient16[1] = 0.0;
        material_ambient16[2] = 0.0;
        material_ambient16[3] =1.0f;
        
        material_diffuse16[0] =  0.5;
        material_diffuse16[1] =  0.0;
        material_diffuse16[2] = 0.0;
        material_diffuse16[3] = 1.0f;
        
        material_specular16[0] = 0.7;
        material_specular16[1] = 0.6;
        material_specular16[2] = 0.6;
        material_specular16[3] = 1.0f;
        
        material_shininess16 = 0.25 * 128;
        
        ////////////////////////// MATERIAL 17
        
        material_ambient17[0] =0.0;
        material_ambient17[1] = 0.0;
        material_ambient17[2] = 0.0;
        material_ambient17[3] =1.0f;
        
        material_diffuse17[0] =  0.55;
        material_diffuse17[1] =  0.55;
        material_diffuse17[2] = 0.55;
        material_diffuse17[3] = 1.0f;
        
        material_specular17[0] = 0.70;
        material_specular17[1] = 0.70;
        material_specular17[2] = 0.70;
        material_specular17[3] = 1.0f;
        
        material_shininess17 =0.25 * 128;
        
        
        ////////////////////////// MATERIAL 18
        
        material_ambient18[0] =0.0;
        material_ambient18[1] = 0.0;
        material_ambient18[2] = 0.0;
        material_ambient18[3] =1.0f;
        
        material_diffuse18[0] =  0.5;
        material_diffuse18[1] =  0.5;
        material_diffuse18[2] = 0.0;
        material_diffuse18[3] = 1.0f;
        
        material_specular18[0] = 0.60;
        material_specular18[1] = 0.60;
        material_specular18[2] = 0.50;
        material_specular18[3] = 1.0f;
        
        material_shininess18 = 0.25 * 128;
        
        ////////////////////////// MATERIAL 19
        
        material_ambient19[0] =0.02;
        material_ambient19[1] = 0.02;
        material_ambient19[2] = 0.02;
        material_ambient19[3] =1.0f;
        
        material_diffuse19[0] =  0.01;
        material_diffuse19[1] =  0.01;
        material_diffuse19[2] = 0.01;
        material_diffuse19[3] = 1.0f;
        
        material_specular19[0] = 0.4;
        material_specular19[1] = 0.4;
        material_specular19[2] = 0.4;
        material_specular19[3] = 1.0f;
        
        material_shininess19 =0.078125 * 128;
        
        
        ////////////////////////// MATERIAL 20
        
        
        material_ambient20[0] =0.0;
        material_ambient20[1] = 0.05;
        material_ambient20[2] = 0.05;
        material_ambient20[3] =1.0f;
        
        material_diffuse20[0] =  0.4;
        material_diffuse20[1] =  0.5;
        material_diffuse20[2] = 0.5;
        material_diffuse20[3] = 1.0f;
        
        material_specular20[0] = 0.04;
        material_specular20[1] = 0.7;
        material_specular20[2] = 0.7;
        material_specular20[3] = 1.0f;
        
        material_shininess20 =0.078125 * 128;
        ////////////////////////// MATERIAL 21
        
        
        material_ambient21[0] =0.0;
        material_ambient21[1] = 0.05;
        material_ambient21[2] = 0.0;
        material_ambient21[3] =1.0f;
        
        material_diffuse21[0] =  0.4;
        material_diffuse21[1] =  0.5;
        material_diffuse21[2] = 0.4;
        material_diffuse21[3] = 1.0f;
        
        material_specular21[0] = 0.04;
        material_specular21[1] = 0.7;
        material_specular21[2] = 0.04;
        material_specular21[3] = 1.0f;
        
        material_shininess21 = 0.078125 * 128;
        
        ////////////////////////// MATERIAL 22
        
        
        material_ambient22[0] =0.05;
        material_ambient22[1] = 0.0;
        material_ambient22[2] = 0.0;
        material_ambient22[3] =1.0f;
        
        material_diffuse22[0] =  0.5;
        material_diffuse22[1] =  0.4;
        material_diffuse22[2] = 0.4;
        material_diffuse22[3] = 1.0f;
        
        material_specular22[0] = 0.7;
        material_specular22[1] = 0.04;
        material_specular22[2] = 0.04;
        material_specular22[3] = 1.0f;
        
        material_shininess22 = 0.078125 * 128;
        
        ////////////////////////// MATERIAL 23
        
        material_ambient23[0] =0.05;
        material_ambient23[1] = 0.05;
        material_ambient23[2] = 0.05;
        material_ambient23[3] =1.0f;
        
        material_diffuse23[0] =  0.5;
        material_diffuse23[1] =  0.5;
        material_diffuse23[2] = 0.5;
        material_diffuse23[3] = 1.0f;
        
        material_specular23[0] = 0.7;
        material_specular23[1] = 0.7;
        material_specular23[2] = 0.7;
        material_specular23[3] = 1.0f;
        
        material_shininess23 =  0.078125 * 128;
        
        ////////////////////////// MATERIAL 24
        
        material_ambient24[0] =0.05;
        material_ambient24[1] = 0.05;
        material_ambient24[2] = 0.0;
        material_ambient24[3] =1.0f;
        
        material_diffuse24[0] =  0.5;
        material_diffuse24[1] =  0.5;
        material_diffuse24[2] = 0.4;
        material_diffuse24[3] = 1.0f;
        
        material_specular24[0] = 0.7;
        material_specular24[1] = 0.7;
        material_specular24[2] = 0.04;
        material_specular24[3] = 1.0f;
        
        material_shininess24 = 0.078125 * 128;
        
        
        
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

-(void) drawSphere
{
    glBindVertexArray(gVao_Sphere);
    vmath::mat4 modelMatrix = vmath::mat4::identity();
    vmath::mat4 viewMatrix = vmath::mat4::identity();
    
    modelMatrix = vmath::translate(0.0f, 0.0f, -1.5f);
    glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(view_matrix_uniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projection_matrix_uniform, 1, GL_FALSE, perspectiveProjectionMatrix);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
    glDrawElements(GL_TRIANGLES, numElemets, GL_UNSIGNED_SHORT, 0);
    
    glBindVertexArray(0);
}


+(Class)layerClass
{
    return([CAEAGLLayer class]);
}

-(void)drawView:(id)sender
{
    
    
    
    [EAGLContext setCurrentContext:eaglContext];
    
    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebuffer);
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    glUseProgram(shaderProgramObject);
    
  
    
    
    glBindVertexArray(gVao_Sphere);
    
    myArrX[1] =     100 * (cos ( 2*3.1415926535 * angleRadianX ));
    myArrX[2] =   100 * (sin ( 2*3.1415926535 * angleRadianX ));
    
    myArrY[0] = 100 * (cos ( 2*3.1415926535 * angleRadianY ));
    myArrY[2] = 100 * (sin (2 * 3.1415 * angleRadianY));
    
    myArrZ[0] =     100 * (cos ( 2*3.1415926535 * angleRadianZ ));
    myArrZ[1] =     100 * (sin ( 2*3.1415926535 * angleRadianZ ));
    
    if(singleTap == 1)
    {
        
        glUniform1i(L_KeyPressed_uniform, 1);
        
        //setting lights properties
        glUniform3fv(La_uniform, 1, light_ambient);
        glUniform3fv(Ld_uniform, 1, lightDiffuse);
        glUniform3fv(Ls_uniform, 1, lightSpecular);
        glUniform4fv(light_position_uniform, 1, lightPosition);
    
    }
 else   if(singleTap == 2)
    {
        
        glUniform1i(L_KeyPressed_uniform, 1);
        
        //setting lights properties
        glUniform3fv(La_uniform, 1, light_ambient);
        glUniform3fv(Ld_uniform, 1, lightDiffuse);
        glUniform3fv(Ls_uniform, 1, lightSpecular);
        glUniform4fv(light_position_uniform, 1, myArrX);
        
    }
 else   if(singleTap == 3)
    {
        
        glUniform1i(L_KeyPressed_uniform, 1);
        
        //setting lights properties
        glUniform3fv(La_uniform, 1, light_ambient);
        glUniform3fv(Ld_uniform, 1, lightDiffuse);
        glUniform3fv(Ls_uniform, 1, lightSpecular);
        glUniform4fv(light_position_uniform, 1, myArrY);
        
    }
 else  if(singleTap == 4)
    {
        
        glUniform1i(L_KeyPressed_uniform, 1);
        
        //setting lights properties
        glUniform3fv(La_uniform, 1, light_ambient);
        glUniform3fv(Ld_uniform, 1, lightDiffuse);
        glUniform3fv(Ls_uniform, 1, lightSpecular);
        glUniform4fv(light_position_uniform, 1, myArrZ);
        
    }
    
    
    //Sphere 1
    
    glViewport(0,0,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient);
    glUniform3fv(Kd_uniform, 1, material_diffuse);
    glUniform3fv(Ks_uniform, 1, material_specular);
    glUniform1f(material_shininess_uniform, material_shininess);
     [self drawSphere];
   
      //Sphere 2
    glViewport(width / 5.8,0,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient2);
    glUniform3fv(Kd_uniform, 1, material_diffuse2);
    glUniform3fv(Ks_uniform, 1, material_specular2);
    glUniform1f(material_shininess_uniform, material_shininess2);
        [self drawSphere];
    
    //Sphere 3
    glViewport(width / 3,0,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient3);
    glUniform3fv(Kd_uniform, 1, material_diffuse3);
    glUniform3fv(Ks_uniform, 1, material_specular3);
    glUniform1f(material_shininess_uniform, material_shininess3);
    [self drawSphere];
    
    //Sphere 4
    glViewport(width / 2,0,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient4);
    glUniform3fv(Kd_uniform, 1, material_diffuse4);
    glUniform3fv(Ks_uniform, 1, material_specular4);
    glUniform1f(material_shininess_uniform, material_shininess4);
     [self drawSphere];
    
    //Sphere 5
    glViewport(width / 1.5,0,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient5);
    glUniform3fv(Kd_uniform, 1, material_diffuse5);
    glUniform3fv(Ks_uniform, 1, material_specular5);
    glUniform1f(material_shininess_uniform, material_shininess5);
    [self drawSphere];
    
    //Sphere 6
    glViewport(width / 1.2,0,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient6);
    glUniform3fv(Kd_uniform, 1, material_diffuse6);
    glUniform3fv(Ks_uniform, 1, material_specular6);
    glUniform1f(material_shininess_uniform, material_shininess6);
    [self drawSphere];
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
   //Sphere 7
    glViewport(0,(GLsizei)width/6.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient7);
    glUniform3fv(Kd_uniform, 1, material_diffuse7);
    glUniform3fv(Ks_uniform, 1, material_specular7);
    glUniform1f(material_shininess_uniform, material_shininess7);
     [self drawSphere];
    
    
    //Sphere 8
    glViewport(width/5.8,(GLsizei)width/6.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient8);
    glUniform3fv(Kd_uniform, 1, material_diffuse8);
    glUniform3fv(Ks_uniform, 1, material_specular8);
    glUniform1f(material_shininess_uniform, material_shininess8);
      [self drawSphere];
    
    //Sphere 9
    glViewport(width/3,(GLsizei)width/6.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient9);
    glUniform3fv(Kd_uniform, 1, material_diffuse9);
    glUniform3fv(Ks_uniform, 1, material_specular9);
    glUniform1f(material_shininess_uniform, material_shininess9);
     [self drawSphere];
    
    //Sphere 10
    glViewport(width/2,(GLsizei)width/6.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient10);
    glUniform3fv(Kd_uniform, 1, material_diffuse10);
    glUniform3fv(Ks_uniform, 1, material_specular10);
    glUniform1f(material_shininess_uniform, material_shininess10);
   [self drawSphere];
    
    //Sphere 11
    glViewport(width/1.5,(GLsizei)width/6.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient11);
    glUniform3fv(Kd_uniform, 1, material_diffuse11);
    glUniform3fv(Ks_uniform, 1, material_specular11);
    glUniform1f(material_shininess_uniform, material_shininess11);
       [self drawSphere];
    
    //Sphere 12
    glViewport(width/1.2,(GLsizei)width/6.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient12);
    glUniform3fv(Kd_uniform, 1, material_diffuse12);
    glUniform3fv(Ks_uniform, 1, material_specular12);
    glUniform1f(material_shininess_uniform, material_shininess12);
      [self drawSphere];
    //////////////////////////////////////////////////////////////////////////////////
    //Sphere 13
    glViewport(0,(GLsizei)width/3.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient13);
    glUniform3fv(Kd_uniform, 1, material_diffuse13);
    glUniform3fv(Ks_uniform, 1, material_specular13);
    glUniform1f(material_shininess_uniform, material_shininess13);
     [self drawSphere];
    
    //Sphere 14
    glViewport(width/5.8,(GLsizei)width/3.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient14);
    glUniform3fv(Kd_uniform, 1, material_diffuse14);
    glUniform3fv(Ks_uniform, 1, material_specular14);
    glUniform1f(material_shininess_uniform, material_shininess14);
     [self drawSphere];
    
    //Sphere 15
    glViewport(width/3,(GLsizei)width/3.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient15);
    glUniform3fv(Kd_uniform, 1, material_diffuse15);
    glUniform3fv(Ks_uniform, 1, material_specular15);
    glUniform1f(material_shininess_uniform, material_shininess15);
    [self drawSphere];
    
    //Sphere 16
    glViewport(width/2,(GLsizei)width/3.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient16);
    glUniform3fv(Kd_uniform, 1, material_diffuse16);
    glUniform3fv(Ks_uniform, 1, material_specular16);
    glUniform1f(material_shininess_uniform, material_shininess16);
      [self drawSphere];
    
    
    //Sphere 17
    glViewport(width/1.5,(GLsizei)width/3.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient17);
    glUniform3fv(Kd_uniform, 1, material_diffuse17);
    glUniform3fv(Ks_uniform, 1, material_specular17);
    glUniform1f(material_shininess_uniform, material_shininess17);
      [self drawSphere];
    
    //Sphere 18
    glViewport(width/1.2,(GLsizei)width/3.3,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient18);
    glUniform3fv(Kd_uniform, 1, material_diffuse18);
    glUniform3fv(Ks_uniform, 1, material_specular18);
    glUniform1f(material_shininess_uniform, material_shininess18);
      [self drawSphere];
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    //Sphere 19
    glViewport(0,(GLsizei)width/2.2,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient19);
    glUniform3fv(Kd_uniform, 1, material_diffuse19);
    glUniform3fv(Ks_uniform, 1, material_specular19);
    glUniform1f(material_shininess_uniform, material_shininess19);
    [self drawSphere];
    
    //Sphere 20
    glViewport(width/5.8,(GLsizei)width/2.2,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient20);
    glUniform3fv(Kd_uniform, 1, material_diffuse20);
    glUniform3fv(Ks_uniform, 1, material_specular20);
    glUniform1f(material_shininess_uniform, material_shininess20);
    [self drawSphere];
    
    //Sphere 21
    glViewport(width/3,(GLsizei)width/2.2,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient21);
    glUniform3fv(Kd_uniform, 1, material_diffuse21);
    glUniform3fv(Ks_uniform, 1, material_specular21);
    glUniform1f(material_shininess_uniform, material_shininess21);
      [self drawSphere];
    
    //Sphere 22
    glViewport(width/2,(GLsizei)width/2.2,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient22);
    glUniform3fv(Kd_uniform, 1, material_diffuse22);
    glUniform3fv(Ks_uniform, 1, material_specular22);
    glUniform1f(material_shininess_uniform, material_shininess22);
    [self drawSphere];
    
    //Sphere 23
    glViewport(width/1.5,(GLsizei)width/2.2,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient23);
    glUniform3fv(Kd_uniform, 1, material_diffuse23);
    glUniform3fv(Ks_uniform, 1, material_specular23);
    glUniform1f(material_shininess_uniform, material_shininess23);
    [self drawSphere];
    
    //Sphere 24
    glViewport(width/1.2,(GLsizei)width/2.2,(GLsizei)width/6,(GLsizei)height/6);
    glUniform3fv(Ka_uniform, 1, material_ambient24);
    glUniform3fv(Kd_uniform, 1, material_diffuse24);
    glUniform3fv(Ks_uniform, 1, material_specular24);
    glUniform1f(material_shininess_uniform, material_shininess24);
    [self drawSphere];
    glUseProgram(0);
    
    glBindRenderbuffer(GL_RENDERBUFFER,colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
    
    angleX = angleX - 0.3f;
    if (angleX > 360.0f)
        angleX = 0.0f;
    angleRadianX = angleX * (3.1415926535 / 180.0);
    
    
    angleY = angleY - 0.3f;
    if (angleY > 360.0f)
        angleY = 0.0f;
    angleRadianY = angleY * (3.1415926535 / 180.0);
    
    angleZ = angleZ - 0.3f;
    if (angleZ > 360.0f)
        angleZ = 0.0f;
    angleRadianZ = angleZ * (3.1415926535 / 180.0);
}


-(void)layoutSubviews
{
 //   GLint width;
  //  GLint height;
    
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
     fwidth = (GLfloat)width;
     fheight = (GLfloat)height;
    
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
    if(singleTap >= 5)
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














































































