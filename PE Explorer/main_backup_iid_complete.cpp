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
		fprintf(stderr, "해당 파일을 읽을 수 없습니다. :%s\n", strerror(errno));
		return -1;
	}

	// buf에 동적 할당하여 file의 바이너리 내용 읽어오기
	size = get_file_content(file, &buf);

	// 각 헤더 포인터 변수에 파일에서의 시작 지점 지정
	IMAGE_DOS_HEADER* idh = (IMAGE_DOS_HEADER*)buf;
	IMAGE_NT_HEADERS* inh = (IMAGE_NT_HEADERS*)(buf + idh->e_lfanew);
	IMAGE_SECTION_HEADER* ish = (IMAGE_SECTION_HEADER*)(buf + idh->e_lfanew + sizeof(inh->Signature) + sizeof(inh->FileHeader) + inh->FileHeader.SizeOfOptionalHeader);


	DWORD val = inh->OptionalHeader.DataDirectory[1].VirtualAddress;
	printf("val : %d, %X\n", val, val);

	int raw = rva_to_raw(file, &buf, val);
	printf("raw : %d, %X\n", raw, raw);

	IMAGE_IMPORT_DESCRIPTOR* iid = (IMAGE_IMPORT_DESCRIPTOR*)(buf + raw); // IID 구조체의 배열 시작 주소(RVA)를 가리킴

	//printf("%p\n", iid);
	if ((inh->FileHeader.Machine == 0x0200) || (inh->FileHeader.Machine == 0x8664))
	{
		printf("\n64bit Program\n\n");
		printf("File Size :%dbyte\n\n", size);
		// dos header 
		print_dos_header(file, idh);

		IMAGE_NT_HEADERS64* inh64 = (IMAGE_NT_HEADERS64*)(buf + idh->e_lfanew);
		//printf("IMAGE_NT_HEADERS64 :%d\n", sizeof(IMAGE_NT_HEADERS64));

		// 파일 포인터를 NT header 위치로 이동
		fseek(file, idh->e_lfanew, SEEK_SET);
		offset = ftell(file);
		// 64bit nt header
		print_nt_header64(file, inh64);

		// section 헤더 처음 부분 가리키기
		fseek(file, (idh->e_lfanew + sizeof(inh64->Signature) + sizeof(inh64->FileHeader) + inh64->FileHeader.SizeOfOptionalHeader), SEEK_SET);
		offset = ftell(file);
		// section 개수 구하기
		WORD section_num = inh64->FileHeader.NumberOfSections;
		// section header
		print_section_header(file, ish, section_num);

		// IMAGE_IMPORT_DESCRIPTOR
		int import_descriptor_size = 0;
		printf("import_descfiptor_size : %d\n", inh64->OptionalHeader.DataDirectory[1].Size);
		DWORD val = inh64->OptionalHeader.DataDirectory[1].VirtualAddress;

		int raw = rva_to_raw(file, &buf, val);
		printf("raw : %d, %X\n", raw, raw);

	}
	else if (inh->FileHeader.Machine == 0x014c)
	{
		printf("\n32bit Program\n\n");
		printf("File Size :%dbyte\n\n", size);


		// dos header 
		print_dos_header(file, idh);

		IMAGE_NT_HEADERS32* inh32 = (IMAGE_NT_HEADERS32*)(buf + idh->e_lfanew);
		// printf("IMAGE_NT_HEADERS32 :%d\n", sizeof(IMAGE_NT_HEADERS32));

		// 파일 포인터를 NT header 위치로 이동
		fseek(file, idh->e_lfanew, SEEK_SET);
		offset = ftell(file);
		// 32bit nt header
		print_nt_header32(file, inh32);

		// section 헤더 처음 부분 가리키기
		fseek(file, (idh->e_lfanew + sizeof(inh32->Signature) + sizeof(inh32->FileHeader) + inh32->FileHeader.SizeOfOptionalHeader), SEEK_SET);
		offset = ftell(file);
		// section 개수 구하기
		WORD section_num = inh32->FileHeader.NumberOfSections;
		// section header
		print_section_header(file, ish, section_num);



		// IMAGE_IMPORT_DESCRIPTOR
		DWORD val = inh32->OptionalHeader.DataDirectory[1].VirtualAddress;
		int raw = rva_to_raw(file, &buf, val);

		fseek(file, raw, SEEK_SET);
		printf("IID structure raw : %X\n", ftell(file));

		// IID 구조체 배열의 크기
		int import_descriptor_size = inh32->OptionalHeader.DataDirectory[1].Size;
		printf("import_descfiptor_size : %d\n", inh32->OptionalHeader.DataDirectory[1].Size);

		for (int i = 0; i < import_descriptor_size; i++, iid++)
		{
			raw = rva_to_raw(file, &buf, iid->Name);
			printf("%s\n", buf + raw);
			//printf("%s\n", (iid + raw));
			//printf("%X\n", raw);
			//printf("%d, %x\n", raw, raw);
			//printf("%x\n", raw);
			//printf("%08X\n", raw);
		}




		//printf("raw : %d, %X\n", raw, raw);

	}


	free(buf);
	fclose(file);

	return 0;
}











