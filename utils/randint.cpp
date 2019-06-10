#include "utils.h"

random_device rd;     //Get a random seed from the OS entropy device, or whatever
mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
                            //and seed it with entropy.

//Define the distribution, by default it goes from 0 to MAX(unsigned long long)
//or what have you.
uniform_int_distribution<unsigned long long> distr;

ull randint(ull upper, ull lower) {
  if (upper == 0) return lower + distr(eng);
  else return lower + (distr(eng) % upper);
}
