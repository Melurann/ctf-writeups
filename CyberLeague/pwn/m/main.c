#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/random.h>

void win()
{
    FILE *file;
    char c = 0;
    file = fopen("/flag.txt", "r");
    if (file)
    {
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fflush(stdout);
    }
    fclose(file);
    exit(0);
}

void readInput(char *target_buffer, int size)
{
    int c;
    int i = 0;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (i < size)
        {
            target_buffer[i] = (char)c;
            i++;
        }
    }
    target_buffer[size - 1] = '\0';
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    char username[010];
    char password[040];

    // Only admins know our seed
    unsigned int seed;
    if (getrandom(&seed, sizeof(seed), 0) == -1)
    {
        puts("Error getting random number");
        exit(2);
    }
    srand(seed);
    unsigned int secret_number = rand() % 16535;

    puts("Username:");
    readInput(username, 010);

    puts("Password:");
    readInput(password, 0x40);

    if (!strcmp(username, "admin") && secret_number == 0xDEADBEEF)
    {
        puts("How did you guess the secret number?!");
        // still under construction
        // TODO:
        // call openFlag
        return 0;
    }
    else
    {
        puts("Invalid credentials! This website is in construction. Only admin can access it.");
        exit(0);
    }
    return 0;
}
