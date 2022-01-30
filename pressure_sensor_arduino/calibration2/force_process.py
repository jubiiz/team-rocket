from matplotlib import pyplot as plt
import numpy as np
import math

def num_to_force(num):
    """
    formula used to convert from the raw arduino output to a force in grams
    """
    force = (math.e**(0.009*num))
    return(force)


def main():
    """
    loads data from "fire_data", plots it
    """
    rockets = [[[], [], []], [[], [], []], [[], [], []]] # a 3x3x3 list to put data for each sensor for each rocket size

    # load all data
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

    # plot the raw sensor data
    x = np.linspace(0, 2990, (300))
    fig, (small_ax, med_ax, big_ax) = plt.subplots(3)
    for i in range(3):
        small_ax.plot(x, rockets[0][i][:300])
    for i in range(3):
        med_ax.plot(x, rockets[1][i][:300])
    for i in range(3):
        big_ax.plot(x, rockets[2][i][:300])

    # calculate and plot the force data
    forces = [[], [], []]
    for i in range(3):
        for j in range(3):
            for k in range(300):
                data = rockets[i][j][k]
                if j == 0:
                    forces[i].append(0)
                forces[i][k] += num_to_force(data)
    fig, (small_force, med_force, big_force) = plt.subplots(3)

    small_force.plot(x, forces[0][:300])

    med_force.plot(x, forces[1][:300])

    big_force.plot(x, forces[2][:300])
    plt.show()

if __name__ == "__main__":
    main()