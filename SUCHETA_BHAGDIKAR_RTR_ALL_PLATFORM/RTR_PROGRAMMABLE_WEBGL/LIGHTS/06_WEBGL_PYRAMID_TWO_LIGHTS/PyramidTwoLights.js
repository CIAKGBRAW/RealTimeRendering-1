var canvas = null;
var gl = null;
var bFullscreen = null;
var canvas_original_width;
var canvas_original_height;

const WebGLMacros =
    {
        VDG_ATTRIBUTE_VERTEX: 0,
        VDG_ATTRIBUTE_COLOR: 1,
        VDG_ATTRIBUTE_NORMAL: 2,
        VDG_ATTRIBUTE_TEXTURE: 3,
    };




var vertexShaderObject;
var fragmentShaderObject;
var shaderProgramObject;

var modelMatrixUniform, viewMatrixUniform, projectionMatrixUniform;

var vao_Pyramid;
var vbo_Position_Pyramid;
var vbo_Color_Pyramid;

var LKeyPressedUniform;

var La_uniform;
var Ld_uniform;
var Ls_uniform;
var light_position_uniform;
var light_position_uniformRed;

var Ka_uniform;
var Kd_uniform;
var Ks_uniform;
var material_shininess_uniform;

var La_uniformRed;
var Ld_uniformRed;
var Ls_uniformRed;

var Ka_uniformRed;
var Kd_uniformRed;
var Ks_uniformRed;
var material_shininess_uniformRed;

var vao_Cube;
var vbo_Position_Cube;
var vbo_Color_Cube;

var mvpUniform;
var u_Translation;




var light_Ambient = [0.0, 0.0, 0.0];
var light_Diffuse = [1.0, 0.0, 0.0];
var light_Specular = [1.0, 0.0, 0.0];
var light_position = [2.0, 1.0, 1.0, 1.0];

var bLKeyPressed = false;

var light_AmbientL1 = [0.0, 0.0, 0.0];
var light_DiffuseL1 = [0.0, 0.0, 1.0];
var light_SpecularL1 = [0.0, 0.0, 1.0];
var light_positionL1 = [-2.0,1.0,1.0,1.0];

var material_ambient = [0.0, 0.0, 0.0];
var material_diffuse = [1.0, 1.0, 1.0];
var material_specular = [1.0, 1.0, 1.0];
var material_shininess = 50.0;


var angleX = 0.0;
var angleY = 0.0;
var angleZ = 0.0;

var perspectiveProjectionMatrix;

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

