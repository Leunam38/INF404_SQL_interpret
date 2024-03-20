CC = clang -g

all: sql_main test_lexeme

%.o: %.c
	$(CC) -c $<

%: %.o
	$(CC) $^ -o $@

table_donnes.o: type_table_donnes.h

test_lexeme.o: analyse_lexicale.h

analyse_lexicale.o: lecture_caracteres.h

lecture_caracteres.o: lecture_caracteres.h

sql_main.o: table_donnes.h

test_lexeme: analyse_lexicale.o lecture_caracteres.o

sql_main: table_donnes.o 





clean:
	rm -f sql_main test_lexeme *.o
