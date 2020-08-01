mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP colorTriangleSquare.mm -framework Cocoa -framework QuartzCore -framework OpenGL
