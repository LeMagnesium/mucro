VERSION = 0.0.9
all:
	# Clean up the directories
	# rm -R bin/ obj/
	# If they do not exist, it creates them
	# TODO: Use a if
	# mkdir bin/ obj/
	gcc ./src/libutil.h -o ./libutil.o
	gcc ./src/mucro.c -o ./mucro

install: all
	chmod a+x ./mucro
	sudo cp ./mucro /usr/local/bin

uninstall: all
	sudo rm /usr/bin/mucro
clear:
	rm -f ./libutil.o
	rm -f ./mucro
clearsrc:
	echo "WARNING: You need the sources to compile again. Download/git pull sources to get them back."
	rm -f libutil.*
	rm -rf src
scratch:
	rm -f ./mucro
	rm -f ./libutil.o
