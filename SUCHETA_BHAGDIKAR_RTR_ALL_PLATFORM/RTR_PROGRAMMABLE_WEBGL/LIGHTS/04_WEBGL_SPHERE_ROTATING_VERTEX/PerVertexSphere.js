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
var light_diffuse = [1.0, 0.0, 0.0];
var light_specular = [1.0, 1.0, 1.0];
var light_position = [0.0, 100.0, 100.0, 1.0];

var light_ambient1 = [0.0, 0.0, 0.0];
var light_diffuse1 = [0.0, 1.0, 0.0];
var light_specular1 = [1.0, 1.0, 1.0];
var light_position1 = [100.0, 0.0, 100.0, 1.0];

var light_ambient2 = [0.0, 0.0, 0.0];
var light_diffuse2 = [0.0, 0.0, 1.0];
var light_specular2 = [1.0, 1.0, 1.0];
var light_position2 = [100.0, 100.0, 0.0, 1.0];


var material_ambient = [0.0, 0.0, 0.0];
var material_diffuse = [1.0, 1.0, 1.0];
var material_specular = [1.0, 1.0, 1.0];
var material_shininess = 50.0;

var sphere = null;

var myArrRed = [0.0, 0.0, 0.0, 0.0];
var myArrGreen = [0.0, 0.0, 0.0, 0.0];
var myArrBlue = [0.0, 0.0, 0.0, 0.0];

var angleRed = 0.0;
var angleGreen = 0.0;
var angleBlue = 0.0;

var perspectiveProjectionMatrix;

var modelMatrixUniform, viewMatrixUniform,projectionMatrixUniform;

var laUniform, ldUniform, lsUniform, lightPositionUniform;
var kaUniform, kdUniform, ksUniform, materialShininessUniform;
var laUniformGreen, ldUniformGreen, lsUniformGreen, lightPositionUniformGreen;
var kaUniformGreen, kdUniformGreen, ksUniformGreen;
var laUniformBlue, ldUniformBlue, lsUniformBlue, lightPositionUniformBlue;
var kaUniformBlue, kdUniformBlue, ksUniformBlue;
var LKeyPressedUniform;

var angle_cube = 0.0;

