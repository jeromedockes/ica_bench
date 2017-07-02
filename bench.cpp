#include <iostream>

#include "bench_utils.h"

int main(int argc, char* argv[])
{
  if (argc != 2){
    std::cout << "specify data file\n";
    return 1;
  }
  bench::bench_directory(argv[1], "/tmp/bench_results");
  return 0;
}
