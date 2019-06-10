#include "hss.h"

HSampleSketch::HSampleSketch(float eps, float del, long long F1, long long n, short k, long long domain):h(domain, domain) {
  // cout << "here1";
  A = ceil(pow(eps, -4.0/k) * pow(n, 1-2.0/k));
  unsigned long long L1 = ceil(log2(F1*F1/A));
  unsigned long long L2 = ceil(log2(domain));
  L = L1 < L2 ? L1 : L2;
  d = ceil(log2(F1 * L * A));
  w = ceil(k*k * A / (eps * eps));
  // cout << "Initialising HSS with " << endl;
  // cout << "eps: " << eps << ", F1: " << F1 << ", n: " << n << endl;
  // cout << "w: " << w << ", d: " << d << ", A: " << A << ", L: " << L << endl;
  for (long i = 0; i < L+1; i++)
    sketches.push_back(CountSketch(w, d, A));
}

HSampleSketch::HSampleSketch(int levels, long width, long depth, long heap_size, long long n):h(n, n) {
  A = heap_size;
  L = levels;
  d = depth;
  w = width;
  for (long i = 0; i < L+1; i++)
    sketches.push_back(CountSketch(w, d, A));
}

int HSampleSketch::level(long long i) {
  long long hashed_value = h.h(i);
  int trailing = countTrailingZeros(hashed_value) + 1;
  int max_level = sketches.size();
  return (trailing <= max_level ? trailing : max_level);
}

void HSampleSketch::add(long long i, long long v) {
  int max_level = level(i);
  for (int current_level = 0; current_level < max_level; current_level++) {
    sketches[current_level].add(i, v);
  }
}

bool HSampleSketch::within_thresholds(int level, long long f_i, double f2_res) {
  // cout << "f2_res: " << f2_res << endl;
  if (level == 0) {
    double multiplier = 8 * pow(2, 1.0/4.0);
    double T0 = multiplier * sqrt(f2_res/(pow(2, level) * A));
    // cout << "Threshold at level 0 " << T0 << endl;
    return f_i >= T0;
  } else {
    double multiplier = 8 * pow(2, -1.0/4.0);
    double T_level = multiplier * sqrt(f2_res/(pow(2, level) * A));
    double T_level_1 = level > 1 ?
      multiplier * sqrt(f2_res/(pow(2, level-1) * A)) :
      8 * pow(2, 1.0/4.0) * sqrt(f2_res/(pow(2, level-1) * A));
    // cout << "Thresholds in sample_groups level " << level << endl;
    // cout << "T_level : " << T_level << ", T_level_1: " << T_level_1 << endl;
    return f_i >= T_level && f_i <= T_level_1;
  }
}

vector<vector<long long>> HSampleSketch::sample_groups(int current_level) {
  vector<vector<long long>> result;
  map<long long, long long> result_map;

  long long f2_res = sketches[0].F2_res();

  // double multiplier = 8 * pow(2, -1.0/4.0);
  // double T_level = multiplier * sqrt(f2_res/(pow(2, current_level) * A));
  // double T_level_1 = current_level > 1 ?
  //   multiplier * sqrt(f2_res/(pow(2, current_level-1) * A)) :
  //   8 * pow(2, 1.0/4.0) * sqrt(f2_res/(pow(2, current_level-1) * A));
  // cout << "T_level : " << T_level << ", T_level_1: " << T_level_1 << endl;


  for (CountSketch & cs: sketches) {
    vector<vector<long long>> top_elements = cs.top();

    for (vector<long long> & element : top_elements) {
      long long i = element[0];
      long long f_i = element[1];
      if (level(i) > current_level && within_thresholds(current_level, f_i, f2_res))
        result_map.emplace(i, f_i);
    }
  }

  for (const auto &item : result_map) {
    // cout << item.first << " : " << item.second << endl;
    result.push_back(vector<long long> {item.first, item.second});
  }
  // cout << "Total in sample_groups level " << current_level << " : " << result_map.size() << endl;
  return result;
}


double HSampleSketch::estimate(short k) {
  double result = 0;
  for (int current_level = 0; current_level <= L; current_level++) {
    // cout << "At level " << current_level << endl;
    // if (current_level > 12) sketches[current_level].print();
    // cout << "Sample group: ";
    for (vector<long long> & element : sample_groups(current_level)) {
      // cout << "(" << element[0] << ", " << element[1] << "), ";
      double f_i = element[1];
      result += pow(2, current_level) * pow(f_i, k);
    }
    // cout << endl;
  }

  return result;
}

// int main() {
//   int levels = 25;
//   int d = 5;
//   int w = 2 * ((float) 100000) / (float) levels;
//   int A = w / 2.0;
//   long domain = 2 << 25;
//   // AMSSketch ams = AMSSketch(s1, s2);
//   HSampleSketch hss = HSampleSketch((int) levels, w, d, A, domain);
//   hss.add(1,1);
//   cout << hss.size();
// //   // HSampleSketch hss = HSampleSketch(0.1, 0.25, 100000, 1000, 3, 2<<31);
// //   // HSampleSketch hss = HSampleSketch(0.25, 0.25, 10000, 1000, 3, 10);
// //   HSampleSketch hss = HSampleSketch((int) 25, 100, 5, 50, 2<<30);
// //   double actual = 0;
// //   for (int i = 1; i < 1001; i++) {
// //     int v = i % 100 + 1;
// //     hss.add(i, v);
// //     actual += v*v*v;
// //   }
// //   // cout << "hss estimate: " << hss.estimate(1) << endl;
// //   cout << "hss estimate(3): " << hss.estimate(3) << ", actual: " << actual << endl;

// //   cout << "\n\n\n\n\n" << endl;

// //   for (int i = 1001; i < 10001; i++) {
// //     int v = i % 100 + 1;
// //     hss.add(i, v);
// //     actual += v*v*v;
// //   }
// //   // cout << "hss estimate: " << hss.estimate(1) << endl;
// //   cout << "hss estimate(3): " << hss.estimate(3) << ", actual: " << actual << endl;
// //   // cout << "hss estimate(3)/actual: " << (double) hss.estimate(3)/actual << endl;

  

// //   cout << "\n\n\n\n\n" << endl;

// //   for (int i = 10001; i < 100001; i++) {
// //     int v = i % 100 + 1;
// //     hss.add(i, v);
// //     actual += v*v*v;
// //   }
// //   // cout << "hss estimate: " << hss.estimate(1) << endl;
// //   cout << "hss estimate(3): " << hss.estimate(3) << ", actual: " << actual << endl;


// //   return 0;
// }