var bLKeyPressed = false;

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

        "uniform vec3 u_La;" +
        "uniform vec3 u_Ld;" +
        "uniform vec3 u_Ls;" +
        "uniform vec3 u_Ka;" +
        "uniform vec3 u_Kd;" +
        "uniform vec3 u_Ks;" +
        "uniform vec4 u_light_position;" +

        "uniform vec3 u_LaGreen;" +
        "uniform vec3 u_LdGreen;" +
        "uniform vec3 u_LsGreen;" +
        "uniform vec3 u_KaGreen;" +
        "uniform vec3 u_KdGreen;" +
        "uniform vec3 u_KsGreen;" +
        "uniform vec4 u_light_positionGreen;" +

        "uniform vec3 u_LaBlue;" +
        "uniform vec3 u_LdBlue;" +
        "uniform vec3 u_LsBlue;" +
        "uniform vec3 u_KaBlue;" +
        "uniform vec3 u_KdBlue;" +
        "uniform vec3 u_KsBlue;" +
        "uniform vec4 u_light_positionBlue;" +

        "uniform float u_material_shininess;" +

        "out vec3 phong_ads_color;" +
        "out vec3 phong_ads_colorGreen;" +
        "out vec3 phong_ads_colorBlue;" +

        "void main(void)" +
        "{" +
        "if(u_LKeyPressed == 1)" +
        "{" +

        "vec4 eyeCoordinates = u_view_matrix*u_model_matrix* vPosition;" +
        "vec3 transformed_normals = normalize(mat3(u_view_matrix*u_model_matrix)*vNormal);" +
        "vec3 viewer_vector = normalize(-eyeCoordinates.xyz);" +

        "vec3 light_direction = normalize(vec3(u_light_position) - eyeCoordinates.xyz);" +
        "float tn_dot_ld = max(dot(transformed_normals,light_direction),0.0);" +
        "vec3 ambient = u_Ls * u_Ka;" +
        "vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" +
        "vec3 reflection_vector=reflect(-light_direction,transformed_normals);" +
        "vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector,viewer_vector),0.0),u_material_shininess);" +
        "phong_ads_color = ambient + diffuse + specular;" +

         "vec3 light_directionGreen = normalize(vec3(u_light_positionGreen) - eyeCoordinates.xyz);" +
        "float tn_dot_ldGreen = max(dot(transformed_normals,light_directionGreen),0.0);" +
        "vec3 ambientGreen = u_LsGreen * u_KaGreen;" +
        "vec3 diffuseGreen = u_LdGreen * u_KdGreen * tn_dot_ldGreen;" +
        "vec3 reflection_vectorGreen=reflect(-light_directionGreen,transformed_normals);" +
        "vec3 specularGreen = u_Ls * u_Ks * pow(max(dot(reflection_vectorGreen,viewer_vector),0.0),u_material_shininess);" +
        "phong_ads_colorGreen = ambientGreen + diffuseGreen + specularGreen;" +

        "vec3 light_directionBlue = normalize(vec3(u_light_positionBlue) - eyeCoordinates.xyz);" +
        "float tn_dot_ldBlue = max(dot(transformed_normals,light_directionBlue),0.0);" +
        "vec3 ambientBlue = u_LsBlue * u_KaBlue;" +
        "vec3 diffuseBlue = u_LdBlue * u_KdBlue * tn_dot_ldBlue;" +
        "vec3 reflection_vectorBlue=reflect(-light_directionBlue,transformed_normals);" +
        "vec3 specularBlue = u_Ls * u_Ks * pow(max(dot(reflection_vectorBlue,viewer_vector),0.0),u_material_shininess);" +
        "phong_ads_colorBlue = ambientBlue + diffuseBlue + specularBlue;" +
        "}" +
        "else" +
        "{" +
         "phong_ads_color =vec3(1.0,1.0,1.0);" +
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
        "in vec3 phong_ads_color;" +
        "in vec3 phong_ads_colorGreen;" +
        "in vec3 phong_ads_colorBlue;" +

        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
          "FragColor = vec4(phong_ads_color,1.0) + vec4(phong_ads_colorGreen,1.0) + vec4(phong_ads_colorBlue,1.0);" +
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

        laUniformGreen = gl.getUniformLocation(shaderProgramObject, "u_LaGreen");
        ldUniformGreen = gl.getUniformLocation(shaderProgramObject, "u_LdGreen");
        lsUniformGreen = gl.getUniformLocation(shaderProgramObject, "u_LsGreen");
        kaUniformGreen = gl.getUniformLocation(shaderProgramObject, "u_KaGreen");
        kdUniformGreen = gl.getUniformLocation(shaderProgramObject, "u_KdGreen");
        ksUniformGreen = gl.getUniformLocation(shaderProgramObject, "u_KsGreen");
        lightPositionUniformGreen = gl.getUniformLocation(shaderProgramObject, "u_light_positionGreen");

        laUniformBlue = gl.getUniformLocation(shaderProgramObject, "u_LaBlue");
        ldUniformBlue = gl.getUniformLocation(shaderProgramObject, "u_LdBlue");
        lsUniformBlue = gl.getUniformLocation(shaderProgramObject, "u_LsBlue");
        kaUniformBlue = gl.getUniformLocation(shaderProgramObject, "u_KaBlue");
        kdUniformBlue = gl.getUniformLocation(shaderProgramObject, "u_KdBlue");
        ksUniformBlue = gl.getUniformLocation(shaderProgramObject, "u_KsBlue");
        lightPositionUniformBlue = gl.getUniformLocation(shaderProgramObject, "u_light_positionBlue");


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
        gl.uniform4fv(lightPositionUniform, myArrRed);

        gl.uniform3fv(kaUniform, material_ambient);
        gl.uniform3fv(kdUniform, material_diffuse);
        gl.uniform3fv(ksUniform,material_specular);
        gl.uniform1f(materialShininessUniform, material_shininess);

        gl.uniform3fv(laUniformGreen, light_ambient1);
        gl.uniform3fv(ldUniformGreen, light_diffuse1);
        gl.uniform3fv(lsUniformGreen, light_specular1);
        gl.uniform4fv(lightPositionUniformGreen, myArrGreen);

        gl.uniform3fv(kaUniformGreen, material_ambient);
        gl.uniform3fv(kdUniformGreen, material_diffuse);
        gl.uniform3fv(ksUniformGreen, material_specular);
        gl.uniform1f(materialShininessUniform, material_shininess);

        gl.uniform3fv(laUniformBlue, light_ambient2);
        gl.uniform3fv(ldUniformBlue, light_diffuse2);
        gl.uniform3fv(lsUniformBlue, light_specular2);
        gl.uniform4fv(lightPositionUniformBlue, myArrBlue);

        gl.uniform3fv(kaUniformBlue, material_ambient);
        gl.uniform3fv(kdUniformBlue, material_diffuse);
        gl.uniform3fv(ksUniformBlue, material_specular);
        gl.uniform1f(materialShininessUniform, material_shininess);
    }
    else {
        gl.uniform1i(LKeyPressedUniform, 0);
    }

   
    mat4.translate(modelMatrix, modelMatrix, [0.0, 0.0, -5.0]);
  //  mat4.rotateX(modelMatrix, modelMatrix,degToRad())
   //  mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    myArrRed[1] = 100 * Math.cos(2 * 3.1415 * degToRad(angleRed));
    myArrRed[2] = 100 * Math.sin(2 * 3.1415 * degToRad(angleRed));

    myArrGreen[0] = 100 * Math.cos(2 * 3.1415 * degToRad(angleGreen));
    myArrGreen[2] = 100 * Math.sin(2 * 3.1415 * degToRad(angleGreen));

    myArrBlue[0] = 100 * Math.cos(2 * 3.1415 * degToRad(angleBlue));
    myArrBlue[1] = 100 * Math.sin(2 * 3.1415 * degToRad(angleBlue));

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);

    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    sphere.draw();


    gl.useProgram(null);
    update();
    requestAnimationFrame(draw, canvas);

}



function degToRad(degrees) {

    return (degrees * Math.PI / 180.0);
}

    function update() {
    
        angleRed = angleRed + 1.0;
        if (angleRed >= 360.0)
            angleRed = 0.0;

        angleGreen = angleGreen - 1.0;
        if (angleGreen >= 360.0)
            angleGreen = 0.0;

        angleBlue = angleBlue - 1.0;
        if (angleBlue >= 360.0)
            angleBlue = 0.0;
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
        }
    }



    function mouseDown() {
        //  alert("mouseDown");
    }



























