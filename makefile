all: cli s

serv:
	gcc -o serv s.c

cli:
	gcc -o cli client.c

clean:
	rm -f serv
	rm -f cli

