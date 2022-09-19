import moviepy
import moviepy.editor

def mp3_from_mp4(mp4_file):
    video = moviepy.editor.VideoFileClip(mp4_file)
    audio = video.audio
    audio.write_audiofile(mp4_file[0:-4] + '.mp3')

if __name__=='__main__':
    mp4_file = input("파일을 입력하세요. : ")
    mp3_from_mp4(mp4_file)