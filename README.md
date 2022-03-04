# SOCK-C
a simple homework about sockets in c. it contains a server and two clients 
###### manuel d'utilisation :
le tp comporte 3 ficher [server.c](./server.c) [client1.c](./client1.c) [client2.c](./client2.c) 
1. tout d'abord il faut compiler les 3 program en utilisant la cmd [make](./MakeFile) ou bien [make compile](./MakeFile)
2. après la compilation on aura 3 executable [server](./server) [client1](./client1) [client2](./client2)
3. on met l'executable dans lulu et on lance ce dernier on specifiant le numero du PORT comme argument > exp : ./server 8000 maintenant le serveur attend une connection sur la port 8000 du serveur lulu
4. les executables des clients doivent s'executer sur une machine dans le meme sou-reseau que lulu et qui peut l'accéder 
5. les clients doivent se connecter au meme port où le serveur tourne > exp: ./client2 8000 ./client1 8000


###### manuel d'utilisation client1 :
lors de l'execution du client1 et pendant 5 iterations on doit fournir un psudo ne (depassant pas 10 caractere)  pour la connection et un entier    


###### manuel d'utilisation client2 :
lors de l'execution du client2 on doit fournir un psudo (ne depassant pas 10 caractere) pour la connection  



###### NOM et PRENOM 
TOUAZI AIMEN RYAD 22113063
