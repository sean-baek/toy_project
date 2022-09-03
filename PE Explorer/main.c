#define _CRT_SECURE_NO_WARNINGS
#include "header.h"

int main(int argc, char** argv)
{
	int size = 0;
	FILE* file = NULL;
	u_char* buf = NULL;

	if (argc < 2)
	{
		printf("인자가 부족합니다.\n분석하실 파일을 입력해주세요.\n");
		return -1;
	}

	if ((file = fopen(argv[1], "rb")) == NULL)
	{
		fprintf(stderr, "해당 파일을 읽을 수 없습니다. : %s\n", strerror(errno));
		return -1;
	}

	// buf에 동적 할당하여 file의 바이너리 내용 읽어오기
	get_file_content(file, &buf);

	// 각 헤더 포인터 변수에 파일에서의 시작 지점 지정
	IMAGE_DOS_HEADER* idh = (IMAGE_DOS_HEADER*)buf;
	IMAGE_NT_HEADERS* inh = (IMAGE_NT_HEADERS*)(buf + idh->e_lfanew);
	IMAGE_SECTION_HEADER* ish = (IMAGE_SECTION_HEADER*)(buf + idh->e_lfanew + 24 + inh->FileHeader.SizeOfOptionalHeader);
	
	// dos header 
	print_dos_header(idh);

	if ((inh->FileHeader.Machine == 0x0200) || (inh->FileHeader.Machine == 0x8664))
	{
		IMAGE_NT_HEADERS64* inh64 = (IMAGE_NT_HEADERS64*)(buf + idh->e_lfanew);
		//printf("IMAGE_NT_HEADERS64 : %d\n", sizeof(IMAGE_NT_HEADERS64));
		printf("64bit Program\n\n");

		// 64bit nt header
		print_nt_header64(inh64);
	}
	else
	{
		IMAGE_NT_HEADERS32* inh32 = (IMAGE_NT_HEADERS32*)(buf + idh->e_lfanew);
		// printf("IMAGE_NT_HEADERS32 : %d\n", sizeof(IMAGE_NT_HEADERS32));
		printf("32bit Program\n\n");

		// 32bit nt header
		print_nt_header32(inh32);
	}
	WORD section_num = inh->FileHeader.NumberOfSections;
	
	// section header
	print_section_header(ish, section_num);

	free(buf);

	return 0;
}

int get_file_size(FILE* fp)
{
	int size = 0;

	// 파일 포인터를 파일의 끝으로 보낸다.
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fprintf(stderr, "파일의 끝으로 이동할 수 없습니다. : %s\n", strerror(errno));
		return -1;
	}

	if ((size = ftell(fp)) == -1)
	{
		fprintf(stderr, "파일에서 현재 위치를 알 수 없습니다. : %s\n", strerror(errno));
		return -1;
	}

	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fprintf(stderr, "파일의 처음으로 이동할 수 없습니다. : %s\n", strerror(errno));
		return -1;
	}

	return size;
}

int get_file_content(FILE* fp, u_char** buf)
{
	int size = 0;

	// 파일 사이즈 구하기
	if ((size = get_file_size(fp)) == -1)
	{
		printf("파일 사이즈를 구하는 데 실패했습니다.\n");
		return -1;
	}
	else
	{
		printf("\nFile Size : %dbyte\n\n", size);

		// 파일 사이즈만큼 메모리 동적 할당 후 0으로 초기화
		if ((*buf = (u_char**)malloc(size)) == NULL)
			printf("malloc() error\n");

		if (memset(*buf, 0x00, size) == NULL)
			printf("memset() error\n");

		// 동적 할당한 메모리에 file 내용 전체 읽기
		if (fread(*buf, 1, size, fp) == NULL)
			printf("fread() error\n");
		
		return 0;
	}
}

