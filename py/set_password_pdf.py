from PyPDF2 import PdfFileWriter, PdfFileReader
import getpass

def set_password_pdf(file):
    pdfwriter = PdfFileWriter()
    pdf = PdfFileReader(file)
    for page_num in range(pdf.numPages):
        pdfwriter.addPage(pdf.getPage(page_num))
    password = getpass.getpass(prompt="Enter Password : ")
    pdfwriter.encrypt(password)
    with open(file,'wb') as f:
        pdfwriter.write(f)
    
if __name__=='__main__':
    file = input("파일을 입력해주세요 : ")
    set_password_pdf(file)
    