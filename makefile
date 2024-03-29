#Baselines
baseline-build: ./baseline/serial.c
	gcc ./baseline/serial.c -lm -o ./bin/serial.out

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
build: ./src/main.c ./src/matrix.c ./src/utils.c ./src/helper.c
	mpicc -o ./bin/main.out ./src/matrix.c ./src/utils.c ./src/helper.c ./src/main.c --openmp

run: ./bin/main.out
	cat tests/test1 | mpirun -n 4 ./bin/main.out test1
	cat tests/K02-01-TC1 | mpirun -n 4 ./bin/main.out K02-01-TC1
	cat tests/K02-01-TC2 | mpirun -n 4 ./bin/main.out K02-01-TC2
	cat tests/K02-01-TC3 | mpirun -n 4 ./bin/main.out K02-01-TC3
	cat tests/K02-01-TC4 | mpirun -n 4 ./bin/main.out K02-01-TC4

build-run: build run


#Tests
test-build: ./src/checker.c
	gcc ./src/checker.c -o ./bin/checker.out

test-run: ./bin/checker.out
	./bin/checker.out

check: test-build test-run

clean:  
	rm src/*.out & rm bin/*.out
