#include <iostream>

#include "neo_ica/ica.h"

#include "bench_utils.h"

namespace bench
{


  ICA_trace::ICA_trace(const filesystem::path& output_dir)
    :output_dir_{output_dir}, start_time_{std::chrono::system_clock::now()}{
        filesystem::create_directories(output_dir_);
  }


  void ICA_trace::reset_time()
  {
    start_time_ = std::chrono::system_clock::now();
  }


  void ICA_trace::operator() (size_t iteration, const arma::mat& weights) const
  {
    auto time = std::chrono::duration_cast<
      std::chrono::milliseconds>(std::chrono::system_clock::now()
                                 - start_time_).count();

    std::ostringstream file_name{};
    file_name << "iteration_" << iteration
              << "_time_" << time << "_milliseconds.csv";

    auto file_path = output_dir_ / file_name.str();
    weights.save(file_path.string(), arma::csv_ascii);
    std::cout << "iteration " << iteration << " -> "
              << file_path.string() << "\n";
  }


  void ICA_trace::operator() (size_t iteration, const double* raw_weights,
                              size_t n_rows, size_t n_cols) const
  {
    arma::mat weights(raw_weights, n_rows, n_cols);
    this->operator()(iteration, weights);
  }


  void bench_file(const filesystem::path& data_file,
                  const filesystem::path& results_dir)
  {
    std::cout << "computing ICA for " << data_file.string() << "\n";
    arma::mat data{};
    data.load(data_file.string(), arma::csv_ascii);
    arma::mat weights(data.n_cols, data.n_cols);
    arma::mat sphere(data.n_cols, data.n_cols);
    std::ostringstream output_dir_name{};
    output_dir_name << "iterations_for_" << data_file.stem().string();
    auto output_dir = results_dir / output_dir_name.str();
    if (filesystem::is_directory(output_dir)){
      std::cerr << "results directory already exists: "
                << output_dir.string() << "\n"
                << "results would get mixed up: stop\n";
      return;
    }
    filesystem::create_directories(output_dir);
    data.save((output_dir / "data.csv").string(), arma::csv_ascii);
    ICA_trace trace(output_dir);
    neo_ica::ica(data.memptr(), weights.memptr(), sphere.memptr(),
                 data.n_cols, data.n_rows, neo_ica::options(), trace);
    std::cout << "results in " << output_dir.string() << "\n";
  }


  void bench_directory(const filesystem::path& data_dir,
                       const filesystem::path& results_dir)
  {
    for (auto file_path: filesystem::directory_iterator(data_dir)){
      bench_file(file_path, results_dir);
    }
    std::cout << "\nresults in " << results_dir.string() << "\n";
  }

}
