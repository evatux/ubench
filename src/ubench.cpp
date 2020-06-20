#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void complain() { printf("invalid driver\n"); exit(2); }

int main(int argc, char **argv) {

    if (argc == 1) complain();

#define CASE(d) \
    if (!strcmp(argv[1], #d)) { int d(int, char **); return d(argc - 2, argv + 2); }

    CASE(cache);

    complain();

    return 0;
}
