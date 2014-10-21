CC=clang

translate: translate.c trie.h
	${CC} translate.c -g -o translate

t_trie: t_trie.c trie.h
	${CC} t_trie.c -g -o t_trie

.PHONY: clean

clean:
	rm -f translate t_trie
