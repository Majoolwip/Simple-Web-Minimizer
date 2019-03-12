CC=gcc
name=web_minimizer
name_test=web_minimizer_test
name_debug=web_minimizer_debug
src=src/main.c src/web_minimizer.c
test_src=test/main.c src/web_minimizer.c
headers=headers/

.PHONY:test

test:
	$(CC) -g $(test_src) -o $(name_test) -I $(headers)

.PHONY: debug
debug:
	$(CC) -g $(src) -o $(name_debug) -I $(headers)

.PHONY: production

production:
	$(CC) $(src) -O3 -o $(name) -I $(headers)
