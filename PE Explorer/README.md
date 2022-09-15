# ISSUE
- EAT 부분이 불완전하다.
    -> KERNEL32.dll을 인자로 줬을 때 EAT를 다 출력하지 못한다.

---

# 구조체 순서

→ IMAGE_DOS_HEADER

→ IMAGE_NT_HEADER(IMAGE_FILE_HEADER, IMAGE_OPTIONAL_HEADER)

    ⇒ IMAGE_OPTIONAL_HEADER → IMAGE_DATA_DIRECTORY[]

        ⇒ IMAGE_DATA_DIRECTORY[1].VirtualAddress(RVA) → IMAGE_IMPORT_DESCRIPTOR[]

            ⇒ IMAGE_IMPORT_DESCRIPTOR[].OriginalFirstThunk → IMAGE_THUNK_DATA.u1.AddressOfData → IMAGE_IMPORT_BY_NAME(HINT, Function Name)

            ⇒ IMAGE_IMPORT_DESCRIPTOR[].FirstThunk → IMAGE_THUNK_DATA.u1.Function(IAT)

→ IMAGE_SECTION_HEADER

- IMAGE_OPTIONAL_HEADER32, IMAGE_OPTIONAL_HEADER64
- IMAGE_THUNK_DATA32, IMAGE_THUNK_DATA64

---

# Info

## dos header
dos header → e_magic
dos header → e_lfanew

## nt header
nt header → signature
nt header → file header → machine, NumberOfSections, SizeOfOptionalHEader, Characteristics
nt header → optional header → magic, AddressOfEntryPoint, ImageBase, SizeOfimage, SizeOfHeader, Subsystem, NumberOfRvaAndSizes, DataDirectory

## Section header
section header → VirtualSize, VirtualAddress, PointerToRawData, Characteristics

---

# RVA To RAW
1. 각 섹션의 VirtualAddress + VirtualSize 값과 PointerToRawData 값을 구한다.
2. RVA 값이 어떤 섹션의 VirtualAddress 보다 크고 VirtualAddress + VirtualSize 보다 작은지 파악한다.
3. RVA 값이 속하는 섹션의 VirtualAddress와 PointerToRawData를 이용해 아래의 연산을 한다.
```bash
RAW : RVA - VirtualAddress + PointerToRawData
```

---

# GetProcAddress() process

1. AddressOfNames 멤버를 이용해 “함수 이름 배열”로 간다.
2. “함수 이름 배열”은 문자열 주소가 저장되어 있다.
문자열 비교(strcmp)를 통해 원하는 함수 이름을 찾는다.
(이때 배열의 인덱스를 name_index라고 임시로 정한다)
3. AddressOfNameOrdinals 멤버를 이용해 “ordinal 배열”로 간다.
4. “Ordinal 배열”에서 name_index로 해당 ordinal 값을 찾는다.
5. AddressOfFunctions 멤버를 이용해 “함수 주소 배열(EAT)”로 간다.
6. “함수 주소 배열(EAT)”에서 이전에 구한 ordinal을 배열 인덱스로 하여 원하는 함수의 시작 주소를 얻는다.

## tip
    함수 이름 없이 Ordinal로만 Export된 함수의 주소를 찾을 수도 있다.
    해당 Oridinal 값에 IMAGE_EXPORT_DIRECTORY.Base 멤버를 뺀 값을 “함수 주소 배열”의 인덱스로 하여 해당 함수 주소를 찾을 수 있다.

---

# images
<img src="./images/1.jpg" width="450%" height="300px" title="start" alt="start"></img></br>
![Alt text](PE Explorer/images/1.jpg)
![Alt text](/images/dos_header.jpg)
![Alt nt_header_file_header](/images/nt_header_file_header.jpg)
![Alt nt_header_optional_header1](/images/nt_header_optional_header_1.jpg)
![Alt nt_header_optional_header2](/images/nt_header_optional_header_2.jpg)
![Alt section_header](/images/section_header.jpg)
![Alt image_import_descriptor](/images/iid.jpg)
![Alt int_eat](/images/int_iat.jpg)
![Alt eat1](/images/eat_1.jpg)
![Alt eat2](/images/eat_2.jpg)

---