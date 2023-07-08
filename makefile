UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
main:
	gcc src/*.c -o main

clean:
	rm main
else
main.exe:
	mingw64/bin/gcc src/*c -o main -I src/regex/include/ -L src/regex/lib/ -l regex -static -m32

clean:
	rm main
endif
