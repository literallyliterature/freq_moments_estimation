#include "utils/utils.h"
#include <algorithm>

class CountSketch {
  private:
    vector<UniversalHash> hashfns;
    vector<UniversalHash> g_hashfns;
    int w;
    int d;
    int maxHeapSize;
    vector<vector<long long>> matrix;
    vector<vector<long long>> heap;

  public:
    CountSketch(int buckets, int hashFns, int heapSize);

    void add(long long i, long long v);

    long long estimate(long long i);

    vector<vector<long long>> top();

    long long F2_res();

    void print();

    long size() {
      return sizeof(w) + sizeof(d)+ sizeof(maxHeapSize)
        + sizeof(hashfns) + sizeof(g_hashfns) + sizeof(matrix) + sizeof(heap)
        + hashfns.size() * hashfns[0].size()
        + g_hashfns.size() * g_hashfns[0].size()
        + matrix.size() * matrix.at(0).size() * sizeof(matrix.at(0).at(0))
        + heap.size() * heap.at(0).size() * sizeof(heap.at(0).at(0));
    }
};