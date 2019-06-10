#include "countsketch.h"

CountSketch::CountSketch(int buckets, int hashFns, int heapSize) {
  long domain = (long) pow(10, 8);
  w = buckets;
  d = hashFns;
  for (int i = 0; i < d; i++) {
    UniversalHash newH (domain, w);
    hashfns.push_back(newH);
    UniversalHash newG (domain, 2);
    g_hashfns.push_back(UniversalHash(domain, 2));
  }
  matrix = vector<vector<long long>> (d, vector<long long> (w, 0));
  maxHeapSize = heapSize;
}

void CountSketch::add(long long i, long long v) {
  vector<long long> values;
  for (int row = 0; row < d; row++) {
    UniversalHash & hashfn = hashfns[row];
    UniversalHash & g_hashfn = g_hashfns[row];
    char g = g_hashfn.h(i) == 1 ? 1 : -1;
    int column = hashfn.h(i);
    matrix.at(row).at(column) += v * g;
    values.push_back(matrix.at(row).at(column) * g);
  }

  long long median_value = median(values);

  // Check if i is already tracked in the heap, and also track the minimum tracked
  vector<long long> * minimum_tracked = heap.data();
  for (vector<long long> &tracked_items : heap) {
    long long tracked_i = tracked_items[0];
    long long tracked_v = tracked_items[1];

    if (tracked_i == i) {
      tracked_items[1] += v;
      return ;
    }

    if (tracked_v < minimum_tracked->at(1)) {
      minimum_tracked = &tracked_items;
    }
  }

  if (heap.size() < maxHeapSize) heap.push_back({i, median_value});
  else if (median_value > minimum_tracked->at(1)) {
    minimum_tracked->at(0) = i;
    minimum_tracked->at(1) = median_value;
  }
}

long long CountSketch::estimate(long long i) {
  vector<long long> values;
  for (int row = 0; row < d; row++) {
    UniversalHash & hashfn = hashfns[row];
    UniversalHash & g_hashfn = g_hashfns[row];
    char g = g_hashfn.h(i) == 1 ? 1 : -1;
    int column = hashfn.h(i);
    values.push_back(matrix.at(row).at(column) * g);
  }

  return median(values);
}

vector<vector <long long>> CountSketch::top() {
  return heap;
}

long long CountSketch::F2_res() {
  vector<vector<long long>> top_items = top();
  for (vector<long long> element : top_items) {
    long long item = element[0];
    long long freq = element[1];

    add(item, -freq);
  }
  vector<long long> row_F2s;
  for (int row = 0; row < d; row++) {
    long long row_F2 = 0;
    for (int column = 0; column < w; column++)
      row_F2 += pow(matrix.at(row).at(column), 2);
    row_F2s.push_back(row_F2);
  }

  // Add back all the elements
  for (vector<long long> element : top_items) {
    long long item = element[0];
    long long freq = element[1];

    add(item, freq);
  }
  return median(row_F2s);
}

void CountSketch::print() {
  // cout << "hashfns size:" << hashfns.size() << endl;
  // cout << "g_hashfns size:" << g_hashfns.size() << endl;
  // cout << "matrix size: " << matrix.size() << endl;
  cout << "heap: ";
  for (vector<long long> & element: top())
    cout << "(" << element[0] << ", " << element[1] << "), ";
  cout << endl;
  // for (vector<long long> row : matrix) {
  //   for (long long cell : row)
  //     cout << cell << '\t';
  //   cout << endl;
  // }
}

// int main() {
//   CountSketch ck (15, 5, 5);
//   for (int i=0; i < 10; i++) {
//     ck.add(i, i);
//   }
//   ck.print();
//   cout << "F2 res: " << ck.F2_res() << endl;
//   return 0;
// }
