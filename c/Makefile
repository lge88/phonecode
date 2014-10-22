CC=clang

all: translate t_trie t_wl t_dict

translate: translate.c translator.h
	${CC} translate.c -O3 -o translate

t_trie: t_trie.c translator.h
	${CC} t_trie.c -g -o t_trie

t_wl: t_wl.c translator.h
	${CC} t_wl.c -g -o t_wl

t_dict: t_dict.c translator.h
	${CC} t_dict.c -g -o t_dict

.PHONY: clean

clean:
	rm -f translate t_trie t_wl
