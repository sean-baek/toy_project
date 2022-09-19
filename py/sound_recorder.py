import sounddevice
from scipy.io.wavfile import write


def sound_recorder(second):
    fs = 44100 # sample_rate
    print("Recording.... \n")
    record_voice = sounddevice.rec(int(second * fs), samplerate = fs, channels=2)
    sounddevice.wait()

    write("out.wav", fs, record_voice)

    print("Finished...\nPlease check it...")

if __name__== '__main__':
    second = int(input("Enter the time duration in second : "))
    sound_recorder(second)