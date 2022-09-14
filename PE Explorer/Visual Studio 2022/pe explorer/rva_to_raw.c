#include "header.h"

int rva_to_raw_dword(FILE* fp, u_char** binary_buf, DWORD rva_value)
{
	int raw;
	IMAGE_DOS_HEADER* rtr_idh = (IMAGE_DOS_HEADER*)*binary_buf;
	IMAGE_NT_HEADERS32* rtr_inh32 = (IMAGE_NT_HEADERS32*)(*binary_buf + rtr_idh->e_lfanew);
	IMAGE_SECTION_HEADER* rtr_ish = (IMAGE_SECTION_HEADER*)(*binary_buf + rtr_idh->e_lfanew + sizeof(rtr_inh32->Signature) + sizeof(rtr_inh32->FileHeader) + rtr_inh32->FileHeader.SizeOfOptionalHeader);


	for (int i = 0; i < rtr_inh32->FileHeader.NumberOfSections; i++, rtr_ish++)
	{
		// rva 값이 현재 section의 max치 메모리 값 이상이거나 min치 메모리 값보다 작으면 진행하지 않고 for문으로 되돌아간다.
		if (rva_value >= rtr_ish->VirtualAddress && rva_value < (rtr_ish->VirtualAddress + rtr_ish->Misc.VirtualSize) )
		{

			raw = rva_value - rtr_ish->VirtualAddress + rtr_ish->PointerToRawData;
			return raw;
		}
	}

	return -1;
}

ULONGLONG rva_to_raw_ulonglong(FILE* fp, u_char** binary_buf, ULONGLONG rva_value)
{
	ULONGLONG raw;
	IMAGE_DOS_HEADER* rtr_idh = (IMAGE_DOS_HEADER*)*binary_buf;
	IMAGE_NT_HEADERS64* rtr_inh64 = (IMAGE_NT_HEADERS64*)(*binary_buf + rtr_idh->e_lfanew);
	IMAGE_SECTION_HEADER* rtr_ish = (IMAGE_SECTION_HEADER*)(*binary_buf + rtr_idh->e_lfanew + sizeof(rtr_inh64->Signature) + sizeof(rtr_inh64->FileHeader) + rtr_inh64->FileHeader.SizeOfOptionalHeader);


	for (int i = 0; i < rtr_inh64->FileHeader.NumberOfSections; i++, rtr_ish++)
	{
		// rva 값이 현재 section의 max치 메모리 값 이상이거나 min치 메모리 값보다 작으면 진행하지 않고 for문으로 되돌아간다.
		if (rva_value >= rtr_ish->VirtualAddress && rva_value < ((ULONGLONG)rtr_ish->VirtualAddress + (ULONGLONG)rtr_ish->Misc.VirtualSize))
		{

			raw = rva_value - rtr_ish->VirtualAddress + rtr_ish->PointerToRawData;
			return raw;
		}
	}

	return -1;
}

// 3번째 인자의 자료형이 DWORD이므로 IMAGE_THUNK_DATA64에서는 이 함수를 사용 못한다.
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