CC = clang -g

all: sql_main

%.o: %.c
	$(CC) -c $<

%: %.o
	$(CC) $^ -o $@

table_donnes.o: type_table_donnes.h

sql_main.o: table_donnes.h

sql_main: table_donnes.o 

clean:
	rm -f sql_main *.o
