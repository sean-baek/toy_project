from PyPDF2 import PdfFileMerger, PdfFileReader

import os
# os.getcwd() : 다른 폴더에서 추출하기 위한 용도

def pdf_merger(file):
    merger = PdfFileMerger()
    for items in os.listdir():
        if items.endswith('.pdf'):
            merger.append(items)
    merger.write("Final_pdf.pdf")
    merger = PdfFileMerger()
    with open(file, 'rb') as f:
        merger.append(PdfFileReader(f))
    merger.close()
    
if __name__=='__main__':
    file = input("현재 폴더에 있는 pdf 파일들을 합칠 원본 pdf 파일을 입려하세요 : ")
    pdf_merger(file)
