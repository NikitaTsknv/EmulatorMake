PROJECT = emulator

SOURCES = main.cpp

$(PROJECT): $(SOURCES)

all: emulator

emulator:
	g++ main.cpp -W -Wall -Werror -o emulator

clean:
	rm -rf emulator