function main()
{
    canvas=document.getElementById("AMC");
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

function toggleFullScreen()
{
    var fullscreen_element = 
        document.fullscreenElement ||
        document.webkitFullscreenElement ||
        document.mozFullScreenElement
    document.msFullscreenElement ||
    null;



    if(fullscreen_element == null)
    {
        if (canvas.requestFullscreen)
            canvas.requestFullscreen();
        else if (canvas.mozRequestFullScreen)
            canvas.mozRequestFullScreen();
        else if(canvas.webkitRequestFullscreen)
            canvas.webkitRequestFullscreen();
        else if (canvas.oRequestFullscreen)
            canvas.oRequestFullscreen();
        else if (canvas.msRequestFullscreen)
            canvas.msRequestFullscreen();
        bFullscreen = true;

    }
    else
    {
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





0


function init() {
    gl = canvas.getContext("webgl2");
    if (gl == null) {
        console.log("Failed to get the rendering context for WEBGL");
        return;
    }

    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;




    //VERTEX SHADER
    var vertexShaderSourceCode =
        "#version 300 es" +
        "\n" +
      	"in vec4 vPosition;" +
        "in vec3 vNormal;" +
        "uniform mat4 u_model_matrix;" +
        "uniform mat4 u_view_matrix;" +
        "uniform mat4 u_projection_matrix;" +
        "uniform int u_lighting_enabledBlue;" +
        "uniform vec3 u_LaBlue;" +
        "uniform vec3 u_LdBlue;" +
        "uniform vec3 u_LsBlue;" +

        "uniform vec3 u_LaRed;" +
        "uniform vec3 u_LsRed;" +
        "uniform vec3 u_LdRed;" +

        "uniform vec4 u_light_positionBlue;" +
        "uniform vec4 u_light_positionRed;" +
        "uniform vec3 u_KaBlue;" +
        "uniform vec3 u_KdBlue;" +
        "uniform vec3 u_KsBlue;" +

        "uniform vec3 u_KaRed;" +
        "uniform vec3 u_KdRed;" +
        "uniform vec3 u_KsRed;" +
        "uniform float u_material_shininess;" +
        "uniform float u_material_shininessRed;" +

        "out vec3 phong_ads_colorBlue;" +
        "out vec3 phong_ads_colorRed;" +
        "void main(void)" +
        "{" +
        "vec4 eye_coordinates= u_view_matrix * u_model_matrix *vPosition;" +
        "vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" +
        "vec3 viewer_vector = normalize(-eye_coordinates.xyz);" +
      
        "vec3 light_directionRed = normalize(vec3(u_light_positionRed) - eye_coordinates.xyz);"+
        "float tn_dot_ld = max(dot(transformed_normals,light_directionRed),0.0);" +
        "vec3 ambientRed = u_LaRed * u_KaRed;" +
        "vec3 diffuseRed = u_LdRed * u_KdRed * tn_dot_ld;" +
        "vec3 reflection_vector = reflect(-light_directionRed,transformed_normals);" +
        "vec3 specularRed =  u_LsRed * u_KsRed * pow(max(dot(reflection_vector,viewer_vector),0.0),u_material_shininessRed);" +
        "phong_ads_colorRed = ambientRed + diffuseRed + specularRed;" +
		
        "vec3 light_directionBlue = normalize(vec3(u_light_positionBlue) - eye_coordinates.xyz);"+
        "float tn_dot_ldBlue = max(dot(transformed_normals,light_directionBlue),0.0);" +
        "vec3 ambientBlue = u_LaBlue * u_KaBlue;" +
        "vec3 diffuseBlue = u_LdBlue * u_KdBlue * tn_dot_ldBlue;" +
        "vec3 reflection_vectorBlue = reflect(-light_directionBlue,transformed_normals);"+
        "vec3 specularBlue = u_LsBlue * u_KsBlue * pow(max(dot(reflection_vectorBlue,viewer_vector),0.0),u_material_shininess);" +
        "phong_ads_colorBlue = ambientBlue + diffuseBlue + specularBlue;" +
       
        "gl_Position =u_projection_matrix *  u_view_matrix * u_model_matrix * vPosition;" +
        "}";

    vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);
    gl.compileShader(vertexShaderObject);
    if(gl.getShaderParameter(vertexShaderObject,gl.COMPILE_STATUS) ==false)
    {
        var error = gl.getShaderInfoLog(vertexShaderObject);
        if(error.length >0)
        {
            alert(error);
            unitialize();
        }
    }


    //fragment shader
    var fragmentShaderSourceCode =
        "#version 300 es" +
        "\n" +
        "precision highp float;" +
        "in vec3 phong_ads_colorBlue;" +
        "in vec3 phong_ads_colorRed;" +
        "out vec4 light;" +
        " vec4 FragColorRed;" +
        " vec4 FragColorBlue;" +
        "uniform int u_lighting_enabledRed;" +
        "void main(void)" +
        "{" +
        "if(u_lighting_enabledRed == 1)" +
        "{" +
        "FragColorRed = vec4 (phong_ads_colorRed,1.0);" +
        "FragColorBlue = vec4(phong_ads_colorBlue,1.0);" +
        "light = FragColorRed + FragColorBlue ;" +
        "}" +
        "else"+
        "{" +
        "light=vec4(1,1,1,1); "+
        "}" +
        "}";
    fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);
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


    //shader program 
    shaderProgramObject = gl.createProgram();
    gl.attachShader(shaderProgramObject, vertexShaderObject);
    gl.attachShader(shaderProgramObject, fragmentShaderObject);


    gl.bindAttribLocation(shaderProgramObject, WebGLMacros.VDG_ATTRIBUTE_VERTEX, "vPosition");
    gl.bindAttribLocation(shaderProgramObject, WebGLMacros.VDG_ATTRIBUTE_NORMAL, "vNormal");

    gl.linkProgram(shaderProgramObject);
    if(!gl.getProgramParameter(shaderProgramObject,gl.LINK_STATUS))
    {
        var error = gl.getProgramInfoLog(shaderProgramObject);
        if(error.length >0)
        {
            alert(error);
            uninitialize();
        }
    }

    modelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_model_matrix");
    viewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_view_matrix");
    projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_projection_matrix");

    La_uniform = gl.getUniformLocation(shaderProgramObject, "u_LaBlue");
    Ld_uniform = gl.getUniformLocation(shaderProgramObject, "u_LdBlue");
    Ls_uniform = gl.getUniformLocation(shaderProgramObject, "u_LsBlue");
    Ka_uniform = gl.getUniformLocation(shaderProgramObject, "u_KaBlue");
    Kd_uniform = gl.getUniformLocation(shaderProgramObject, "u_KdBlue");
    Ks_uniform = gl.getUniformLocation(shaderProgramObject, "u_KsBlue");
    light_position_uniform = gl.getUniformLocation(shaderProgramObject, "u_light_positionBlue");

    La_uniformRed = gl.getUniformLocation(shaderProgramObject, "u_LaRed");
    Ld_uniformRed = gl.getUniformLocation(shaderProgramObject, "u_LdRed");
    Ls_uniformRed = gl.getUniformLocation(shaderProgramObject, "u_LsRed");
    Ka_uniformRed = gl.getUniformLocation(shaderProgramObject, "u_KaRed");
    Kd_uniformRed = gl.getUniformLocation(shaderProgramObject, "u_KdRed");
    Ks_uniformRed = gl.getUniformLocation(shaderProgramObject, "u_KsRed");
    light_position_uniformRed = gl.getUniformLocation(shaderProgramObject, "u_light_positionRed");
    LKeyPressedUniform = gl.getUniformLocation(shaderProgramObject, "u_lighting_enabledRed");
    material_shininess_uniformRed = gl.getUniformLocation(shaderProgramObject, "u_material_shininessRed");
    material_shininess_uniform = gl.getUniformLocation(shaderProgramObject, "u_material_shininess");

    var pyramidVertices = new Float32Array([	//FRONT FACE
		0.0, 1.0, 0.0,
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,

    //RIGHT FACE
		0.0, 1.0, 0.0,
		1.0, -1.0, 1.0,
		1.0, -1.0, -1.0,

    //LEFT FACE
		0.0, 1.0, 0.0,
		-1.0, -1.0, 1.0,
		-1.0, -1.0, -1.0,

    //BACK FACE
		0.0, 1.0, 0.0,
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0]);

    var colorVerticesPyramid = new Float32Array([

  	//FRONT FACE
		0.0,0.447214,0.894427,
		0.0,0.447214,0.894427,
		0.0,0.447214,0.894427,

    //RIGHT FACE
		0.894427, 0.447214, 0.0,
		0.894427, 0.447214, 0.0,
		0.894427, 0.447214, 0.0,

    //LEFT FACE
		-0.894427, 0.447214, 0.0,
		-0.894427, 0.447214, 0.0,
		-0.894427, 0.447214, 0.0,

    //BACK FACE
		0.0, 0.447214, -0.894427,
		0.0, 0.447214, -0.894427,
		0.0, 0.447214, -0.894427,
		]);


    vao_Pyramid = gl.createVertexArray();
    gl.bindVertexArray(vao_Pyramid);
    
    vbo_Position_Pyramid = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Position_Pyramid);
    gl.bufferData(gl.ARRAY_BUFFER, pyramidVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.VDG_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.VDG_ATTRIBUTE_VERTEX);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    vbo_Color_Pyramid = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Color_Pyramid);
    gl.bufferData(gl.ARRAY_BUFFER, colorVerticesPyramid, gl.STATIC_DRAW);
    gl.vertexAttribPointer(WebGLMacros.VDG_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(WebGLMacros.VDG_ATTRIBUTE_NORMAL);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);
    bLKeyPressed = false;
    perspectiveProjectionMatrix = mat4.create();

}


