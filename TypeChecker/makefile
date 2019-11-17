EXEC = run
CC = g++
CFLAGS = -c -Wall

$(EXEC) : Parser.o lexer.o inputbuf.o

	$(CC) -o $(EXEC) Parser.o lexer.o inputbuf.o

Parser.o: Parser.h Parser.cpp
	$(CC) $(CFLAGS) Parser.cpp

lexer.o: lexer.h lexer.cc
	$(CC) $(CFLAGS) lexer.cc

inputbuf.o: inputbuf.h inputbuf.cc
	$(CC) $(CFLAGS) inputbuf.cc


clean :
	rm *.o