int get_file_size(FILE* fp)
{
	int size = 0;

	// 파일 포인터를 파일의 끝으로 보낸다.
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fprintf(stderr, "파일의 끝으로 이동할 수 없습니다. :%s\n", strerror(errno));
		return -1;
	}

	if ((size = ftell(fp)) == -1)
	{
		fprintf(stderr, "파일에서 현재 위치를 알 수 없습니다. :%s\n", strerror(errno));
		return -1;
	}

	rewind(fp);

	/*
	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fprintf(stderr, "파일의 처음으로 이동할 수 없습니다. :%s\n", strerror(errno));
		return -1;
	}*/

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
		// 파일 사이즈만큼 메모리 동적 할당 후 0으로 초기화
		if ((*buf = (u_char*)malloc(size + 1)) == NULL)
			printf("malloc() error\n");

		if (memset(*buf, 0x00, size) == NULL)
			printf("memset() error\n");

		// 이미 어떠한 작업을 거친 파일 포인터를 get_file_content에 넘겨도 동작하도록 하기 위함
		rewind(fp);

		// 동적 할당한 메모리에 file 내용 전체 읽기
		if (fread(*buf, 1, size, fp) == 0)
			printf("fread() error\n");

		rewind(fp);

		return size;
	}
}

// size 인자 만큼 파일 포인터를 이동하여 해당 위치의 offset 값을 반환한다.
int get_file_offset(FILE* fp, int size)
{
	fseek(fp, size, SEEK_CUR);
	return ftell(fp);
}

void print_dos_header(FILE* fp, IMAGE_DOS_HEADER* idh)
{

	printf("========== [Dos Header] ==========\n");
	// 파일의 처음으로 이동하여
	fseek(fp, 0, SEEK_SET);

	// offset 가져오기
	offset = ftell(fp);
	printf("[%08X] : e_magic[%dbyte]\t:%04X\n", offset, sizeof(idh->e_magic), idh->e_magic);
	offset = get_file_offset(fp, sizeof(idh->e_magic));

	printf("[%08X] : e_cblp[%dbyte]\t:%04X\n", offset, sizeof(idh->e_cblp), toupper(idh->e_cblp));
	offset = get_file_offset(fp, sizeof(idh->e_cblp));

	printf("[%08X] : e_cp[%dbyte]\t:%04X\n", offset, sizeof(idh->e_cp), idh->e_cp);
	offset = get_file_offset(fp, sizeof(idh->e_cp));

	printf("[%08X] : e_crlc[%dbyte]\t:%04X\n", offset, sizeof(idh->e_crlc), idh->e_crlc);
	offset = get_file_offset(fp, sizeof(idh->e_crlc));

	printf("[%08X] : e_cparhdr[%dbyte]\t:%04X\n", offset, sizeof(idh->e_cparhdr), idh->e_cparhdr);
	offset = get_file_offset(fp, sizeof(idh->e_cparhdr));

	printf("[%08X] : e_minalloc[%dbyte]\t:%04X\n", offset, sizeof(idh->e_minalloc), idh->e_minalloc);
	offset = get_file_offset(fp, sizeof(idh->e_minalloc));

	printf("[%08X] : e_maxalloc[%dbyte]\t:%04X\n", offset, sizeof(idh->e_maxalloc), idh->e_maxalloc);
	offset = get_file_offset(fp, sizeof(idh->e_maxalloc));

	printf("[%08X] : e_ss[%dbyte]\t:%04X\n", offset, sizeof(idh->e_ss), idh->e_ss);
	offset = get_file_offset(fp, sizeof(idh->e_ss));

	printf("[%08X] : e_sp[%dbyte]\t:%04X\n", offset, sizeof(idh->e_sp), idh->e_sp);
	offset = get_file_offset(fp, sizeof(idh->e_sp));

	printf("[%08X] : e_csum[%dbyte]\t:%04X\n", offset, sizeof(idh->e_csum), idh->e_csum);
	offset = get_file_offset(fp, sizeof(idh->e_csum));

	printf("[%08X] : e_ip[%dbyte]\t:%04X\n", offset, sizeof(idh->e_ip), idh->e_ip);
	offset = get_file_offset(fp, sizeof(idh->e_ip));

	printf("[%08X] : e_cs[%dbyte]\t:%04X\n", offset, sizeof(idh->e_cs), idh->e_cs);
	offset = get_file_offset(fp, sizeof(idh->e_cs));

	printf("[%08X] : e_lfarlc[%dbyte]\t:%04X\n", offset, sizeof(idh->e_lfarlc), idh->e_lfarlc);
	offset = get_file_offset(fp, sizeof(idh->e_lfarlc));

	printf("[%08X] : e_ovno[%dbyte]\t:%04X\n", offset, sizeof(idh->e_ovno), idh->e_ovno);
	offset = get_file_offset(fp, sizeof(idh->e_ovno));

	for (int i = 0; i < 4; i++)
	{
		printf("[%08X] : e_res[%d][%dbyte]\t:%04X\n", offset, i, sizeof(idh->e_res[i]), idh->e_res[i]);
		offset = get_file_offset(fp, sizeof(idh->e_res[i]));
	}

	printf("[%08X] : e_oemid[%dbyte]\t:%04X\n", offset, sizeof(idh->e_oemid), idh->e_oemid);
	offset = get_file_offset(fp, sizeof(idh->e_oemid));

	printf("[%08X] : e_oeminfo[%dbyte]\t:%04X\n", offset, sizeof(idh->e_oeminfo), idh->e_oeminfo);
	offset = get_file_offset(fp, sizeof(idh->e_oeminfo));


	for (int i = 0; i < 10; i++)
	{
		printf("[%08X] : e_res2[%d][%dbyte]\t:%04X\n", offset, i, sizeof(idh->e_res2[i]), idh->e_res2[i]);
		offset = get_file_offset(fp, sizeof(idh->e_res2[i]));
	}


	printf("[%08X] : e_lfanew[%dbyte]\t:%08X\n", offset, sizeof(idh->e_lfanew), idh->e_lfanew);
	offset = get_file_offset(fp, sizeof(idh->e_lfanew));

	printf("==================================\n\n");
}

