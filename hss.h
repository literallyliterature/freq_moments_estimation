#include "countsketch.h"

class HSampleSketch {
  private:
    UniversalHash h;
    vector<CountSketch> sketches;
  public:
    int A;
    long L;
    long d;
    long w;
    HSampleSketch(float eps, float del, long long F1, long long n, short k, long long domain);
    HSampleSketch(int levels, long width, long depth, long heap_size, long long n);
    int level(long long i);
    void add(long long i, long long v);
    bool within_thresholds(int level, long long f_i, double f2_res);
    vector<vector<long long>> sample_groups(int level);
    double estimate(short k);
    long size() {
      return sizeof(A) + sizeof(L) + sizeof(d) + sizeof(w)
        + sizeof(sketches)
        + h.size() + sketches.size() * sketches[0].size();
    }
};
