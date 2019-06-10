// #include "countsketch.h"

class RecursiveSketch {
  private:
    vector<CountSketch> sketches;
    vector<UniversalHash> hashes;
  public:
    int phi;
    long w;
    long d;
    int A;
    RecursiveSketch(float eps, float del, long long n, short k, float alpha);
    RecursiveSketch(int levels, long width, long depth, long heap_size, long domain);
    void add(long long i, long long v);
    long long recursive_y(int level, short k);
    long long estimate(short k);
    long size() {
      return sizeof(phi) + sizeof(w) + sizeof(d) + sizeof(A)
        + sizeof(sketches) + sizeof(hashes)
        + hashes.size() * hashes[0].size() + sketches.size() * sketches[0].size();
    }
};
