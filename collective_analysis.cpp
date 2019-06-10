#include "hss.h"
#include "recursive.h"
#include "ams.h"
#include "simple_sketch.h"
#include <benchmark/benchmark.h>
#include <fstream>

string INPUT_FILES[] = {
  "./distributions/uniform_100.txt",      // 0
  "./distributions/uniform_1k.txt",       // 1
  "./distributions/uniform_10k.txt",      // 2
  "./distributions/normal_0.05_10k.txt",  // 3
  "./distributions/normal_0.1_10k.txt",   // 4
  "./distributions/normal_0.25_10k.txt",  // 5
  "./distributions/normal_0.5_10k.txt",   // 6
  "./distributions/uniform_100k.txt",     // 7
  "./distributions/normal_0.05_100k.txt", // 8
  "./distributions/normal_0.1_100k.txt",  // 9
  "./distributions/normal_0.25_100k.txt", // 10
  "./distributions/normal_0.5_100k.txt",  // 11
  "./distributions/uniform_1M.txt",       // 12
  "./distributions/normal_0.05_1M.txt",   // 13
  "./distributions/normal_0.1_1M.txt",    // 14
  "./distributions/normal_0.25_1M.txt",   // 15
  "./distributions/normal_0.5_1M.txt",    // 16
  "./distributions/kosarak.dat",          // 17
  "./distributions/Webdocs.dat",          // 18
  "./distributions/uniform_10M.txt",      // 19
};

double INPUT_FS[] = {
  100,5334,371118,2.883e+07,2.38068e+09,2.04298e+11,1.7997e+13,1.61543e+15,1.47049e+17,1.35308e+19,1.25575e+21,
  1000,52263,3.53219e+06,2.66879e+08,2.14524e+10,1.7934e+12,1.54038e+14,1.34948e+16,1.20024e+18,1.08032e+20,9.81863e+21,
  10000,503925,3.36287e+07,2.52277e+09,2.01865e+11,1.68291e+13,1.44344e+15,1.26417e+17,1.12501e+19,1.01393e+21,9.2326e+22,
  10000,43307,271437,2.15052e+06,2.00124e+07,2.10117e+08,2.428e+09,3.03738e+10,4.06681e+11,5.78029e+12,8.66735e+13,
  10000,83331,1.04527e+06,1.66069e+07,3.09419e+08,6.48537e+09,1.48811e+11,3.66504e+12,9.54569e+13,2.59949e+15,7.33747e+16,
  10000,199277,6.15956e+06,2.43688e+08,1.14083e+10,6.05721e+11,3.54761e+13,2.24641e+15,1.51464e+17,1.07461e+19,7.94821e+20,
  10000,400404,2.49592e+07,1.98098e+09,1.85171e+11,1.95602e+13,2.27452e+15,2.85796e+17,3.82611e+19,5.39744e+21,7.95228e+23,
  99999,5.04675e+06,3.37666e+08,2.54324e+10,2.04442e+12,1.71258e+14,1.47597e+16,1.2988e+18,1.16129e+20,1.05163e+22,9.62392e+23,
  99997,434196,2.74546e+06,2.20091e+07,2.07629e+08,2.21553e+09,2.61143e+10,3.34727e+11,4.61405e+12,6.78258e+13,1.05606e+15,
  100000,831663,1.04455e+07,1.66865e+08,3.13851e+09,6.67329e+10,1.56425e+12,3.97313e+13,1.07965e+15,3.10785e+16,9.40146e+17,
  99998,2.0339e+06,6.39312e+07,2.55346e+09,1.19883e+11,6.35384e+12,3.70759e+14,2.34065e+16,1.57769e+18,1.12343e+20,8.37728e+21,
  99999,4.04053e+06,2.54452e+08,2.03927e+10,1.92599e+12,2.06157e+14,2.44346e+16,3.15849e+18,4.40672e+20,6.58762e+22,1.04942e+25,
  999905,5.04685e+07,3.37982e+09,2.54675e+11,2.04749e+13,1.71537e+15,1.47912e+17,1.30345e+19,1.16927e+21,1.06599e+23,9.8841e+24,
  999893,4.34577e+06,2.75015e+07,2.20862e+08,2.08924e+09,2.23618e+10,2.6431e+11,3.3949e+12,4.68554e+13,6.89157e+14,1.07329e+16,
  999882,8.32478e+06,1.04686e+08,1.6751e+09,3.15939e+10,6.75006e+11,1.59487e+13,4.10177e+14,1.13567e+16,3.35866e+17,1.05507e+19,
  999873,2.03024e+07,6.37004e+08,2.54486e+10,1.19806e+12,6.38447e+13,3.75777e+15,2.40253e+17,1.64849e+19,1.20266e+21,9.25988e+22,
  999877,4.02209e+07,2.52191e+09,2.0142e+11,1.89638e+13,2.0224e+15,2.38479e+17,3.05959e+19,4.22126e+21,6.2071e+23,9.65664e+25,
  41270,8.01902e+06,8.03042e+11,3.68068e+17,1.91145e+23,1.03829e+29,5.79973e+34,3.30435e+40,1.90997e+46,1.11573e+52,6.56826e+57,
  5.26766e+06,2.99887e+08,4.04804e+13,1.86844e+19,1.49634e+25,1.57213e+31,1.87484e+37,2.37912e+43,3.12104e+49,4.17459e+55,5.65284e+61,
  9.98834e+06,5.04997e+08,3.38953e+10,2.56255e+12,2.07104e+14,1.75047e+16,1.53272e+18,1.38778e+20,1.3059e+22,1.29341e+24,1.37629e+26,
};

int retrieve(int fileIndex, int k) {
  return 11 * fileIndex + k;
}

float epsilon_error(double estimated_value, int fileIndex, int k) {
  double stored_value = INPUT_FS[retrieve(fileIndex, k)];
  return (double)abs(1.0 - estimated_value / stored_value);
}

const int REPETITIONS = 10;
const short default_k = 3;
const long long
  default_domain = 2<<30;