void print_dos_header(IMAGE_DOS_HEADER* idh)
{
	printf("========== [Dos Header] ==========\n");
	printf("- e_magic[%dbyte]\t: %04X\n", sizeof(idh->e_magic), tolower(idh->e_magic));
	printf("- e_cblp[%dbyte]\t\t: %04X\n", sizeof(idh->e_cblp), toupper(idh->e_cblp));
	printf("- e_cp[%dbyte]\t\t: %04X\n", sizeof(idh->e_cp), idh->e_cp);
	printf("- e_crlc[%dbyte]\t\t: %04X\n", sizeof(idh->e_crlc), idh->e_crlc);
	printf("- e_cparhdr[%dbyte]\t: %04X\n", sizeof(idh->e_cparhdr), idh->e_cparhdr);
	printf("- e_minalloc[%dbyte]\t: %04X\n", sizeof(idh->e_minalloc), idh->e_minalloc);
	printf("- e_maxalloc[%dbyte]\t: %04X\n", sizeof(idh->e_maxalloc), idh->e_maxalloc);
	printf("- e_ss[%dbyte]\t\t: %04X\n", sizeof(idh->e_ss), idh->e_ss);
	printf("- e_sp[%dbyte]\t\t: %04X\n", sizeof(idh->e_sp), idh->e_sp);
	printf("- e_csum[%dbyte]\t\t: %04X\n", sizeof(idh->e_csum), idh->e_csum);
	printf("- e_ip[%dbyte]\t\t: %04X\n", sizeof(idh->e_ip), idh->e_ip);
	printf("- e_cs[%dbyte]\t\t: %04X\n", sizeof(idh->e_cs), idh->e_cs);
	printf("- e_lfarlc[%dbyte]\t: %04X\n", sizeof(idh->e_lfarlc), idh->e_lfarlc);
	printf("- e_ovno[%dbyte]\t\t: %04X\n", sizeof(idh->e_ovno), idh->e_ovno);
	for (int i = 0; i < 4; i++)
		printf("- e_res[%d][%dbyte]\t: %04X\n", i, sizeof(idh->e_ovno), idh->e_res[i]);
	printf("- e_oemid[%dbyte]\t: %04X\n", sizeof(idh->e_oemid), idh->e_oemid);
	printf("- e_oeminfo[%dbyte]\t: %04X\n", sizeof(idh->e_oeminfo), idh->e_oeminfo);
	for (int i = 0; i < 10; i++)
		printf("- e_res2[%d][%dbyte]\t: %04X\n", i, sizeof(idh->e_res2[i]), idh->e_res2[i]);
	printf("- e_lfanew[%dbyte]\t: %08X\n", sizeof(idh->e_lfanew), idh->e_lfanew);
	printf("==================================\n\n");
}

