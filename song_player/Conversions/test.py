#convert .wav to samples exported as .txt file
#where each sample is a new line
import scipy.signal as sps
import soundfile as sf
import numpy as np
import os

#----------
# user inputs without error checking because lazy
print("Enter file names with extensions.")
print("Select an option by entering '1' or '2': ")
print("(1) Use default filenames \n(2) Choose own files")
mode = input()

if(mode == '2'):
    sname = input("Enter audio file name: ")
    fname = input("Enter new file name: ")
else:
    sname = 'first.wav'
    fname = 'song_int_array.txt'
    
print("Audio file name: " + sname)
print("New file name: " + fname + "\n")
#----------

#chosen default sample rate as 8k
samplerate = 8000 

# take the data from the source .wav sound file, kept file sample rate for self debug
data, FILEsamplerate = sf.read(sname)
print("file sample rate ", FILEsamplerate)
print("# file samples ", data.size, "\n")

# rewrite the data into a temp .wav sound file at chosen sample rate
#sf.write('new_file.wav', data, samplerate)

# read the new data, can double check new sample rate with NEWsamplerate
#new_data, NEWsamplerate = sf.read('new_file.wav')
numSamples = round(len(data)*float(samplerate)/FILEsamplerate)
new_data = sps.resample(data, numSamples)

# can check the used sample rate below
sf.write('new_file.wav', new_data, samplerate)
obsData, NEWsamplerate = sf.read('new_file.wav')
print("new sample rate ", NEWsamplerate)
print("# new samples ", new_data.size, "\n")

# convert the data to 16-bit integers
new_data = (new_data * 32767).astype(np.int16)
print(new_data)

# throw into a new text file. make new file if doesn't exist
try: 
    f = open(fname, "x") 
    print("New file \'" + fname + "\' made. Writing contents...")
except:
    print("File \'" + fname + "\' exists. Rewriting contents...")

# overwrite to new file
f = open(fname, "w") 

# save as .txt file
np.savetxt(fname, new_data, fmt='%d', delimiter=' ')

# can now delete temp new_file.wav
#os.remove('new_file.wav')

# close the file
f.close()

print("Finished writing into \'" + fname + "\'")