const float
  default_eps = (float) 25.0 / 100.0,
  default_del = (float) 25.0 / 100.0;









static void fn_FUNCTION_TEMPLATE(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = default_eps,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  double ams_estimates[11];
  double hss_estimates[11];
  double recursive_estimates[11];

  std::ifstream infile(filename);

  AMSSketch ams = AMSSketch(eps, del, n, k);
  HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        ams.add(a, 1);
        hss.add(a, 1);
        recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        ams.add(a, b);
        hss.add(a, b);
        recursive.add(a, b);
      }
    }

    for (short i=3; i < 4; i++) {
      ams_estimates[i] = ams.estimate(i);
      hss_estimates[i] = hss.estimate(i);
      recursive_estimates[i] = recursive.estimate(i);
    }
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  state.counters["ams_space"] = ams.size();
  state.counters["ams_estimate(3)"] = ams_estimates[3];
  state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  state.counters["hss_space"] = hss.size();
  state.counters["hss_estimate(3)"] = hss_estimates[3];
  state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}
// BENCHMARK(fn_FUNCTION_TEMPLATE)
//   ->Arg(0)->Arg(1)
//   ->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)
//   ->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(11)
  // ->Arg(12)->Arg(13)->Arg(14)->Arg(15)->Arg(16)
  // ->Arg(17)->Arg(18)-> Arg(19)

  // ->RangeMultiplier(2)
  // ->Range(8, 8192)

  // ->Args({13, 25, 20})->Args({12, 45, 20})->Args({12, 40, 20})->Args({12, 35, 20})->Args({12, 30, 20})->Args({12, 25, 20})->Args({12, 20, 20})->Args({12, 15, 20})->Args({12, 10, 20})
  // ->Args({12, 50, 10})->Args({12, 45, 10})->Args({12, 40, 10})->Args({12, 35, 10})->Args({12, 30, 10})->Args({12, 25, 10})->Args({12, 20, 10})->Args({12, 15, 10})->Args({12, 10, 10})
  // ->Args({12, 50, 5})->Args({12, 45, 5})->Args({12, 40, 5})->Args({12, 35, 5})->Args({12, 30, 5})->Args({12, 25, 5})->Args({12, 20, 5})->Args({12, 15, 5})->Args({12, 10, 5})
  // ->Args({12, 50, 2})->Args({12, 45, 2})->Args({12, 40, 2})->Args({12, 35, 2})->Args({12, 30, 2})->Args({12, 25, 2})->Args({12, 20, 2})->Args({12, 15, 2})->Args({12, 10, 2})
  // ->Args({12, 50, 1})->Args({12, 45, 1})->Args({12, 40, 1})->Args({12, 35, 1})->Args({12, 30, 1})->Args({12, 25, 1})->Args({12, 20, 1})->Args({12, 15, 1})->Args({12, 10, 1})
  // ->Repetitions(REPETITIONS)
  // ->Unit(benchmark::kMillisecond);
















static void fn_01_hss_recursive_all_dists(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = default_eps,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  // double ams_estimates[11];
  double hss_estimates[11];
  double recursive_estimates[11];

  std::ifstream infile(filename);

  // AMSSketch
  //   sketch = AMSSketch(s1, s2, F1);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  HSampleSketch hss = HSampleSketch((int) 10, 1000, 4, 1000, 2<<30);
  RecursiveSketch recursive = RecursiveSketch((int) 10, 1000, 4, 1000, 2<<30);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        // ams.add(a, 1);
        hss.add(a, 1);
        recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        // ams.add(a, b);
        hss.add(a, b);
        recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 11; i++) {
    // ams_estimates[i] = ams.estimate(i);
    hss_estimates[i] = hss.estimate(i);
    recursive_estimates[i] = recursive.estimate(i);
  }

  // state.counters["n"] = n;
  // state.counters["F1"] = F1;
  // state.counters["eps"] = eps;
  // state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  // state.counters["ams_space"] = ams.size();
  // state.counters["ams_estimate(3)"] = ams_estimates[3];
  // state.counters["number_of_ams_estimators"] = sketch.numberOfEstimators();

  state.counters["hss_space"] = hss.size();
  state.counters["hss_estimate(3)"] = hss_estimates[3];
  state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  state.counters["hss_error(4)"] = epsilon_error(hss_estimates[4], file_index, 4);
  state.counters["hss_error(5)"] = epsilon_error(hss_estimates[5], file_index, 5);
  state.counters["hss_error(6)"] = epsilon_error(hss_estimates[6], file_index, 6);
  state.counters["hss_error(7)"] = epsilon_error(hss_estimates[7], file_index, 7);
  state.counters["hss_error(8)"] = epsilon_error(hss_estimates[8], file_index, 8);
  state.counters["hss_error(9)"] = epsilon_error(hss_estimates[9], file_index, 9);
  state.counters["hss_error(10)"] = epsilon_error(hss_estimates[10], file_index, 10);
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_error(4)"] = epsilon_error(recursive_estimates[4], file_index, 4);
  state.counters["recursive_error(5)"] = epsilon_error(recursive_estimates[5], file_index, 5);
  state.counters["recursive_error(6)"] = epsilon_error(recursive_estimates[6], file_index, 6);
  state.counters["recursive_error(7)"] = epsilon_error(recursive_estimates[7], file_index, 7);
  state.counters["recursive_error(8)"] = epsilon_error(recursive_estimates[8], file_index, 8);
  state.counters["recursive_error(9)"] = epsilon_error(recursive_estimates[9], file_index, 9);
  state.counters["recursive_error(10)"] = epsilon_error(recursive_estimates[10], file_index, 10);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_01_hss_recursive_all_dists)
//   ->Arg(0)->Arg(1)
//   ->Arg(2)->Arg(3)
//   ->Arg(4)->Arg(5)->Arg(6)
//   ->Arg(7)->Arg(8)
//   ->Arg(9)->Arg(10)->Arg(11)
//   ->Arg(12)->Arg(13)
//   // ->Arg(14)->Arg(15)->Arg(16)
//   ->Arg(17)
//   // ->Arg(18)-> Arg(19)
//   ->Repetitions(1)
//   ->Unit(benchmark::kMillisecond);




