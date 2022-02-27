build: serial.c
	gcc serial.c -lm -o serial

run:
	cat tests/test1 | ./serial

build-run: build run