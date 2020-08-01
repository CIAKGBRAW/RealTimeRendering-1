var canvas = null;
var context = null;

function main()
{
    canvas = document.getElementById("AMC");

    if (!canvas)
        console.log("Obtaining Canvas Failed\n");
    else
        console.log("Obtaining canvas Succeeded\n");

    //print canvas width and height
    console.log("canvas width:" + canvas.width + "and canvas Height : " + canvas.height);

    //get 2D Context
    context = canvas.getContext("2d");
    if (!context)
        console.log("Obtaining 2D Context Failed\n");
    else
        console.log("Obtaining 2D Context Succeeded\n");

    //fill canvas
    context.fillStyle = "black";
    context.fillRect(0, 0, canvas.width, canvas.height);

    //draw text
    drawText("Hello World!!");

    //register KeyBoards keyDown event handler
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);


}



function drawText(text)
{
    context.textAlign = "center";
    context.textBaseline = "middle";

    context.font = "48px sans-serif";

    context.fillStyle = "white";

    context.fillText(text, canvas.width / 2, canvas.height / 2);
  }





function toggleFullScreen()
{

    var fullscreen_element =
     document.fullscreenElement ||
     document.webkitFullscreenElement ||
     document.mozFullScreeenElement ||
     document.msFullScreenElement ||
     null;


    if(fullscreen_element==null)
    {
        if (canvas.requestFullscreen)
            canvas.requestFullscreen();
        else if (canvas.mozRequestFullScreen)
            canvas.mozRequestFullScreen();
        else if (canvas.webkitRequestFullscreen)
            canvas.webkitRequestFullscreen();
        else if (canvas.msReqestFullscreen)
            canvas.msReqestFullscreen();
    }
    else 
    {
        if (document.exitFullscreen)
            document.exitFullscreen();
        else if (document.mozCancelFullScreen)
            document.mozCancelFullScreen();
        else if (document.webkitExitFullscreen)
            document.webkitExitFullscreen();
        else if (document.msExitFullscreen)
            document.msExitFullscreen();
    }

}






function keyDown(event)
{
    switch(event.keyCode)
    {
        case 70:
            toggleFullScreen();

            //repaint

            drawText("Hello World!!");
            break;
    }
}



function mouseDown()
{
    alert("Mouse is clicked");
}









