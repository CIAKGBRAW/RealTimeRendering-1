//onload function
function main()
{

    var canvas = document.getElementById("AMC");
    if (!canvas)
        console.log("Obtaining  Cnvas Failed!");
    else
        console.log("Obtaining Canvas Succeded");

    //print canvas width and height
    console.log("Canvas Width:  " + canvas.width + " And Canvas Height:" + canvas.height);

    //get 2D Context
    var context = canvas.getContext("2d");

    if (!context)
        console.log("Obtaining 2D Context Failed\n");
    else
        console.log("Obtaining 2D Context succeded\n");

    //fill canvas with black color
    context.fillStyle = "black";
    context.fillRect(0, 0, canvas.width, canvas.height);

    context.textAlign = "center";
    context.textBaseline = "middle";

    var str = "Hello World!";

    context.font = "48px sans-serif";

    context.fillStyle = "white";

    context.fillText(str, canvas.width / 2, canvas.height / 2);
}