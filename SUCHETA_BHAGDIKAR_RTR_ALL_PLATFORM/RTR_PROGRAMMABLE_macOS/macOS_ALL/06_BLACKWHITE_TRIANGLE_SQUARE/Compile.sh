mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP blackWhiteTriangleSquare.mm -framework Cocoa -framework QuartzCore -framework OpenGL
