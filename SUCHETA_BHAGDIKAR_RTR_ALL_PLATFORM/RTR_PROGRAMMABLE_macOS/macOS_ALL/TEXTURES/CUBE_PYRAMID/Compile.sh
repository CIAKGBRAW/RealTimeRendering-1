mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP 3DCubePyramid.mm -framework Cocoa -framework QuartzCore -framework OpenGL
