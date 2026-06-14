#include <core/Engine.hpp>
#include <iostream>

int main(void) {
  try {
    Engine engine;
    engine.run();
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << std::endl;
  }

  return 0;
}
