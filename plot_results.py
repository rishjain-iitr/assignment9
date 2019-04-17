import numpy as np
import pandas as pd
from matplotlib import pyplot as plt


page_fault_file = "result_fault_count.csv"
run_time_file = "result_avg_run_time.csv"

def plot_data(df):
    num_pages_values = df['num_pages'].unique()

    for num_pages in num_pages_values:
        temp_df = df[df['num_pages'] == int(num_pages)].drop(['num_pages'], axis = 1)
        plot_title = "Num pages = " + str(num_pages)
        temp_df.plot(x = 'num_frames', y = ['lru_counter', 'lru_stack', 'lru_aging', 'lru_clock'],
                kind = 'bar', title = plot_title)


# Plot page fault data.
df = pd.read_csv(page_fault_file)
plot_data(df)

# Plot avg algorithm run time data.
df = pd.read_csv(run_time_file)
plot_data(df)

plt.show()
