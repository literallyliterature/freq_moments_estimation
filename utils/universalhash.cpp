#include "utils.h"

UniversalHash::UniversalHash(ull domainInput, ull rangeInput) {
  domain = domainInput;
  range = rangeInput;
  seed = randint();
  // std::cout << "seed: " << seed << std::endl;
}

long long UniversalHash::h(ull x) {
  void* buffer = &x;
  size_t length = sizeof x;
  unsigned long long hashed = XXH64(buffer, length, seed);
  return hashed % range;
}