// Kept ams separate to avoid longer streams, about 10k estimators
static void fn_02_ams_2pass_all_dists(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = 100,
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = 0.5,
    del = 0.1;

  string filename = INPUT_FILES[file_index];

  double ams_estimates[11];
  // double hss_estimates[11];
  // double recursive_estimates[11];

  std::ifstream infile(filename);

  AMSSketch ams = AMSSketch(eps, del, n, k, F1);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        ams.add(a, 1);
        // hss.add(a, 1);
        // recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        ams.add(a, b);
        // hss.add(a, b);
        // recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 11; i++) {
    ams_estimates[i] = ams.estimate(i);
    // hss_estimates[i] = hss.estimate(i);
    // recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  state.counters["ams_space"] = ams.size();
  state.counters["ams_estimate(3)"] = ams_estimates[3];
  state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  state.counters["ams_error(4)"] = epsilon_error(ams_estimates[4], file_index, 4);
  state.counters["ams_error(5)"] = epsilon_error(ams_estimates[5], file_index, 5);
  state.counters["ams_error(6)"] = epsilon_error(ams_estimates[6], file_index, 6);
  state.counters["ams_error(7)"] = epsilon_error(ams_estimates[7], file_index, 7);
  state.counters["ams_error(8)"] = epsilon_error(ams_estimates[8], file_index, 8);
  state.counters["ams_error(9)"] = epsilon_error(ams_estimates[9], file_index, 9);
  state.counters["ams_error(10)"] = epsilon_error(ams_estimates[10], file_index, 10);
  state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  // state.counters["hss_space"] = hss.size();
  // state.counters["hss_estimate(3)"] = hss_estimates[3];
  // state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  // state.counters["hss_heap_size_at_each_level"] = hss.A;
  // state.counters["hss_levels"] = hss.L;
  // state.counters["hss_CS_depth"] = hss.d;
  // state.counters["hss_CS_width"] = hss.w;

  // state.counters["recursive_space"] = recursive.size();
  // state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  // state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  // state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  // state.counters["recursive_phi"] = recursive.phi;
  // state.counters["recursive_CS_depth"] = recursive.d;
  // state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_02_ams_2pass_all_dists)
//   ->Arg(0)->Arg(1)
//   ->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)
//   ->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(11)
//   ->Arg(12)->Arg(13)->Arg(14)->Arg(15)->Arg(16)
//   ->Arg(17)->Arg(18)-> Arg(19)
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);





