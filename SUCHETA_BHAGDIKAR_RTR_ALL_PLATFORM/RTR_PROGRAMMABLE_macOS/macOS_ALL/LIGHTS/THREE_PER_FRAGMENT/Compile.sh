mkdir -p OGLPP.app/Contents/MacOS

clang++ -o OGLPP.app/Contents/MacOS/OGLPP threeLightsPerFragment.mm -framework Cocoa -framework QuartzCore -framework OpenGL
