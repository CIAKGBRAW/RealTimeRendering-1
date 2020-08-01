mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP lightsPerVertex2.mm -framework Cocoa -framework QuartzCore -framework OpenGL
