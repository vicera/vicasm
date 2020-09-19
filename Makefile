# Temporary Makefile

vicasm: parser.cc main.cc
	g++ parser.cc main.cc -Wall -Wextra -pedantic -o vicasm
