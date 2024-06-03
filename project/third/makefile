CFLAGS = -g -Wall -std=c++11

all:
	g++ $(CFLAGS) main.cpp -o main
	./main < ../chc/test\ 0
test:
	g++ $(CFLAGS) main.cpp -o main
	./main < ../chc/test\ 0
	./main < ../chc/test\ 1
	./main < ../chc/test\ 2
	./main < ../chc/test\ 3
	./main < ../chc/test\ 4
	./main < ../chc/test\ 5
clean:
	rm -rf ./main
