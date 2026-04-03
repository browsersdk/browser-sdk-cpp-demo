#include "stdafx.h"

int main(int argc, char **argv) {

  Utils::MainProc([&](const std::string &input, bool &exit) {
    if (input == "q") {
      exit = true;
    } else if (input == "init") {

    } else {
      std::cout << "Unknown command: " << input << std::endl;
    }
  });
  return 0;
}