
.PHONY: run clean


gcc_option = -std=c++17 -Wall --pedantic-errors

program: main.cpp
	g++ $(gcc_option) -o $@ $<


run: program
	./program


clean: 
	rm -f program
