import pyttsx3

def audio_english_text(file):
    file = open(file, 'r')
    text = file.readlines()
    engine = pyttsx3.init()
    for i in text:
        engine.say(i)
        engine.runAndWait()
        
if __name__=='__main__':
    file = input("파일을 입력하세요 : ")
    audio_english_text(file)