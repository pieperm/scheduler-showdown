
FLAGS = -g -std=c++17
LIBRARIES = -lpthread

.PHONY: default run

default: run

run:
	g++ ${FLAGS} *.cpp ${LIBRARIES} -o program

clean:
	-@rm -rf *.o program core