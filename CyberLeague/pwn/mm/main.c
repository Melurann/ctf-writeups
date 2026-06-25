#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include <sys/random.h>
#include <unistd.h>

char flag[64];
int balance = 10;
char user_pin[10] = {0};
char username[64] = {0};

int strhash(const char *str)
{
  int hash = 5381;
  int c;

  while ((c = *str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}

void readNumber(int *number)
{
  scanf("%d", number);
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    continue;
}

void gamble()
{
  while (1)
  {
    printf("Your balance is: %d\n", balance);
    printf("What do you want to do?\n");
    printf("1. Gamble\n");
    printf("2. Buy flag\n");
    printf("3. Exit\n");

    int choice = 0;
    readNumber(&choice);

    if (choice == 1)
    {
      int bet = 0;
      printf("Enter your bet: \n");
      readNumber(&bet);

      if (bet <= 0 || bet >= 1000)
      {
        printf("Invalid bet!\n");
        continue;
      }

      if (bet > balance)
      {
        printf("You don't have enough money!\n");
        continue;
      }

      int number = 0;
      char input[100] = {0};

      printf("Who wins the election? \n");
      read(0, input, sizeof(input) - 1);
      printf("Betting on: ");
      printf(input);
      printf("\n");

      number = strhash(input);

      int random_number = rand() % __INT32_MAX__;

      if (number == random_number)
      {
        balance += bet;
        printf("You won!\n");
      }
      else
      {
        balance -= bet;
        printf("You lost!\n");
      }

      random_number = 0;

      if (balance <= 0)
      {
        printf("You are out of money!\n");
        break;
      }

      printf("Your balance is: %d\n", balance);
    }
    else if (choice == 2)
    {
      if (balance == 0x13370000)
      {
        FILE *flag_file = fopen("/flag.txt", "r");
        if (flag_file == NULL)
        {
          printf("Flag file is missing!\n");
          break;
        }
        fread(flag, 1, sizeof(flag), flag_file);
        fclose(flag_file);
        balance -= 0x13370000;
        printf("You bought the flag!\n");
      }
      else
      {
        printf("You don't have enough money!\n");
      }
    }
    else if (choice == 3)
    {
      break;
    }
    else
    {
      printf("Invalid choice!\n");
    }
  }

  char feedback[64] = {0};
  printf("Please leave your feedback: ");
  read(0, feedback, sizeof(feedback) - 1);
  printf("Thank you for your feedback: \n");
  printf(feedback);
}

int main()
{
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  char vip_pin[8] = {0};
  unsigned int seed = 0;
  getrandom(&seed, sizeof(seed), 0);
  srand(seed);
  seed = 0;

  for (int i = 0; i < 8; i++)
  {
    vip_pin[i] = rand() % 10 + '0';
  }

  printf("Welcome to our online casino!\n");
  printf("Please enter your username: ");
  fgets(username, sizeof(username) - 1, stdin);

  printf("Welcome: ");
  printf(username);

  printf("Please enter your pin: ");
  fgets(user_pin, sizeof(user_pin) - 1, stdin);
  user_pin[8] = 0;

  if (!memcmp(vip_pin, user_pin, 8))
  {
    printf("You are a VIP player!\n");

    gamble();
  }
  else
  {
    printf("You are not allowed to gamble!\n");
  }

  return 0;
}