static void fn_03_ams_s1_s2(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = INPUT_FS[retrieve(file_index, 1)];

  string filename = INPUT_FILES[file_index];

  double ams_estimates[11];
  // double hss_estimates[11];
  // double recursive_estimates[11];

  std::ifstream infile(filename);

  int s1 = state.range(1);
  int s2 = 5;

  AMSSketch ams = AMSSketch(s1, s2, F1);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        ams.add(a, 1);
        // hss.add(a, 1);
        // recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        ams.add(a, b);
        // hss.add(a, b);
        // recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 11; i++) {
    ams_estimates[i] = ams.estimate(i);
    // hss_estimates[i] = hss.estimate(i);
    // recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  // state.counters["eps"] = eps;
  // state.counters["del"] = del;
  state.counters["file_index"] = file_index;
  state.counters["s1"] = s1;
  state.counters["s2"] = s2;

  state.counters["ams_space"] = ams.size();
  state.counters["ams_estimate(3)"] = ams_estimates[3];
  state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  state.counters["ams_error(4)"] = epsilon_error(ams_estimates[4], file_index, 4);
  state.counters["ams_error(5)"] = epsilon_error(ams_estimates[5], file_index, 5);
  state.counters["ams_error(6)"] = epsilon_error(ams_estimates[6], file_index, 6);
  state.counters["ams_error(7)"] = epsilon_error(ams_estimates[7], file_index, 7);
  state.counters["ams_error(8)"] = epsilon_error(ams_estimates[8], file_index, 8);
  state.counters["ams_error(9)"] = epsilon_error(ams_estimates[9], file_index, 9);
  state.counters["ams_error(10)"] = epsilon_error(ams_estimates[10], file_index, 10);
  state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  // state.counters["hss_space"] = hss.size();
  // state.counters["hss_estimate(3)"] = hss_estimates[3];
  // state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  // state.counters["hss_heap_size_at_each_level"] = hss.A;
  // state.counters["hss_levels"] = hss.L;
  // state.counters["hss_CS_depth"] = hss.d;
  // state.counters["hss_CS_width"] = hss.w;

  // state.counters["recursive_space"] = recursive.size();
  // state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  // state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  // state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  // state.counters["recursive_phi"] = recursive.phi;
  // state.counters["recursive_CS_depth"] = recursive.d;
  // state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_03_ams_s1_s2)
//   ->Args({2, 1})->Args({2, 2})->Args({2, 4})->Args({2, 5})
//   ->Args({2, 6})->Args({2, 8})->Args({2, 10})->Args({2, 12})
//   ->Args({2, 15})->Args({2, 20})->Args({2, 24})
//   ->Args({3, 1})->Args({3, 2})->Args({3, 4})->Args({3, 5})
//   ->Args({3, 6})->Args({3, 8})->Args({3, 10})->Args({3, 12})
//   ->Args({3, 15})->Args({3, 20})->Args({3, 24})
  // ->Args({17, 100})->Args({17, 400})->Args({17, 1000})
  // ->Repetitions(REPETITIONS)
  // ->Unit(benchmark::kMillisecond);




static void fn_04_ams_2pass_eps_del(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = (float) state.range(1) / 100.0,
    del = (float) state.range(2) / 100.0;

  string filename = INPUT_FILES[file_index];

  double ams_estimates[11];
  // double hss_estimates[11];
  // double recursive_estimates[11];

  std::ifstream infile(filename);

  AMSSketch ams = AMSSketch(eps, del, n, k, F1);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        ams.add(a, 1);
        // hss.add(a, 1);
        // recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        ams.add(a, b);
        // hss.add(a, b);
        // recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 4; i++) {
    ams_estimates[i] = ams.estimate(i);
    // hss_estimates[i] = hss.estimate(i);
    // recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  state.counters["ams_space"] = ams.size();
  state.counters["ams_estimate(3)"] = ams_estimates[3];
  state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  // state.counters["hss_space"] = hss.size();
  // state.counters["hss_estimate(3)"] = hss_estimates[3];
  // state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  // state.counters["hss_heap_size_at_each_level"] = hss.A;
  // state.counters["hss_levels"] = hss.L;
  // state.counters["hss_CS_depth"] = hss.d;
  // state.counters["hss_CS_width"] = hss.w;

  // state.counters["recursive_space"] = recursive.size();
  // state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  // state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  // state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  // state.counters["recursive_phi"] = recursive.phi;
  // state.counters["recursive_CS_depth"] = recursive.d;
  // state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_04_ams_2pass_eps_del)
//   ->Args({0, 50, 20})->Args({0, 45, 20})->Args({0, 40, 20})->Args({0, 35, 20})->Args({0, 30, 20})->Args({0, 25, 20})->Args({0, 20, 20})->Args({0, 15, 20})->Args({0, 10, 20})
//   ->Args({0, 50, 10})->Args({0, 45, 10})->Args({0, 40, 10})->Args({0, 35, 10})->Args({0, 30, 10})->Args({0, 25, 10})->Args({0, 20, 10})->Args({0, 15, 10})->Args({0, 10, 10})
//   ->Args({0, 50, 5})->Args({0, 45, 5})->Args({0, 40, 5})->Args({0, 35, 5})->Args({0, 30, 5})->Args({0, 25, 5})->Args({0, 20, 5})->Args({0, 15, 5})->Args({0, 10, 5})
//   ->Args({0, 50, 2})->Args({0, 45, 2})->Args({0, 40, 2})->Args({0, 35, 2})->Args({0, 30, 2})->Args({0, 25, 2})->Args({0, 20, 2})->Args({0, 15, 2})->Args({0, 10, 2})
//   ->Args({0, 50, 1})->Args({0, 45, 1})->Args({0, 40, 1})->Args({0, 35, 1})->Args({0, 30, 1})->Args({0, 25, 1})->Args({0, 20, 1})->Args({0, 15, 1})->Args({0, 10, 1})
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);




static void fn_05_ams_2pass_s1_s2(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = INPUT_FS[retrieve(file_index, 1)],
    s1 = state.range(1),
    s2 = state.range(2);
  // float
    // eps = (float) state.range(1) / 100.0,
    // del = (float) state.range(2) / 100.0;

  string filename = INPUT_FILES[file_index];

  double ams_estimates[11];
  // double hss_estimates[11];
  // double recursive_estimates[11];

  std::ifstream infile(filename);

  AMSSketch ams = AMSSketch(s1, s2, F1);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        ams.add(a, 1);
        // hss.add(a, 1);
        // recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        ams.add(a, b);
        // hss.add(a, b);
        // recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 4; i++) {
    ams_estimates[i] = ams.estimate(i);
    // hss_estimates[i] = hss.estimate(i);
    // recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  state.counters["s1"] = s1;
  state.counters["s2"] = s2;
  state.counters["file_index"] = file_index;

  state.counters["ams_space"] = ams.size();
  state.counters["ams_estimate(3)"] = ams_estimates[3];
  state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  // state.counters["hss_space"] = hss.size();
  // state.counters["hss_estimate(3)"] = hss_estimates[3];
  // state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  // state.counters["hss_heap_size_at_each_level"] = hss.A;
  // state.counters["hss_levels"] = hss.L;
  // state.counters["hss_CS_depth"] = hss.d;
  // state.counters["hss_CS_width"] = hss.w;

  // state.counters["recursive_space"] = recursive.size();
  // state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  // state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  // state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  // state.counters["recursive_phi"] = recursive.phi;
  // state.counters["recursive_CS_depth"] = recursive.d;
  // state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_05_ams_2pass_s1_s2)
//   ->Args({5, 50, 20})->Args({5, 100, 20})->Args({5, 150, 20})->Args({5, 200, 20})->Args({5, 250, 20})
//   ->Args({5, 50, 10})->Args({5, 100, 10})->Args({5, 150, 10})->Args({5, 200, 10})->Args({5, 250, 10})
//   ->Args({5, 50, 5})->Args({5, 100, 5})->Args({5, 150, 5})->Args({5, 200, 5})->Args({5, 250, 5})
//   ->Args({5, 50, 2})->Args({5, 100, 2})->Args({5, 150, 2})->Args({5, 200, 2})->Args({5, 250, 2})
//   ->Args({5, 50, 1})->Args({5, 100, 1})->Args({5, 150, 1})->Args({5, 200, 1})->Args({5, 250, 1})
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);










static void fn_06_ams_1pass_update_time(benchmark::State& state) {
  long long
    s1 = state.range(1),
    s2 = state.range(2);

  AMSSketch ams = AMSSketch(s1, s2);

  for (auto _ : state)
    for (int j=0; j<1000; j++)
      ams.add(state.range(0), state.range(0));
    
  state.counters["s1"] = s1;
  state.counters["s2"] = s2;
  state.counters["v"] = state.range(0);
  state.counters["ams_space"] = ams.size();
  state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

}

// BENCHMARK(fn_06_ams_1pass_update_time)
  // ->Args({1, 5, 20})->Args({1, 20, 20})->Args({1, 50, 20})->Args({1, 100, 20})->Args({1, 150, 20})->Args({1, 200, 20})
  // ->Args({2, 5, 20})->Args({2, 20, 20})->Args({2, 50, 20})->Args({2, 100, 20})->Args({2, 150, 20})->Args({2, 200, 20})
  // ->Args({5, 5, 20})->Args({5, 20, 20})->Args({5, 50, 20})->Args({5, 100, 20})->Args({5, 150, 20})->Args({5, 200, 20})
  // ->Args({10, 5, 20})->Args({10, 20, 20})->Args({10, 50, 20})->Args({10, 100, 20})->Args({10, 150, 20})->Args({10, 200, 20})
  // ->Args({20, 5, 20})->Args({20, 20, 20})->Args({20, 50, 20})->Args({20, 100, 20})->Args({20, 150, 20})->Args({20, 200, 20})
  // ->Args({50, 5, 20})->Args({50, 20, 20})->Args({50, 50, 20})->Args({50, 100, 20})->Args({50, 150, 20})->Args({50, 200, 20})
  // ->Repetitions(REPETITIONS)
  // ->Unit(benchmark::kMillisecond);












static void fn_07_hss_update_time(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain;

  int multiplier = state.range(0);
  HSampleSketch hss = HSampleSketch((int) 3*multiplier, multiplier * 50, multiplier + 1, multiplier * 10, 2<<28);
  // HSampleSketch hss = HSampleSketch((int) 5*state.range(0), 100, (int) 5, 10, 2<<28);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {
    for (int j=0; j<1000; j++)
      hss.add(j, j);
      // recursive.add(1, 1);
  }

  state.counters["hss_space"] = hss.size();
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  // state.counters["recursive_space"] = recursive.size();
  // state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  // state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  // state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  // state.counters["recursive_phi"] = recursive.phi;
  // state.counters["recursive_CS_depth"] = recursive.d;
  // state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_07_hss_update_time)
//   ->Arg(1)
//   ->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)
//   ->Arg(7)->Arg(8)->Arg(9)->Arg(10)
// // ->Arg(11)
// //   ->Arg(12)->Arg(13)->Arg(14)->Arg(15)->Arg(16)
// //   ->Arg(17)->Arg(18)-> Arg(19)->Arg(20)
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);









