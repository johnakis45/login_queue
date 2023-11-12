all : prod_cons.c 
	gcc -pthread prod_cons.c -o prod-cons.out 

run : prod-cons.out
	./prod-cons.out

clean:
	rm -f prod-cons.out	