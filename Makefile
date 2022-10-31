CFLAGS += -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE
CC = g++

all: list.exe


list.exe: main.o listdy.o listdydump.o
	$(CC) -o list.exe main.o listdy.o listdydump.o $(CFLAGS)

main.o: main.cpp
	$(CC) -o main.o main.cpp -c $(CFLAGS)

listdy.o: listdy.cpp 
	$(CC) -o listdy.o listdy.cpp -c $(CFLAGS)

listdydump.o: listdydump.cpp 
	$(CC) -o listdydump.o listdydump.cpp -c $(CFLAGS)

clean:
	rm *.o
	clear
	
.PHONY: clean