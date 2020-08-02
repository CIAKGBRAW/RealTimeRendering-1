var canvas = null;
var context = null;

function main()
{
    //get canvas element
    canvas = document.getElementById("AMC");
    if (!canvas)
        console.log("Obtaining canvas failed\n");
    else
        console.log("Obtaining canvas succeeded\n");

    console.log("Canvas width : " + canvas.width + "And Canvas Height: " + canvas.height);

    //get 2d context
    context = canvas.getContext("2d");
    if (!context)
        console.log("Obtaining 2D Context failed\n");
    else
        console.log("Obtaining 2D Context succeeded");

    //fill canvas with black color
    context.fillStyle = "black";
    context.fillRect(0, 0, canvas.width, canvas.height);

    //center the text
    context.textAlign = "center";
    context.textBaseLINE = "middle";

    //text
    var str = "Hello World!!";

    //text font
    context.font = "48px sans-serif";

    //text color
    context.fillStyle = "white";
    
    //display the text in center
    context.fillText(str, canvas.width / 2, canvas.height / 2);

    //register keyboards keydown event handler
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);

}


function keyDown(event)
{
    alert("A key is Pressed");

}












