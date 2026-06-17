#include "app.h"
#include <cstring>
#include <string>

int main(int argc, char *argv[]) {
  bool test = argc == 2 && strcmp(argv[1], "--test") == 0;
  App app;
  app.run(test);
  return 0;
}
