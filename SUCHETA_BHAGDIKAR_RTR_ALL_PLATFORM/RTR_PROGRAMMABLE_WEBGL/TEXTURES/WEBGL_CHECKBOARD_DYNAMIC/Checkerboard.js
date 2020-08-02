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
            VDG_ATTRIBUTE_TEXTURE0: 3,
        };




    var vertexShaderObject;
    var fragmentShaderObject;
    var shaderProgramObject;

    var checkImageHeight=64;
    var checkImageWidth=64;
 //   var checkImage;
    //checkImage[64]= new Array();
    //checkImage[64][64] = new Array();
   // var checkImage[64][64][4];
    var checkImage = new Uint8Array(64 * 64 * 4);
    var inum = 0;


    var vao_Smiley;

    var vbo_position;
    var vbo_texture;
    var mvpUniform;


    var checkerboard_texture;
    var uniform_texture0_sampler;

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




    function init() {

        gl = canvas.getContext("webgl2");
        if (gl == null) {
            console.log("Failed to get the rendering context for WEBGL");
            return;
        }

        gl.viewportWidth = canvas.width;
        gl.viewportHeight = canvas.height;

    //    checkImageWidth = 64;
    //    checkImageHeight = 64;


        //VERTEX SHADER
        var vertexShaderSourceCode =
            "#version 300 es" +
            "\n" +
              "in vec4 vPosition;" +
              "in vec2 vTexture0_Coord;" +
              "out vec2 out_texture0_coord;" +
              "uniform mat4 u_mvp_matrix;" +
              "void main(void)" +
              "{" +
              "gl_Position = u_mvp_matrix*vPosition;" +
              "out_texture0_coord = vTexture0_Coord;" +
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
             "in vec2 out_texture0_coord;" +
             "uniform highp sampler2D u_texture0_sampler;" +
             "out vec4 FragColor;" +
             "void main(void)" +
             "{" +
             "FragColor =texture(u_texture0_sampler,out_texture0_coord);" +
             "}"

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
        gl.bindAttribLocation(shaderProgramObject, WebGLMacros.VDG_ATTRIBUTE_TEXTURE0, "vTexture0_Coord");

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


        //Load smiley textures
        checkerboard_texture = gl.createTexture();
        checkerboard_texture.image = new Image();
         console.log("after Image object");
   //      smiley_texture.image.src = "";
         console.log("before Image onload");
         checkerboard_texture.image.onload = LoadGLTextures()
        {
            console.log("Entering Image function");
  
            MakeCheckImage();
            gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);

            gl.bindTexture(gl.TEXTURE_2D, checkerboard_texture);
            console.log("After bind textures");
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
            console.log("Before texImage2d");
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, checkImageWidth, checkImageHeight, 0, gl.RGBA, gl.UNSIGNED_BYTE, checkImage);
    
         }

         function LoadGLTextures() {
             console.log("Entering LoadGLTextures");
      
         }

   
        mvpUniform = gl.getUniformLocation(shaderProgramObject, "u_mvp_matrix");
        uniform_texture0_sampler = gl.getUniformLocation(shaderProgramObject, "u_texture0_sampler");

        console.log("After uniforms");

        //var smileyVertices = new Float32Array([	//FRONT FACE
    	//-2.0,-1.0,0.0,
		//-2.0,1.0,0.0,
		// 0.0,1.0,0.0,
		// 0.0,-1.0,0.0
        //]);

        var smileyCoords=new Float32Array([

            0.0, 0.0,
            0.0, 1.0,
            1.0, 1.0,
            1.0, 0.0,    
        ]);
    


        vao_Smiley = gl.createVertexArray();
        gl.bindVertexArray(vao_Smiley);
    
        vbo_position = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position);
        gl.bufferData(gl.ARRAY_BUFFER, null, gl.DYNAMIC_DRAW);
        gl.vertexAttribPointer(WebGLMacros.VDG_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(WebGLMacros.VDG_ATTRIBUTE_VERTEX);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        vbo_texture = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
        gl.bufferData(gl.ARRAY_BUFFER, smileyCoords, gl.STATIC_DRAW);
        gl.vertexAttribPointer(WebGLMacros.VDG_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(WebGLMacros.VDG_ATTRIBUTE_TEXTURE0);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        gl.bindVertexArray(null);


        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clearDepth(1.0);
     //   gl.enable(gl.CULL_FACE);
        gl.enable(gl.DEPTH_TEST);
        gl.depthFunc(gl.LEQUAL);
        perspectiveProjectionMatrix = mat4.create();
        console.log("Exitting init");

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
        mat4.perspective(perspectiveProjectionMatrix, 45.0, parseFloat(canvas.width) / parseFloat(canvas.height), 1.0, 30.0);
        console.log("exiting resize");

    }




    function degToRad(degrees) {

        return (degrees * Math.PI / 180.0);
    }



    function MakeCheckImage()
    {
        console.log("Inside MakeCheck Image");
        var i, j,k,b;
        var c;
     
        for (i = 0;i < 64; i++)
        {
            for (j = 0; j < 64; j++)
            {
          
                b = (((i & 0x8) == 0) ^ ((j & 0x8) == 0));

                k = (b) ? 1 : 0;
      
                c = k * 255;
                checkImage[inum] = (c);
        //        console.log("Inside MakeCheck Image val", checkImage[inum]);

                inum++;
               
                checkImage[inum] = (c);
            //         console.log("Inside MakeCheck Image val", checkImage[inum]);
                inum++;
              
                checkImage[inum] = (c);
        //          console.log("Inside MakeCheck Image val", checkImage[inum]);
                inum++;
               
                checkImage[inum] = 255;
        //           console.log("Inside MakeCheck Image val", checkImage[inum]);

                inum++;
                 
            }
        }
        console.log("Exitting make check image");

    }


    function draw()
    {
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        gl.useProgram(shaderProgramObject);

        var modelViewMatrix = mat4.create();
        var modelViewProjectionMatrix = mat4.create();

   
        mat4.identity(modelViewMatrix);
        mat4.identity(modelViewProjectionMatrix);

        mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -3.0]);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        //////////////////// FRONT QUAD

        gl.bindVertexArray(vao_Smiley);
        gl.uniformMatrix4fv(mvpUniform, false, modelViewProjectionMatrix);
        gl.bindTexture(gl.TEXTURE_2D, checkerboard_texture);
        gl.uniform1i(uniform_texture0_sampler, 0);

        var checkerboardVertices = new Float32Array([	//FRONT FACE
        -2.0,-1.0,0.0,
        -2.0,1.0,0.0,
         0.0,1.0,0.0,
         0.0,-1.0,0.0
        ]);

       
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position);
        gl.bufferData(gl.ARRAY_BUFFER, checkerboardVertices, gl.DYNAMIC_DRAW);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
   
        gl.bindVertexArray(null);


        //////////////////// TITLED QUAD

        gl.bindVertexArray(vao_Smiley);
        gl.bindTexture(gl.TEXTURE_2D, checkerboard_texture);
        gl.uniform1i(uniform_texture0_sampler, 0);

        var checkerboardVertices2 = new Float32Array([	//FRONT FACE
      	1.0,-1.0,0.0,
		1.0,1.0,0.0,
		2.41421,1.0,-1.41421,
		2.41421,-1.0,-1.41421
        ]);


        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position);
        gl.bufferData(gl.ARRAY_BUFFER, checkerboardVertices2, gl.DYNAMIC_DRAW);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);

        gl.bindVertexArray(null);
        gl.useProgram(null);
        requestAnimationFrame(draw, canvas);

    }



    function uninitialize()
    {
        if (checkerboard_texture)
        {
            gl.deleteVertexArray(checkerboard_texture);
            checkerboard_texture = null;
        }

      
        if (vao_Smiley)
        {
            gl.deleteBuffer(vao_Smiley);
            vao_Smiley = null;
        }

      

        if (vbo_texture) {
            gl.deleteBuffer(vbo_texture);
            vbo_texture = null;
        }


        if (vbo_position) {
            gl.deleteBuffer(vbo_position);
            vbo_position = null;
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

            case 70:
                toggleFullScreen();
                break;
        }
    }



    function mouseDown()
    {
      //  alert("mouseDown");
    }

















































