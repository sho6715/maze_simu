
CPPOPTIONS="-Wno-deprecated"
HEADERS="common.h map_cmd.h Maze.h search.h typedefine.h"

all: maze.exe

clean:
	rm *.o *.exe

maze.exe: map_cmd.o Maze.o Mazesimu_VS.o search.o
	g++ -o $@ $^

.cpp.o: ${HEADERS}
	g++ ${CPPOPTIONS} -c $<


.SUFFIXES:	.h .cpp .o .exe