function resize()
{

    if(bFullscreen==true)
    {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else
    {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    gl.viewport(0, 0, canvas.width, canvas.height);

    mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 0.1, 100.0);

}




function degToRad(degrees) {

    return (degrees * Math.PI / 180.0);
}



function draw()
{
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.useProgram(shaderProgramObject);

    var modelViewMatrix = mat4.create();
    var projectionMatrix = mat4.create();
    var modelMatrix = mat4.create();
    var viewMatrix = mat4.create();

    if (bLKeyPressed == true) {

        gl.uniform3fv(La_uniform, light_AmbientL1);
        gl.uniform3fv(Ld_uniform, light_DiffuseL1);
        gl.uniform3fv(Ls_uniform, light_SpecularL1);
        gl.uniform4fv(light_position_uniform, light_positionL1);

        gl.uniform3fv(Ka_uniform, material_ambient);
        gl.uniform3fv(Kd_uniform, material_diffuse);
        gl.uniform3fv(Ks_uniform, material_specular);
        gl.uniform1f(material_shininess_uniform, material_shininess);

        gl.uniform1i(LKeyPressedUniform, 1);

        gl.uniform3fv(La_uniformRed, light_Ambient);
        gl.uniform3fv(Ld_uniformRed, light_Diffuse);
        gl.uniform3fv(Ls_uniformRed, light_Specular);
        gl.uniform4fv(light_position_uniformRed, light_position);

        gl.uniform3fv(Ka_uniformRed, material_ambient);
        gl.uniform3fv(Kd_uniformRed, material_diffuse);
        gl.uniform3fv(Ks_uniformRed, material_specular);
        gl.uniform1f(material_shininess_uniformRed, material_shininess);

    }
    else {
        gl.uniform1i(LKeyPressedUniform, 0);
    }


    mat4.translate(modelMatrix, modelMatrix, [0.0,0.0,-6.0]);
    mat4.rotateY(modelMatrix, modelMatrix, degToRad(angleY));
  //  mat4.scale(modelViewMatrix, modelViewMatrix, [0.75, 0.75, 0.75]);


    mat4.multiply(perspectiveProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    gl.bindVertexArray(vao_Pyramid);

    gl.drawArrays(gl.TRIANGLES, 0, 12);

    gl.bindVertexArray(null);


    gl.useProgram(null);
    update();
    requestAnimationFrame(draw, canvas);

}


function update() {
    angleX = angleX + 2.0;
    if (angleX >= 360.0)
        angleX = 0.0;

    angleY = angleY + 2.0;
    if (angleY >= 360.0)
        angleY = 0.0;

    
    angleZ = angleZ + 2.0;
    if (angleZ >= 360.0)
        angleZ = 0.0;

}

function uninitialize()
{
    if (vao_Pyramid)
    {
        gl.deleteBuffer(vao_Pyramid);
        vao_Pyramid = null;
    }


    if(shaderProgramObject)
    {
        if(fragmentShaderObject)
        {
            gl.detachShader(shaderProgramObject, fragmentShaderObject);
            gl.deleteShader(fragmentShaderObject);
            fragmentShaderObject = null;
        }



        if(vertexShaderObject)
        {
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



function mouseDown()
{
  //  alert("mouseDown");
}
















