
gcc_option = -std=c++17 -Wall --pedantic-errors

program : main.cpp all.h all.h.gch
	g++ $(gcc_option) -include all.h $< -o $@


all.h.gch : all.h
	g++ $(gcc_option) -x c++-header -o $@ $<


run : program
	./program

clean :
	rm -f ./program
	rm -f ./all.h.gch	


.PHONY : run clean

