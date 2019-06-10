#include "hss.h"
#include "recursive.h"
#include "ams.h"
#include "simple_sketch.h"
#include <fstream>

std::ostream&
operator<<( std::ostream& dest, __uint128_t value )
{
    std::ostream::sentry s( dest );
    if ( s ) {
        __uint128_t tmp = value < 0 ? -value : value;
        char buffer[ 128 ];
        char* d = std::end( buffer );
        do
        {
            -- d;
            *d = "0123456789"[ tmp % 10 ];
            tmp /= 10;
        } while ( tmp != 0 );
        if ( value < 0 ) {
            -- d;
            *d = '-';
        }
        int len = std::end( buffer ) - d;
        if ( dest.rdbuf()->sputn( d, len ) != len ) {
            dest.setstate( std::ios_base::badbit );
        }
    }
    return dest;
}

string files[] = {
  "./distributions/uniform_100.txt",
  "./distributions/uniform_1k.txt",
  "./distributions/uniform_10k.txt",
  "./distributions/uniform_100k.txt",
  "./distributions/uniform_1M.txt",
  "./distributions/uniform_10M.txt",
  "./distributions/normal_0.05_10k.txt",
  "./distributions/normal_0.05_100k.txt",
  "./distributions/normal_0.05_1M.txt",
  "./distributions/normal_0.1_10k.txt",
  "./distributions/normal_0.1_100k.txt",
  "./distributions/normal_0.1_1M.txt",
  "./distributions/normal_0.25_10k.txt",
  "./distributions/normal_0.25_100k.txt",
  "./distributions/normal_0.25_1M.txt",
  "./distributions/normal_0.5_10k.txt",
  "./distributions/normal_0.5_100k.txt",
  "./distributions/normal_0.5_1M.txt",
  "./distributions/kosarak.dat",
  "./distributions/Webdocs.dat",
};

int main() {
  for (auto filename : files) {
    SimpleSketch
      ss;
    std::ifstream infile(filename);
    if (filename == "./distributions/Webdocs.dat" || filename == "./distributions/kosarak.dat") {
      long a;
      while (infile >> a) {
        ss.add(a, 1);
        // cout << "(" << a << ", 1)" << endl;
      }
    } else {
      long a, b;
      char c;
      while (infile >> a >> c >> b) {
        ss.add(a, b);
        // cout << "(" << a << ", " << b << ")" << endl;
      }
    }

    cout
      << filename
      << ',' << ss.estimate(0)
      << ',' << ss.estimate(1)
      << ',' << ss.estimate(2)
      << ',' << ss.estimate(3)
      << ',' << ss.estimate(4)
      << ',' << ss.estimate(5)
      << ',' << ss.estimate(6)
      << ',' << ss.estimate(7)
      << ',' << ss.estimate(8)
      << ',' << ss.estimate(9)
      << ',' << ss.estimate(10)
      << endl;
  }
}
