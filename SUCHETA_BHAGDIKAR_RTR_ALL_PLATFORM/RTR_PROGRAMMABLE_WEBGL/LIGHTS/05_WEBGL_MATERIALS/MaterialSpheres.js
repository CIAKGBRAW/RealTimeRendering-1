//Diffuse Light
var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;


const WebGLMacros =
    {
        VDG_ATTRIBUTE_VERTEX: 0,
        VDG_ATTRIBUTE_COLOR: 1,
        VDG_ATTRIBUTE_NORMAL: 2,
        VDG_ATTRIBUTE_TEXTURE0: 3,
    };


var vertexShaderObject;
var fragmentShaderObject;
var shaderProgramObject;

var light_ambient = [0.0, 0.0, 0.0];
var light_diffuse = [1.0, 1.0, 1.0];
var light_specular = [1.0, 1.0, 1.0];
var light_position = [100.0, 100.0, 100.0, 1.0];

//var material_ambient = [0.0, 0.0, 0.0];
//var material_diffuse = [1.0, 1.0, 1.0];
//var material_specular = [1.0, 1.0, 1.0];
//var material_shininess = 50.0;

var material_ambient = [0.0215,0.1745,0.0215];
var  material_diffuse = [0.07568,0.61424,0.07568];
var  material_specular = [0.633,0.633,0.633];
var  material_shininess = 0.6 * 128;


var material_ambient1 = [0.135,0.2225,0.1575];
var material_diffuse1 = [0.54,0.89,0.63];
var material_specular1 = [0.316228,0.316228,0.316228];
var material_shininess1 = 0.1 * 128;

var material_ambient2 = [ 0.05375,0.05,0.06625];
var material_diffuse2 = [0.18275,0.17,0.22525];
var material_specular2 = [0.332741,0.328634,0.346435];
var material_shininess2 =  0.3 * 128;

var material_ambient3 = [  0.25,0.20725,0.20725 ];
var material_diffuse3 = [ 1.0,0.829,0.829];
var material_specular3 = [ 0.296648,0.296648,0.296648];
var material_shininess3 = 0.088 * 128;

var material_ambient4= [ 0.175,0.01175,0.01175];
var material_diffuse4 = [  0.61424,0.04136,0.04136];
var material_specular4 = [ 0.727811,0.626959,0.626959];
var material_shininess4 = 0.6 * 128;

var material_ambient5 = [ 0.1,0.182725,0.1745];
var material_diffuse5 = [ 0.396,0.74151,0.69102];
var material_specular5 = [0.297254,0.30829,0.306678];
var material_shininess5 = 0.1 * 128;

var material_ambient1_2= [  0.329412,0.223529,0.027451];
var material_diffuse1_2 = [  0.780392,0.568627,0.113725];
var material_specular1_2 = [0.992175,0.941176,0.807843];
var material_shininess1_2 = 0.21794872 * 128;

var material_ambient2_2 = [ 0.2125,0.1275,0.054 ];
var material_diffuse2_2 = [   0.714,0.4284,0.18144 ];
var material_specular2_2 = [0.393548,0.271906,0.166721];
var material_shininess2_2 =  0.2 * 128;


var material_ambient3_2 = [  0.25,0.25,0.25  ];
var material_diffuse3_2 = [    0.4,0.4,0.4];
var material_specular3_2 = [0.774597,0.774597,0.774597];
var material_shininess3_2 = 0.6 * 128;

var material_ambient4_2 = [  0.19125,0.0735,0.0225];
var material_diffuse4_2 = [   0.7038,0.27048,0.0828];
var material_specular4_2 = [0.256777,0.137622,0.086014];
var material_shininess4_2 = 0.1 * 128;

var material_ambient5_2 = [    0.24725,0.1995,0.0745];
var material_diffuse5_2 = [   0.75164,0.60648,0.22648];
var material_specular5_2 = [0.628281,0.555802,0.366065];
var material_shininess5_2 = 0.4 * 128;

