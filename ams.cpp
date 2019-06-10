#include "ams.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "utils/utils.h"

AMS::AMS() {
  numberOfTracked = 0;
  tracked = -1;
  keep_tracking_same = false;
}

AMS::AMS(long long streamLength) {
  numberOfTracked = -1;
  // tracked now stores the streamLength from which to start tracking
  tracked = randint(streamLength, 1);
  keep_tracking_same = true;
}

void AMS::add(long long i, long long v, long long streamLength) {
  if (keep_tracking_same) {
    if (numberOfTracked == -1) {
      if (streamLength < tracked && tracked <= streamLength + v) {
        numberOfTracked = streamLength + v - tracked + 1;
        tracked = i;
      }
    } else if (i == tracked) {
      numberOfTracked += v;
    }
  } else {
    for (long long j=0; j < v; j++) {
      int randomEvent = randint(streamLength + j);
      if (randomEvent == 0) {
        tracked = i;
        numberOfTracked = 1;
      } else if (tracked == i) numberOfTracked += 1;
    }
  }
}

double AMS::estimate(short k, long long streamLength) {
  return streamLength * (std::pow(numberOfTracked, k) - std::pow(numberOfTracked - 1, k));
}

AMSSketch::AMSSketch(float eps, float del, long long n, short k) {
  streamLength = 0;
  long long s1 = 8 * k * pow(n, 1-1/k) / (eps*eps);
  // s1 = s1 < (2<<10) ? s1 : (2<<10);
  int s2 = (int)ceil(2 * std::log(1/del));
  // cout << "eps: " << eps << endl;
  // cout << "Initialising " << s1 * s2 << " AMS Sketches" << endl;
  // cout << "s1 = " << 8 * k * pow(n, 1-1/k) / (eps*eps) << endl;
  for (int p=0; p < s2; p++) {
    vector<AMS> row_estimators;
    for (long long q=0; q < s1; q++)
      row_estimators.push_back(AMS());
    estimators.push_back(row_estimators);
  }
  // cout << "Initialised" << endl;
}

AMSSketch::AMSSketch(float eps, float del, long long n, short k, long long exactStreamLength) {
  streamLength = 0;
  long long s1 = 8 * k * pow(n, 1-1/k) / (eps*eps);
  int s2 = (int)ceil(2 * std::log(1/del));
  for (int p=0; p < s2; p++) {
    vector<AMS> row_estimators;
    for (long long q=0; q < s1; q++)
      row_estimators.push_back(AMS(exactStreamLength));
    estimators.push_back(row_estimators);
  }
}

AMSSketch::AMSSketch(long long s1, int s2) {
  streamLength = 0;
  for (int p=0; p < s2; p++) {
    vector<AMS> row_estimators;
    for (long long q=0; q < s1; q++)
      row_estimators.push_back(AMS());
    estimators.push_back(row_estimators);
  }
}

AMSSketch::AMSSketch(long long s1, int s2, long long exactStreamLength) {
  streamLength = 0;
  for (int p=0; p < s2; p++) {
    vector<AMS> row_estimators;
    for (long long q=0; q < s1; q++)
      row_estimators.push_back(AMS(exactStreamLength));
    estimators.push_back(row_estimators);
  }
}

void AMSSketch::add(long long i, long long v) {
  for (std::vector<AMS> & row : estimators)
    for (AMS & ams : row)
      ams.add(i, v, streamLength);
  streamLength += v;
}

double AMSSketch::estimate(short k) {
  std::vector<double> means;

  for (std::vector<AMS> & row : estimators) {
    std::vector<double> rowResults;
    for (AMS & ams : row) {
      double s = ams.estimate(k, streamLength);
      if (s != 0)
        rowResults.push_back(s);
    }

    double sum = 0;
    for (double s : rowResults)
      sum += s;
    means.push_back((double) sum / (rowResults.size()));
  }

  return median(means);
}

// int main() {
//   // Seed with current time
//   std::srand(std::time(0));
//   AMSSketch sketch (0.5, 0.5, 100, 3);
//   std::cout << sketch.estimate(1) << std::endl;

//   sketch.add(5, 5);
//   std::cout << sketch.estimate(1) << std::endl;

//   // long long random_v = randint(higher, 0);
//   // long long random_i = randint(domain, 0);
//   // ss.add(random_i, random_v);
//   // ams.add(random_i, random_v);
//   return 0;
// }
