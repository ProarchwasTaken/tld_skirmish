import matplotlib.pyplot as plt
from sys import argv
from matplotlib.lines import Line2D


"""This script doesn't do anything directly to the game itself. It's made
to test and debug the formula which is used to calculate the window of
time of which a parry could be performed."""
def main():
    if len(argv) > 1:
        calculateResult()
    else:
        generatePlot();


def calculateResult():
    print("Command line argument detected")
    guard_pierce = float(argv[1])

    result = formula(guard_pierce)
    result = clamp(parry_window=result)
    print(f"Calulation Result: {result}")


def generatePlot():
    percentage: list[float] = [x / 100 for x in range(100)]
    parry_window: list[float] = list()

    for guard_pierce in percentage:
        result: float = formula(guard_pierce)
        result = clamp(parry_window=result)

        parry_window.append(result)

    line: Line2D = plt.plot(percentage, parry_window)[0]
    print(line.get_ydata())

    plt.xlabel("Guard Pierce")
    plt.ylabel("Parry Window")

    plt.show()


def formula(guard_pierce: float) -> float:
    default_window: float = 0.20
    subtrahend: float = (2 * (guard_pierce ** 2)) / 5

    return default_window - subtrahend;


def clamp(parry_window: float) -> float:
    minimum_window: float = 0.05

    if parry_window < minimum_window:
        parry_window = minimum_window

    return parry_window


if __name__ == "__main__":
    main()