void print_nt_header32(FILE* fp, IMAGE_NT_HEADERS32* inh32)
{

	printf("========== [NT Header 32] ==========\n\n");
	printf("[%08X] :Signature[%dbyte]\t\t\t:%08X\n\n", offset, sizeof(inh32->Signature), inh32->Signature);
	offset = get_file_offset(fp, sizeof(inh32->Signature));

	printf("---------- (FileHeader) ----------\n");
	printf("[%08X] : Machine[%dbyte]\t\t\t:%04X\n", offset, sizeof(inh32->FileHeader.Machine), inh32->FileHeader.Machine);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.Machine));

	printf("[%08X] : NumberOfSections[%dbyte]\t\t:%04X\n", offset, sizeof(inh32->FileHeader.NumberOfSections), inh32->FileHeader.NumberOfSections);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.NumberOfSections));

	printf("[%08X] : TimeDateStamp[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->FileHeader.TimeDateStamp), inh32->FileHeader.TimeDateStamp);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.TimeDateStamp));

	printf("[%08X] : PointerToSymbolTable[%dbyte]\t:%08X\n", offset, sizeof(inh32->FileHeader.PointerToSymbolTable), inh32->FileHeader.PointerToSymbolTable);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.PointerToSymbolTable));

	printf("[%08X] : NumberOfSymbols[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->FileHeader.NumberOfSymbols), inh32->FileHeader.NumberOfSymbols);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.NumberOfSymbols));

	printf("[%08X] : SizeOfOptionalHeader[%dbyte]\t:%04X\n", offset, sizeof(inh32->FileHeader.SizeOfOptionalHeader), inh32->FileHeader.SizeOfOptionalHeader);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.SizeOfOptionalHeader));

	printf("[%08X] : Characteristics[%dbyte]\t\t:%04X\n", offset, sizeof(inh32->FileHeader.Characteristics), inh32->FileHeader.Characteristics);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.Characteristics));
	printf("----------------------------------\n\n");

	printf("---------- (OptionalHeader32) ----------\n");
	printf("[%08X] : Magic[%dbyte]\t\t\t:%04X\n", offset, sizeof(inh32->OptionalHeader.Magic), inh32->OptionalHeader.Magic);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.Magic));

	printf("[%08X] : MajorLinkerVersion[%dbyte]\t\t:%02X\n", offset, sizeof(inh32->OptionalHeader.MajorLinkerVersion), inh32->OptionalHeader.MajorLinkerVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MajorLinkerVersion));

	printf("[%08X] : MinorLinkerVersion[%dbyte]\t\t:%02X\n", offset, sizeof(inh32->OptionalHeader.MinorLinkerVersion), inh32->OptionalHeader.MinorLinkerVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MinorLinkerVersion));

	printf("[%08X] : SizeofCode[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfCode), inh32->OptionalHeader.SizeOfCode);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfCode));

	printf("[%08X] : SizeofInitializedData[%dbyte]\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfInitializedData), inh32->OptionalHeader.SizeOfInitializedData);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfInitializedData));

	printf("[%08X] : SizeOfUninitializedData[%dbyte]\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfUninitializedData), inh32->OptionalHeader.SizeOfUninitializedData);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfUninitializedData));

	printf("[%08X] : AddressOfEntryPoint[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.AddressOfEntryPoint), inh32->OptionalHeader.AddressOfEntryPoint);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.AddressOfEntryPoint));

	printf("[%08X] : BaseOfCode[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.BaseOfCode), inh32->OptionalHeader.BaseOfCode);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.BaseOfCode));

	printf("[%08X] : BaseOfData[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.BaseOfData), inh32->OptionalHeader.BaseOfData);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.BaseOfData));

	printf("[%08X] : ImageBase[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.ImageBase), inh32->OptionalHeader.ImageBase);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.ImageBase));

	printf("[%08X] : SectionAlignment[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SectionAlignment), inh32->OptionalHeader.SectionAlignment);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SectionAlignment));

	printf("[%08X] : FileAlignment[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.FileAlignment), inh32->OptionalHeader.FileAlignment);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.FileAlignment));

	printf("[%08X] : MajorOperatingSystemVersion[%dbyte]\t:%04X\n", offset, sizeof(inh32->OptionalHeader.MajorOperatingSystemVersion), inh32->OptionalHeader.MajorOperatingSystemVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MajorOperatingSystemVersion));

	printf("[%08X] : MinorOperatingSystemVersion[%dbyte]\t:%04X\n", offset, sizeof(inh32->OptionalHeader.MinorOperatingSystemVersion), inh32->OptionalHeader.MinorOperatingSystemVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MinorOperatingSystemVersion));

	printf("[%08X] : MajorImageVersion[%dbyte]\t\t:%04X\n", offset, sizeof(inh32->OptionalHeader.MajorImageVersion), inh32->OptionalHeader.MajorImageVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MajorImageVersion));

	printf("[%08X] : MinorImageVersion[%dbyte]\t\t:%04X\n", offset, sizeof(inh32->OptionalHeader.MinorImageVersion), inh32->OptionalHeader.MinorImageVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MinorImageVersion));

	printf("[%08X] : MajorSubsystemVersion[%dbyte]\t:%04X\n", offset, sizeof(inh32->OptionalHeader.MajorSubsystemVersion), inh32->OptionalHeader.MajorSubsystemVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MajorOperatingSystemVersion));

	printf("[%08X] : MinorSubsystemVersion[%dbyte]\t:%04X\n", offset, sizeof(inh32->OptionalHeader.MinorSubsystemVersion), inh32->OptionalHeader.MinorSubsystemVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MinorSubsystemVersion));

	printf("[%08X] : Win32VersionValue[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.Win32VersionValue), inh32->OptionalHeader.Win32VersionValue);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.Win32VersionValue));

	printf("[%08X] : SizeOfImage[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfImage), inh32->OptionalHeader.SizeOfImage);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfImage));

	printf("[%08X] : SizeOfHeaders[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfHeaders), inh32->OptionalHeader.SizeOfHeaders);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfHeaders));

	printf("[%08X] : CheckSum[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.CheckSum), inh32->OptionalHeader.CheckSum);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.CheckSum));

	printf("[%08X] : Subsystem[%dbyte]\t\t\t:%04X\n", offset, sizeof(inh32->OptionalHeader.Subsystem), inh32->OptionalHeader.Subsystem);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.Subsystem));

	printf("[%08X] : DllCharacteristics[%dbyte]\t\t:%04X\n", offset, sizeof(inh32->OptionalHeader.DllCharacteristics), inh32->OptionalHeader.DllCharacteristics);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DllCharacteristics));

	printf("[%08X] : SizeOfStackReserve[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfStackReserve), inh32->OptionalHeader.SizeOfStackReserve);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfStackReserve));

	printf("[%08X] : SizeOfStackCommit[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfStackCommit), inh32->OptionalHeader.SizeOfStackCommit);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfStackCommit));

	printf("[%08X] : SizeOfHeapReserve[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfHeapReserve), inh32->OptionalHeader.SizeOfHeapReserve);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfHeapReserve));

	printf("[%08X] : SizeOfHeapCommit[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfHeapCommit), inh32->OptionalHeader.SizeOfHeapCommit);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfHeapCommit));

	printf("[%08X] : LoaderFlags[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.LoaderFlags), inh32->OptionalHeader.LoaderFlags);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.LoaderFlags));

	printf("[%08X] : NumberOfRvaAndSizes[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.NumberOfRvaAndSizes), inh32->OptionalHeader.NumberOfRvaAndSizes);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.NumberOfRvaAndSizes));

	/*for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++)
	{
		printf("- DataDirectory[%d].VirtualAddress[%dbyte]\t:%08X\n", i, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
		printf("- DataDirectory[%d].Size[%dbyte]\t\t\t:%08X\n", i, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);
	}
	*/

	print_inh32_datadirectory(fp, inh32);


	printf("----------------------------------------\n");
	printf("========================================\n\n");
}

void print_nt_header64(FILE* fp, IMAGE_NT_HEADERS64* inh64)
{
	printf("========== [NT Header 64] ==========\n\n");
	printf("[%08X] : Signature[%dbyte]\t\t\t:%08X\n\n", offset, sizeof(inh64->Signature), inh64->Signature);
	offset = get_file_offset(fp, sizeof(inh64->Signature));

	printf("---------- (FileHeader) ----------\n");
	printf("[%08X] : Machine[%dbyte]\t\t\t:%04X\n", offset, sizeof(inh64->FileHeader.Machine), inh64->FileHeader.Machine);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.Machine));

	printf("[%08X] : NumberOfSections[%dbyte]\t\t:%04X\n", offset, sizeof(inh64->FileHeader.NumberOfSections), inh64->FileHeader.NumberOfSections);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.NumberOfSections));

	printf("[%08X] : TimeDateStamp[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->FileHeader.TimeDateStamp), inh64->FileHeader.TimeDateStamp);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.TimeDateStamp));

	printf("[%08X] : PointerToSymbolTable[%dbyte]\t:%08X\n", offset, sizeof(inh64->FileHeader.PointerToSymbolTable), inh64->FileHeader.PointerToSymbolTable);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.PointerToSymbolTable));

	printf("[%08X] : NumberOfSymbols[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->FileHeader.NumberOfSymbols), inh64->FileHeader.NumberOfSymbols);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.NumberOfSymbols));

	printf("[%08X] : SizeOfOptionalHeader[%dbyte]\t:%04X\n", offset, sizeof(inh64->FileHeader.SizeOfOptionalHeader), inh64->FileHeader.SizeOfOptionalHeader);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.SizeOfOptionalHeader));

	printf("[%08X] : Characteristics[%dbyte]\t\t:%04X\n", offset, sizeof(inh64->FileHeader.Characteristics), inh64->FileHeader.Characteristics);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.Characteristics));
	printf("------------------------------------\n\n");

	printf("---------- (OptionalHeader64) ----------\n");
	printf("[%08X] : Magic[%dbyte]\t\t\t:%04X\n", offset, sizeof(inh64->OptionalHeader.Magic), inh64->OptionalHeader.Magic);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.Magic));

	printf("[%08X] : MajorLinkerVersion[%dbyte]\t\t:%02X\n", offset, sizeof(inh64->OptionalHeader.MajorLinkerVersion), inh64->OptionalHeader.MajorLinkerVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MajorLinkerVersion));

	printf("[%08X] : MinorLinkerVersion[%dbyte]\t\t:%02X\n", offset, sizeof(inh64->OptionalHeader.MinorLinkerVersion), inh64->OptionalHeader.MinorLinkerVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MinorLinkerVersion));

	printf("[%08X] : SizeofCode[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfCode), inh64->OptionalHeader.SizeOfCode);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfCode));

	printf("[%08X] : SizeofInitializedData[%dbyte]\t:%08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfInitializedData), inh64->OptionalHeader.SizeOfInitializedData);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfInitializedData));

	printf("[%08X] : SizeOfUninitializedData[%dbyte]\t:%08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfUninitializedData), inh64->OptionalHeader.SizeOfUninitializedData);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfUninitializedData));

	printf("[%08X] : AddressOfEntryPoint[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.AddressOfEntryPoint), inh64->OptionalHeader.AddressOfEntryPoint);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.AddressOfEntryPoint));

	printf("[%08X] : BaseOfCode[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.BaseOfCode), inh64->OptionalHeader.BaseOfCode);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.BaseOfCode));


	printf("[%08X] : ImageBase[%dbyte]\t\t\t:%016llX\n", offset, sizeof(inh64->OptionalHeader.ImageBase), inh64->OptionalHeader.ImageBase);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.ImageBase));

	printf("[%08X] : SectionAlignment[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.SectionAlignment), inh64->OptionalHeader.SectionAlignment);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SectionAlignment));

	printf("[%08X] : FileAlignment[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.FileAlignment), inh64->OptionalHeader.FileAlignment);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.FileAlignment));

	printf("[%08X] : MajorOperatingSystemVersion[%dbyte]\t:%04X\n", offset, sizeof(inh64->OptionalHeader.MajorOperatingSystemVersion), inh64->OptionalHeader.MajorOperatingSystemVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MajorOperatingSystemVersion));

	printf("[%08X] : MinorOperatingSystemVersion[%dbyte]\t:%04X\n", offset, sizeof(inh64->OptionalHeader.MinorOperatingSystemVersion), inh64->OptionalHeader.MinorOperatingSystemVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MinorOperatingSystemVersion));

	printf("[%08X] : MajorImageVersion[%dbyte]\t\t:%04X\n", offset, sizeof(inh64->OptionalHeader.MajorImageVersion), inh64->OptionalHeader.MajorImageVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MajorImageVersion));

	printf("[%08X] : MinorImageVersion[%dbyte]\t\t:%04X\n", offset, sizeof(inh64->OptionalHeader.MinorImageVersion), inh64->OptionalHeader.MinorImageVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MinorImageVersion));

	printf("[%08X] : MajorSubsystemVersion[%dbyte]\t:%04X\n", offset, sizeof(inh64->OptionalHeader.MajorSubsystemVersion), inh64->OptionalHeader.MajorSubsystemVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MajorSubsystemVersion));

	printf("[%08X] : MinorSubsystemVersion[%dbyte]\t:%04X\n", offset, sizeof(inh64->OptionalHeader.MinorSubsystemVersion), inh64->OptionalHeader.MinorSubsystemVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MinorSubsystemVersion));

	printf("[%08X] : Win32VersionValue[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.Win32VersionValue), inh64->OptionalHeader.Win32VersionValue);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.Win32VersionValue));

	printf("[%08X] : SizeOfImage[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfImage), inh64->OptionalHeader.SizeOfImage);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfImage));

	printf("[%08X] : SizeOfHeaders[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfHeaders), inh64->OptionalHeader.SizeOfHeaders);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfHeaders));

	printf("[%08X] : CheckSum[%d]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.CheckSum), inh64->OptionalHeader.CheckSum);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.CheckSum));

	printf("[%08X] : Subsystem[%dbyte]\t\t\t:%04X\n", offset, sizeof(inh64->OptionalHeader.Subsystem), inh64->OptionalHeader.Subsystem);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.Subsystem));

	printf("[%08X] : DllCharacteristics[%dbyte]\t\t:%04X\n", offset, sizeof(inh64->OptionalHeader.DllCharacteristics), inh64->OptionalHeader.DllCharacteristics);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DllCharacteristics));

	printf("[%08X] : SizeOfStackReserve[%dbyte]\t\t:%016llX\n", offset, sizeof(inh64->OptionalHeader.SizeOfStackReserve), inh64->OptionalHeader.SizeOfStackReserve);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfStackReserve));

	printf("[%08X] : SizeOfStackCommit[%dbyte]\t\t:%016llX\n", offset, sizeof(inh64->OptionalHeader.SizeOfStackCommit), inh64->OptionalHeader.SizeOfStackCommit);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfStackCommit));

	printf("[%08X] : SizeOfHeapReserve[%dbyte]\t\t:%016llX\n", offset, sizeof(inh64->OptionalHeader.SizeOfHeapReserve), inh64->OptionalHeader.SizeOfHeapReserve);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfHeapReserve));

	printf("[%08X] : SizeOfHeapCommit[%dbyte]\t\t:%016llX\n", offset, sizeof(inh64->OptionalHeader.SizeOfHeapCommit), inh64->OptionalHeader.SizeOfHeapCommit);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfHeapCommit));

	printf("[%08X] : LoaderFlags[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.LoaderFlags), inh64->OptionalHeader.LoaderFlags);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.LoaderFlags));

	printf("[%08X] : NumberOfRvaAndSizes[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.NumberOfRvaAndSizes), inh64->OptionalHeader.NumberOfRvaAndSizes);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.NumberOfRvaAndSizes));

	/*
	for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++)
	{
		printf("- DataDirectory[%d].VirtualAddress[%dbyte]\t:%08X\n", i, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
		printf("- DataDirectory[%d].Size[%dbyte]\t\t\t:%08X\n", i, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	}
	*/
	print_inh64_datadirectory(fp, inh64);
	printf("------------------------------------\n");
	printf("====================================\n\n");
}

void print_section_header(FILE* fp, IMAGE_SECTION_HEADER* ish, WORD section_num)
{
	printf("========== [SECTION HEADERS] ==========\n\n");

	for (int i = 0; i < section_num; i++, ish++)
	{
		//printf("%p\n", ish);
		printf("[%08X] : Section Name\t\t:%s\n", offset, ish->Name);
		offset = get_file_offset(fp, sizeof(ish->Name));

		//printf("- PhysicalAddress[%dbyte]\t:%08X\n", sizeof(ish->Misc.PhysicalAddress), ish->Misc.PhysicalAddress);
		printf("[%08X] : VirtualSize[%dbyte]\t\t:%08X\n", offset, sizeof(ish->Misc.VirtualSize), ish->Misc.VirtualSize);
		offset = get_file_offset(fp, sizeof(ish->Misc.VirtualSize));

		printf("[%08X] : VirtualAddress[%dbyte]\t:%08X\n", offset, sizeof(ish->VirtualAddress), ish->VirtualAddress);
		offset = get_file_offset(fp, sizeof(ish->VirtualAddress));

		printf("[%08X] : SizeOfRawData[%dbyte]\t:%08X\n", offset, sizeof(ish->SizeOfRawData), ish->SizeOfRawData);
		offset = get_file_offset(fp, sizeof(ish->SizeOfRawData));

		printf("[%08X] : PointerToRawData[%dbyte]\t:%08X\n", offset, sizeof(ish->PointerToRawData), ish->PointerToRawData);
		offset = get_file_offset(fp, sizeof(ish->PointerToRawData));

		printf("[%08X] : PointerToRelocations[%dbyte]:%08X\n", offset, sizeof(ish->PointerToRelocations), ish->PointerToRelocations);
		offset = get_file_offset(fp, sizeof(ish->PointerToRelocations));

		printf("[%08X] : PointerToLinenumbers[%dbyte]:%08X\n", offset, sizeof(ish->PointerToLinenumbers), ish->PointerToLinenumbers);
		offset = get_file_offset(fp, sizeof(ish->PointerToLinenumbers));

		printf("[%08X] : NumberOfRelocations[%dbyte]\t:%04X\n", offset, sizeof(ish->NumberOfRelocations), ish->NumberOfRelocations);
		offset = get_file_offset(fp, sizeof(ish->NumberOfRelocations));

		printf("[%08X] : NumberOfLineNumbers[%dbyte]\t:%04X\n", offset, sizeof(ish->NumberOfLinenumbers), ish->NumberOfLinenumbers);
		offset = get_file_offset(fp, sizeof(ish->NumberOfLinenumbers));

		printf("[%08X] : Characteristics[%dbyte]\t:%08X\n\n", offset, sizeof(ish->Characteristics), ish->Characteristics);
		offset = get_file_offset(fp, sizeof(ish->Characteristics));

		printf("=======================================\n\n");
	}
}

void print_inh32_datadirectory(FILE* fp, IMAGE_NT_HEADERS32* inh32)
{
	int i = 0;

	printf("[%08X] : EXPORT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : EXPORT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : IMPORT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : IMPORT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : RESOURCE Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : RESOURCE Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : EXCEPTION Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : EXCEPTION Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : SECURITY Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : SECURITY Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : BASERELOC Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : BASERELOC Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : DEBUG Directory RVA[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : DEBUG Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : COPYRIGHT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : COPYRIGHT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : GLOBALPTR Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : GLOBALPTR Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : TLS Directory RVA[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : TLS Directory Size[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : LOAD_CONFIG Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : LOAD_CONFIG Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : BOUND_IMPORT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : BOUND_IMPORT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : IAT Directory RVA[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : IAT Directory Size[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : DELAY_IMPORT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : DELAY_IMPORT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : COM_DESCRIPTOR Directory RVA[%dbyte]\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : COM_DESCRIPTOR Directory Size[%dbyte]\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;

	printf("[%08X] : Reserved Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : Reserved Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);

	i++;
}


void print_inh64_datadirectory(FILE* fp, IMAGE_NT_HEADERS64* inh64)
{
	int i = 0;

	printf("[%08X] : EXPORT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : EXPORT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : IMPORT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : IMPORT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : RESOURCE Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : RESOURCE Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : EXCEPTION Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : EXCEPTION Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : SECURITY Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : SECURITY Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : BASERELOC Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : BASERELOC Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : DEBUG Directory RVA[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : DEBUG Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : COPYRIGHT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : COPYRIGHT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : GLOBALPTR Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : GLOBALPTR Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : TLS Directory RVA[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : TLS Directory Size[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : LOAD_CONFIG Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : LOAD_CONFIG Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : BOUND_IMPORT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : BOUND_IMPORT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : IAT Directory RVA[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : IAT Directory Size[%dbyte]\t\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : DELAY_IMPORT Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : DELAY_IMPORT Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : COM_DESCRIPTOR Directory RVA[%dbyte]\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : COM_DESCRIPTOR Directory Size[%dbyte]\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;

	printf("[%08X] : Reserved Directory RVA[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress));
	printf("[%08X] : Reserved Directory Size[%dbyte]\t\t:%08X\n", offset, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DataDirectory[i].Size));
	i++;
}





int rva_to_raw(FILE* fp, u_char** binary_buf, DWORD rva_value)
{
	printf("\nrva_value : %X\n", rva_value);
	IMAGE_DOS_HEADER* rtr_idh = (IMAGE_DOS_HEADER*)*binary_buf;
	IMAGE_NT_HEADERS* rtr_inh = (IMAGE_NT_HEADERS*)(*binary_buf + rtr_idh->e_lfanew);

	// CPU의 값이 x86일 때
	if (rtr_inh->FileHeader.Machine == 0x014c)
	{
		IMAGE_NT_HEADERS32* rtr_inh32 = (IMAGE_NT_HEADERS32*)(*binary_buf + rtr_idh->e_lfanew);
		IMAGE_SECTION_HEADER* rtr_ish32 = (IMAGE_SECTION_HEADER*)(*binary_buf + rtr_idh->e_lfanew + sizeof(rtr_inh32->Signature) + sizeof(rtr_inh32->FileHeader) + rtr_inh32->FileHeader.SizeOfOptionalHeader);

		//printf("1\n");
		//printf("number of section : %d\n", rtr_inh32->FileHeader.NumberOfSections);
		for (int i = 0; i < rtr_inh32->FileHeader.NumberOfSections; i++, rtr_ish32++)
		{
			//printf("2\n");
			if ((rva_value >= (rtr_ish32->VirtualAddress + rtr_ish32->Misc.VirtualSize)) || (rva_value < rtr_ish32->VirtualAddress))
			{
				//printf("3\n");
				//printf("%s\n", rtr_ish32->Name);
				//printf("rtr_ish32->VirtualAddress : %x\n", rtr_ish32->VirtualAddress);
				//printf("rtr_ish32->VirtualAddress + rtr_ish32->Misc.VirtualSize: %X\n", rtr_ish32->VirtualAddress + rtr_ish32->Misc.VirtualSize);
				//printf("rva_to_raw failed\n");
				continue;
			}
			// rva_value의 값이 메모리에서의 해당 섹션의 max 크기보다 작고, 해당 섹션의 min 크기보다 크면
			else //if ((rva_value < (rtr_ish32->VirtualAddress + rtr_ish32->Misc.VirtualSize)) && (rva_value > rtr_ish32->VirtualAddress))
			{
				// RVA - VirtualAddress + PointerToRawData
				//printf("4\n");
				//printf("rtr_ish -> virtualaddress : %X\n", rtr_ish32->VirtualAddress);
				//printf("rtr_ish -> PointerToRawData : %X\n", rtr_ish32->PointerToRawData);

				//printf("raw : %X\n", rva_value - rtr_ish32->VirtualAddress + rtr_ish32->PointerToRawData);
				printf("%s\n", rtr_ish32->Name);
				//printf("rtr_ish32->VirtualAddress : %x\n", rtr_ish32->VirtualAddress);
				//printf("rtr_ish32->VirtualAddress + rtr_ish32->Misc.VirtualSize: %X\n", rtr_ish32->VirtualAddress + rtr_ish32->Misc.VirtualSize);
				//printf("rva_to_raw failed\n");
				int raw = rva_value - rtr_ish32->VirtualAddress + rtr_ish32->PointerToRawData;
				printf("raw value : %X\n", raw);
				return raw;
			}

		}
		//printf("5\n");
	}
	else if (rtr_inh->FileHeader.Machine == 0x0200 || rtr_inh->FileHeader.Machine == 0x8664)
	{
		IMAGE_NT_HEADERS64* rtr_inh64 = (IMAGE_NT_HEADERS64*)(binary_buf + rtr_idh->e_lfanew);
		IMAGE_SECTION_HEADER* rtr_ish64 = (IMAGE_SECTION_HEADER*)(*binary_buf + rtr_idh->e_lfanew + sizeof(rtr_inh64->Signature) + sizeof(rtr_inh64->FileHeader) + rtr_inh64->FileHeader.SizeOfOptionalHeader);

		for (int i = 0; i < rtr_inh64->FileHeader.NumberOfSections; i++, rtr_ish64++)
		{
			if ((rva_value > (rtr_ish64->VirtualAddress + rtr_ish64->Misc.VirtualSize)) && (rva_value < rtr_ish64->VirtualAddress))
			{
				continue;
			}
			// rva_value의 값이 메모리에서의 해당 섹션의 max 크기보다 작고, 해당 섹션의 min 크기보다 크면
			else //if ((rva_value < (rtr_ish64->VirtualAddress + rtr_ish64->Misc.VirtualSize)) && (rva_value > rtr_ish64->VirtualAddress))
			{
				// RVA - VirtualAddress + PointerToRawData
				//printf("rtr_ish -> virtualaddress : %X\n", rtr_ish64->VirtualAddress);
				//printf("rtr_ish -> PointerToRawData : %X\n", rtr_ish64->PointerToRawData);

				//printf("raw : %X\n", rva_value - rtr_ish64->VirtualAddress + rtr_ish64->PointerToRawData);

				int raw = rva_value - rtr_ish64->VirtualAddress + rtr_ish64->PointerToRawData;
				return raw;
			}

		}
	}
	else
	{
		printf("Unknown CPU Value\n");
		return 0;
	}

	return 0;
}