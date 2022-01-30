import tensorflow as tf
from pyexpat import model
import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

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

def normalize_data(labels, x_train):
    n_labels = []
    n_x_train = []

    l_denom = 2000
    x_denom = 1024

    for label in labels:
        n_labels.append(label/l_denom)

    for triplet in x_train:
        n_triplet = []
        for data in triplet:
            n_triplet.append(data/x_denom)
        n_x_train.append(n_triplet)

    return(n_labels, n_x_train)

def get_trained_model(x, y):
    model = tf.keras.models.Sequential()
    model.add(tf.keras.layers.Flatten(input_shape=(3,)))
    model.add(tf.keras.layers.Dropout(0.1, input_shape=(3,)))
    model.add(tf.keras.layers.Dense(units =64))
    model.add(tf.keras.layers.Dense(units =64))
    model.add(tf.keras.layers.Dense(units=64))
    model.add(tf.keras.layers.Dense(units=64))
    model.add(tf.keras.layers.Dense(units=1, activation='relu'))

    # compiler and optimizer hyperparameters from: https://stackoverflow.com/questions/44843581/what-is-the-difference-between-model-fit-an-model-evaluate-in-keras#:~:text=fit%20%28%29%20is%20for%20training%20the%20model%20with,loss%20value%20and%20metrics%20values%20for%20the%20model.
    model.compile(loss="mse", optimizer="adam")
    model.fit(x, y, epochs=300)
    model.evaluate(x, y)  

    return(model)

def plot_fire_force(x_data, labels, model):
    numx = 300
    fig, (ax1, ax2) = plt.subplots(2, 1)

    if len(x_data[0]) < 300:
        numx = len(x_data[0])
        x = np.linspace(0, 3000, numx)
        ax2.plot(x, labels)
    x = np.linspace(0, 3000, numx)



    _, n_x_data = normalize_data([], x_data)
    weights = []
    sensor_values = []
    for i in range(len(x_data[0])):
        sensor_values.append([x_data[0][i], x_data[1][i], x_data[2][i]])
        weights.append((model.predict([[n_x_data[0][i], n_x_data[1][i], n_x_data[2][i]]]))[0][0]*2000)



    for i in range(3):
        ax1.plot(x, x_data[i][:300])
        plt.legend([f"sensor {i}"])
    ax2.plot(x, weights[:300]) 
   
    plt.legend(["real weight", "estimated thrust"])

    plt.title("estimated motor thrust (g) and sensor values over time (ms)")

    plt.show()    


def main():
    filename = "cleaned_data.csv"

    labels, x_train = load_data(filename)
    n_labels, n_x_train = normalize_data(labels, x_train)

    graph_data(labels, x_train)

    model = get_trained_model(n_x_train, n_labels)
    model.save("calibration2.h5")

    model = tf.keras.models.load_model("calibration2.h5")

    r0, r1, r2 = load_fire_data()

    rs_x = [[], [], []]
    x_train.sort(key=lambda d: d[0])
    for triplet in x_train:
        for i in range(3):
           rs_x[i].append(triplet[i])


    plot_fire_force(rs_x, labels, model)

    plot_fire_force(r0, labels, model)

    plot_fire_force(r1, labels, model)

    plot_fire_force(r2, labels, model)




if __name__ == "__main__":
    main()