mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP lightsBlinn.mm -framework Cocoa -framework QuartzCore -framework OpenGL
