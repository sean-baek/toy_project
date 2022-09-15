#include "header.h"

int offset = 0;

int main(int argc, char** argv)
{
	FILE* file = NULL;
	u_char* buf = NULL;
	int file_size = 0, raw = 0;

	if (argc < 2)
	{
		printf("�м��Ͻ� ������ �Է����ּ���.\n");
		return -1;
	}

	if ((file = fopen(argv[1], "rb")) == NULL)
	{
		printf("�ش� ������ ���� �� �����ϴ�.\n%s\n", strerror(errno));
		return -1;
	}
	
	// ������ ũ�⸦ ���ϰ� ������ �д´�.
	// ��ȯ���� ������ �������̴�.
	file_size = get_file_content(file, &buf);

	/* dos header */
	IMAGE_DOS_HEADER* idh = (IMAGE_DOS_HEADER*)buf;

	/* dos header->e_lfanew : NT header ����ü�� ���� offset �� */
	IMAGE_NT_HEADERS* inh = (IMAGE_NT_HEADERS*)(buf + idh->e_lfanew);

	/* FILE HEADER : NT header ����ü�� ���� �κп��� + 4byte�� offset */
	//IMAGE_FILE_HEADER* ifh = (IMAGE_FILE_HEADER*)(buf + idh->e_lfanew + inh->Signature);

	/* OPTIONAL header : NT header ����ü�� ���� �κ� + 4byte + file header ũ�� �� offset */
	//IMAGE_OPTIONAL_HEADER* ioh = (IMAGE_OPTIONAL_HEADER*)(buf + idh->e_lfanew + inh->Signature + sizeof(inh->FileHeader));
	
	/* Section header : NT header ����ü�� ���� �κ� + 4byte + file header ũ�� + sizeofOptionalheader�� �� �� offset */
	IMAGE_SECTION_HEADER* ish = (IMAGE_SECTION_HEADER*)(buf + idh->e_lfanew + sizeof(inh->Signature) + sizeof(inh->FileHeader) + inh->FileHeader.SizeOfOptionalHeader);
	

	/* todo */

	// ���α׷��� dos �������� �˻�
	if ((idh->e_magic != IMAGE_DOS_SIGNATURE) || (inh->Signature != IMAGE_NT_SIGNATURE))
	{
		printf("�ش� ������ PE ������ �ƴմϴ�.\n");
		return -1;
	}


	// 32bit ���α׷��� ��
	//if (inh->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 || inh->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	if (inh->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		IMAGE_NT_HEADERS32* inh32 = (IMAGE_NT_HEADERS32*)(buf + idh->e_lfanew);

		printf("\n32bit Program\n\n");
		
		// ������ ũ�Ⱑ ������ ���
		printf("File Size : %d byte\n\n", file_size);
		
		/* dos header */
		print_dos_header(file, idh);

		// ���� �����͸� NT header ��ġ�� �̵�
		//offset = set_file_offset(file, idh->e_lfanew);
		/* 32bit�� NT Header */

		print_nt_header32(file, idh, inh32);


		// ���� �����͸� Section header ��ġ�� �̵�
		offset = set_file_offset(file, (idh->e_lfanew + sizeof(inh32->Signature) + sizeof(inh32->FileHeader) + inh32->FileHeader.SizeOfOptionalHeader));		
		// print_section_header()���� for���� ���� section ���� ���ϱ�
		WORD section_num = inh32->FileHeader.NumberOfSections;
		/* Section header */
		print_section_header(file, ish, section_num);

		/* IMAGE_IMPORT_DESCRIPTOR */
		printf("========== [IMAGE_IMPORT_DESCRIPTOR] ==========\n\n");
		// IMAGE_IMPORT_DESCRIPTOR ����ü �迭�� ���� �ּ� RVA ���� RAW�� ��ȯ
		raw = rva_to_raw_dword(file, &buf, inh32->OptionalHeader.DataDirectory[1].VirtualAddress);
		// IMPORT Directory ���Ͽ����� �ּ�
		printf("IMPORT DESCRIPTOR RAW : %X\n\n", raw);
		// IMAGE_IMPORT_DESCRIPTOR ����ü �迭�� ���� �ּҸ� ����
		IMAGE_IMPORT_DESCRIPTOR* iid = (IMAGE_IMPORT_DESCRIPTOR*)(buf + raw);
		// IID ����ü �迭�� ũ��
		int import_descriptor_size = inh32->OptionalHeader.DataDirectory[1].Size / sizeof(IMAGE_IMPORT_DESCRIPTOR);
		// IID ����ü �� ���
		offset = set_file_offset(file, raw);
		print_image_import_descriptor(file, &buf, iid, import_descriptor_size);
		printf("===============================================\n\n");


		/* IMAGE_EXPORT_DERECTORY */
		printf("========== [IMAGE_EXPORT_DIRECTORY] ==========\n\n");
		// EXPORT directory�� ���� Ȯ��
		if (inh32->OptionalHeader.DataDirectory[0].VirtualAddress == 0x00000000)
			printf("EXPORT DIRECTORY�� �������� �ʽ��ϴ�.\n");
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
	// 64bit ���α׷��� ��
	//else if (inh->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64 || inh->OptionalHeader.Magic == IMAGE_FILE_MACHINE_IA64 || inh->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	else if (inh->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		IMAGE_NT_HEADERS64* inh64 = (IMAGE_NT_HEADERS64*)(buf + idh->e_lfanew);

		printf("\n64bit Program\n\n");


		// ������ ũ�Ⱑ ������ ���
		printf("File Size : %d byte\n\n", file_size);


		/* dos header */
		print_dos_header(file, idh);


		// ���� �����͸� NT header ��ġ�� �̵�
		//offset = set_file_offset(file, idh->e_lfanew);
		/*64bit�� NT Header*/
		print_nt_header64(file, idh, inh64);


		// ���� �����͸� Section header ��ġ�� �̵�
		offset = set_file_offset(file, idh->e_lfanew + sizeof(inh64->Signature) + sizeof(inh64->FileHeader) + inh64->FileHeader.SizeOfOptionalHeader);
		// print_section_header()���� for���� ���� section ���� ���ϱ�
		WORD section_num = inh64->FileHeader.NumberOfSections;
		/* Section header */
		print_section_header(file, ish, section_num);


		/* IMAGE_IMPORT_DESCRIPTOR */
		printf("==================== [IMAGE_IMPORT_DESCRIPTOR] ====================\n\n");
		// IMAGE_IMPORT_DESCRIPTOR ����ü �迭�� ���� �ּ� RVA ���� RAW�� ��ȯ
		raw = rva_to_raw_dword(file, &buf, inh64->OptionalHeader.DataDirectory[1].VirtualAddress);
		printf("IMPORT Directory RAW : %X\n\n", raw);
		// IMAGE_IMPORT_DESCRIPTOR ����ü �迭�� ���� �ּҸ� ����
		IMAGE_IMPORT_DESCRIPTOR* iid = (IMAGE_IMPORT_DESCRIPTOR*)(buf + raw);
		// IID ����ü �迭�� ũ��
		int import_descriptor_size = inh64->OptionalHeader.DataDirectory[1].Size / sizeof(IMAGE_DATA_DIRECTORY);
		// IID ����ü �� ���
		offset = set_file_offset(file, raw);
		print_image_import_descriptor(file, &buf, iid, import_descriptor_size);
		printf("===================================================================\n\n");


		/* IMAGE_EXPORT_DERECTORY */
		printf("========== [IMAGE_EXPORT_DIRECTORY] ==========\n\n");
		// EXPORT directory�� ���� Ȯ��
		if (inh64->OptionalHeader.DataDirectory[0].VirtualAddress == 0x00000000)
			printf("EXPORT DIRECTORY�� �������� �ʽ��ϴ�.\n");
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