# sample Makefile
RES = main.cpp
EXE = myprogam
all:
	g++ $(RES) -O3 -o $(EXE)
clean:
	rm $(EXE)
