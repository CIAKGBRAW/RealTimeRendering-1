mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP 24lightsPerFragment.mm -framework Cocoa -framework QuartzCore -framework OpenGL