static void fn_08_recursive_update_time(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain;


  // HSampleSketch hss = HSampleSketch((int) 5*state.range(0), 100, (int) 5, 10, 2<<28);
  // RecursiveSketch recursive = RecursiveSketch((int) (4.5*state.range(0)), 100, (int) 5, 10, 2<<28);
  int multiplier = state.range(0);
  RecursiveSketch recursive = RecursiveSketch((int) 3*multiplier, multiplier * 50, multiplier + 1, multiplier * 10, 2<<28);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {
    for (int j=0; j<1000; j++)
      // hss.add(j, j);
      recursive.add(j, j);
  }

  // state.counters["hss_space"] = hss.size();
  // state.counters["hss_heap_size_at_each_level"] = hss.A;
  // state.counters["hss_levels"] = hss.L;
  // state.counters["hss_CS_depth"] = hss.d;
  // state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  // state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  // state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_08_recursive_update_time)
//   ->Arg(1)
//   ->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)
//   ->Arg(7)->Arg(8)->Arg(9)->Arg(10)
// // ->Arg(11)
// //   ->Arg(12)->Arg(13)->Arg(14)->Arg(15)->Arg(16)
// //   ->Arg(17)->Arg(18)-> Arg(19)->Arg(20)
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);






static void fn_09_hss_recursive_eps(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = (float) state.range(1) / 100.0,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  // double ams_estimates[11];
  double hss_estimates[11];
  double recursive_estimates[11];

  std::ifstream infile(filename);

  // AMSSketch ams = AMSSketch(eps, del, n, k);
  HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        // ams.add(a, 1);
        hss.add(a, 1);
        recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        // ams.add(a, b);
        hss.add(a, b);
        recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 4; i++) {
    // ams_estimates[i] = ams.estimate(i);
    hss_estimates[i] = hss.estimate(i);
    recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  // state.counters["ams_space"] = ams.size();
  // state.counters["ams_estimate(3)"] = ams_estimates[3];
  // state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  state.counters["hss_space"] = hss.size();
  state.counters["hss_estimate(3)"] = hss_estimates[3];
  state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_09_hss_recursive_eps)
//   ->Args({5, 50})->Args({5, 45})->Args({5, 40})->Args({5, 35})->Args({5, 30})
//   ->Args({5, 25})->Args({5, 20})->Args({5, 15})->Args({5, 10})->Args({5, 5})
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);








static void fn_10_hss_recursive_k(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = state.range(1);
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = default_eps,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  // double ams_estimates[11];
  double hss_estimates[11];
  double recursive_estimates[11];

  std::ifstream infile(filename);

  // AMSSketch ams = AMSSketch(eps, del, n, k);
  HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        // ams.add(a, 1);
        hss.add(a, 1);
        recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        // ams.add(a, b);
        hss.add(a, b);
        recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 4; i++) {
    // ams_estimates[i] = ams.estimate(i);
    hss_estimates[i] = hss.estimate(i);
    recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  // state.counters["ams_space"] = ams.size();
  // state.counters["ams_estimate(3)"] = ams_estimates[3];
  // state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  state.counters["hss_space"] = hss.size();
  state.counters["hss_estimate(3)"] = hss_estimates[3];
  state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_10_hss_recursive_k)
//   ->Args({5, 3})->Args({5, 4})->Args({5, 5})->Args({5, 6})
//   ->Args({5, 7})->Args({5, 8})->Args({5, 9})->Args({5, 10})
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);








