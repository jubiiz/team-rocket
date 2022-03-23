from matplotlib import pyplot as plt
import pandas as pd

def load_data(filename):
    df = pd.read_csv(filename)
    print(df.head())
    return(df)

def main():
    df = load_data("yeast0.csv")

    fig, axs = plt.subplots(2, 3)

    for index, line in enumerate(df):
        print(axs)
        ax = axs[index%2][index%3]
        ax.plot(list(range(143)), df[line])
        ax.set_title(line)

    plt.show()

if __name__ == "__main__":
    main()