var material_ambient6_2 = [ 0.19225,0.19225,0.19225];
var material_diffuse6_2 = [  0.50754,0.50754,0.50754];
var material_specular6_2 = [ 0.508273,0.508273,0.508273];
var material_shininess6_2 = 0.4 * 128;

var material_ambient1_3 = [0.0,0.0,0.0];
var material_diffuse1_3 = [ 0.01,0.01,0.01];
var material_specular1_3 = [ 0.50,0.50,0.50];
var material_shininess1_3 = 0.25 * 128;

var material_ambient2_3 = [ 0.0,0.1,0.06 ];
var material_diffuse2_3 = [0.0,0.50980392,0.50980392];
var material_specular2_3 = [0.50196078,0.50196078,0.50196078];
var material_shininess2_3 =0.25 * 128;

var material_ambient3_3 = [ 0.0,0.0,0.0];
var material_diffuse3_3 = [0.1,0.35,0.1];
var material_specular3_3 = [0.45,0.55,0.45];
var material_shininess3_3 = 0.25 * 128;

var material_ambient4_3 = [ 0.0,0.0,0.0];
var material_diffuse4_3 = [0.5,0.0,0.0];
var material_specular4_3 = [0.7,0.6,0.6];
var material_shininess4_3 = 0.25 * 128;

var material_ambient5_3 = [0.0,0.0,0.0];
var material_diffuse5_3 = [0.55,0.55,0.55];
var material_specular5_3 = [ 0.70,0.70,0.70];
var material_shininess5_3 =0.25 * 128;

var material_ambient6_3 = [  0.0,0.0,0.0];
var material_diffuse6_3 = [0.5,0.5,0.0];
var material_specular6_3 = [ 0.60,0.60,0.50];
var material_shininess6_3 =0.25 * 128;

var material_ambient1_4 = [ 0.02,0.02,0.02];
var material_diffuse1_4 = [0.01,0.01,0.01];
var material_specular1_4 = [ 0.4,0.4,0.4];
var material_shininess1_4 =0.078125 * 128;

var material_ambient2_4 = [ 0.0,0.05,0.05];
var material_diffuse2_4 = [0.4,0.5,0.5];
var material_specular2_4 = [ 0.04,0.7,0.7];
var  material_shininess2_4 = 0.078125 * 128;

var material_ambient3_4 = [ 0.0,0.05,0.0];
var material_diffuse3_4 = [0.4,0.5,0.4];
var material_specular3_4 = [ 0.04,0.7,0.04];
var material_shininess3_4 = 0.078125 * 128;

var material_ambient4_4 = [ 0.05,0.0,0.0];
var  material_diffuse4_4 = [ 0.5,0.4,0.4];
var material_specular4_4 = [0.7,0.04,0.04];
var material_shininess4_4 = 0.078125 * 128;

var material_ambient5_4 = [  0.05,0.05,0.05];
var material_diffuse5_4 = [ 0.5,0.5,0.5];
var material_specular5_4 = [0.7,0.7,0.7];
var material_shininess5_4 = 0.078125 * 128;

var material_ambient6_4 = [  0.05,0.05,0.0];
var material_diffuse6_4 = [ 0.5,0.5,0.4];
var material_specular6_4 = [0.7,0.7,0.04];
var material_shininess6_4 = 0.078125 * 128;R



var myArrX = [0.0, 0.0, 0.0, 0.0];
var myArrY = [0.0, 0.0, 0.0, 0.0];
var myArrZ = [0.0, 0.0, 0.0, 0.0];

var angleX = 0.0;
var angleY=0.0;
var angleZ =0.0;

var sphere = null;

var perspectiveProjectionMatrix;

var modelMatrixUniform, viewMatrixUniform,projectionMatrixUniform;
var laUniform, ldUniform, lsUniform, lightPositionUniform;
var kaUniform, kdUniform, ksUniform, materialShininessUniform;
var LKeyPressedUniform;

var angle_cube = 0.0;

var bLKeyPressed = false;
var bXKeyPressed = false;
var bYKeyPressed = false;
var bZKeyPressed = false;

