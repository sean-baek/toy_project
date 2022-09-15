#include "header.h"

void print_image_export_directory(FILE* fp, u_char** buf, IMAGE_EXPORT_DIRECTORY* ied)
{
	int raw = 0, raw2 = 0, num_of_names = 0, num_of_functions = 0;
	WORD ordinal = 0;
	DWORD eat_rva = 0, index = 0;
	IMAGE_EXPORT_DIRECTORY* pied = (IMAGE_EXPORT_DIRECTORY*)ied;

	// EXPORT dll ���
	printf("[%08X] - Characteristics[%dbyte]\t\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(pied->Characteristics), pied->Characteristics, rva_to_raw_dword(fp, buf, pied->Characteristics) );
	offset = get_file_offset(fp, sizeof(pied->Characteristics));

	printf("[%08X] - TimeDateStamp[%dbyte]\t\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(pied->TimeDateStamp), pied->TimeDateStamp, rva_to_raw_dword(fp, buf, pied->TimeDateStamp));
	offset = get_file_offset(fp, sizeof(pied->TimeDateStamp));

	printf("[%08X] - MajorVersion[%dbyte]\t\t: %04X(RVA), %04X(RAW)\n", offset, sizeof(pied->MajorVersion), pied->MajorVersion, rva_to_raw_dword(fp, buf, pied->MajorVersion));
	offset = get_file_offset(fp, sizeof(pied->MajorVersion));

	printf("[%08X] - MinorVersion[%dbyte]\t\t: %04X(RVA), %04X(RAW)\n", offset, sizeof(pied->MinorVersion), pied->MinorVersion, rva_to_raw_dword(fp, buf, pied->MinorVersion));
	offset = get_file_offset(fp, sizeof(pied->MinorVersion));

	raw = rva_to_raw_dword(fp, buf, ied->Name);
	printf("[%08X] - Name[%dbyte]\t\t\t: %08X(RVA), %08X(RAW), %s\n", offset, sizeof(pied->Name), ied->Name, raw, *buf + raw);
	offset = get_file_offset(fp, sizeof(pied->Name));

	printf("[%08X] - Base[%dbyte]\t\t\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(pied->Base), pied->Base, rva_to_raw_dword(fp, buf, pied->Base));
	offset = get_file_offset(fp, sizeof(pied->Base));

	printf("[%08X] - NumberOfFunctions[%dbyte]\t\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(pied->NumberOfFunctions), pied->NumberOfFunctions, rva_to_raw_dword(fp, buf, pied->NumberOfFunctions));
	offset = get_file_offset(fp, sizeof(pied->NumberOfFunctions));

	printf("[%08X] - NumberOfNames[%dbyte]\t\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(pied->NumberOfNames), pied->NumberOfNames, rva_to_raw_dword(fp, buf, pied->NumberOfNames));
	offset = get_file_offset(fp, sizeof(pied->NumberOfNames));

	printf("[%08X] - AddressOfFunctions[%dbyte]\t\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(pied->AddressOfFunctions), pied->AddressOfFunctions, rva_to_raw_dword(fp, buf, pied->AddressOfFunctions));
	offset = get_file_offset(fp, sizeof(pied->AddressOfFunctions));

	printf("[%08X] - AddressOfNames[%dbyte]\t\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(pied->AddressOfNames), pied->AddressOfNames, rva_to_raw_dword(fp, buf, pied->AddressOfNames));
	offset = get_file_offset(fp, sizeof(pied->AddressOfNames));

	printf("[%08X] - AddressOfNameOrdinals[%dbyte]\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(pied->AddressOfNameOrdinals), pied->AddressOfNameOrdinals, rva_to_raw_dword(fp, buf, pied->AddressOfNameOrdinals));
	offset = get_file_offset(fp, sizeof(pied->AddressOfNameOrdinals));

	printf("\n----------------------------------------\n\n");
	printf("-------------------- ( EAT ) --------------------\n\n");

	// �̸� �迭�� ����, �Լ� �迭�� ����
	num_of_names = pied->NumberOfNames;
	num_of_functions = pied->NumberOfFunctions;

	// �̸� �迭�� �ּ�(�̸� �迭�� �ִ� �� 4byte RVA ������ ����Ű�� ����)
	raw = rva_to_raw_dword(fp, buf, pied->AddressOfNames);
	DWORD *pnames =(DWORD*)(*buf + raw);

	// ordinals
	raw = rva_to_raw_dword(fp, buf, pied->AddressOfNameOrdinals);
	WORD* pordinals = (WORD*)(*buf + raw);

	// function
	raw = rva_to_raw_dword(fp, buf, pied->AddressOfFunctions);
	DWORD* pfunctions = (DWORD*)(*buf + raw);

	printf("number of names : %d\n\n", num_of_names);
	// EXPORT �Լ��� ���
	for (int i = 0; i < num_of_names; i++, pnames++)
	{
		// EXPORT �Լ� �̸�
		raw = rva_to_raw_dword(fp, buf, *pnames);

		// EXPORT �Լ� �̸��� �ش��ϴ� ordinal
		// i = name_index, 
		ordinal = *(pordinals + i);

		// EXPORT �Լ� �ּ� �迭���� + ordinal�� ��ġ�� �ش��ϴ� ��
		eat_rva = *(pfunctions + ordinal);

		for (int j = 0; j < num_of_functions; j++, pfunctions++)
		{
			if (eat_rva == *pfunctions)
			{
				index = (j+1);
				break;
			}
		}

		printf("%d\n", i);
		printf("%s\n%04X(Name Ordinal), %08X(Function Ordinal), %08X(Name RVA), %08X(Name RAW), %08X(Function RVA)\n\n",*buf + raw, ordinal, index, *pnames, raw, eat_rva);
		printf("-------------------------------------------------\n\n");
	}


	//�Լ� �̸� ���� Ordinal�θ� Export�� �Լ��� �ּҸ� ã�� ���� �ִ�.
	/*
	for (int i = 0; i < num_of_names; i++, pordinals++)
	{
		//printf("raw : %d\n", pied->Base);
		index = *pordinals - pied->Base;
		printf("index : %d\n", index);
		eat_rva = *(pfunctions + index);
		printf("test : %04X\n", eat_rva);
	}
	*/
}