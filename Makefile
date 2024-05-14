CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11 -s -O2
TFLAGS = -Wall -Wextra -pedantic -std=c++11 -g -O2

SRCS = parser.cc lexer.cc inputbuf.cc

parser:
	$(CC) $(CFLAGS) -c $(SRCS)

debug:
	$(CC) $(TFLAGS) -c $(SRCS)

test:
	$(CC) $(TFLAGS) $(SRCS) -o parser_lex_test

clean:
	$(RM) *.o
	$(RM) parser_lex_test

run_test:
	./parser_lex_test