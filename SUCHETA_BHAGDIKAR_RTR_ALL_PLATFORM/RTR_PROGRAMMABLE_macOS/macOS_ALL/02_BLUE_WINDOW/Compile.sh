mkdir -p bluewindow1.app/Contents/MacOS

clang++ -o bluewindow1.app/Contents/MacOS/OGLPP BlueWindow.mm -framework Cocoa -framework QuartzCore -framework OpenGL
