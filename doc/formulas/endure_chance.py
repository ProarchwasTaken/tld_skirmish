import matplotlib.pyplot as plt


def formula(morale_percent: float, life_percent: float) -> float:
    return round((morale_percent * life_percent) / 1.25, 3)


def clamp(percentage: float) -> float:
    if percentage < 0.0:
        return 0.0
    elif percentage > 1.0:
        return 1.0
    else:
        return percentage


def performCalulations(percentages: list[float], 
                       value: float) -> list[float]: 
    endure_chance: list[float] = list()

    for morale_percentage in percentages:
        result: float = clamp(formula(morale_percentage, value))
        endure_chance.append(result)

    return endure_chance


def createLines(life_values: list[float]) -> None:
    percentages: list[float] = [x / 100 for x in range(1, 100)]

    file = open("endure_data.txt", "w")

    for value in life_values:
        file.write(f"=====<Life Percentage: {value} >=====\n")
        file.write(f"20% Morale - {formula(0.2, value)}\n")
        file.write(f"50% Morale - {formula(0.5, value)}\n")
        file.write(f"100% Morale - {formula(1.0, value)}\n\n")

        endure_chance = performCalulations(percentages, value);
        plt.plot(percentages, endure_chance)[0]

    file.close();
    print("Data and results are saved in: endure_data.txt")


def main():
    createLines([0.03, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])

    plt.xlabel("Percentage")
    plt.ylabel("Endure Chance")
    plt.show()


if __name__ == "__main__":
    main()