var requestAnimationFrame =
      window.requestAnimationFrame ||
      window.webkitRequestAnimationFrame ||
      window.mozRequestAnimationFrame ||
      window.oRequestAnimationFrame ||
      window.msRequestAnimationFrame;


var cancelAnimationFrame =
    window.cancelAnimationFrame ||
    window.webkitCancelRequestAnimationFrame ||
    window.webkitCancelAnimationFrame ||
    window.mozCancelRequestAnimationFrame ||
    window.mozCancelAnimationFrame ||
    window.oCancelRequestAnimationFrame ||
    window.oCancelAnimationFrame ||
    window.msCancelRequestAnimationFrame ||
    window.msCancelAnimationFrame;

function main() {
    canvas = document.getElementById("AMC");
    if (!canvas)
        console.log("Obtaining Canvas Failed\n");
    else
        console.log("Obtaining Canvas Succeeded\n");


    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;

    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);
    window.addEventListener("resize", resize, false);

    init();

    resize();
    draw();
}



function toggleFullScreen() {
    var fullscreen_element =
        document.fullscreenElement ||
        document.webkitFullscreenElement ||
        document.mozFullScreenElement
    document.msFullscreenElement ||
    null;



    if (fullscreen_element == null) {
        if (canvas.requestFullscreen)
            canvas.requestFullscreen();
        else if (canvas.mozRequestFullScreen)
            canvas.mozRequestFullScreen();
        else if (canvas.webkitRequestFullscreen)
            canvas.webkitRequestFullscreen();
        else if (canvas.oRequestFullscreen)
            canvas.oRequestFullscreen();
        else if (canvas.msRequestFullscreen)
            canvas.msRequestFullscreen();
        bFullscreen = true;

    }
    else {
        if (document.exitFullscreen)
            document.exitFullscreen();
        else if (document.mozCancelFullScreen)
            document.mozCancelFullScreen();
        else if (document.webkitExitFullScreen)
            document.webkitExitFullScreen();
        else if (document.msExitFullscreen)
            document.msExitFullscreen();
        bFullscreen = false;
    }
}







