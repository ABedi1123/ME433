import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function
import csv

#Step 1: Load the data from CSV
t = [] # column 0
data1 = [] # column 1
#data2 = [] # column 2

with open('sigD.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column
        #data2.append(float(row[2])) # third column

#for i in range(len(t)):
    # print the data to verify it was read
    #print(str(t[i]) + ", " + str(data1[i]))


#FFT
Fs = 1 / (t[1] - t[0]) # sample rate
Ts = t[-1]; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = data1 # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization the frequency domain
Y = Y[range(int(n/2))]


#Moving Average Filter
#def moving_avg_filter(signal, window):
 #   for i in range(0, len(signal) - window):
  #      avg = 0
   #     for j in range(0, window):
    #        avg = avg + signal[i+j]
     #   avg = avg / window
      #  data2.append(avg)

# IIR Filter function
#def iir_filter(signal, A, B):
 #   assert A + B == 1, "A + B must equal 1"
  #  filtered_signal = np.zeros(len(signal))
   # filtered_signal[0] = signal[0]  # initialize with the first value
    #for i in range(1, len(signal)):
     #   filtered_signal[i] = A * filtered_signal[i - 1] + B * signal[i]
    #return filtered_signal


# FIR Filter design
def fir_filter(signal, cutoff_freq, sample_rate, numtaps):
    # Generate the filter coefficients using a windowed sinc function
    nyquist_rate = sample_rate / 2
    normalized_cutoff = cutoff_freq / nyquist_rate
    taps = np.sinc(2 * normalized_cutoff * (np.arange(numtaps) - (numtaps - 1) / 2))
    window = np.hamming(numtaps)
    taps *= window
    taps /= np.sum(taps)
    
    # Apply the filter to the signal
    filtered_signal = np.convolve(signal, taps, mode='same')
    return filtered_signal, taps


#Apply Moving Average Filter
#window = 500
#moving_avg_filter(data1, window)

# Apply IIR Filter
#A = 0.9
#B = 0.1
#filtered_signal_iir = iir_filter(data1, A, B)

# Parameters for FIR Filter
cutoff_freq = 100 # Example cutoff frequency
numtaps = 101  # Number of filter coefficients (odd number is typical for symmetric filters)

# Apply FIR Filter
filtered_signal_fir, fir_taps = fir_filter(data1, cutoff_freq, Fs, numtaps)


#Plot original and IIR filtered signals
fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t, data1, 'black', label="Original Signal")
ax1.plot(t, filtered_signal_fir, 'r', label="FIR Signal")
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.legend()

#t2 = t[window:len(t)]

#FFT
#Fs2 = 1 / (t2[1] - t2[0]) # sample rate
#Ts2 = t2[-1]; # sampling interval
#ts2 = np.arange(0,t2[-1],Ts) # time vector
#y2 = data2 # the data to make the fft from
#n2 = len(y2) # length of the signal
#k2 = np.arange(n2)
#T2 = n2/Fs2
#frq2 = k2/T2 # two sides frequency range
#frq2 = frq2[range(int(n2/2))] # one side frequency range
#Y2 = np.fft.fft(y2)/n2 # fft computing and normalization the frequency domain
#Y2 = Y2[range(int(n2/2))]

#FFT of filtered signal (IIR)
#frq2 = frq #frequency range remains the same
#Y2 = np.fft.fft(filtered_signal_iir) / len(filtered_signal_iir)
#Y2 = Y2[range(int(len(filtered_signal_iir) / 2))]

# FFT of filtered signal (FIR)
Y_fir = np.fft.fft(filtered_signal_fir) / len(filtered_signal_fir)  # fft computing and normalization
Y_fir = Y_fir[range(int(len(filtered_signal_fir) / 2))]


#Plot FFTs
ax2.loglog(frq,abs(Y),'black', label = "Original FFT") # plotting the fft
ax2.loglog(frq ,abs(Y_fir),'red', label = "FIR FFT")
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
ax2.legend()

plt.tight_layout()
plt.show()

# Print filter information
print(f"FIR Filter Coefficients (first 10): {fir_taps[:10]}")
print(f"Number of Taps: {numtaps}")
print(f"Cutoff Frequency: {cutoff_freq} Hz")




