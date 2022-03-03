#Baselines
baseline-build: ./src/serial.c
	gcc ./src/serial.c -lm -o ./bin/serial.out

baseline-run: ./bin/serial.out
	cat tests/test1 | ./bin/serial.out

baseline-build-run: baseline-build baseline-run

baseline-test: ./bin/serial.out
	cat tests/test1 | ./bin/serial.out test1
	cat tests/K02-01-TC1 | ./bin/serial.out K02-01-TC1
	cat tests/K02-01-TC2 | ./bin/serial.out K02-01-TC2
	cat tests/K02-01-TC3 | ./bin/serial.out K02-01-TC3
	cat tests/K02-01-TC4 | ./bin/serial.out K02-01-TC4

#Actual Programs
build: ./src/main.c
	mpicc -o ./bin/main.out ./src/matrix.c ./src/main.c --openmp

run: ./bin/main.out
	cat tests/test1 | ./bin/main.out test1
	cat tests/K02-01-TC1 | ./bin/main.out K02-01-TC1
	cat tests/K02-01-TC2 | ./bin/main.out K02-01-TC2
	cat tests/K02-01-TC3 | ./bin/main.out K02-01-TC3
	cat tests/K02-01-TC4 | ./bin/main.out K02-01-TC4

build-run: build run


#Tests
test-build: ./src/test.c
	mpicc -o ./bin/test.out ./src/test.c --openmp

test-run: ./bin/test.out
	mpiexec -n 6 ./bin/test.out

test: test-build test-run

clean:  
	rm src/*.out
	rm bin/*.out
