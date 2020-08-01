mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP staticSmiley.mm -framework Cocoa -framework QuartzCore -framework OpenGL