static void fn_11_ams_2pass_wrong_exact_stream_length(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = 100000;
  float
    eps = default_eps,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  double ams_estimates[11];
  // double hss_estimates[11];
  // double recursive_estimates[11];

  std::ifstream infile(filename);

  AMSSketch ams = AMSSketch(eps, del, n, k, F1);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        ams.add(a, 1);
        // hss.add(a, 1);
        // recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        ams.add(a, b);
        // hss.add(a, b);
        // recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 4; i++) {
    ams_estimates[i] = ams.estimate(i);
    // hss_estimates[i] = hss.estimate(i);
    // recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["given_F1"] = F1;
  state.counters["actual_F1"] = INPUT_FS[retrieve(file_index, 1)];
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  state.counters["ams_space"] = ams.size();
  state.counters["ams_estimate(3)"] = ams_estimates[3];
  state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  // state.counters["hss_space"] = hss.size();
  // state.counters["hss_estimate(3)"] = hss_estimates[3];
  // state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  // state.counters["hss_heap_size_at_each_level"] = hss.A;
  // state.counters["hss_levels"] = hss.L;
  // state.counters["hss_CS_depth"] = hss.d;
  // state.counters["hss_CS_width"] = hss.w;

  // state.counters["recursive_space"] = recursive.size();
  // state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  // state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  // state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  // state.counters["recursive_phi"] = recursive.phi;
  // state.counters["recursive_CS_depth"] = recursive.d;
  // state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_11_ams_2pass_wrong_exact_stream_length)
//   ->Arg(0)->Arg(1)
//   ->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)
//   // ->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(11)
//   // ->Arg(12)->Arg(13)->Arg(14)->Arg(15)->Arg(16)
//   // ->Arg(17)->Arg(18)-> Arg(19)
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);










static void fn_12_hss_recursive_wrong_n(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(17, 0)], // Kosarak
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = default_eps,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  // double ams_estimates[11];
  double hss_estimates[11];
  double recursive_estimates[11];

  std::ifstream infile(filename);

  // AMSSketch ams = AMSSketch(eps, del, n, k, F1);
  HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        // ams.add(a, 1);
        hss.add(a, 1);
        recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        // ams.add(a, b);
        hss.add(a, b);
        recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 4; i++) {
    // ams_estimates[i] = ams.estimate(i);
    hss_estimates[i] = hss.estimate(i);
    recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["actual_n"] = INPUT_FS[retrieve(file_index, 0)];
  state.counters["given_n"] = n;
  state.counters["F1"] = F1;
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  // state.counters["ams_space"] = ams.size();
  // state.counters["ams_estimate(3)"] = ams_estimates[3];
  // state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  // state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  state.counters["hss_space"] = hss.size();
  state.counters["hss_estimate(3)"] = hss_estimates[3];
  state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_12_hss_recursive_wrong_n)
//   ->Arg(0)->Arg(1)
//   ->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)
//   ->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(11)
//   // ->Arg(12)->Arg(13)->Arg(14)->Arg(15)->Arg(16)
//   ->Arg(17)
//   // ->Arg(18)-> Arg(19)
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);







static void fn_13_hss_recursive_wrong_F1(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = INPUT_FS[retrieve(file_index, 0)],
    F1 = 500000;
  float
    eps = default_eps,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  // double ams_estimates[11];
  double hss_estimates[11];
  double recursive_estimates[11];

  std::ifstream infile(filename);

  // AMSSketch ams = AMSSketch(eps, del, n, k, F1);
  HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        // ams.add(a, 1);
        hss.add(a, 1);
        recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        // ams.add(a, b);
        hss.add(a, b);
        recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 4; i++) {
    // ams_estimates[i] = ams.estimate(i);
    hss_estimates[i] = hss.estimate(i);
    recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["given_F1"] = F1;
  state.counters["actual_F1"] = INPUT_FS[retrieve(file_index, 1)];
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  // state.counters["ams_space"] = ams.size();
  // state.counters["ams_estimate(3)"] = ams_estimates[3];
  // state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  // state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  state.counters["hss_space"] = hss.size();
  state.counters["hss_estimate(3)"] = hss_estimates[3];
  state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_13_hss_recursive_wrong_F1)
//   ->Arg(0)->Arg(1)
//   ->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)
//   ->Arg(7)->Arg(8)->Arg(9)->Arg(10)->Arg(11)
//   // ->Arg(12)->Arg(13)->Arg(14)->Arg(15)->Arg(16)
//   ->Arg(17)
//   // ->Arg(18)-> Arg(19)
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);






static void fn_14_ams_2pass_update_time(benchmark::State& state) {
  long long
    s1 = state.range(1),
    s2 = state.range(2);

  long stream_length = 1000*state.range(0);
  AMSSketch ams = AMSSketch(s1, s2, stream_length);

  for (auto _ : state)
    for (int j=0; j<1000; j++)
      ams.add(state.range(0), state.range(0));
    
  state.counters["s1"] = s1;
  state.counters["s2"] = s2;
  state.counters["v"] = state.range(0);
  state.counters["ams_space"] = ams.size();
  state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

}

// BENCHMARK(fn_14_ams_2pass_update_time)
//   ->Args({1, 5, 20})->Args({1, 20, 20})->Args({1, 50, 20})->Args({1, 100, 20})->Args({1, 150, 20})->Args({1, 200, 20})
//   ->Args({1, 350, 20})->Args({1, 500, 20})->Args({1, 1000, 20})
//   ->Args({2, 5, 20})->Args({2, 20, 20})->Args({2, 50, 20})->Args({2, 100, 20})->Args({2, 150, 20})->Args({2, 200, 20})
//   ->Args({5, 5, 20})->Args({5, 20, 20})->Args({5, 50, 20})->Args({5, 100, 20})->Args({5, 150, 20})->Args({5, 200, 20})
//   ->Args({10, 5, 20})->Args({10, 20, 20})->Args({10, 50, 20})->Args({10, 100, 20})->Args({10, 150, 20})->Args({10, 200, 20})
//   ->Args({20, 5, 20})->Args({20, 20, 20})->Args({20, 50, 20})->Args({20, 100, 20})->Args({20, 150, 20})->Args({20, 200, 20})
//   ->Args({50, 5, 20})->Args({50, 20, 20})->Args({50, 50, 20})->Args({50, 100, 20})->Args({50, 150, 20})->Args({50, 200, 20})
//   ->Repetitions(REPETITIONS)
//   ->Unit(benchmark::kMillisecond);








