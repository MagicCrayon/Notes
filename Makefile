all:
	    gcc -Wall -ggdb main.c -o main
clean:
	rm main
cleanall:
	rm main
	rm .notes.dat
release:
	gcc main.c -o main
