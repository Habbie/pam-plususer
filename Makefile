all: pam_plususer.so

install: pam_plususer.so
	sudo install 

pam_plususer.o: pam_plususer.c
	gcc -fPIC -g -c pam_plususer.c

pam_plususer.so: pam_plususer.o
	gcc -shared -o pam_plususer.so pam_plususer.o -lpam