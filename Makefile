VERSION = 0.0.8
BUILD = 0098
all:
	# Clean up the directories
	# rm -R bin/ obj/
	# If they do not exist, it creates them
	# TODO: Use a if
	mkdir bin/ obj/
	gcc ./src/lib/libutil.h -o ./obj/libutil.o
	gcc ./src/mucro.c -o ./bin/mucro

install: all
	sudo cp ./bin/mucro /usr/bin

uninstall: all
	rm -R ./bin
	rm -R ./obj
	sudo rm /usr/bin/mucro
