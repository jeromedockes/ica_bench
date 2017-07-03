#ifndef ICA_BENCH_BENCH_H
#define ICA_BENCH_BENCH_H

#include <chrono>

#include <boost/filesystem.hpp>

#include <armadillo>

namespace bench
{

  namespace filesystem = boost::filesystem;


  class ICA_trace{
    
  public:

    ICA_trace(const filesystem::path& output_dir);

    void reset_time();

    void operator() (size_t iteration, const arma::mat& weights) const;

    // raw_weights must be in column-major order
    void operator() (size_t iteration, const double* raw_weights,
                     size_t n_rows, size_t n_cols) const;

  private:

    filesystem::path output_dir_;
    std::chrono::system_clock::time_point start_time_;
  };


  void bench_file(const filesystem::path& data_file,
                  const filesystem::path& results_dir);


  void bench_directory(const filesystem::path& data_dir,
                       const filesystem::path& results_dir);

}
#endif