function init()
{
    gl = canvas.getContext("webgl2");
    if(gl==null)
    {
        console.log("Failed to get the rendering context for WEBGL\n");
        return;
    }

    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;

    //vertex shader
    var vertexShaderSourceCode =
        "#version 300 es" +
        "\n" +
        "in vec4 vPosition;" +
        "in vec3 vNormal;" +
        "uniform mat4 u_model_matrix;" +
        "uniform mat4 u_view_matrix;" +
        "uniform mat4 u_projection_matrix;" +
        "uniform mediump int u_LKeyPressed;" +
        "uniform vec4 u_light_position;" +
        "out vec3 transformed_normals;" +
        "out vec3 light_direction;" +
        "out vec3 viewer_vector;" +
        "void main(void)" +
        "{" +
        "if(u_LKeyPressed == 1)" +
        "{" +
        "vec4 eyeCoordinates = u_view_matrix*u_model_matrix* vPosition;" +
        " transformed_normals = normalize(mat3(u_view_matrix*u_model_matrix)*vNormal);" +
        "light_direction = vec3(u_light_position) - eyeCoordinates.xyz;" +
        " viewer_vector = -eyeCoordinates.xyz;" +
        "}" +
         "gl_Position=u_projection_matrix * u_view_matrix*u_model_matrix * vPosition;" +
        "}";




    vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);
    gl.compileShader(vertexShaderObject);
    if(gl.getShaderParameter(vertexShaderObject,gl.COMPILE_STATUS)== false)
    {
        var error = gl.getShaderInfoLog(vertexShaderObject);
        if(error.length>0)
        {
            alert(error);
            uninitialize();
        }
    }


    //fragment shader
    var fragmentShaderSourceCode =
        "#version 300 es" +
        "\n" +
        "precision highp float;" +
        "in vec3 transformed_normals;" +
        "in vec3 light_direction;" +
        "in vec3 viewer_vector;" +
         "uniform vec3 u_La;" +
        "uniform vec3 u_Ld;" +
        "uniform vec3 u_Ls;" +
        "uniform vec3 u_Ka;" +
        "uniform vec3 u_Kd;" +
        "uniform vec3 u_Ks;" +
        "uniform float u_material_shininess;" +
        "uniform int u_LKeyPressed;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "vec3 phong_ads_color;" +
         "if(u_LKeyPressed == 1)" +
        "{" +
         "vec3 normalized_transformed_normals=normalize(transformed_normals);" +
         "vec3 normalized_light_direction = normalize(light_direction);" +
         "vec3 normalized_viewer_vector = normalize(viewer_vector);" +
         "vec3 ambient = u_Ls * u_Ka;" +
         "float tn_ld_dot = max(dot(normalized_transformed_normals,normalized_light_direction),0.0);" +
         "vec3 diffuse = u_Ld * u_Kd * tn_ld_dot;" +
         "vec3 reflection_vector = reflect(-normalized_light_direction,normalized_transformed_normals);" +
         "vec3 specular = u_Ls * u_Ks*pow(max(dot(reflection_vector,normalized_viewer_vector),0.0),u_material_shininess);" +
         "phong_ads_color = ambient + diffuse + specular; "+
        "}" +
        "else" +
        "{" +
        "phong_ads_color = vec3(1.0,1.0,1.0); " +
        "}" +
        "FragColor = vec4(phong_ads_color,1.0);" +
        "}";


        fragmentShaderObject=gl.createShader(gl.FRAGMENT_SHADER);
        gl.shaderSource(fragmentShaderObject,fragmentShaderSourceCode);
        gl.compileShader(fragmentShaderObject);
        if(gl.getShaderParameter(fragmentShaderObject,gl.COMPILE_STATUS)==false)
        {
            var error = gl.getShaderInfoLog(fragmentShaderObject);
            if(error.length > 0)
            {
                alert(error);
                uninitialize();
            }
        }

        shaderProgramObject = gl.createProgram();
        gl.attachShader(shaderProgramObject,vertexShaderObject);
        gl.attachShader(shaderProgramObject,fragmentShaderObject);

        gl.bindAttribLocation(shaderProgramObject,WebGLMacros.VDG_ATTRIBUTE_VERTEX,"vPosition");
        gl.bindAttribLocation(shaderProgramObject,WebGLMacros.VDG_ATTRIBUTE_NORMAL,"vNormal");

        //linking
        gl.linkProgram(shaderProgramObject);
        if(gl.getProgramParameter(shaderProgramObject,gl.LINK_STATUS))
        {
            var error = gl.getProgramInfoLog(shaderProgramObject);
            if(error.length > 0)
            {
                alert(error);
                uninitialize();
            }
        }



        modelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_model_matrix");
        viewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_view_matrix");

        projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_projection_matrix");

        LKeyPressedUniform = gl.getUniformLocation(shaderProgramObject,"u_LKeyPressed");

        laUniform = gl.getUniformLocation(shaderProgramObject, "u_La");
        ldUniform = gl.getUniformLocation(shaderProgramObject, "u_Ld");
        lsUniform = gl.getUniformLocation(shaderProgramObject, "u_Ls");
        kaUniform = gl.getUniformLocation(shaderProgramObject, "u_Ka");
        kdUniform = gl.getUniformLocation(shaderProgramObject, "u_Kd");
        ksUniform = gl.getUniformLocation(shaderProgramObject, "u_Ks");
        lightPositionUniform = gl.getUniformLocation(shaderProgramObject, "u_light_position");
        materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "u_material_shininess");

    
        sphere = new Mesh();
        makeSphere(sphere, 2.0, 30, 30);

        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clearDepth(1.0);
        gl.enable(gl.CULL_FACE);
        gl.enable(gl.DEPTH_TEST);
        gl.depthFunc(gl.LEQUAL);
        perspectiveProjectionMatrix = mat4.create();
}





function resize() {

    if (bFullscreen == true) {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    gl.viewport(0, 0, canvas.width, canvas.height);
    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);

}



