UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
main:
	gcc src/*.c -o bin/rich

clean:
	rm bin/rich
else
main.exe:
	mingw64/bin/gcc src/*c -o bin/rich.exe -I src/regex/include/ -L src/regex/lib/ -l regex -static -m32

clean:
	rm bin/rich.exe
endif
