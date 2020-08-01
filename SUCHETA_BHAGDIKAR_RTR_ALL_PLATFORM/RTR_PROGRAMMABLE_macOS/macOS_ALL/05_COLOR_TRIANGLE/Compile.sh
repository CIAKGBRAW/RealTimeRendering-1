mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP color_Triangle.mm -framework Cocoa -framework QuartzCore -framework OpenGL
