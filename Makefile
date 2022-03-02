compile: client1.c client2.c server.c
	gcc client1.c -o client1
	gcc client2.c -o client2
	gcc -pthread server.c -o server

deploy: client1 client2 server
	scp client1 lucy:
	scp client2 lucy:
	scp server lulu: