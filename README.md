# Kart

## Description
This is a car game written in c++, using OpenGL library version 3.3.
In this game you can move your car up, down, left and right direction and you must dodge bullets coming to you.
You can also shoot a projectile that destroy the first bullet it collide.
Your goal is to reach the yellow star on the right.
Everytime you reach the goal you gain an ammo and increases the level difficulty by adding another bullet to dodge.
When you hit a bullet the game end; to leave the game you need to press `ESC`.

## Game Instruction
 - `W` => move UP
 - `S` => move DOWN
 - `A` => move LEFT
 - `D` => move RIGHT
 - `space` => shoot a projectile (if you have enough ammmo)
 - `ESC` => if you press thi key the game will close in every state the game is

## Run Instructions
### Windows

If you run this program from Windows you must run it from VisualStudio and opent the solution file with `.sln` extension.
After you opened the project you can press the Start button on the editor.

### Linux

If you use linux you must have installed the following libraries:
 - `freetype2` library in the `/usr/include/` folder (not in !freetype folder!)
 - `glm` library
 - `make` command to compile the project
 - `gcc` installed
These library can easly be installed using a normal package manager (apt, pacman, etc.)

Next you need to go in the CG-Project1 folder and you should see a MakeFile, and you can easily run the following code:
```
make
```
If you want to apply some modification you can also use this command to remove other compiled files and recompile your modification (`-j 6` means make uses 6 threads, in my laptop was the optimal value):
```
make clean ; make -j 6
```
Finally you can run the application using:
```
./main.exe
```
