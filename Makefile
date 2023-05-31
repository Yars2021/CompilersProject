compiler:
	flex -o compiler.lex.c compiler.l
	bison -o compiler.tab.c -vd compiler.y
	cc -o compiler ast.c compiler.lex.c compiler.tab.c -lm -ll -O3
clean:
	rm -f compiler.lex.c compiler.tab.* compiler.output compiler
