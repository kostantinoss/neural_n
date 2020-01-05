import matplotlib.pyplot as plt
import numpy as np

file = open("clusters.txt")

array = np.loadtxt(file, delimiter=',', usecols=(0,1))

plt.scatter(array[:,0], array[:,1], s=10, marker="+")
plt.show()
