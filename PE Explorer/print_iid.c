#include "header.h"

void print_image_import_descriptor(FILE* fp, u_char** buf, IMAGE_IMPORT_DESCRIPTOR* iid, int size)
{
	IMAGE_DOS_HEADER* piid_idh = (IMAGE_DOS_HEADER*)*buf;
	IMAGE_NT_HEADERS* piid_inh = (IMAGE_NT_HEADERS*)(*buf + piid_idh->e_lfanew);
	IMAGE_OPTIONAL_HEADER* piid_ioh = (IMAGE_OPTIONAL_HEADER*)(*buf + piid_idh->e_lfanew + sizeof(piid_inh->Signature) + sizeof(piid_inh->FileHeader));
	IMAGE_IMPORT_DESCRIPTOR* piid = (IMAGE_IMPORT_DESCRIPTOR*)iid;
	int raw = 0;
	ULONGLONG ull_raw = 0;

	// IMPORT DLL ���ϵ� ���
	for (int i = 0; i < size; i++, piid++)
	{
		// IMAGE_IMPORT_DESCRIPTOR ����ü�� ������ �κ��� 0���� �Ǿ� ����
		if (piid->Characteristics == 0x00000000 && piid->OriginalFirstThunk == 0x00000000 && piid->TimeDateStamp == 0x00000000 && piid->ForwarderChain == 0x00000000 && piid->Name == 0x00000000 && piid->FirstThunk == 0x00000000)
			break;

		printf("[%08X] - OriginalFirstThunk[%dbyte]\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(piid->OriginalFirstThunk), piid->OriginalFirstThunk, rva_to_raw_dword(fp, buf, piid->OriginalFirstThunk));
		offset = get_file_offset(fp, sizeof(piid->OriginalFirstThunk));

		printf("[%08X] - TimeDateStamp[%dbyte]\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(piid->TimeDateStamp), piid->TimeDateStamp, rva_to_raw_dword(fp, buf, piid->TimeDateStamp));
		offset = get_file_offset(fp, sizeof(piid->TimeDateStamp));

		printf("[%08X] - ForwarderChain[%dbyte]\t: %08X(RVA), %08X(RAW)\n", offset, sizeof(piid->ForwarderChain), piid->ForwarderChain, rva_to_raw_dword(fp, buf, piid->ForwarderChain));
		offset = get_file_offset(fp, sizeof(piid->ForwarderChain));

		raw = rva_to_raw_dword(fp, buf, piid->Name);
		printf("[%08X] - Name[%dbyte]\t\t: %08X(RVA), %08X(RAW), %s\n", offset, sizeof(piid->Name), piid->Name, raw, *buf + raw);
		offset = get_file_offset(fp, sizeof(piid->Name));

		printf("[%08X] - FirstThunk[%dbyte]\t\t: %08X(RVA), %08X(RAW)\n\n", offset, sizeof(piid->FirstThunk), piid->FirstThunk, rva_to_raw_dword(fp, buf, piid->FirstThunk));
		offset = get_file_offset(fp, sizeof(piid->FirstThunk));

		printf("-----------------------------------------------\n\n");
	}

	// dll ���̺귯���� �̸��� ��������� �� ���̺귯���� �Լ����� hint�� �̸� ���
	printf("-------------- (IMAGE NAME TABLE, IMAGE IAT TABLE) --------------\n\n");
	piid = (IMAGE_IMPORT_DESCRIPTOR*)iid;
	
	if (piid_ioh->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		// IMAGE_IMPORT_DESCRIPTOR ����ü�� ũ�⸸ŭ �ݺ��Ѵ�.
		for (int i = 0; i < size; i++, piid++)
		{
			if (piid->Characteristics == 0x00000000 && piid->OriginalFirstThunk == 0x00000000 && piid->TimeDateStamp == 0x00000000 && piid->ForwarderChain == 0x00000000 && piid->Name == 0x00000000 && piid->FirstThunk == 0x00000000)
				break;

			// IMPORT �Լ��� � dll ���̺귯���� �����ִ��� Ȯ���ϱ� ���� ���̺귯�� �̸� ���
			raw = rva_to_raw_dword(fp, buf, piid->Name);
			printf("[%08X] - [***** %s *****]\n\n", raw, *buf + raw);

			// IMAGE IMPORT Descriptor ����ü�� OriginalFirstThunk ����ü�� �ִ� ��(RVA)�� IMAGE_THUNK_DATA�� ��� ������ ���̴�.
			raw = rva_to_raw_dword(fp, buf, piid->OriginalFirstThunk);
			IMAGE_THUNK_DATA32* itd_oft32 = (IMAGE_THUNK_DATA32*)(*buf + raw);

			raw = rva_to_raw_dword(fp, buf, piid->FirstThunk);
			IMAGE_THUNK_DATA32* itd_ft32 = (IMAGE_THUNK_DATA32*)(*buf + raw);

			// IMAGE THUNK DATA ����ü�� �������� 0x00000000 ���̴�.
			// �ش� ���� �ƴ� ���� �ݺ��Ͽ� dll ���̺귯�� �� �Լ��� ���
			for (; itd_oft32->u1.AddressOfData != 0x00000000; itd_oft32++, itd_ft32++)
			{
				raw = rva_to_raw_dword(fp, buf, itd_oft32->u1.AddressOfData);
				IMAGE_IMPORT_BY_NAME* iibn32 = (IMAGE_IMPORT_BY_NAME*)(*buf + raw);

				fseek(fp, raw, SEEK_SET);
				printf("[%08X] - Hint : %X\n", ftell(fp), iibn32->Hint);

				offset = get_file_offset(fp, sizeof(iibn32->Hint));
				printf("[%08X] - Name : %s()\n", offset, iibn32->Name);

				// IAT ���� ���
				fseek(fp, rva_to_raw_dword(fp, buf, piid->FirstThunk), SEEK_SET);
				offset = ftell(fp);
				printf("[%08X] - IAT : %X(RVA), %X(RAW)\n\n", offset, itd_ft32->u1.Function, rva_to_raw_dword(fp, buf, itd_ft32->u1.Function));
			}
			printf("----------------------------------------\n\n");
		}
		printf("-----------------------------------------------------------------\n\n");
	}
	else if (piid_ioh->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		// IMAGE_IMPORT_DESCRIPTOR ����ü�� ũ�⸸ŭ �ݺ��Ѵ�.
		for (int i = 0; i < size; i++, piid++)
		{
			if (piid->Characteristics == 0x00000000 && piid->OriginalFirstThunk == 0x00000000 && piid->TimeDateStamp == 0x00000000 && piid->ForwarderChain == 0x00000000 && piid->Name == 0x00000000 && piid->FirstThunk == 0x00000000)
				break;

			// IMPORT �Լ��� � dll ���̺귯���� �����ִ��� Ȯ���ϱ� ���� ���̺귯�� �̸� ���
			raw = rva_to_raw_dword(fp, buf, piid->Name);
			printf("[%08X] - [***** %s *****]\n\n", raw, *buf + raw);

			// IMAGE IMPORT Descriptor ����ü�� OriginalFirstThunk ����ü�� �ִ� ��(RVA)�� IMAGE_THUNK_DATA�� ��� ������ ���̴�.
			raw = rva_to_raw_dword(fp, buf, piid->OriginalFirstThunk);
			IMAGE_THUNK_DATA64* itd_oft64 = (IMAGE_THUNK_DATA64*)(*buf + raw);

			raw = rva_to_raw_dword(fp, buf, piid->FirstThunk);
			IMAGE_THUNK_DATA64* itd_ft64 = (IMAGE_THUNK_DATA64*)(*buf + raw);

			// IMAGE THUNK DATA ����ü�� �������� 0x00000000 ���̴�.
			// �ش� ���� �ƴ� ���� �ݺ��Ͽ� dll ���̺귯�� �� �Լ��� ���
			for (; itd_oft64->u1.AddressOfData != 0x00000000; itd_oft64++, itd_ft64++)
			{
				ull_raw = rva_to_raw_ulonglong(fp, buf, itd_oft64->u1.AddressOfData);
				IMAGE_IMPORT_BY_NAME* iibn64 = (IMAGE_IMPORT_BY_NAME*)(*buf + ull_raw);

				fseek(fp, (long)ull_raw, SEEK_SET);
				printf("[%08X] - Hint : %X\n", ftell(fp), iibn64->Hint);

				offset = get_file_offset(fp, sizeof(iibn64->Hint));
				printf("[%08X] - Name : %s()\n", offset, iibn64->Name);

				// IAT ���� ���
				fseek(fp, rva_to_raw_dword(fp, buf, piid->FirstThunk), SEEK_SET);
				offset = ftell(fp);
				printf("[%08X] - IAT : %llX(RVA), %llX(RAW)\n\n", offset, itd_ft64->u1.Function, rva_to_raw_ulonglong(fp, buf, itd_ft64->u1.Function));
			}
			printf("----------------------------------------\n\n");
		}
		printf("-----------------------------------------------------------------\n\n");
	}

	
}