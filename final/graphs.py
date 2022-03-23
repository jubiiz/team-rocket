import pandas as pd
import numpy as np
from matplotlib import pyplot as plt

# Time (ms),Temperature (deg C),Pressure (hpa),Humidity (%),Altitude (m),ax (m/s²),ay (m/s²),az (m/s²),gx (rad/s),gy (rad/s),gz (rad/s)

def plot_env(df):
    fig, ax = plt.subplots(2, 2)
    print(ax[0][0])
    ax[0][0].plot(df["Time (ms)"], df["Temperature (deg C)"])
    ax[0][0].set_title("Temperature (deg C)")

    ax[0][1].plot(df["Time (ms)"], df["Pressure (hpa)"])
    ax[0][1].set_title("Pressure (hpa")

    ax[1][0].plot(df["Time (ms)"], df["Humidity (%)"])
    ax[1][0].set_title("Humidity (%)")

    ax[1][1].plot(df["Time (ms)"], df["Altitude (m)"])
    ax[1][1].set_title("Altitude (m)")

def plot_mpu(df):
    fig, ax = plt.subplots(2, 3)
    ax[0][0].plot(df["Time (ms)"], df["ax (m/s²)"])
    ax[0][0].set_title("ax (m/s²)")

    ax[0][1].plot(df["Time (ms)"], df["ay (m/s²)"])
    ax[0][1].set_title("ay (m/s²)")

    ax[0][2].plot(df["Time (ms)"], df["az (m/s²)"])
    ax[0][2].set_title("az (m/s²)")

    ax[1][0].plot(df["Time (ms)"], df["gx (rad/s)"])
    ax[1][0].set_title("gx (rad/s)")

    ax[1][1].plot(df["Time (ms)"], df["gy (rad/s)"])
    ax[1][1].set_title("gy (rad/s)")

    ax[1][2].plot(df["Time (ms)"], df["gz (rad/s)"])
    ax[1][2].set_title("gz (rad/s)")



def main():
    data_filename = "pre.csv"
    full_df = pd.read_csv(data_filename)
    plot_env(full_df)
    plot_mpu(full_df)
    plt.show()

if __name__ == "__main__":
    main()
