import pandas as pd
import numpy as np
from matplotlib import pyplot as plt

def plot_env(df):
    fig, ax = plt.subplots(2, 2)
    df.plot("Time (ms)", "Temperature (deg C)")
    df.plot("Time (ms)", "Pressure (hpa)")
    df.plot("Time (ms)", "Humidity (%)")
    df.plot("Time (ms)", "Altitude (m)")

    

def main():
    data_filename = "pre.csv"
    full_df = pd.read_csv(data_filename)
    plot_env(full_df)
    #plot_mpu(data_filename)
    
    plt.show()

if __name__ == "__main__":
    main()
