#include <vector>

class AMS {
  private:
    long long numberOfTracked;
    long long tracked;
    bool keep_tracking_same;
  public:
    AMS();
    AMS(long long streamLength);
    void add(long long i, long long v, long long streamLength);
    double estimate(short k, long long streamLength);
    long size() {
      return sizeof(numberOfTracked) + sizeof(tracked);
    }
};

class AMSSketch {
  private:
    long long streamLength;
  public:
    std::vector< std::vector<AMS> > estimators;
    AMSSketch(float eps, float del, long long n, short k);
    AMSSketch(long long s1, int s2);
    AMSSketch(float eps, float del, long long n, short k, long long exactStreamLength);
    AMSSketch(long long s1, int s2, long long exactStreamLength);
    void add(long long i, long long v);
    double estimate(short k);
    long long numberOfEstimators() {
      return estimators.size() * estimators[0].size();
    }
    long size() {
      long estimatorsSize = sizeof(estimators[0][0]) * estimators.size() * estimators[0].size();
      return sizeof(streamLength) + estimatorsSize;
    }
};
