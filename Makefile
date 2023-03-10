build:main
main:main.c
	gcc -Wall -Wextra main.c ll.c ht.c -o main -lm -g -std=c99 -std=gnu99
clean:
	rm main
