FLAGS = 
LIBS = -lglfw -lGLU -lGL -lX11 -lfreetype -lXrandr -lpthread -lXi -ldl

glad = ./include/glad/glad.c
gladh = ./include/glad/glad.h
game = ./Game/Game.cpp
gameh = ./Game/Game.cpp
win = ./Window/Window.cpp
winh = ./Window/Window.hpp
intsh = ./Shape/ComplexShape2D.hpp
square = ./Shape/Square.cpp
squareh = ./Shape/Square.hpp
shape = ./Shape/Shape.cpp
shapeh = ./Shape/Shape.hpp
scene = ./Scene/Scene.cpp
sceneh = ./Scene/Scene.hpp
tscene = ./Scene/TextScene.cpp
tsceneh = ./Scene/TextScene.hpp
shader = ./Shader/Shader.cpp
shaderh = ./Shader/Shader.hpp
herm = ./Math/Hermite.cpp
hermh = ./Math/Hermite.hpp
curve = ./Shape/Curve.cpp
curveh = ./Shape/Curve.hpp
text = ./Text/SimpleText.cpp
texth = ./Text/SimpleText.hpp
actionh = ./Shape/Action.hpp
bullet = ./Shape/Bullet.cpp
bulleth = ./Shape/Bullet.hpp
libh = Lib.hpp
util = ./Utils/utils.cpp
utilh = ./Utils/utils.hpp

all: main.exe

main.exe: main.o window.o shape.o shader.o scene.o tscene.o helper.o game.o square.o curve.o bullet.o text.o glad.o
	g++ ${FLAGS} -o main.exe main.o window.o square.o helper.o game.o shape.o glad.o curve.o shader.o text.o bullet.o tscene.o scene.o ${LIBS}

main.o: main.cpp ${gladh} ${libh} ${intsh} ${gameh}
	gcc -c ${FLAGS} main.cpp -o main.o

text.o: ${text} ${texth} ${libh} ${shaderh}
	gcc -c ${FLAGS} ${text} -o text.o

window.o: ${win} ${winh} ${libh} ${gladh}
	gcc -c ${FLAGS} ${win} -o window.o

shape.o: ${shape} ${shapeh} ${libh} ${gladh} ${intsh} ${shaderh} ${action}
	gcc -c ${FLAGS} ${shape} -o shape.o

game.o: ${game} ${gameh} ${action}
	gcc -c ${FLAGS} ${game} -o game.o

shader.o: ${shader} ${shaderh} ${shapeh} ${gladh}
	gcc -c ${FLAGS} ${shader} -o shader.o

scene.o: ${scene} ${sceneh} ${libh} ${gladh}
	gcc -c ${FLAGS} ${scene} -o scene.o

bullet.o: ${bullet} ${bulleth} ${intsh}
	gcc -c ${FLAGS} ${bullet} -o bullet.o

tscene.o: ${tscene} ${tsceneh} ${libh}
	gcc -c ${FLAGS} ${tscene} -o tscene.o

square.o: ${square} ${squareh} ${libh} ${gladh} ${intsh} ${shaderh} ${action}
	gcc -c ${FLAGS} ${square} -o square.o

curve.o: ${curve} ${curveh} ${libh} ${gladh} ${intsh} ${shaderh} ${action}
	gcc -c ${FLAGS} ${curve} -o curve.o

helper.o: ${util} ${utilh}
	gcc -c ${FLAGS} ${util} -o helper.o

glad.o: ${glad} ${gladh}
	gcc -c ${FLAGS} ${glad} -o glad.o

.PHONY:	clean

clean:
	-rm -f *.o *.exe

