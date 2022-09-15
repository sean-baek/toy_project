#include "header.h"

int offset = 0;

int main(int argc, char** argv)
{
	FILE* file = NULL;
	u_char* buf = NULL;
	int file_size = 0, raw = 0;

	if (argc < 2)
	{
		printf("분석하실 파일을 입력해주세요.\n");
		return -1;
	}

	if ((file = fopen(argv[1], "rb")) == NULL)
	{
		printf("해당 파일을 읽을 수 없습니다.\n%s\n", strerror(errno));
		return -1;
	}
	
	// 파일의 크기를 구하고 내용을 읽는다.
	// 반환값은 파일의 사이즈이다.
	file_size = get_file_content(file, &buf);

	/* dos header */
	IMAGE_DOS_HEADER* idh = (IMAGE_DOS_HEADER*)buf;

	/* dos header->e_lfanew : NT header 구조체의 시작 offset 값 */
	IMAGE_NT_HEADERS* inh = (IMAGE_NT_HEADERS*)(buf + idh->e_lfanew);

	/* FILE HEADER : NT header 구조체의 시작 부분에서 + 4byte한 offset */
	//IMAGE_FILE_HEADER* ifh = (IMAGE_FILE_HEADER*)(buf + idh->e_lfanew + inh->Signature);

	/* OPTIONAL header : NT header 구조체의 시작 부분 + 4byte + file header 크기 한 offset */
	//IMAGE_OPTIONAL_HEADER* ioh = (IMAGE_OPTIONAL_HEADER*)(buf + idh->e_lfanew + inh->Signature + sizeof(inh->FileHeader));
	
	/* Section header : NT header 구조체의 시작 부분 + 4byte + file header 크기 + sizeofOptionalheader의 값 한 offset */
	IMAGE_SECTION_HEADER* ish = (IMAGE_SECTION_HEADER*)(buf + idh->e_lfanew + sizeof(inh->Signature) + sizeof(inh->FileHeader) + inh->FileHeader.SizeOfOptionalHeader);
	

	/* todo */

	// 프로그램이 dos 파일인지 검사
	if ((idh->e_magic != IMAGE_DOS_SIGNATURE) || (inh->Signature != IMAGE_NT_SIGNATURE))
	{
		printf("해당 파일은 PE 파일이 아닙니다.\n");
		return -1;
	}


	// 32bit 프로그램일 때
	//if (inh->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 || inh->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	if (inh->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		IMAGE_NT_HEADERS32* inh32 = (IMAGE_NT_HEADERS32*)(buf + idh->e_lfanew);

		printf("\n32bit Program\n\n");
		
		// 파일의 크기가 몇인지 출력
		printf("File Size : %d byte\n\n", file_size);
		
		/* dos header */
		print_dos_header(file, idh);

		// 파일 포인터를 NT header 위치로 이동
		//offset = set_file_offset(file, idh->e_lfanew);
		/* 32bit용 NT Header */

		print_nt_header32(file, idh, inh32);


		// 파일 포인터를 Section header 위치로 이동
		offset = set_file_offset(file, (idh->e_lfanew + sizeof(inh32->Signature) + sizeof(inh32->FileHeader) + inh32->FileHeader.SizeOfOptionalHeader));		
		// print_section_header()에서 for문에 사용될 section 개수 구하기
		WORD section_num = inh32->FileHeader.NumberOfSections;
		/* Section header */
		print_section_header(file, ish, section_num);

		/* IMAGE_IMPORT_DESCRIPTOR */
		printf("========== [IMAGE_IMPORT_DESCRIPTOR] ==========\n\n");
		// IMAGE_IMPORT_DESCRIPTOR 구조체 배열의 시작 주소 RVA 값을 RAW로 변환
		raw = rva_to_raw_dword(file, &buf, inh32->OptionalHeader.DataDirectory[1].VirtualAddress);
		// IMPORT Directory 파일에서의 주소
		printf("IMPORT DESCRIPTOR RAW : %X\n\n", raw);
		// IMAGE_IMPORT_DESCRIPTOR 구조체 배열의 실제 주소를 지정
		IMAGE_IMPORT_DESCRIPTOR* iid = (IMAGE_IMPORT_DESCRIPTOR*)(buf + raw);
		// IID 구조체 배열의 크기
		int import_descriptor_size = inh32->OptionalHeader.DataDirectory[1].Size / sizeof(IMAGE_IMPORT_DESCRIPTOR);
		// IID 구조체 값 출력
		offset = set_file_offset(file, raw);
		print_image_import_descriptor(file, &buf, iid, import_descriptor_size);
		printf("===============================================\n\n");


		/* IMAGE_EXPORT_DERECTORY */
		printf("========== [IMAGE_EXPORT_DIRECTORY] ==========\n\n");
		// EXPORT directory의 유무 확인
		if (inh32->OptionalHeader.DataDirectory[0].VirtualAddress == 0x00000000)
			printf("EXPORT DIRECTORY가 존재하지 않습니다.\n");
		else
		{
			raw = rva_to_raw_dword(file, &buf, inh32->OptionalHeader.DataDirectory[0].VirtualAddress);
			printf("EXPORT Directory RAW : %08X\n\n", raw);

			IMAGE_EXPORT_DIRECTORY* ied = (IMAGE_EXPORT_DIRECTORY*)(buf + raw);
			int export_directory_size = inh32->OptionalHeader.DataDirectory[0].Size / sizeof(IMAGE_EXPORT_DIRECTORY);
			offset = set_file_offset(file, raw);
			print_image_export_directory(file, &buf, ied);
		}
		printf("\n==============================================\n\n");
	}
	// 64bit 프로그램일 때
	//else if (inh->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 || inh->OptionalHeader.Magic == IMAGE_FILE_MACHINE_IA64 || inh->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	else if (inh->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		IMAGE_NT_HEADERS64* inh64 = (IMAGE_NT_HEADERS64*)(buf + idh->e_lfanew);

		printf("\n64bit Program\n\n");


		// 파일의 크기가 몇인지 출력
		printf("File Size : %d byte\n\n", file_size);


		/* dos header */
		print_dos_header(file, idh);


		// 파일 포인터를 NT header 위치로 이동
		//offset = set_file_offset(file, idh->e_lfanew);
		/*64bit용 NT Header*/
		print_nt_header64(file, idh, inh64);


		// 파일 포인터를 Section header 위치로 이동
		offset = set_file_offset(file, idh->e_lfanew + sizeof(inh64->Signature) + sizeof(inh64->FileHeader) + inh64->FileHeader.SizeOfOptionalHeader);
		// print_section_header()에서 for문에 사용될 section 개수 구하기
		WORD section_num = inh64->FileHeader.NumberOfSections;
		/* Section header */
		print_section_header(file, ish, section_num);


		/* IMAGE_IMPORT_DESCRIPTOR */
		printf("==================== [IMAGE_IMPORT_DESCRIPTOR] ====================\n\n");
		// IMAGE_IMPORT_DESCRIPTOR 구조체 배열의 시작 주소 RVA 값을 RAW로 변환
		raw = rva_to_raw_dword(file, &buf, inh64->OptionalHeader.DataDirectory[1].VirtualAddress);
		printf("IMPORT Directory RAW : %X\n\n", raw);
		// IMAGE_IMPORT_DESCRIPTOR 구조체 배열의 실제 주소를 지정
		IMAGE_IMPORT_DESCRIPTOR* iid = (IMAGE_IMPORT_DESCRIPTOR*)(buf + raw);
		// IID 구조체 배열의 크기
		int import_descriptor_size = inh64->OptionalHeader.DataDirectory[1].Size / sizeof(IMAGE_DATA_DIRECTORY);
		// IID 구조체 값 출력
		offset = set_file_offset(file, raw);
		print_image_import_descriptor(file, &buf, iid, import_descriptor_size);
		printf("===================================================================\n\n");


		/* IMAGE_EXPORT_DERECTORY */
		printf("========== [IMAGE_EXPORT_DIRECTORY] ==========\n\n");
		// EXPORT directory의 유무 확인
		if (inh64->OptionalHeader.DataDirectory[0].VirtualAddress == 0x00000000)
			printf("EXPORT DIRECTORY가 존재하지 않습니다.\n");
		else
		{
			raw = rva_to_raw_dword(file, &buf, inh64->OptionalHeader.DataDirectory[0].VirtualAddress);
			printf("EXPORT Directory RAW : %X\n", raw);

			IMAGE_EXPORT_DIRECTORY* ied = (IMAGE_EXPORT_DIRECTORY*)(buf + raw);
			//int export_directory_size = inh64->OptionalHeader.DataDirectory[0].Size / sizeof(IMAGE_EXPORT_DIRECTORY);
			int export_directory_size = inh64->OptionalHeader.DataDirectory[0].Size / sizeof(IMAGE_EXPORT_DIRECTORY);
			offset = set_file_offset(file, raw);
			print_image_export_directory(file, &buf, ied);
		}
		printf("\n==============================================\n\n");
	}

	free(buf);
	fclose(file);
	return 0;
}