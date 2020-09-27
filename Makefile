FILES = parser.cc assembler.cc main.cc logging.cc
HEADERS = parser.h assembler.h logging.h
FLAGS = -Wall -Wextra -pedantic
EXENAME = vicasm
CC = g++

$(EXENAME): $(FILES) $(HEADERS)
	$(CC) $(FILES) $(FLAGS) -o $(EXENAME)
