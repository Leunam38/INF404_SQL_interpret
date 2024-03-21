CC = clang -g

all: sql_main test_lexeme test_syntaxique

%.o: %.c
	$(CC) -c $<

%: %.o
	$(CC) $^ -o $@

table_donnes.o: type_table_donnes.h

test_lexeme.o: analyse_lexicale.h

test_syntaxique.o: analyse_lexicale.h analyse_syntaxique.h

analyse_lexicale.o: lecture_caracteres.h

lecture_caracteres.o: lecture_caracteres.h

analyse_syntaxique.o: analyse_lexicale.h

sql_main.o: table_donnes.h

test_lexeme: analyse_lexicale.o lecture_caracteres.o

sql_main: table_donnes.o 

test_syntaxique: analyse_lexicale.o analyse_syntaxique.o lecture_caracteres.o





clean:
	rm -f sql_main test_lexeme test_syntaxique *.o
