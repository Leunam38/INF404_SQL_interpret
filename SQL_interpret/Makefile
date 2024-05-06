CC = clang -g

all: test_lexeme interpreteur

%.o: %.c
	$(CC) -c $<

%: %.o
	$(CC) $^ -o $@

table_affectation.o: type_table_donnes.h

ast_construction.o: type_ast.h

lecture_caracteres.o: lecture_caracteres.h

analyse_lexicale.o: lecture_caracteres.h

test_lexeme.o: analyse_lexicale.h

table_donnes.o: table_affectation.h ast_construction.h 


interpreteur.o: analyse_lexicale.h analyse_syntaxique.h  table_affectation.h



analyse_syntaxique.o: analyse_lexicale.h table_affectation.h table_donnes.h ast_construction.h ast_parcours.h requete_SQL.h

requete_SQL.o: analyse_lexicale.h table_affectation.h table_donnes.h ast_construction.h type_table_donnes.h ast_parcours.h


ast_parcours.o: type_ast.h table_affectation.h analyse_lexicale.h table_donnes.h type_table_donnes.h

test_lexeme: analyse_lexicale.o lecture_caracteres.o

interpreteur: analyse_lexicale.o requete_SQL.o analyse_syntaxique.o table_affectation.o lecture_caracteres.o table_donnes.o ast_construction.o ast_parcours.o





clean:
	rm -f sql_main test_lexeme test_syntaxique *.o
