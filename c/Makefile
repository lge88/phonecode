CC=clang

translate: translate.c translator.h
	${CC} translate.c -O3 -o translate

t_trie: t_trie.c translator.h
	${CC} t_trie.c -g -o t_trie

.PHONY: clean

clean:
	rm -f translate t_trie
