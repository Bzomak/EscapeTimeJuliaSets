all:
	gcc -o EscapeTime EscapeTime.c complex.c -Wall -g -lglut -lGLU

mac:
	gcc -o EscapeTime EscapeTime.c complex.c -Wall -g -framework OpenGL -framework GLUT

clean:
	rm EscapeTime
