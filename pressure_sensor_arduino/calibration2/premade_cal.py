#import tensorflow as tf
import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

#######################
## calibration from https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr?fbclid=IwAR2HzFYEOMAhbUk6rGqZ-C22KE8T8YReQbQuDvUNlmbmpstJsAxHIiiBJzo
######################

def load_data(filename):
    """
    loads data from csv file "filename"
    """
    df = pd.read_csv(filename, header=None)
    labels = []
    triplets = []
    
    for i in range(len(df)):
        labels.append(int(df.iloc[i][0]))
        triplets.append(df.iloc[i][1:4].tolist())

    return(labels, triplets)

def load_fire_data():
    """
    data loading function specifically for the data log from the arduino dry-fire test
    """
    rockets = [[[], [], []], [[], [], []], [[], [], []]] # a 3x3x3 list to put data for each sensor for each rocket size

    with open("fire_data.txt", "r") as r:
        rocket_size = -1  
        for line in r:
            if line[0] == "-":
                rocket_size += 1
            elif len(line) == 0:
                continue
            else:
                line_data = line.split(",")
                for i in range(len(line_data)):
                    try:
                        force_data = float(line_data[i])
                        if i%2 == 0 and i<6:
                            rockets[rocket_size][i//2].append(force_data)
                    except Exception as e:
                        pass
                        #print(e.__class__)
    return(rockets[0], rockets[1], rockets[2])

def graph_data(labels, x_train):
    """
    data plotting function for calibration data
    """
    fig, ax = plt.subplots()
    sensors = [[], [], []]
    for item in x_train:
        for i in range(3):
            sensors[i].append(item[i])

    for i in range(3):
        ax.scatter(sensors[i], labels)

    plt.title("sensor output vs weight (g)")
    plt.legend(["sensor 1", "sensor 2", "sensor 3"])
    plt.show()


def calculate_force(readings):
    forces = []
    for reading in readings:
        if reading ==0:
            force = 0
        else:
            voltage = (reading*5000)/1023
            resistance = ((5000-voltage)*10000)/voltage
            conductance = 1000000/resistance

            if(conductance <=1000):
                force = conductance/80
            else:
                force = (conductance-1000)/30
        forces.append(force)

    total = 0
    for force in forces:
        total += force

    return(total/0.00981)


def plot_fire_force(x_data, labels):
    """
    plotting function that also plots weight prediction
    """
    numx = 300
    fig, (ax1, ax2) = plt.subplots(2, 1)

    # changes x axis parameters for calibration data only
    if len(x_data[0]) < 300:
        numx = len(x_data[0])
        x = np.linspace(0, 3000, numx)
        ax2.plot(x, labels, label="labels")
    x = np.linspace(0, 3000, numx)

    # makes a list of predicted weights
    weights = []
    for i in range(len(x_data[0])):
        weights.append(calculate_force([x_data[0][i], x_data[1][i], x_data[2][i]]))

    # plots the newly created data
    for i in range(3):
        ax1.plot(x, x_data[i][:300])
        plt.legend([f"sensor {i}"])
    ax2.plot(x, weights[:300]) 
   
    plt.legend(["real weight", "estimated thrust"])
    plt.title("estimated motor thrust (g) and sensor values over time (ms)")
    plt.show()    


def main():
    """
    imports sensor data from <filename>
    formats and normalizes data
    graphs raw sensor data
    optionnaly trains an ANN model on that data
    uses that model to graph predicted forces from new data
    """

    filename = "cleaned_data.csv"

    labels, x_train = load_data(filename)

    graph_data(labels, x_train)

    r0, r1, r2 = load_fire_data()

    # rearrange data 
    rs_x = [[], [], []]
    x_train.sort(key=lambda d: d[0])
    for triplet in x_train:
        for i in range(3):
           rs_x[i].append(triplet[i])

    # shows raw data and force estimation for the calibration data and rocket sizes 0 to 2 (small med big)
    plot_fire_force(rs_x, labels)

    plot_fire_force(r0, labels)

    plot_fire_force(r1, labels)

    plot_fire_force(r2, labels)




if __name__ == "__main__":
    main()