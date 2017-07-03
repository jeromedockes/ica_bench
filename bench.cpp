#include <iostream>

#include "bench_utils.h"


int main(int argc, char* argv[])
{
  if (argc != 3){
    std::cout << "Usage: bench <data directory> <results directory>\n";
    return 1;
  }

  bench::bench_directory(argv[1], argv[2]);
  return 0;
}
