#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/random.h>

#define JIT_AREA (void*)0x42420000

int main()
{
    char size[050];

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    printf("Welcome to our great JIT verifier!\n");
    printf("Please give us your shellcode and we will verify it for you!\n");

    printf("How long is your shellcode?\n");
    read(0, size, 0x50);

    char *mem = mmap(JIT_AREA, atol(size), 7, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED)
    {
        printf("Failed to allocate memory\n");
        return 1;
    }

    printf("Give us your shellcode\n");

    read(0, mem, atol(size));

    printf("Verifying your shellcode\n");
    // TODO: Implement shellcode verifier

    printf("Looks good to me!\n");

    return 0;
}
