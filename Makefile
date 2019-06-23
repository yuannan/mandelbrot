# Specify what typing 'make' on its own will compile
default: fractal

# For Linux/MacOS, include the advanced debugging options

fractal: display.c fractal.c
	clang $^ -o $@ -std=c11 -Wall -pedantic -g -lSDL2\
	    -fsanitize=undefined -fsanitize=address
test: fractal.c display.c
	clang -Dtest -std=c11 -Wall -pedantic -g -lSDL2 $^ -o $@ \
	    -fsanitize=undefined -fsanitize=address
