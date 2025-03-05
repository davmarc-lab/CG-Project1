# Isaac Remake

## Description
Isaac Remake is a simple game written in C++ using OpenGL library.

In this game you can move a character "Isaac" and the goal is to survive.
You can shoot at enemies, but you'll need more then you hit to kill them.

## Instruction
 - `W` => move UP
 - `S` => move DOWN
 - `A` => move LEFT
 - `D` => move RIGHT
 - `UP_ARROW` => shoot a projectile to the top
 - `LEFT_ARROW` => shoot a projectile to the left
 - `DOWN_ARROW` => shoot a projectile to the bottom
 - `RIGHT_ARROW` => shoot a projectile to the right
 - `ESC` => if you press thi key the game will close in every state the game is

## How to Run
### Windows

### Linux
If you use linux you must have installed the following libraries:
 - `freetype2` library in the `/usr/include/` folder (not in !freetype folder!)
 - `glm` library
 - `glfw` library
 - `premake5` command to generate the make files
 - `make` command to compile the project
 - `gcc` installed
These library can easly be installed using a normal package manager (apt, pacman, etc.)

You can easily run the following code to generate the Makefiles:
```
premake5 gmake
```

Next you need to compile the project using:
```
make
```
or
```
make -j [NUM_THREAD]
```

The executable is located in: `bin/linux-x86-64/Debug/App/`
