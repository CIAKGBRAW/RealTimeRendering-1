mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP ortho_Triangle.mm -framework Cocoa -framework QuartzCore -framework OpenGL
