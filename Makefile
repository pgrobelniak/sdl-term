all: v
WARN=-Wall -Wno-unused-parameter -Wno-parentheses -Wno-unused-result
DEPS=`sdl2-config --libs --cflags` -lm
v: main.c scancodes.h vt52rom.h
	cc $(WARN) -O3 -o v main.c $(DEPS)