static void fn_15_hss_recursive_real_dists(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = 100,
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = (float) state.range(1) / 100,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  double ams_estimates[11];
  double hss_estimates[11];
  double recursive_estimates[11];

  std::ifstream infile(filename);

  // AMSSketch
  //   ams = AMSSketch(40*state.range(1), 5, F1);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  int multiplier = state.range(1);
  int levels = multiplier;
  int w = 200;
  int d = 5;
  int A = 100;
  HSampleSketch hss = HSampleSketch((int) levels, w, d, A, domain);
  RecursiveSketch recursive = RecursiveSketch((int) levels, w, d, A, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        // ams.add(a, 1);
        hss.add(a, 1);
        recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        // ams.add(a, b);
        hss.add(a, b);
        recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 11; i++) {
    // ams_estimates[i] = ams.estimate(i);
    hss_estimates[i] = hss.estimate(i);
    recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  // state.counters["ams_space"] = ams.size();
  // state.counters["ams_estimate(3)"] = ams_estimates[3];
  // state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  // state.counters["ams_error(4)"] = epsilon_error(ams_estimates[4], file_index, 4);
  // state.counters["ams_error(5)"] = epsilon_error(ams_estimates[5], file_index, 5);
  // state.counters["ams_error(6)"] = epsilon_error(ams_estimates[6], file_index, 6);
  // state.counters["ams_error(7)"] = epsilon_error(ams_estimates[7], file_index, 7);
  // state.counters["ams_error(8)"] = epsilon_error(ams_estimates[8], file_index, 8);
  // state.counters["ams_error(9)"] = epsilon_error(ams_estimates[9], file_index, 9);
  // state.counters["ams_error(10)"] = epsilon_error(ams_estimates[10], file_index, 10);
  // state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  state.counters["hss_space"] = hss.size();
  state.counters["hss_estimate(3)"] = hss_estimates[3];
  state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  state.counters["hss_error(4)"] = epsilon_error(hss_estimates[4], file_index, 4);
  state.counters["hss_error(5)"] = epsilon_error(hss_estimates[5], file_index, 5);
  state.counters["hss_error(6)"] = epsilon_error(hss_estimates[6], file_index, 6);
  state.counters["hss_error(7)"] = epsilon_error(hss_estimates[7], file_index, 7);
  state.counters["hss_error(8)"] = epsilon_error(hss_estimates[8], file_index, 8);
  state.counters["hss_error(9)"] = epsilon_error(hss_estimates[9], file_index, 9);
  state.counters["hss_error(10)"] = epsilon_error(hss_estimates[10], file_index, 10);
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_error(4)"] = epsilon_error(recursive_estimates[4], file_index, 4);
  state.counters["recursive_error(5)"] = epsilon_error(recursive_estimates[5], file_index, 5);
  state.counters["recursive_error(6)"] = epsilon_error(recursive_estimates[6], file_index, 6);
  state.counters["recursive_error(7)"] = epsilon_error(recursive_estimates[7], file_index, 7);
  state.counters["recursive_error(8)"] = epsilon_error(recursive_estimates[8], file_index, 8);
  state.counters["recursive_error(9)"] = epsilon_error(recursive_estimates[9], file_index, 9);
  state.counters["recursive_error(10)"] = epsilon_error(recursive_estimates[10], file_index, 10);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_15_hss_recursive_real_dists)
// //   ->Args({0, 1})->Args({1, 1})->Args({2, 1})->Args({3, 1})
// //   ->Args({4, 1})->Args({5, 1})->Args({6, 1})->Args({7, 1})
// //   ->Args({8, 1})->Args({11, 1})
// //   ->Args({17, 1})->Args({18, 1})
// //   ->Args({0, 2})->Args({1, 2})->Args({2, 2})->Args({3, 2})
// //   ->Args({4, 2})->Args({5, 2})->Args({6, 2})->Args({7, 2})
// //   ->Args({8, 2})->Args({11, 2})
// //   ->Args({17, 2})->Args({18, 2})
// // // //   ->Args({2, 2})->Args({2, 3})->Args({2, 5})
// // // //   ->Args({2, 10})->Args({2, 15})->Args({2, 20})
// // // //   ->Args({2, 25})->Args({2, 30})->Args({2, 35})->Args({2, 40})
// // // //   ->Args({3, 2})->Args({3, 3})->Args({3, 5})
// // // //   ->Args({3, 10})->Args({3, 15})->Args({3, 20})
// // // //   ->Args({3, 25})->Args({3, 30})->Args({3, 35})->Args({3, 40})
//   ->Args({2, 2})->Args({2, 4})->Args({2, 6})->Args({2, 8})->Args({2, 10})
//   ->Args({2, 12})->Args({2, 14})->Args({2, 16})->Args({2, 18})->Args({2, 20})
//   ->Args({2, 22})->Args({2, 24})->Args({2, 26})->Args({2, 28})->Args({2, 30})
//   ->Args({17, 2})->Args({17, 4})->Args({17, 6})->Args({17, 8})->Args({17, 10})
//   ->Args({17, 12})->Args({17, 14})->Args({17, 16})->Args({17, 18})->Args({17, 20})
//   ->Args({17, 22})->Args({17, 24})->Args({17, 26})->Args({17, 28})->Args({17, 30})
  
// // // //   ->Args({3, 25})->Args({3, 30})->Args({3, 35})->Args({3, 40})
//   ->Repetitions(10)
//   ->Unit(benchmark::kMillisecond);





static void fn_16_ams_hss_recursive_real_dists(benchmark::State& state) {
  int
    file_index = state.range(0);
  short
    k = default_k;
  long long
    domain = default_domain,
    n = 100,
    F1 = INPUT_FS[retrieve(file_index, 1)];
  float
    eps = (float) state.range(1) / 100,
    del = default_del;

  string filename = INPUT_FILES[file_index];

  double ams_estimates[11];
  double hss_estimates[11];
  double recursive_estimates[11];

  std::ifstream infile(filename);

  // AMSSketch
  //   ams = AMSSketch(40*state.range(1), 5, F1);
  // HSampleSketch hss = HSampleSketch(eps, del, F1, n, k, domain);
  long long s1 = state.range(1);
  int s2 = 5;
  int levels = 25;
  int d = 5;
  int w = 2 * ((float) s1) / (float) levels;
  int A = w / 2.0;
  // AMSSketch ams = AMSSketch(s1, s2);
  HSampleSketch hss = HSampleSketch((int) levels, w, d, A, domain);
  RecursiveSketch recursive = RecursiveSketch((int) levels, w, d, A, domain);
  // RecursiveSketch recursive = RecursiveSketch(eps, del, n, k, 0.1);

  for (auto _ : state) {

    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        // ams.add(a, 1);
        hss.add(a, 1);
        recursive.add(a, 1);
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        // ams.add(a, b);
        hss.add(a, b);
        recursive.add(a, b);
      }
    }
  }

  for (short i=3; i < 11; i++) {
    // ams_estimates[i] = ams.estimate(i);
    hss_estimates[i] = hss.estimate(i);
    recursive_estimates[i] = recursive.estimate(i);
  }

  state.counters["n"] = n;
  state.counters["F1"] = F1;
  state.counters["eps"] = eps;
  state.counters["del"] = del;
  state.counters["file_index"] = file_index;

  // state.counters["ams_space"] = ams.size();
  // state.counters["ams_estimate(3)"] = ams_estimates[3];
  // state.counters["ams_error(3)"] = epsilon_error(ams_estimates[3], file_index, 3);
  // state.counters["ams_error(4)"] = epsilon_error(ams_estimates[4], file_index, 4);
  // state.counters["ams_error(5)"] = epsilon_error(ams_estimates[5], file_index, 5);
  // state.counters["ams_error(6)"] = epsilon_error(ams_estimates[6], file_index, 6);
  // state.counters["ams_error(7)"] = epsilon_error(ams_estimates[7], file_index, 7);
  // state.counters["ams_error(8)"] = epsilon_error(ams_estimates[8], file_index, 8);
  // state.counters["ams_error(9)"] = epsilon_error(ams_estimates[9], file_index, 9);
  // state.counters["ams_error(10)"] = epsilon_error(ams_estimates[10], file_index, 10);
  // state.counters["number_of_ams_estimators"] = ams.numberOfEstimators();

  state.counters["hss_space"] = hss.size();
  state.counters["hss_estimate(3)"] = hss_estimates[3];
  state.counters["hss_error(3)"] = epsilon_error(hss_estimates[3], file_index, 3);
  state.counters["hss_error(4)"] = epsilon_error(hss_estimates[4], file_index, 4);
  state.counters["hss_error(5)"] = epsilon_error(hss_estimates[5], file_index, 5);
  state.counters["hss_error(6)"] = epsilon_error(hss_estimates[6], file_index, 6);
  state.counters["hss_error(7)"] = epsilon_error(hss_estimates[7], file_index, 7);
  state.counters["hss_error(8)"] = epsilon_error(hss_estimates[8], file_index, 8);
  state.counters["hss_error(9)"] = epsilon_error(hss_estimates[9], file_index, 9);
  state.counters["hss_error(10)"] = epsilon_error(hss_estimates[10], file_index, 10);
  state.counters["hss_heap_size_at_each_level"] = hss.A;
  state.counters["hss_levels"] = hss.L;
  state.counters["hss_CS_depth"] = hss.d;
  state.counters["hss_CS_width"] = hss.w;

  state.counters["recursive_space"] = recursive.size();
  state.counters["recursive_estimate(3)"] = recursive_estimates[3];
  state.counters["recursive_error(3)"] = epsilon_error(recursive_estimates[3], file_index, 3);
  state.counters["recursive_error(4)"] = epsilon_error(recursive_estimates[4], file_index, 4);
  state.counters["recursive_error(5)"] = epsilon_error(recursive_estimates[5], file_index, 5);
  state.counters["recursive_error(6)"] = epsilon_error(recursive_estimates[6], file_index, 6);
  state.counters["recursive_error(7)"] = epsilon_error(recursive_estimates[7], file_index, 7);
  state.counters["recursive_error(8)"] = epsilon_error(recursive_estimates[8], file_index, 8);
  state.counters["recursive_error(9)"] = epsilon_error(recursive_estimates[9], file_index, 9);
  state.counters["recursive_error(10)"] = epsilon_error(recursive_estimates[10], file_index, 10);
  state.counters["recursive_heap_size_at_each_level"] = recursive.A;
  state.counters["recursive_phi"] = recursive.phi;
  state.counters["recursive_CS_depth"] = recursive.d;
  state.counters["recursive_CS_width"] = recursive.w;

}

// BENCHMARK(fn_16_ams_hss_recursive_real_dists)
//   // ->Args({2, 100})->Args({2, 200})->Args({2, 400})->Args({2, 600})->Args({2, 800})
//   // ->Args({2, 1000})->Args({2, 1200})->Args({2, 1400})
//   // ->Args({2, 1700})->Args({2, 2000})
//   // ->Args({17, 100})->Args({17, 200})->Args({17, 400})->Args({17, 600})->Args({17, 800})
//   // ->Args({17, 1000})->Args({17, 1200})->Args({17, 1400})
//   // ->Args({17, 1700})->Args({17, 2000})
//   // ->Args({18, 1200})->Args({18, 1400})
//   // ->Args({18, 10000000})
//   ->Args({18, 50000})
//   ->Args({18, 50000})
//   ->Args({18, 50000})
//   // ->Args({18, 60000})->Args({18, 40000})
//   // ->Args({18, 20000})->Args({18, 10000})
//   ->Repetitions(1)
//   ->Unit(benchmark::kMillisecond);




BENCHMARK_MAIN();
