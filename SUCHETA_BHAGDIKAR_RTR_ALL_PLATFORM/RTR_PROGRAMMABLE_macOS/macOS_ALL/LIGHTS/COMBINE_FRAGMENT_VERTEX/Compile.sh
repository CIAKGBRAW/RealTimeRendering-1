mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP1.app/Contents/MacOS/OGLPP lightsPerVertex1.mm -framework Cocoa -framework QuartzCore -framework OpenGL