void print_nt_header32(IMAGE_NT_HEADERS32* inh32)
{
	printf("========== [NT Header 32] ==========\n\n");
	printf("- Signature[%dbyte]\t\t: %08X\n\n",sizeof(inh32->Signature), inh32->Signature);
	printf("---------- (FileHeader) ----------\n");
	printf("- Machine[%dbyte]\t\t: %04X\n", sizeof(inh32->FileHeader.Machine), inh32->FileHeader.Machine);
	printf("- NumberOfSections[%dbyte]\t: %04X\n", sizeof(inh32->FileHeader.NumberOfSections), inh32->FileHeader.NumberOfSections);
	printf("- TimeDateStamp[%dbyte]\t\t: %08X\n", sizeof(inh32->FileHeader.TimeDateStamp), inh32->FileHeader.TimeDateStamp);
	printf("- PointerToSymbolTable[%dbyte]\t: %08X\n", sizeof(inh32->FileHeader.PointerToSymbolTable), inh32->FileHeader.PointerToSymbolTable);
	printf("- NumberOfSymbols[%dbyte]\t: %08X\n", sizeof(inh32->FileHeader.NumberOfSymbols), inh32->FileHeader.NumberOfSymbols);
	printf("- SizeOfOptionalHeader[%dbyte]\t: %04X\n", sizeof(inh32->FileHeader.SizeOfOptionalHeader), inh32->FileHeader.SizeOfOptionalHeader);
	printf("- Characteristics[%dbyte]\t: %04X\n", sizeof(inh32->FileHeader.Characteristics), inh32->FileHeader.Characteristics);
	printf("----------------------------------\n\n");
	printf("---------- (OptionalHeader32) ----------\n");
	printf("- Magic[%dbyte]\t\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.Magic), inh32->OptionalHeader.Magic);
	printf("- MajorLinkerVersion[%dbyte]\t\t: %02X\n", sizeof(inh32->OptionalHeader.MajorLinkerVersion), inh32->OptionalHeader.MajorLinkerVersion);
	printf("- MinorLinkerVersion[%dbyte]\t\t: %02X\n", sizeof(inh32->OptionalHeader.MinorLinkerVersion), inh32->OptionalHeader.MinorLinkerVersion);
	printf("- SizeofCode[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfCode), inh32->OptionalHeader.SizeOfCode);
	printf("- SizeofInitializedData[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfInitializedData), inh32->OptionalHeader.SizeOfInitializedData);
	printf("- SizeOfUninitializedData[%dbyte]\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfUninitializedData), inh32->OptionalHeader.SizeOfUninitializedData);
	printf("- AddressOfEntryPoint[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.AddressOfEntryPoint), inh32->OptionalHeader.AddressOfEntryPoint);
	printf("- BaseOfCode[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.BaseOfCode), inh32->OptionalHeader.BaseOfCode);
	printf("- BaseOfData[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.BaseOfData), inh32->OptionalHeader.BaseOfData);

	printf("- ImageBase[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.ImageBase), (unsigned int)inh32->OptionalHeader.ImageBase);
	printf("- SectionAlignment[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.SectionAlignment), inh32->OptionalHeader.SectionAlignment);
	printf("- FileAlignment[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.FileAlignment), inh32->OptionalHeader.FileAlignment);
	printf("- MajorOperatingSystemVersion[%dbyte]\t: %04X\n", sizeof(inh32->OptionalHeader.MajorOperatingSystemVersion), inh32->OptionalHeader.MajorOperatingSystemVersion);
	printf("- MinorOperatingSystemVersion[%dbyte]\t: %04X\n", sizeof(inh32->OptionalHeader.MinorOperatingSystemVersion), inh32->OptionalHeader.MinorOperatingSystemVersion);
	printf("- MajorImageVersion[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.MajorImageVersion), inh32->OptionalHeader.MajorImageVersion);
	printf("- MinorImageVersion[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.MinorImageVersion), inh32->OptionalHeader.MinorImageVersion);
	printf("- MajorSubsystemVersion[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.MajorSubsystemVersion), inh32->OptionalHeader.MajorSubsystemVersion);
	printf("- MinorSubsystemVersion[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.MinorSubsystemVersion), inh32->OptionalHeader.MinorSubsystemVersion);
	printf("- Win32VersionValue[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.Win32VersionValue), inh32->OptionalHeader.Win32VersionValue);
	printf("- SizeOfImage[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfImage), inh32->OptionalHeader.SizeOfImage);
	printf("- SizeOfHeaders[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfHeaders), inh32->OptionalHeader.SizeOfHeaders);
	printf("- CheckSum[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.CheckSum), inh32->OptionalHeader.CheckSum);
	printf("- Subsystem[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.Subsystem), inh32->OptionalHeader.Subsystem);
	printf("- DllCharacteristics[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.DllCharacteristics), inh32->OptionalHeader.DllCharacteristics);
	printf("- SizeOfStackReserve[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfStackReserve), (unsigned int)inh32->OptionalHeader.SizeOfStackReserve);
	printf("- SizeOfStackCommit[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfStackCommit), (unsigned int)inh32->OptionalHeader.SizeOfStackCommit);
	printf("- SizeOfHeapReserve[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfHeapReserve), (unsigned int)inh32->OptionalHeader.SizeOfHeapReserve);
	printf("- SizeOfHeapCommit[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.SizeOfHeapCommit), (unsigned int)inh32->OptionalHeader.SizeOfHeapCommit);
	printf("- LoaderFlags[%dbyte]\t\t\t: %04X\n", sizeof(inh32->OptionalHeader.LoaderFlags), inh32->OptionalHeader.LoaderFlags);
	printf("- NumberOfRvaAndSizes[%dbyte]\t\t: %04X\n", sizeof(inh32->OptionalHeader.NumberOfRvaAndSizes), inh32->OptionalHeader.NumberOfRvaAndSizes);
	for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++)
	{
		printf("- DataDirectory[%d].VirtualAddress[%dbyte] : %08X\n", i, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
		printf("- DataDirectory[%d].VirtualAddress[%dbyte] : %08X\n", i, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	}
	printf("----------------------------------------\n");
	printf("========================================\n\n");
}

void print_nt_header64(IMAGE_NT_HEADERS64* inh64)
{
	printf("========== [NT Header 64] ==========\n\n");
	printf("- Signature[%dbyte]\t\t: %08X\n\n", sizeof(inh64->Signature), inh64->Signature);
	printf("---------- (FileHeader) ----------\n");
	printf("- Machine[%dbyte]\t\t\t: %04X\n", sizeof(inh64->FileHeader.Machine), inh64->FileHeader.Machine);
	printf("- NumberOfSections[%dbyte]\t\t: %04X\n", sizeof(inh64->FileHeader.NumberOfSections), inh64->FileHeader.NumberOfSections);
	printf("- TimeDateStamp[%dbyte]\t\t\t: %08X\n", sizeof(inh64->FileHeader.TimeDateStamp), inh64->FileHeader.TimeDateStamp);
	printf("- PointerToSymbolTable[%dbyte]\t\t: %08X\n", sizeof(inh64->FileHeader.PointerToSymbolTable), inh64->FileHeader.PointerToSymbolTable);
	printf("- NumberOfSymbols[%dbyte]\t\t: %08X\n", sizeof(inh64->FileHeader.NumberOfSymbols), inh64->FileHeader.NumberOfSymbols);
	printf("- SizeOfOptionalHeader[%dbyte]\t\t: %04X\n", sizeof(inh64->FileHeader.SizeOfOptionalHeader), inh64->FileHeader.SizeOfOptionalHeader);
	printf("- Characteristics[%dbyte]\t\t: %04X\n", sizeof(inh64->FileHeader.Characteristics), inh64->FileHeader.Characteristics);
	printf("------------------------------------\n\n");
	printf("---------- (OptionalHeader64) ----------\n");
	printf("- Magic[%dbyte]\t\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.Magic), inh64->OptionalHeader.Magic);
	printf("- MajorLinkerVersion[%dbyte]\t\t: %02X\n", sizeof(inh64->OptionalHeader.MajorLinkerVersion), inh64->OptionalHeader.MajorLinkerVersion);
	printf("- MinorLinkerVersion[%dbyte]\t\t: %02X\n", sizeof(inh64->OptionalHeader.MinorLinkerVersion), inh64->OptionalHeader.MinorLinkerVersion);
	printf("- SizeofCode[%dbyte]\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfCode), inh64->OptionalHeader.SizeOfCode);
	printf("- SizeofInitializedData[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfInitializedData), inh64->OptionalHeader.SizeOfInitializedData);
	printf("- SizeOfUninitializedData[%dbyte]\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfUninitializedData), inh64->OptionalHeader.SizeOfUninitializedData);
	printf("- AddressOfEntryPoint[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.AddressOfEntryPoint), inh64->OptionalHeader.AddressOfEntryPoint);
	printf("- BaseOfCode[%dbyte]\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.BaseOfCode), inh64->OptionalHeader.BaseOfCode);

	printf("- ImageBase[%dbyte]\t\t\t: %08X\n", sizeof(inh64->OptionalHeader.ImageBase), (unsigned int)inh64->OptionalHeader.ImageBase);
	printf("- SectionAlignment[%dbyte]\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.SectionAlignment), inh64->OptionalHeader.SectionAlignment);
	printf("- FileAlignment[%dbyte]\t\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.FileAlignment), inh64->OptionalHeader.FileAlignment);
	printf("- MajorOperatingSystemVersion[%dbyte]\t: %04X\n", sizeof(inh64->OptionalHeader.MajorOperatingSystemVersion), inh64->OptionalHeader.MajorOperatingSystemVersion);
	printf("- MinorOperatingSystemVersion[%dbyte]\t: %04X\n", sizeof(inh64->OptionalHeader.MinorOperatingSystemVersion), inh64->OptionalHeader.MinorOperatingSystemVersion);
	printf("- MajorImageVersion[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.MajorImageVersion), inh64->OptionalHeader.MajorImageVersion);
	printf("- MinorImageVersion[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.MinorImageVersion), inh64->OptionalHeader.MinorImageVersion);
	printf("- MajorSubsystemVersion[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.MajorSubsystemVersion), inh64->OptionalHeader.MajorSubsystemVersion);
	printf("- MinorSubsystemVersion[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.MinorSubsystemVersion), inh64->OptionalHeader.MinorSubsystemVersion);
	printf("- Win32VersionValue[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.Win32VersionValue), inh64->OptionalHeader.Win32VersionValue);
	printf("- SizeOfImage[%dbyte]\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfImage), inh64->OptionalHeader.SizeOfImage);
	printf("- SizeOfHeaders[%dbyte]\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfHeaders), inh64->OptionalHeader.SizeOfHeaders);
	printf("- CheckSum[%d]\t\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.CheckSum), inh64->OptionalHeader.CheckSum);
	printf("- Subsystem[%dbyte]\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.Subsystem), inh64->OptionalHeader.Subsystem);
	printf("- DllCharacteristics[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.DllCharacteristics), inh64->OptionalHeader.DllCharacteristics);
	printf("- SizeOfStackReserve[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfStackReserve), (unsigned int)inh64->OptionalHeader.SizeOfStackReserve);
	printf("- SizeOfStackCommit[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfStackCommit), (unsigned int)inh64->OptionalHeader.SizeOfStackCommit);
	printf("- SizeOfHeapReserve[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfHeapReserve), (unsigned int)inh64->OptionalHeader.SizeOfHeapReserve);
	printf("- SizeOfHeapCommit[%dbyte]\t\t: %04X\n", sizeof(inh64->OptionalHeader.SizeOfHeapCommit), (unsigned int)inh64->OptionalHeader.SizeOfHeapCommit);
	printf("- LoaderFlags[%dbyte]\t\t\t\t: %04X\n", sizeof(inh64->OptionalHeader.LoaderFlags), inh64->OptionalHeader.LoaderFlags);
	printf("- NumberOfRvaAndSizes[%dbyte]\t: %04X\n", sizeof(inh64->OptionalHeader.NumberOfRvaAndSizes), inh64->OptionalHeader.NumberOfRvaAndSizes);
	for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++)
	{
		printf("- DataDirectory[%d].VirtualAddress[%dbyte] : %08X\n", i, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
		printf("- DataDirectory[%d].VirtualAddress[%dbyte] : %08X\n", i, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);

	}

	printf("------------------------------------\n");
	printf("====================================\n\n");
}

void print_section_header(IMAGE_SECTION_HEADER* ish, WORD section_num)
{
	printf("========== [SECTION HEADERS] ==========\n\n");

	for (int i = 0; i < section_num; i++)
	{
		//printf("%p\n", ish);
		printf("- Name\t\t\t\t: %s\n", ish->Name);
		printf("- PhysicalAddress[%dbyte]\t: %08X\n", sizeof(ish->Misc.PhysicalAddress), ish->Misc.PhysicalAddress);
		printf("- VirtualSize[%dbyte]\t\t: %08X\n", sizeof(ish->Misc.VirtualSize), ish->Misc.VirtualSize);
		printf("- VirtualAddress[%dbyte]\t\t: %08X\n", sizeof(ish->VirtualAddress), ish->VirtualAddress);
		printf("- SizeOfRawData[%dbyte]\t\t: %08X\n", sizeof(ish->SizeOfRawData), ish->SizeOfRawData);
		printf("- PointerToRawData[%dbyte]\t: %08X\n", sizeof(ish->PointerToRawData), ish->PointerToRawData);
		printf("- PointerToRelocations[%dbyte]\t: %08X\n", sizeof(ish->PointerToRelocations), ish->PointerToRelocations);
		printf("- PointerToLinenumbers[%dbyte]\t: %08X\n", sizeof(ish->PointerToLinenumbers), ish->PointerToLinenumbers);
		printf("- NumberOfRelocations[%dbyte]\t: %04X\n", sizeof(ish->NumberOfRelocations), ish->NumberOfRelocations);
		printf("- NumberOfLineNumbers[%dbyte]\t: %04X\n", sizeof(ish->NumberOfLinenumbers), ish->NumberOfLinenumbers);
		printf("- Characteristics[%dbyte]\t: %08X\n\n", sizeof(ish->Characteristics), ish->Characteristics);
		printf("=======================================\n\n");

		ish++;	
	}
}