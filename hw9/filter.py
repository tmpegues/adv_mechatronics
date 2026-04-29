import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

files = ['sigA', 'sigB', 'sigC', 'sigD']

fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2,2)
axes = [ax1, ax2, ax3, ax4]
for i in range(len(files)):
    t = [] # column 0
    data1 = [] # column 1

    with open('./dsp/'+files[i]+'.csv') as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            data1.append(float(row[1])) # second column

    axes[i].plot(t,data1,'b-*')
    axes[i].set_title(files[i])
    axes[i].set_xlabel('Time [s]')
    axes[i].set_ylabel('Signal')
plt.show()


