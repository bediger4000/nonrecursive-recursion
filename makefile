all: cpup4 cpup5

cpup4: cpup4.c
	c99 -z execstack -g -Wall -Wextra -o cpup4 cpup4.c

cpup5: cpup5.c
	clang -z execstack -g -Wall -Wextra -o cpup5 cpup5.c
