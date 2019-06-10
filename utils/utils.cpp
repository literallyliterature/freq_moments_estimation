#include "utils.h"

// From https://stackoverflow.com/questions/2114797/compute-median-of-values-stored-in-vector-c
long long median(std::vector<long long> scores)
{
  size_t size = scores.size();

  if (size == 0)
  {
    return 0;  // Undefined, really.
  }
  else
  {
    std::sort(scores.begin(), scores.end());
    if (size % 2 == 0)
    {
      return (long long) ((scores[size / 2 - 1] + scores[size / 2]) / 2);
    }
    else 
    {
      return (long long) scores[size / 2];
    }
  }
}

// From https://stackoverflow.com/questions/2114797/compute-median-of-values-stored-in-vector-c
double median(std::vector<double> scores)
{
  size_t size = scores.size();

  if (size == 0)
  {
    return 0;  // Undefined, really.
  }
  else
  {
    std::sort(scores.begin(), scores.end());
    if (size % 2 == 0)
    {
      return (double) ((scores[size / 2 - 1] + scores[size / 2]) / 2);
    }
    else 
    {
      return (double) scores[size / 2];
    }
  }
}

int countTrailingZeros(ull x) {
  if (x == 0) return 0;

  int count = 0;
  while ((x & 1) == 0)
  {
      x = x >> 1;
      count++;
  }
  return count;
}

unsigned long long lowerPowerOf2(unsigned long long n) { 
  unsigned int p = 1;
  if (n && !(n & (n - 1)))
    return n;

  while (p < n)
    p <<= 1;
    
  return p >> 1;
} 
