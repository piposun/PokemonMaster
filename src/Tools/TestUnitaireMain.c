#include "logger.h"
#include "openFile.h"

int main(int argc, char const *argv[]) {
  int numTest = 1;
  char *pathName = "testBase.base";
  FILE *file;

  // Test logger
  INFO("Test %d %s", numTest, "OK"); numTest++;
  DEBUG("Test %d %s", numTest, "OK"); numTest++;
  WARN("Test %d %s", numTest, "OK"); numTest++;
  ERROR("Test %d %s", numTest, "OK");

  // Test openfile
  if (openFile(&file, pathName, "r+") == 1) {
    INFO("Ouverture du fichier %s", pathName);
  }

  return 0;
}
