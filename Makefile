.PHONY: debug
debug: main.cpp sub1.cpp
	clang++ -std=c++14 -O3 -g main.cpp
	clang++ -std=c++14 -O3 -g sub1.cpp
	clang++ -o main main.o sub1.o
