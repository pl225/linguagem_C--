all: 	
		clear
		lex lexica.l
		yacc -d sintatica.y
		g++ -std=c++0x -o glf y.tab.c -lfl

		./glf < exemplo.foca
