import numpy as np
import matplotlib.pyplot as plt

def plot12():
    data = []
    
    for i in 
    
    N = len(data)
    x = np.arange(1, N+1)
    y = [num for (s, num) in data]
    labels = [s for (s, num) in data]
    width = 1
    plt.bar(x, y, width, color="y")
    plt.ylabel("Intensity")
    plt.xticks(x + width / 2, labels)
    plt.show()

if __name__ == "__main__":
    plot12()
