#include "header.h"

void print_image_export_directory(FILE* fp, u_char** buf, IMAGE_EXPORT_DIRECTORY* ied)
{
	int raw = 0, num_of_names = 0, num_of_functions = 0;
	IMAGE_EXPORT_DIRECTORY* pied = (IMAGE_EXPORT_DIRECTORY*)ied;

	// EXPORT dll 출력
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

	// 이름 배열의 개수, 함수 배열의 개수
	num_of_names = pied->NumberOfNames;
	num_of_functions = pied->NumberOfFunctions;

	// EXPORT 함수 이름들 실제 RAW 위치
	raw = rva_to_raw_dword(fp, buf, pied->Name);
	char* offset_export_func_names = (char*)(*buf + raw + (strlen(*buf + raw) + 1));
		

	// 이름 배열의 주소(이름 배열에 있는 각 4byte RVA 값들을 가리키기 위해)
	raw = rva_to_raw_dword(fp, buf, pied->AddressOfNames);
	DWORD *pnames =(DWORD*)(*buf + raw);

	// ordinals
	raw = rva_to_raw_dword(fp, buf, pied->AddressOfNameOrdinals);
	WORD* pordinals = (WORD*)(*buf + raw);

	// function
	raw = rva_to_raw_dword(fp, buf, pied->AddressOfFunctions);
	DWORD* pfunctions = (DWORD*)(*buf + raw);

	printf("number of names : %d\n\n", num_of_names);

	// Export 함수 이름 배열(RAW)의 주소를 백업하여 사용
	char* p_offset_export_func_names = offset_export_func_names;

	// EXPORT 함수들 정보 출력
	for (int i = 0; i < num_of_names; i++, p_offset_export_func_names += (strlen(p_offset_export_func_names) + 1))
	{
		short ordinal = -1;
		DWORD eat_rva = 0, n_index = -1, f_index = -1;
		DWORD* ppnames = pnames;
		DWORD* ppfunctions = pfunctions;
		
		for (int j = 0; j < num_of_names; j++, ppnames++)
		{
			// VirtualOfNames RAW 주소에 있는 RVA 값들을 raw로 변환하여 해당 위치에 있는 문자열이
			// EXPORT 함수들의 이름 배열(RAW)에 있는 문자열과 일치한지 검사
			raw = rva_to_raw_dword(fp, buf, *ppnames);
			if (!strcmp(p_offset_export_func_names, *buf + raw))
			{
				n_index = j;
				break;
			}
		}		
		
		// EXPORT 함수 이름에 해당하는 ordinal
		// n_index = name_index, 
		if (n_index != -1)
		{
			ordinal = *(pordinals + n_index);
			printf("count : %d\n\n", i + 1);
			//printf("ordinal : %X\n", ordinal);
		}

		// EXPORT 함수 주소 배열에서 + ordinal한 위치에 해당하는 값
		if (ordinal != -1)
		{
			eat_rva = *(ppfunctions + ordinal);
			//printf("eat_rva : %X\n", eat_rva);
		}
		

		// function ordinal 구하기
		if (eat_rva != 0)
		{
			for (int k = 0; k < num_of_functions; k++, ppfunctions++)
			{
				if (eat_rva == *ppfunctions)
				{
					f_index = (k + 1);
					break;
				}
			}
		}
		
		if (eat_rva != 0)
		{
			printf("%s\n%d(Name Index), %04X(Name Ordinal), %08X(Name RVA), %08X(Name RAW), %08X(Function Ordinal), %08X(Function RVA)\n\n", p_offset_export_func_names, n_index, ordinal, *ppnames, raw, f_index, eat_rva);
			printf("-------------------------------------------------\n\n");
		}
		
		system("pause");
	}


	//함수 이름 없이 Ordinal로만 Export된 함수의 주소를 찾을 수도 있다.
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