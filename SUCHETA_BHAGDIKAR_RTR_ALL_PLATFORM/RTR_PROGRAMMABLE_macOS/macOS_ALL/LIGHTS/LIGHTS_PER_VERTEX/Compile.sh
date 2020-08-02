mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP lightsPerVertex.mm -framework Cocoa -framework QuartzCore -framework OpenGL
