SRC=src/main.c src/auth.c src/auth.h src/definitions.h src/operations.c src/operations.h src/utils.c src/utils.h src/files.c src/files.h

employee: src/main.c
	@gcc -o employee ${SRC} -lpam -lpam_misc -Wall

clean:
	@rm employee