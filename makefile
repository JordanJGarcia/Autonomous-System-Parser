proj_1: main.o sysData.o
	g++ -o proj_1 main.o sysData.o

main.o: main.cpp sysData.h
	g++ -c -std=c++11 main.cpp

sysData.o: sysData.cpp sysData.h
	g++ -c -std=c++11 sysData.cpp
clean:
	rm *.o proj_1 results