function draw() {
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    var modelMatrix = mat4.create();
    var viewMatrix = mat4.create();

    if(bLKeyPressed == true)
    {
        gl.uniform1i(LKeyPressedUniform, 1);

        gl.uniform3fv(laUniform, light_ambient);
        gl.uniform3fv(ldUniform, light_diffuse);
        gl.uniform3fv(lsUniform, light_specular);
        gl.uniform4fv(lightPositionUniform, light_position);

    }

    myArrX[1] = 100 * Math.cos(2 * 3.1415 * degToRad(angleX));
    myArrX[2] = 100 * Math.sin(2 * 3.1415 * degToRad(angleX));

    myArrY[0] = 100 * Math.cos(2 * 3.1415 * degToRad(angleY));
    myArrY[2] = 100 * Math.sin(2 * 3.1415 * degToRad(angleY));

    myArrZ[0] = 100 * Math.cos(2 * 3.1415 * degToRad(angleZ));
    myArrZ[1] = 100 * Math.sin(2 * 3.1415 * degToRad(angleZ));


    if(bXKeyPressed == true)
    {
       // gl.uniform1i(LKeyPressedUniform, 1);

        gl.uniform3fv(laUniform, light_ambient);
        gl.uniform3fv(ldUniform, light_diffuse);
        gl.uniform3fv(lsUniform, light_specular);
        gl.uniform4fv(lightPositionUniform, myArrX);
    }

    if (bYKeyPressed == true)
    {
        gl.uniform3fv(laUniform, light_ambient);
        gl.uniform3fv(ldUniform, light_diffuse);
        gl.uniform3fv(lsUniform, light_specular);
        gl.uniform4fv(lightPositionUniform, myArrY);
    }

    if (bZKeyPressed == true)
    {
        gl.uniform3fv(laUniform, light_ambient);
        gl.uniform3fv(ldUniform, light_diffuse);
        gl.uniform3fv(lsUniform, light_specular);
        gl.uniform4fv(lightPositionUniform, myArrZ);
    }
  
    mat4.translate(modelMatrix, modelMatrix, [0.0, 0.0, -5.0]);
   //  mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    gl.viewport(0, 0, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient);
    gl.uniform3fv(kdUniform, material_diffuse);
    gl.uniform3fv(ksUniform, material_specular);
    gl.uniform1f(materialShininessUniform, material_shininess);
    sphere.draw();

    gl.viewport(canvas.width / 5.8, 0, canvas.width / 6, canvas.height / 6);            //230
    gl.uniform3fv(kaUniform, material_ambient1);
    gl.uniform3fv(kdUniform, material_diffuse1);
    gl.uniform3fv(ksUniform, material_specular1);
    gl.uniform1f(materialShininessUniform, material_shininess1);
    sphere.draw();

    gl.viewport(canvas.width / 3.0, 0, canvas.width / 6, canvas.height / 6);            //470
    gl.uniform3fv(kaUniform, material_ambient2);
    gl.uniform3fv(kdUniform, material_diffuse2);
    gl.uniform3fv(ksUniform, material_specular2);
    gl.uniform1f(materialShininessUniform, material_shininess2);
    sphere.draw();

    gl.viewport(canvas.width / 2.0, 0, canvas.width / 6, canvas.height / 6);                         //700
    gl.uniform3fv(kaUniform, material_ambient3);
    gl.uniform3fv(kdUniform, material_diffuse3);
    gl.uniform3fv(ksUniform, material_specular3);
    gl.uniform1f(materialShininessUniform, material_shininess3);
    sphere.draw();

    gl.viewport(canvas.width / 1.5, 0, canvas.width / 6, canvas.height / 6);                       //950
    gl.uniform3fv(kaUniform, material_ambient4);
    gl.uniform3fv(kdUniform, material_diffuse4);
    gl.uniform3fv(ksUniform, material_specular4);
    gl.uniform1f(materialShininessUniform, material_shininess4);
    sphere.draw();

    gl.viewport(canvas.width / 1.2, 0, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient5);
    gl.uniform3fv(kdUniform, material_diffuse5);
    gl.uniform3fv(ksUniform, material_specular5);
    gl.uniform1f(materialShininessUniform, material_shininess5);
    sphere.draw();

    ////////////////////
    gl.viewport(0, canvas.width / 6.5, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient1_2);
    gl.uniform3fv(kdUniform, material_diffuse1_2);
    gl.uniform3fv(ksUniform, material_specular1_2);
    gl.uniform1f(materialShininessUniform, material_shininess1_2);
    sphere.draw();

    gl.viewport(canvas.width / 5.8, canvas.width / 6.5, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient2_2);
    gl.uniform3fv(kdUniform, material_diffuse2_2);
    gl.uniform3fv(ksUniform, material_specular2_2);
    gl.uniform1f(materialShininessUniform, material_shininess2_2);
    sphere.draw();

    gl.viewport(canvas.width / 3.0, canvas.width / 6.5, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient3_2);
    gl.uniform3fv(kdUniform, material_diffuse3_2);
    gl.uniform3fv(ksUniform, material_specular3_2);
    gl.uniform1f(materialShininessUniform, material_shininess3_2);
    sphere.draw();

    gl.viewport(canvas.width / 2.0, canvas.width / 6.5, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient4_2);
    gl.uniform3fv(kdUniform, material_diffuse4_2);
    gl.uniform3fv(ksUniform, material_specular4_2);
    gl.uniform1f(materialShininessUniform, material_shininess4_2);
    sphere.draw();

    gl.viewport(canvas.width / 1.5, canvas.width / 6.5, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient5_2);
    gl.uniform3fv(kdUniform, material_diffuse5_2);
    gl.uniform3fv(ksUniform, material_specular5_2);
    gl.uniform1f(materialShininessUniform, material_shininess5_2);
    sphere.draw();

    gl.viewport(canvas.width / 1.2, canvas.width / 6.5, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient6_2);
    gl.uniform3fv(kdUniform, material_diffuse6_2);
    gl.uniform3fv(ksUniform, material_specular6_2);
    gl.uniform1f(materialShininessUniform, material_shininess6_2);
    sphere.draw();

    ////////////////////////////////

    //12
    gl.viewport(0, canvas.width / 3.4, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient1_3);
    gl.uniform3fv(kdUniform, material_diffuse1_3);
    gl.uniform3fv(ksUniform, material_specular1_3);
    gl.uniform1f(materialShininessUniform, material_shininess1_3);
    sphere.draw();

    gl.viewport(canvas.width / 5.8, canvas.width / 3.4, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient2_3);
    gl.uniform3fv(kdUniform, material_diffuse2_3);
    gl.uniform3fv(ksUniform, material_specular2_3);
    gl.uniform1f(materialShininessUniform, material_shininess2_3);
    sphere.draw();

    gl.viewport(canvas.width / 3.0, canvas.width / 3.4, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient3_3);
    gl.uniform3fv(kdUniform, material_diffuse3_3);
    gl.uniform3fv(ksUniform, material_specular3_3);
    gl.uniform1f(materialShininessUniform, material_shininess3_3);
    sphere.draw();

    gl.viewport(canvas.width / 2.0, canvas.width / 3.4, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient4_3);
    gl.uniform3fv(kdUniform, material_diffuse4_3);
    gl.uniform3fv(ksUniform, material_specular4_3);
    gl.uniform1f(materialShininessUniform, material_shininess4_3);
    sphere.draw();

    gl.viewport(canvas.width / 1.5, canvas.width / 3.4, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient5_3);
    gl.uniform3fv(kdUniform, material_diffuse5_3);
    gl.uniform3fv(ksUniform, material_specular5_3);
    gl.uniform1f(materialShininessUniform, material_shininess5_3);
    sphere.draw();

    gl.viewport(canvas.width / 1.2, canvas.width / 3.4, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient6_3);
    gl.uniform3fv(kdUniform, material_diffuse6_3);
    gl.uniform3fv(ksUniform, material_specular6_3);
    gl.uniform1f(materialShininessUniform, material_shininess6_3);
    sphere.draw();

    ///////////////////////////////////////////
    gl.viewport(0, canvas.width / 2.0 - 70, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient1_4);
    gl.uniform3fv(kdUniform, material_diffuse1_4);
    gl.uniform3fv(ksUniform, material_specular1_4);
    gl.uniform1f(materialShininessUniform, material_shininess1_4);
    sphere.draw();

    gl.viewport(canvas.width / 5.8, canvas.width / 2.0 - 70, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient2_4);
    gl.uniform3fv(kdUniform, material_diffuse2_4);
    gl.uniform3fv(ksUniform, material_specular2_4);
    gl.uniform1f(materialShininessUniform, material_shininess2_4);
    sphere.draw();

    gl.viewport(canvas.width / 3.0, canvas.width / 2.0 - 70, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient3_4);
    gl.uniform3fv(kdUniform, material_diffuse3_4);
    gl.uniform3fv(ksUniform, material_specular3_4);
    gl.uniform1f(materialShininessUniform, material_shininess3_4);
    sphere.draw();

    gl.viewport(canvas.width / 2.0, canvas.width / 2.0 - 70, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient4_4);
    gl.uniform3fv(kdUniform, material_diffuse4_4);
    gl.uniform3fv(ksUniform, material_specular4_4);
    gl.uniform1f(materialShininessUniform, material_shininess4_4);
    sphere.draw();

    gl.viewport(canvas.width / 1.5, canvas.width / 2.0 - 70, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient5_4);
    gl.uniform3fv(kdUniform, material_diffuse5_4);
    gl.uniform3fv(ksUniform, material_specular5_4);
    gl.uniform1f(materialShininessUniform, material_shininess5_4);
    sphere.draw();

    gl.viewport(canvas.width / 1.2, canvas.width / 2.0 - 70, canvas.width / 6, canvas.height / 6);
    gl.uniform3fv(kaUniform, material_ambient6_4);
    gl.uniform3fv(kdUniform, material_diffuse6_4);
    gl.uniform3fv(ksUniform, material_specular6_4);
    gl.uniform1f(materialShininessUniform, material_shininess6_4);
    sphere.draw();
    //sphere.draw();


    gl.useProgram(null);
    update();
    requestAnimationFrame(draw, canvas);

}


    function update() {
     //   degToRad(angleX);
        angleX = angleX + 0.5;
        if (angleX >= 360.0)
            angleX = 0.0;


        angleY = angleY - 0.5;
        if (angleY >= 360.0)
            angleY = 0.0;


        angleZ = angleZ - 0.5;
        if (angleZ >= 360.0)
            angleZ = 0.0;

        }



    function degToRad(degrees) {

        return (degrees * Math.PI / 180.0);
    }


    function uninitialize() {
      

        if (sphere) {
            sphere.deallocate();
            sphere.null;
        }

       
        if (shaderProgramObject) {
            if (fragmentShaderObject) {
                gl.detachShader(shaderProgramObject, fragmentShaderObject);
                gl.deleteShader(fragmentShaderObject);
                fragmentShaderObject = null;
            }



            if (vertexShaderObject) {
                gl.detachShader(shaderProgramObject, vertexShaderObject);
                gl.deleteShader(vertexShaderObject);
                vertexShaderObject = null;
            }

            gl.deleteProgram(shaderProgramObject);
            shaderProgramObject = null;
        }
    }






    function keyDown(event) {

        switch (event.keyCode) {
            case 27:
                window.close();
                uninitialize();
                break;
            case 76: // for 'L' or 'l'
                if (bLKeyPressed == false)
                    bLKeyPressed = true;
                else
                    bLKeyPressed = false;
                break;
            case 70:
                toggleFullScreen();
                break;  

            case 88:
                if (bXKeyPressed == false)
                    bXKeyPressed = true;
                else
                    bXKeyPressed = false;
                break;
            case 89:
                if (bYKeyPressed == false)
                    bYKeyPressed = true;
                else
                    bYKeyPressed = false;
                break;
            case 90:
                if (bZKeyPressed == false)
                    bZKeyPressed = true;
                else
                    bZKeyPressed = false;
                break;


        }
    }



    function mouseDown() {
        //  alert("mouseDown");
    }




























