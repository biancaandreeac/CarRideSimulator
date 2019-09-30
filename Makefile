build:  main.cpp
	g++ --std=c++11 -Wall -Wextra *.cpp -o tema2

.PHONY: clean

run:
	./main

clean:
	rm out/*/*
	rm tema2
	rm time.out