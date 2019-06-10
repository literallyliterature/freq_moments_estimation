class SimpleSketch {
  private:
    map <long long, long long>
      simple_sketch;
  public:
    SimpleSketch() {}
    void add(long long i, long long v) {
      map<long long, long long>::iterator existing = simple_sketch.find(i);
      if (existing != simple_sketch.end())
        existing->second += v;
      else
        simple_sketch.emplace(i, v);
    }

    double estimate(short k) {
      double sum = 0;
      for (const auto & item : simple_sketch)
        sum += abs(pow(item.second, k));
      return sum;
    }
    long size() {
      return simple_sketch.size() * (sizeof(pair<long long, long long>) + 2*sizeof(long long));
    }
};
