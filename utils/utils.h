typedef long long ull;

#include <random>
#include <limits>
#include <climits>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

#include "xxHash-dev/xxhash.h"

using namespace std;

ull randint(ull upper = 0, ull lower = 0);

class UniversalHash {
  private:
    ull seed, domain, range;
  public:
    UniversalHash(ull domain, ull range);
    ull h(ull x);
    int size() {
      return sizeof(seed) + sizeof(domain) + sizeof(range);
    }
};


long long median(std::vector<long long> scores);
double median(std::vector<double> scores);

int countTrailingZeros(ull x);

unsigned long long lowerPowerOf2(unsigned long long n);
