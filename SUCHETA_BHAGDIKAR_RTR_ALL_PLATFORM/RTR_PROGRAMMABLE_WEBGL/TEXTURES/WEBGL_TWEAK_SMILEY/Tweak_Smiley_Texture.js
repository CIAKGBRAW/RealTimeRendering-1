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


    var IsDigitPressed;

    var vertexShaderObject;
    var fragmentShaderObject;
    var shaderProgramObject;


    var checkImageHeight = 64;
    var checkImageWidth = 64;
    var checkImage = new Uint8Array(64 * 64 * 4);
    var inum = 0;


    var vao_Smiley;
    var vao_WhiteQuad;

    var texname;

    var vbo_position;
    var vbo_texture;
    var mvpUniform;

    var checkImageHeigth = 64;
    var checkImageWidth = 64;


    var smiley_texture = 0;
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
        smiley_texture = gl.createTexture();
        smiley_texture.image = new Image();
        smiley_texture.image.src = "Smiley.png";
        smiley_texture.image.onload = function ()
        {
            gl.bindTexture(gl.TEXTURE_2D, smiley_texture);
            gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, smiley_texture.image);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
            gl.bindTexture(gl.TEXTURE_2D, null);
        }




        ////////////////////////////////////////////////////////////////White Quad////////////////////////////////////////
       

          var whiteQuadTexcoords = new Float32Array([
               0.0,0.0,
        		0.0,1.0,
        		1.0,1.0,
        		1.0,0.0
          ]);


          var whiteQuadVertices = new Float32Array([	//FRONT FACE
                 -1.0, -1.0, 0.0,
        		-1.0, 1.0, 0.0,
        		1.0, 1.0, 0.0,
        		1.0, -1.0, 0.0
          ]);

        //For White Quad
        texname = gl.createTexture();
        texname.image = new Image();
        console.log("after Image object");
        console.log("before Image onload");
        texname.image.onload = LoadGLTextures()
        {
            console.log("Entering Image function");

            MakeCheckImage();
            gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);

            gl.bindTexture(gl.TEXTURE_2D, texname);
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
        
        function MakeCheckImage()
        {
            console.log("Inside MakeCheck Image");
            var i, j, k, b;
            var c;

            for (i = 0; i < 64; i++) {
                for (j = 0; j < 64; j++) {

                     checkImage[inum] = 255;
                    inum++;

                    checkImage[inum] = 255;
                    inum++;

                    checkImage[inum] = 255;
                    inum++;

                    checkImage[inum] = 255;
                    inum++;

                }
            }
            console.log("Exitting make check image");

        }


        vao_WhiteQuad = gl.createVertexArray();
        gl.bindVertexArray(vao_WhiteQuad);

        vbo_position = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position);
        gl.bufferData(gl.ARRAY_BUFFER, whiteQuadVertices, gl.STATIC_DRAW);
        gl.vertexAttribPointer(WebGLMacros.VDG_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(WebGLMacros.VDG_ATTRIBUTE_VERTEX);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        vbo_texture = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
        gl.bufferData(gl.ARRAY_BUFFER, whiteQuadTexcoords, gl.STATIC_DRAW);
        gl.vertexAttribPointer(WebGLMacros.VDG_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(WebGLMacros.VDG_ATTRIBUTE_TEXTURE0);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        gl.bindVertexArray(null);



        mvpUniform = gl.getUniformLocation(shaderProgramObject, "u_mvp_matrix");
        uniform_texture0_sampler = gl.getUniformLocation(shaderProgramObject, "u_texture0_sampler");


        var smileyVertices = new Float32Array([	//FRONT FACE
    	-1.0,-1.0,0.0,
		-1.0,1.0,0.0,
		1.0,1.0,0.0,
		1.0,-1.0,0.0,
        ]);

        //var smileyCoords=new Float32Array([

        //    0.0, 0.0,
        //    0.0, 1.0,
        //    1.0, 1.0,
        //    1.0, 0.0,    
        //]);
    


        vao_Smiley = gl.createVertexArray();
        gl.bindVertexArray(vao_Smiley);
    
        vbo_position = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position);
        gl.bufferData(gl.ARRAY_BUFFER, smileyVertices, gl.STATIC_DRAW);
        gl.vertexAttribPointer(WebGLMacros.VDG_ATTRIBUTE_VERTEX, 3, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(WebGLMacros.VDG_ATTRIBUTE_VERTEX);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        vbo_texture = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
        gl.bufferData(gl.ARRAY_BUFFER, null, gl.DYNAMIC_DRAW);
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
        gl.clear(gl.COLOR_BUFFER_BIT);

        gl.useProgram(shaderProgramObject);

        var modelViewMatrix = mat4.create();
        var modelViewProjectionMatrix = mat4.create();

   
        mat4.identity(modelViewMatrix);
        mat4.identity(modelViewProjectionMatrix);

        mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -6.0]);
        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

        gl.uniformMatrix4fv(mvpUniform, false, modelViewProjectionMatrix);
        gl.uniform1i(uniform_texture0_sampler, 0);

        gl.bindVertexArray(vao_Smiley);
        gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
     /*   gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
        gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);*/

        var smileyCoords = new Float32Array([
                0.0, 0.0,
                0.0, 1.0,
                1.0, 1.0,
                1.0, 0.0,
        ]);
        var smileyCoords2 = new Float32Array([
              0.0, 0.0,
              0.0, 0.5,
              0.5, 0.5,
              0.5, 0.0,
        ]);
        var smileyCoords3 = new Float32Array([
               0.0, 0.0,
               0.0, 2.0,
               2.0, 2.0,
               2.0, 0.0,
        ]);


        var smileyCoords4 = new Float32Array([
               0.5, 0.5,
               0.5, 0.5,
               0.5, 0.5,
               0.5, 0.5,
                ]);

        if(IsDigitPressed == 1)
        {
            gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
            gl.bufferData(gl.ARRAY_BUFFER, smileyCoords2, gl.DYNAMIC_DRAW);
            gl.bindBuffer(gl.ARRAY_BUFFER, null);
            gl.bindTexture(gl.TEXTURE_2D, smiley_texture);

        }
        else if (IsDigitPressed == 2)
        {            
            gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
            gl.bufferData(gl.ARRAY_BUFFER, smileyCoords, gl.DYNAMIC_DRAW);
            gl.bindBuffer(gl.ARRAY_BUFFER, null);
            gl.bindTexture(gl.TEXTURE_2D, smiley_texture);

        }
        else if (IsDigitPressed == 3)
        {
            gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
            gl.bufferData(gl.ARRAY_BUFFER, smileyCoords3, gl.DYNAMIC_DRAW);
            gl.bindBuffer(gl.ARRAY_BUFFER, null);
            gl.bindTexture(gl.TEXTURE_2D, smiley_texture);

        }
        else if (IsDigitPressed == 4)
        {
            gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
            gl.bufferData(gl.ARRAY_BUFFER, smileyCoords4, gl.DYNAMIC_DRAW);
            gl.bindBuffer(gl.ARRAY_BUFFER, null);
            gl.bindTexture(gl.TEXTURE_2D, smiley_texture);

        }
        else
        {
            //gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
            //gl.bufferData(gl.ARRAY_BUFFER, smileyCoords, gl.DYNAMIC_DRAW);
            //gl.bindBuffer(gl.ARRAY_BUFFER, null);
          //  gl.bindTexture(gl.TEXTURE_2D, texname);
            gl.bindVertexArray(vao_WhiteQuad);
            gl.bindTexture(gl.TEXTURE_2D, texname);
            gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
            gl.bindVertexArray(null);
        }

        gl.bindVertexArray(null);


        gl.useProgram(null);
        requestAnimationFrame(draw, canvas);

    }



    function uninitialize()
    {
        if (smiley_texture)
        {
            gl.deleteVertexArray(smiley_texture);
            smiley_texture = null;
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

            case 0x31:
                IsDigitPressed = 1;
                break;
            case 0x32:
                IsDigitPressed = 2;
                break;
            case 0x33:
                IsDigitPressed = 3;
                break;
            case 0x34:
                IsDigitPressed = 4;
                break;
        }
    }



    function mouseDown()
    {
      //  alert("mouseDown");
    }

















































