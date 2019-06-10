#include "countsketch.h"
#include "recursive.h"

RecursiveSketch::RecursiveSketch(float eps, float del, long long n, short k, float alpha) {
  phi = floor(log2(log2(n)));
  w = ceil(pow(k/eps, 2) * pow(alpha, -2/k) * pow(n, 1-2/k));
  d = ceil(log2(100 * phi * n));
  A = ceil(pow(phi, 3) / pow(eps, 2));

  // cout << "Initialising RecursiveSketch with " << endl;
  // cout << "eps: " << eps << ", F1: " << F1 << ", n: " << n << endl;
  // cout << "w: " << w << ", d: " << d << ", A: " << A << ", phi: " << phi << endl;

  for (int i=0; i < phi+1; i++) {
    sketches.push_back(CountSketch(w, d, A));
    hashes.push_back(UniversalHash(n, 2));
  }
}

RecursiveSketch::RecursiveSketch(int levels, long width, long depth, long heap_size, long domain) {
  phi = levels;
  w = width;
  d = depth;
  A = heap_size;

  // cout << "Initialising RecursiveSketch with " << endl;
  // cout << "eps: " << eps << ", F1: " << F1 << ", n: " << n << endl;
  // cout << "w: " << w << ", d: " << d << ", A: " << A << ", phi: " << phi << endl;

  for (int i=0; i < phi+1; i++) {
    sketches.push_back(CountSketch(w, d, A));
    hashes.push_back(UniversalHash(domain, 2));
  }
}

void RecursiveSketch::add(long long i, long long v) {
  sketches[0].add(i,v);
  for (int current_level = 1; current_level <= phi && hashes[current_level].h(i) == 1;) {
    sketches[current_level].add(i, v);
    current_level++;
  }
}

long long RecursiveSketch::recursive_y(int level, short k) {
  // cout << "called recursive_y(" << level << ", " << k << ")" << endl;
  long long s = 0;
  if (level == phi) {
    // cout << "level: " << level << ", phi: " << phi << endl;
    for (vector<long long> & element : sketches[sketches.size() - 1].top())
      s += pow(element[1], k);

    // cout << "heap at level " << phi << ": ";
    // sketches[sketches.size() - 1].print();
    // cout << "result at level " << phi << ": " << s << endl;
    return s;
  }

  // cout << "calculating s at level " << level << ": ";
  for (vector<long long> & element : sketches[level].top()) {
    s += (1 - 2 * hashes[level+1].h(element[0])) * pow(element[1], k);
    // cout << s << endl;
  }

  long long result = 2 * recursive_y(level + 1, k) + s;
  // cout << "heap at level " << level << ": ";
  // sketches[level].print();
  // cout << "result at level " << level << ": " << result << endl;
  return result;
}

long long RecursiveSketch::estimate(short k) {
  return recursive_y(0, k);
}

// int main() {
//   // for (int j=0; j < 10; j++) {
//     // RecursiveSketch recursive = RecursiveSketch((float) 0.25, 0.25, 1000000, 3, 0.1);
//     RecursiveSketch recursive = RecursiveSketch((int) log2(log2(1000)), 1000, 10, 50, 1000);
//     double actual = 0;
//     for (int i = 1; i < 1001; i++) {
//       int v = i;
//       recursive.add(i, v);
//       actual += v*v*v;
//     }
//     // cout << "recursive estimate: " << recursive.estimate(1) << endl;
//     // cout << "recursive estimate(3): " << recursive.estimate(3) << endl;
//     cout << "actual: " << actual << endl;
//     cout << "recursive estimate(3)/actual: " << (double) recursive.estimate(3)/actual << endl;
//   // }
//   return 0;
// }
