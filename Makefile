all:
	@if [ ! -e ./bin/ ]; then mkdir ./bin/; fi
	@gcc ./src/mucro.c -o ./bin/mucro

install: all
	@chmod a+x ./bin/mucro
	@# Copy for bash
	@sudo cp ./bin/mucro /usr/local/bin/
	@# Copy for sh
	@sudo cp ./bin/mucro /usr/bin/

uninstall:
	@if [ -e /usr/local/bin/mucro ]; then sudo rm /usr/local/bin/mucro; fi
	@if [ -e /usr/bin/mucro ]; then sudo rm /usr/bin/mucro; fi
clear:
	@if [ -e ./bin ] && [ -x ./bin/mucro ]; then rm -r bin/ ; fi
clearsrc:
	@echo "WARNING: You need the sources to compile again. Download/git pull sources to get them back."
	@if [ -e ./src/ ]; then rm -r src; fi
