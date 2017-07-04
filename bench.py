import os
import glob

import numpy as np
import pandas as pd


def get_data(results_dir):
    data = np.loadtxt(os.path.join(results_dir, 'data.csv'), delimiter=',')
    return data


def get_iterations(results_dir):
    iteration_files = glob.glob(os.path.join(results_dir, 'iteration_*.csv'))
    iteration_files = pd.DataFrame(iteration_files, columns=['file'])
    details = iteration_files.file.str.extract(
        r'iteration_(?P<iteration>\d+)_time_'
        r'(?P<time_milliseconds>\d+)_milliseconds\.csv', expand=False)
    details['iteration'] = pd.to_numeric(details['iteration'])
    details['time_milliseconds'] = pd.to_numeric(details['time_milliseconds'])
    iteration_info = pd.concat([iteration_files, details], axis=1)
    iteration_info.sort_values('iteration', inplace=True)
    for index, row in iteration_info.iterrows():
        weights = np.loadtxt(row['file'], delimiter=',')
        yield dict(dict(row), weights=weights)
