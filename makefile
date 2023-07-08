UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
main:
	gcc src/*.c -o rich

clean:
	rm rich
else
main.exe:
	mingw64/bin/gcc src/*c -o rich.exe -I src/regex/include/ -L src/regex/lib/ -l regex -static -m32

clean:
	rm rich.exe
endif
