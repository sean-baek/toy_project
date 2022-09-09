#define _CRT_SECURE_NO_WARNINGS
#include "header.h"

int main(int argc, char** argv)
{
	int size = 0, raw = 0;
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

	// 64bit 환경에서의 프로그램일 때
	if ((inh->FileHeader.Machine == 0x0200) || (inh->FileHeader.Machine == 0x8664))
	{
		printf("\n64bit Program\n\n");
		printf("File Size : %dbyte\n\n", size);

		/*show dos header*/
		print_dos_header(file, idh);



		// 64bit용 NT Header 포인터 변수 선언 및 buf에서 위치 지정
		IMAGE_NT_HEADERS64* inh64 = (IMAGE_NT_HEADERS64*)(buf + idh->e_lfanew);

		// 파일 포인터를 NT Header 위치로 이동
		fseek(file, idh->e_lfanew, SEEK_SET);
		offset = ftell(file);

		/*show 64bit용 NT Header*/ 
		print_nt_header64(file, inh64);



		// offset 출력을 위한 section header 처음 부분 가리키기
		fseek(file, (idh->e_lfanew + sizeof(inh64->Signature) + sizeof(inh64->FileHeader) + inh64->FileHeader.SizeOfOptionalHeader), SEEK_SET);
		offset = ftell(file);

		// print_section_header()에서 for문에 사용될 section 개수 구하기
		WORD section_num = inh64->FileHeader.NumberOfSections;
		
		/*show section header */ 
		print_section_header(file, ish, section_num);


		
		/* IMAGE_IMPORT_DESCRIPTOR */ 
		//DWORD val = inh->OptionalHeader.DataDirectory[1].VirtualAddress;
		raw = rva_to_raw(file, &buf, inh64->OptionalHeader.DataDirectory[1].VirtualAddress);
		IMAGE_IMPORT_DESCRIPTOR* iid64 = (IMAGE_IMPORT_DESCRIPTOR*)(buf + raw); // IID 구조체의 배열 시작 주소(RVA)를 가리킴

		int import_descriptor_size = 0;
		DWORD val = inh64->OptionalHeader.DataDirectory[1].VirtualAddress;

		raw = rva_to_raw(file, &buf, val);
		printf("raw : %d, %X\n", raw, raw);

	}
	// 32bit 환경에서의 프로그램일 때
	else if(inh->FileHeader.Machine == 0x014c)
	{
		printf("\n32bit Program\n\n");
		printf("File Size :%dbyte\n\n", size);


		/* dos header */
		print_dos_header(file, idh);

		// 32bit용 NT Header 포인터 변수 선언 및 buf에서 위치 지정
		IMAGE_NT_HEADERS32* inh32 = (IMAGE_NT_HEADERS32*)(buf + idh->e_lfanew);

		// 파일 포인터를 NT header 위치로 이동
		fseek(file, idh->e_lfanew, SEEK_SET);
		offset = ftell(file);
		/*show 32bit용 NT Header*/
		print_nt_header32(file, inh32);


		// offset 출력을 위한 section header 처음 부분 가리키기
		fseek(file, (idh->e_lfanew + sizeof(inh32->Signature) + sizeof(inh32->FileHeader) + inh32->FileHeader.SizeOfOptionalHeader), SEEK_SET);
		offset = ftell(file);
		// print_section_header()에서 for문에 사용될 section 개수 구하기
		WORD section_num = inh32->FileHeader.NumberOfSections;
		/*show section header */
		print_section_header(file, ish, section_num);



		// IMAGE_IMPORT_DESCRIPTOR
		printf("========== [IMAGE_IMPORT_DESCRIPTOR] ==========\n\n");
		// IMAGE_IMPORT_DESCRIPTOR 구조체 배열의 시작 주소 RVA 값을 RAW로 변환
		raw = rva_to_raw(file, &buf, inh32->OptionalHeader.DataDirectory[1].VirtualAddress);
		printf("IMPORT Directory RAW : %X\n\n", raw);
		// IMAGE_IMPORT_DESCRIPTOR 구조체 배열의 실제 주소를 지정
		IMAGE_IMPORT_DESCRIPTOR* iid = (IMAGE_IMPORT_DESCRIPTOR*)(buf + raw);
		// IID 구조체 배열의 크기
		int import_descriptor_size = inh32->OptionalHeader.DataDirectory[1].Size;

		fseek(file, raw, SEEK_SET);
		offset = ftell(file);
		
		print_image_import_descriptor(file, &buf, iid, import_descriptor_size);
		printf("===============================================\n\n");


		// INT
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
		{
			printf("malloc() error\n");
			return 0;
		}

		if (buf == NULL)
		{
			printf("동적 메모리를 할당할 수 없습니다.\n");
			return 0;
		}

		if (memset(*buf, 0x00, size) == NULL)
		{
			printf("memset() error\n");
			return 0;
		}

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

void print_dos_header(FILE *fp ,IMAGE_DOS_HEADER* idh)
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
	//printf("rva value : %X\n", rva_value);
	int raw;
	IMAGE_DOS_HEADER* rtr_idh = (IMAGE_DOS_HEADER*)*binary_buf;
	IMAGE_NT_HEADERS* rtr_inh = (IMAGE_NT_HEADERS*)(*binary_buf + rtr_idh->e_lfanew);

	// CPU의 값이 x86일 때
	if (rtr_inh->FileHeader.Machine == 0x014c)
	{
		IMAGE_NT_HEADERS32* rtr_inh32 = (IMAGE_NT_HEADERS32*)(*binary_buf + rtr_idh->e_lfanew);
		IMAGE_SECTION_HEADER* rtr_ish32 = (IMAGE_SECTION_HEADER*)(*binary_buf + rtr_idh->e_lfanew + sizeof(rtr_inh32->Signature) + sizeof(rtr_inh32->FileHeader) + rtr_inh32->FileHeader.SizeOfOptionalHeader);
		
		
		for (int i = 0; i < rtr_inh32->FileHeader.NumberOfSections; i++, rtr_ish32++)
		{
			// rva 값이 현재 section의 max치 메모리 값 이상이거나 min치 메모리 값보다 작으면 진행하지 않고 for문으로 되돌아간다.
			if (rva_value >= (rtr_ish32->VirtualAddress + rtr_ish32->Misc.VirtualSize) || rva_value < rtr_ish32->VirtualAddress)
			{
				
				continue;
			}
			else
			{
				raw = rva_value - rtr_ish32->VirtualAddress + rtr_ish32->PointerToRawData;
				return raw;
			}
			
		}
	}
	else if (rtr_inh->FileHeader.Machine == 0x0200 || rtr_inh->FileHeader.Machine == 0x8664)
	{
		IMAGE_NT_HEADERS64* rtr_inh64 = (IMAGE_NT_HEADERS64*)(binary_buf + rtr_idh->e_lfanew);
		IMAGE_SECTION_HEADER* rtr_ish64 = (IMAGE_SECTION_HEADER*)(*binary_buf + rtr_idh->e_lfanew + sizeof(rtr_inh64->Signature) + sizeof(rtr_inh64->FileHeader) + rtr_inh64->FileHeader.SizeOfOptionalHeader);

		for (int i = 0; i < rtr_inh64->FileHeader.NumberOfSections; i++, rtr_ish64++)
		{
			// rva 값이 현재 section의 max치 메모리 값 이상이거나 min치 메모리 값보다 작으면 진행하지 않고 for문으로 되돌아간다.
			if ((rva_value >= (rtr_ish64->VirtualAddress + rtr_ish64->Misc.VirtualSize)) && (rva_value < rtr_ish64->VirtualAddress))
			{
				continue;
			}
			else
			{
				raw = rva_value - rtr_ish64->VirtualAddress + rtr_ish64->PointerToRawData;
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

void print_image_import_descriptor(FILE* fp, u_char** buf, IMAGE_IMPORT_DESCRIPTOR* iid, int size)
{
	IMAGE_IMPORT_DESCRIPTOR* piid = (IMAGE_IMPORT_DESCRIPTOR *)iid;
	int raw = 0;
	for (int i = 0; i < size; i++, piid++)
	{
		if (piid->Characteristics == 0x00000000 && piid->OriginalFirstThunk == 0x00000000 && piid->TimeDateStamp == 0x00000000 && piid->ForwarderChain == 0x00000000 && piid->Name == 0x00000000 && piid->FirstThunk == 0x00000000)
			break;

		printf("[%08X] : OriginalFirstThunk[%dbyte]\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(piid->OriginalFirstThunk), piid->OriginalFirstThunk, rva_to_raw(fp, buf, piid->OriginalFirstThunk));
		offset = get_file_offset(fp, sizeof(piid->OriginalFirstThunk));

		printf("[%08X] : TimeDateStamp[%dbyte]\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(piid->TimeDateStamp), piid->TimeDateStamp, rva_to_raw(fp, buf, piid->TimeDateStamp));
		offset = get_file_offset(fp, sizeof(piid->TimeDateStamp));

		printf("[%08X] : ForwarderChain[%dbyte]\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(piid->ForwarderChain), piid->ForwarderChain, rva_to_raw(fp, buf, piid->ForwarderChain));
		offset = get_file_offset(fp, sizeof(piid->ForwarderChain));

		raw = rva_to_raw(fp, buf, piid->Name);
		printf("[%08X] : Name[%dbyte]\t\t: %08X(RVA), %s\n", offset, sizeof(piid->Name), piid->Name, *buf + raw);
		offset = get_file_offset(fp, sizeof(piid->Name));

		printf("[%08X] : FirstThunk[%dbyte]\t\t: %08X(RVA), %08X(RAW)\n\n", offset, sizeof(piid->FirstThunk), piid->FirstThunk, rva_to_raw(fp, buf, piid->FirstThunk));
		offset = get_file_offset(fp, sizeof(piid->FirstThunk));

		printf("-----------------------------------------------\n");
	}

	// dll 라이브러리들 이름은 출력했으니 각 라이브러리의 함수들의 hint와 이름 출력
	printf("\n-------------- (IMAGE NAME TABLE) --------------\n\n");
	piid = (IMAGE_IMPORT_DESCRIPTOR*)iid;
	for (int i = 0; i < size; i++, piid++)
	{
		if (piid->Characteristics == 0x00000000 && piid->OriginalFirstThunk == 0x00000000 && piid->TimeDateStamp == 0x00000000 && piid->ForwarderChain == 0x00000000 && piid->Name == 0x00000000 && piid->FirstThunk == 0x00000000)
			break;

		// IMPORT 함수가 어떤 dll 라이브러리에 속해있는지 확인하기 위해 라이브러리 이름 출력
		offset = get_file_offset(fp, sizeof(piid->ForwarderChain));
		raw = rva_to_raw(fp, buf, piid->Name);
		printf("\n[%08X] [***** %s *****]\n", offset, *buf + raw);

		// IMAGE IMPORT Descriptor 구조체의 OriginalFirstThunk 구조체에 있는 값(RVA)은 IMAGE_THUNK_DATA의 멤버 변수의 값이다.
		raw = rva_to_raw(fp, buf, piid->OriginalFirstThunk);
		IMAGE_THUNK_DATA32* itd_oft32 = (IMAGE_THUNK_DATA32*)(*buf + raw);
		
		raw = rva_to_raw(fp, buf, piid->FirstThunk);
		IMAGE_THUNK_DATA32* itd_ft32 = (IMAGE_THUNK_DATA32*)(*buf + raw);

		// IMAGE THUNK DATA 구조체의 마지막은 0x00000000 값이다.
		// 해당 값이 아닐 동안 반복하여 dll 라이브러리 속 함수들 출력
		for ( ;itd_oft32->u1.AddressOfData != 0x00000000; itd_oft32++, itd_ft32++)
		{
			raw = rva_to_raw(fp, buf, itd_oft32->u1.AddressOfData);
			IMAGE_IMPORT_BY_NAME* iibn32 = (IMAGE_IMPORT_BY_NAME*)(*buf + raw);

			fseek(fp, raw, SEEK_SET);
			printf("[%08X] iibn32->Hint : %X\n", ftell(fp), iibn32->Hint);

			offset = get_file_offset(fp, sizeof(iibn32->Hint));
			printf("[%08X] iibn32->Name : %s()\n", offset, iibn32->Name);

			fseek(fp, rva_to_raw(fp, buf, piid->FirstThunk), SEEK_SET);
			offset = ftell(fp);
			printf("[%08X] IAT : %X(RVA), %X(RAW)\n\n", offset, itd_ft32->u1.Function, rva_to_raw(fp, buf, itd_ft32->u1.Function));
		}

		printf("\n");
	}
	printf("-----------------------------------------------\n");
}