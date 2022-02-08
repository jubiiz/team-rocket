from matplotlib import pyplot as plt
import numpy as np
import math

def num_to_force(num):
    """
    formula used to convert from the raw arduino output to a force in grams
    """
    force = 4.5267*(math.e**(0.0055*num))
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
    x = np.linspace(0, 300, (300))
    fig, (small_ax, med_ax, big_ax) = plt.subplots(3)
    for i in range(3):
        small_ax.plot(x, rockets[0][i][:300])
    small_ax.set_title("A8-3 rocket")
    small_ax.set_ylabel("Output ±50%")

    for i in range(3):
        med_ax.plot(x, rockets[1][i][:300])
    med_ax.set_title("D12-5 rocket")
    med_ax.set_ylabel("Output ±50%")

    for i in range(3):
        big_ax.plot(x, rockets[2][i][:300])
    big_ax.set_title("E16-8 rocket")
    plt.ylabel("Output ±50%")
    plt.xlabel("Number of Data Entries")

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
    small_force.set_title("A8-3 rocket")
    small_force.set_ylabel("Output ±50%")

    med_force.plot(x, forces[1][:300])
    med_force.set_title("D12-5 rocket")
    med_force.set_ylabel("Output ±50%")

    big_force.plot(x, forces[2][:300])
    big_force.set_title("E16-8 rocket")
    big_force.set_ylabel("Output ±50%")
    big_force.set_xlabel("Number of Data Entries")

    plt.show()

if __name__ == "__main__":
    main()