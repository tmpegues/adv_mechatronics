import matplotlib.pyplot as plt
import numpy as np

dt = 1.0/10000.0 # 10kHz
t = np.arange(0.0, 1.0, dt) # 10s
# a constant plus 100Hz and 1000Hz
s = 4.0 * np.sin(2 * np.pi * 100 * t) + 0.25 * np.sin(2 * np.pi * 1000 * t) + 25

n = len(s) # length of the signal
k = np.arange(n) # List of integer frequencies?
T = n/len(t)

frq1 = k/T # two sides frequency range
frq = frq1[range(int(n/2))] # one side frequency range

Y = np.fft.fft(s)/len(s) # fft computing and normalization
Y = Y[range(int(len(s)/2))]

fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,s,'b')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'b') # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()