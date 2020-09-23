FILES = parser.cc assembler.cc main.cc
FLAGS = -Wall -Wextra -pedantic
EXENAME = vicasm
CC = g++

$(EXENAME): $(FILES)
	$(CC) $(FILES) $(FLAGS) -o $(EXENAME)
