VERSION = 0.0.8
BUILD = 0096
all:
	rm -R bin/ obj/
	mkdir bin/ obj/
	gcc libutil.h -o ./obj/libutil.o
	gcc mucro.c -o ./bin/mucro

install: all
	sudo cp ./bin/mucro /usr/bin

uninstall: all
	rm -R ./bin
	rm -R ./obj
	sudo rm /usr/bin/mucro
