CFLAGS = -g -Wall -std=c++11

all:
	g++ $(CFLAGS) main.cpp -o main
	./main < ../chc/test\ 0
test:
	g++ $(CFLAGS) main.cpp -o main
	./main < ../chc/test\ 0
	./main < ../chc/test\ 1
clean:
	rm -rf ./main
