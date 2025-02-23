// Name: Hunter Anderson
// Email: andershu@oregonstate.edu
// Date: 2 March 2025
// Description: Prints the current working directory

#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("%s\n", getenv("PWD"));
  return 0;
}

