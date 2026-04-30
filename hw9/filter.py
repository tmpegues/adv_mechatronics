import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

def plot_s_fft(ax_signal, ax_fft, file):
    """
    Plot the raw signal, filtered data (if wanted), and FFT

    Args:
    ----
    ax_signal (mpl axes): The axes to plot the signals on
    ax_fft (mpl axes): _The axes to plot the FFTs on
    file (str): Data source
    """
    t = [] # column 0
    signal = [] # column 1

    with open('./dsp/'+file+'.csv') as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            signal.append(float(row[1])) # second column

    n = len(signal) # length of the signal
    k = np.arange(n) # List of integer frequencies?
    T = n/len(t)
    frq1 = k/T
    frq = frq1[range(int(n/2))]
    Y = np.fft.fft(signal)/len(signal) # fft computing and normalization
    Y = Y[range(int(len(signal)/2))]

    ax_signal.plot(t, signal)
    ax_signal.set_title(file+' Signal')
    ax_signal.set_xlabel('Time [s]')
    ax_signal.set_ylabel('Signal')

    ax_fft.loglog(frq,abs(Y),'b') # plotting the fft
    ax_fft.set_title(file+' FFT')
    ax_fft.set_xlabel('Freq (Hz)')
    ax_fft.set_ylabel('|Y(freq)|')

def plot_averaged(ax_signal, ax_fft, file, average_length):
    """
    Plot the raw signal, average smoothed signal, and FFT

    Args:
    ----
    ax_signal (mpl axes): The axes to plot the signals on
    ax_fft (mpl axes): _The axes to plot the FFTs on
    file (str): Data source
    """
    t = [] # column 0
    signal = [] # column 1
    smoothed_signal = []
    average_values = []

    with open('./dsp/'+file+'.csv') as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            signal.append(float(row[1])) # second column
            average_values.append(signal[-1])
            if len(average_values) > average_length:
                average_values.pop(0)
            smoothed_signal.append(sum(average_values)/len(average_values))

    ax_signal.plot(t, signal, 'k', label='Raw Signal')
    ax_signal.plot(t, smoothed_signal, 'r', label='Average Smoothed Signal')

    ax_signal.set_title(f'{file} and Smoothed Signal ({average_length} points)')
    ax_signal.set_xlabel('Time [s]')
    ax_signal.set_ylabel('Signal')
    ax_signal.legend()


    n = len(signal) # length of the signal
    k = np.arange(n) # List of integer frequencies?
    T = n/len(t)
    frq1 = k/T
    frq = frq1[range(int(n/2))]
    Y = np.fft.fft(signal)/len(signal) # fft computing and normalization
    Y = Y[range(int(len(signal)/2))]

    ax_fft.loglog(frq,abs(Y),'k', label='Raw FFT') # plotting the fft

    n = len(smoothed_signal) # length of the signal
    k = np.arange(n) # List of integer frequencies?
    T = n/len(t)
    frq1 = k/T
    frq = frq1[range(int(n/2))]
    Y = np.fft.fft(smoothed_signal)/len(smoothed_signal) # fft computing and normalization
    Y = Y[range(int(len(smoothed_signal)/2))]

    ax_fft.loglog(frq,abs(Y),'r', label='Average Smoothed FFT') # plotting the fft
    ax_fft.set_title(file+' FFT')
    ax_fft.set_xlabel('Freq (Hz)')
    ax_fft.set_ylabel('|Y(freq)|')
    ax_fft.legend()


files = ['sigA', 'sigB', 'sigC', 'sigD']
fig, ((ax1, ax2, ax3, ax4), (ax5, ax6, ax7, ax8)) = plt.subplots(2, 4)
axes = [ax1, ax2, ax3, ax4, ax5, ax6, ax7, ax8]
averages = [5, 5, 5, 5]
for i in range(len(files)):
    plot_averaged(axes[2*i], axes[2*i+1], files[i], averages[i])
    # t = [] # column 0
    # data1 = [] # column 1

    # with open('./dsp/'+files[i]+'.csv') as f:
    #     # open the csv file
    #     reader = csv.reader(f)
    #     for row in reader:
    #         # read the rows 1 one by one
    #         t.append(float(row[0])) # leftmost column
    #         data1.append(float(row[1])) # second column

    # freq = t[-1]-t[0]/len(t)


    # axes[i].plot(t,data1,'b-*')
    # axes[i].set_title(files[i])
    # axes[i].set_xlabel('Time [s]')
    # axes[i].set_ylabel('Signal')
plt.show()


