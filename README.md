# Self-relocating C function

This curiosity, written in reasonably strict C, contains a
function which relocates itself in memory. It looks like it
recurses, but it actually doesn't: each call is to a new copy
of the original function.

It won't work on x86_64 Linux unless you do a special compilation
(with GNU C):

    c99 -z execstack -g -Wall -Wextra -o cpup4 cpup4.c

By default, GNU C compiler get the hardware to mark the heap as non-executable.
And yes, despite its weird appearance, `cpup4.c` is valid C99.

The `cpup5.c` variant uses `alloca()` instead of malloc, and it seems
to need `clang` instead of `gcc` to compile. Once you get it to compile,
it still works.

## Historical Note

This code started life under SunOS 4.0.3 on a M68020 based Sun
3/260, about 1990. Since then, I have compiled and run it
under quite a few Unix variants (at least SunOS, Solaris, Ultrix,
NeXTStep, Irix, NetBSD, Linux). It works on hardware ranging from
an M68010, SPARC, Mips R2000, R3000, R4000, and many others. It
probably won't work on M68040 or Alpha CPUs without in-line
assembly code to flush those CPU's instruction caches.

It will probably never work on Hewlett-Packard's ghastly HP-PA CPU
because of that architecture's poorly-documented segmentation. But
HP-PA has shuffled off this mortal coil, so it really doesn't matter.
