#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

typedef int (*fxptr)(
		int, int, int (*)(const char *, ...),
		void *,
		void *(*)(void *, const void *, size_t),
		void *(*)(size_t),
		void (*)(void *),
		size_t
);

char *signal_string(int sig);
void  signal_handler(int sig);
int   main(int ac, char **av);
int copyup(
		int i,
		int j,
		int (*xptr)(const char *, ...),
		void *yptr,
		void *(*bptr)(void *, const void *, size_t),
		void *(*mptr)(size_t),
		void (*ffptr)(void *),
		size_t	size
);
void f2(void);

/* return a string for the most common signals this program
 * will generate.  Probably could replace this with strerror()
 */
char *
signal_string(sig)
		int sig;
{
		char *bpr = "Don't know what signal";

		switch (sig)
		{
		case SIGILL:
				bpr = "Illegal instruction";
				break;
		case SIGSEGV:
				bpr = "Segmentation violation";
				break;
		case SIGBUS:
				bpr = "Bus error";
				break;
		}

		return bpr;
}

/* Use of fprintf() seems sketchy.  I think that signal_handler() doesn't
 * need special compiler treatment like generating Position Independent
 * Code.  It stays in one place, and the kernel knows that place.
 */
void
signal_handler(int sig)
{
		(void)fprintf(
				stderr,
				"%s: sig = 0x%x\n",
				signal_string(sig),
				sig
		);

		exit(99);
}

int
main(int ac, char **av)
{
		int i, j;

		/* check to see if cmd line has a number on it */
		if (ac < 2) {
				printf("not enough arguments\n");
				exit(99);
		}
		/* install 3 different signal handlers - avoid core dumps */
		if (-1 == (i = (long)signal(SIGSEGV, signal_handler)))
		{
				perror("Installing SIGSEGV signal failed");
				exit(33);
		}
		if (-1 == (i = (long)signal(SIGILL, signal_handler)))
		{
				perror("Installing SIGILL signal handler failed");
				exit(33);
		}
		if (-1 == (i = (long)signal(SIGBUS, signal_handler)))
		{
				perror("Installing SIGBUS signal handler failed");
				exit(33);
		}

		setbuf(stdout, NULL);

		/*
		 * print out addresses of original functions so there is something to
		 * reference during recursive function copying and calling 
		 */
		printf(
	   "main = %p, copyup %p, memcpy %p, malloc %p, printf %p, free %p, size %ld\n",
	   main, copyup, memcpy, malloc, printf, free, (size_t)f2 - (size_t)copyup);

		if ((i = atoi(*(av + 1))) < 1) {
				printf(" i = %d, i must be > 1\n", i);
				exit(99);
		}

		printf(" going for %d recursions\n", i);

		j = copyup(1, i, printf, copyup, memcpy, malloc, free, (size_t)f2 - (size_t)copyup);

		printf("copyup at %p returned %d\n", copyup, j);


		return 1;
}

int
copyup(
		int i, int j,
		int   (*xptr)(const char *, ...),
		void *yptr,
		void *(*bptr)(void *, const void*, size_t),
		void *(*mptr)(size_t),
		void  (*ffptr)(void *),
		size_t	 size
)
{
		fxptr fptr;
		int k;

		if (i == j)
		{
				(*xptr)("function at %p got to %d'th copy\n", yptr, i);
				return i;
		} else
				(*xptr)("function at %p, i = %d, j = %d\n", yptr, i, j);

		if (!(fptr = (fxptr)(*mptr)(size)))
		{
				(*xptr)("ran out of memory allocating new function\n");
				return -1;
		}

		(*bptr)(fptr, yptr, size);

		k = (*fptr)(i + 1, j, xptr, (void *)fptr, bptr, mptr, ffptr, size);

		(*xptr)("function at %p got %d back from function at %p\n",
				yptr, k, fptr);

		(*ffptr)(fptr);

		return (k + 1);
}

void f2(void) {return;}
