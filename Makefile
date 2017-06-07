all: Client

Client: main.o
	g++ main.o -o Client

main.o: main.cpp
	g++ -c main.cpp

.PHONY : clean
clean:
	rm -rf *.o Client