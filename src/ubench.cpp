#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void complain() { printf("invalid driver\n"); exit(2); }

namespace cache { int doit(int, char**); }
namespace ways { int doit(int, char**); }

int main(int argc, char **argv) {

    if (argc == 1) complain();

#define CASE(name) \
    if (!strcmp(argv[1], #name)) return name ::doit(argc - 2, argv + 2);

    CASE(cache);
    CASE(ways);

    complain();

    return 0;
}
