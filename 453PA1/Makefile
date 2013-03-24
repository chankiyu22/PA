all: main.o sc.o p2p.o
	g++ -o PA1 main.o sc.o p2p.o -lpthread

sc.o: sc.cpp sc.h Connect.h
	g++ -c sc.cpp

p2p.o: p2p.cpp p2p.h Connect.h
	g++ -c p2p.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm sc.o p2p.o main.o
