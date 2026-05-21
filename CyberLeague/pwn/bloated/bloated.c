#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
  char name[0x20];
  int calories;
} typedef food;

food foods[] = {{"pizza", 285},  {"burger", 254}, {"fries", 265},
                {"soda", 150},   {"salad", 194},  {"apple", 95},
                {"banana", 105}, {"orange", 62},  {"grapes", 67},
                {"water", 0}};

int main() {
  food eat = {0};

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  puts("=============================");
  puts("Menu:");
  for (size_t i = 0; i < sizeof(foods) / sizeof(food); ++i) {
    printf("- %s  \t %3d calories\n", foods[i].name, foods[i].calories);
  }
  puts("=============================");

  while (eat.calories < 1000) {

    puts("What do I want to eat?");
    fgets(eat.name, sizeof(food), stdin);
    eat.name[strcspn(eat.name, "\n")] = 0;

    for (size_t i = 0; i < sizeof(foods) / sizeof(food); ++i) {
      if (strcmp(eat.name, foods[i].name) == 0) {
        printf("Eating %d calories\n", foods[i].calories);
        eat.calories += foods[i].calories;
        break;
      }
    }
  }

  printf("I'm full after eating %d calories\n", eat.calories);

  if (eat.calories == 1337)
    puts(getenv("FLAG"));

  exit(EXIT_SUCCESS);
}
