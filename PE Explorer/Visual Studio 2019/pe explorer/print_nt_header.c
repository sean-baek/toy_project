#include "header.h"

void print_nt_header32(FILE* fp, IMAGE_DOS_HEADER* idh, IMAGE_NT_HEADERS32* inh32)
{
	// 파일 포인터를 NT header 위치로 이동
	offset = set_file_offset(fp, idh->e_lfanew);

	printf("=============== [NT Header 32] ===============\n\n");
	printf("[%08X] - Signature[%dbyte]\t\t\t: %08X\n\n", offset, sizeof(inh32->Signature), inh32->Signature);
	offset = get_file_offset(fp, sizeof(inh32->Signature));

	printf("-------------------- (FileHeader) --------------------\n");
	printf("[%08X] - Machine[%dbyte]\t\t\t: %04X\n", offset, sizeof(inh32->FileHeader.Machine), inh32->FileHeader.Machine);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.Machine));

	printf("[%08X] - NumberOfSections[%dbyte]\t\t: %04X\n", offset, sizeof(inh32->FileHeader.NumberOfSections), inh32->FileHeader.NumberOfSections);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.NumberOfSections));

	printf("[%08X] - TimeDateStamp[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->FileHeader.TimeDateStamp), inh32->FileHeader.TimeDateStamp);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.TimeDateStamp));

	printf("[%08X] - PointerToSymbolTable[%dbyte]\t: %08X\n", offset, sizeof(inh32->FileHeader.PointerToSymbolTable), inh32->FileHeader.PointerToSymbolTable);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.PointerToSymbolTable));

	printf("[%08X] - NumberOfSymbols[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->FileHeader.NumberOfSymbols), inh32->FileHeader.NumberOfSymbols);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.NumberOfSymbols));

	printf("[%08X] - SizeOfOptionalHeader[%dbyte]\t: %04X\n", offset, sizeof(inh32->FileHeader.SizeOfOptionalHeader), inh32->FileHeader.SizeOfOptionalHeader);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.SizeOfOptionalHeader));

	printf("[%08X] - Characteristics[%dbyte]\t\t: %04X\n", offset, sizeof(inh32->FileHeader.Characteristics), inh32->FileHeader.Characteristics);
	offset = get_file_offset(fp, sizeof(inh32->FileHeader.Characteristics));
	printf("------------------------------------------------------\n\n");

	printf("--------------- (OptionalHeader 32) ---------------\n");
	printf("[%08X] - Magic[%dbyte]\t\t\t: %04X\n", offset, sizeof(inh32->OptionalHeader.Magic), inh32->OptionalHeader.Magic);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.Magic));

	printf("[%08X] - MajorLinkerVersion[%dbyte]\t\t: %02X\n", offset, sizeof(inh32->OptionalHeader.MajorLinkerVersion), inh32->OptionalHeader.MajorLinkerVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MajorLinkerVersion));

	printf("[%08X] - MinorLinkerVersion[%dbyte]\t\t: %02X\n", offset, sizeof(inh32->OptionalHeader.MinorLinkerVersion), inh32->OptionalHeader.MinorLinkerVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MinorLinkerVersion));

	printf("[%08X] - SizeofCode[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfCode), inh32->OptionalHeader.SizeOfCode);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfCode));

	printf("[%08X] - SizeofInitializedData[%dbyte]\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfInitializedData), inh32->OptionalHeader.SizeOfInitializedData);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfInitializedData));

	printf("[%08X] - SizeOfUninitializedData[%dbyte]\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfUninitializedData), inh32->OptionalHeader.SizeOfUninitializedData);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfUninitializedData));

	printf("[%08X] - AddressOfEntryPoint[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.AddressOfEntryPoint), inh32->OptionalHeader.AddressOfEntryPoint);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.AddressOfEntryPoint));

	printf("[%08X] - BaseOfCode[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.BaseOfCode), inh32->OptionalHeader.BaseOfCode);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.BaseOfCode));

	printf("[%08X] - BaseOfData[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.BaseOfData), inh32->OptionalHeader.BaseOfData);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.BaseOfData));

	printf("[%08X] - ImageBase[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.ImageBase), inh32->OptionalHeader.ImageBase);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.ImageBase));

	printf("[%08X] - SectionAlignment[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SectionAlignment), inh32->OptionalHeader.SectionAlignment);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SectionAlignment));

	printf("[%08X] - FileAlignment[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.FileAlignment), inh32->OptionalHeader.FileAlignment);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.FileAlignment));

	printf("[%08X] - MajorOperatingSystemVersion[%dbyte]\t: %04X\n", offset, sizeof(inh32->OptionalHeader.MajorOperatingSystemVersion), inh32->OptionalHeader.MajorOperatingSystemVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MajorOperatingSystemVersion));

	printf("[%08X] - MinorOperatingSystemVersion[%dbyte]\t: %04X\n", offset, sizeof(inh32->OptionalHeader.MinorOperatingSystemVersion), inh32->OptionalHeader.MinorOperatingSystemVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MinorOperatingSystemVersion));

	printf("[%08X] - MajorImageVersion[%dbyte]\t\t: %04X\n", offset, sizeof(inh32->OptionalHeader.MajorImageVersion), inh32->OptionalHeader.MajorImageVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MajorImageVersion));

	printf("[%08X] - MinorImageVersion[%dbyte]\t\t: %04X\n", offset, sizeof(inh32->OptionalHeader.MinorImageVersion), inh32->OptionalHeader.MinorImageVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MinorImageVersion));

	printf("[%08X] - MajorSubsystemVersion[%dbyte]\t: %04X\n", offset, sizeof(inh32->OptionalHeader.MajorSubsystemVersion), inh32->OptionalHeader.MajorSubsystemVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MajorOperatingSystemVersion));

	printf("[%08X] - MinorSubsystemVersion[%dbyte]\t: %04X\n", offset, sizeof(inh32->OptionalHeader.MinorSubsystemVersion), inh32->OptionalHeader.MinorSubsystemVersion);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.MinorSubsystemVersion));

	printf("[%08X] - Win32VersionValue[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.Win32VersionValue), inh32->OptionalHeader.Win32VersionValue);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.Win32VersionValue));

	printf("[%08X] - SizeOfImage[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfImage), inh32->OptionalHeader.SizeOfImage);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfImage));

	printf("[%08X] - SizeOfHeaders[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfHeaders), inh32->OptionalHeader.SizeOfHeaders);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfHeaders));

	printf("[%08X] - CheckSum[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.CheckSum), inh32->OptionalHeader.CheckSum);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.CheckSum));

	printf("[%08X] - Subsystem[%dbyte]\t\t\t: %04X\n", offset, sizeof(inh32->OptionalHeader.Subsystem), inh32->OptionalHeader.Subsystem);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.Subsystem));

	printf("[%08X] - DllCharacteristics[%dbyte]\t\t: %04X\n", offset, sizeof(inh32->OptionalHeader.DllCharacteristics), inh32->OptionalHeader.DllCharacteristics);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.DllCharacteristics));

	printf("[%08X] - SizeOfStackReserve[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfStackReserve), inh32->OptionalHeader.SizeOfStackReserve);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfStackReserve));
	
	printf("[%08X] - SizeOfStackCommit[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfStackCommit), inh32->OptionalHeader.SizeOfStackCommit);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfStackCommit));

	printf("[%08X] - SizeOfHeapReserve[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfHeapReserve), inh32->OptionalHeader.SizeOfHeapReserve);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfHeapReserve));

	printf("[%08X] - SizeOfHeapCommit[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.SizeOfHeapCommit), inh32->OptionalHeader.SizeOfHeapCommit);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.SizeOfHeapCommit));

	printf("[%08X] - LoaderFlags[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.LoaderFlags), inh32->OptionalHeader.LoaderFlags);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.LoaderFlags));

	printf("[%08X] - NumberOfRvaAndSizes[%dbyte]\t\t: %08X\n", offset, sizeof(inh32->OptionalHeader.NumberOfRvaAndSizes), inh32->OptionalHeader.NumberOfRvaAndSizes);
	offset = get_file_offset(fp, sizeof(inh32->OptionalHeader.NumberOfRvaAndSizes));
	
	/*for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++)
	{
		printf("- DataDirectory[%d].VirtualAddress[%dbyte]\t:%08X\n", i, sizeof(inh32->OptionalHeader.DataDirectory[i].VirtualAddress), inh32->OptionalHeader.DataDirectory[i].VirtualAddress);
		printf("- DataDirectory[%d].Size[%dbyte]\t\t\t:%08X\n", i, sizeof(inh32->OptionalHeader.DataDirectory[i].Size), inh32->OptionalHeader.DataDirectory[i].Size);
	}
	*/

	print_inh32_datadirectory(fp, inh32);


	printf("--------------------------------------------------\n\n");
	printf("==================================================\n\n");
}


void print_nt_header64(FILE* fp, IMAGE_DOS_HEADER* idh, IMAGE_NT_HEADERS64* inh64)
{
	// 파일 포인터를 NT header 위치로 이동
	offset = set_file_offset(fp, idh->e_lfanew);

	printf("=============== [NT Header 64] ===============\n\n");
	printf("[%08X] - Signature[%dbyte]\t\t\t: %08X\n\n", offset, sizeof(inh64->Signature), inh64->Signature);
	offset = get_file_offset(fp, sizeof(inh64->Signature));

	printf("-------------------- (FileHeader) --------------------\n");
	printf("[%08X] - Machine[%dbyte]\t\t\t: %04X\n", offset, sizeof(inh64->FileHeader.Machine), inh64->FileHeader.Machine);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.Machine));

	printf("[%08X] - NumberOfSections[%dbyte]\t\t: %04X\n", offset, sizeof(inh64->FileHeader.NumberOfSections), inh64->FileHeader.NumberOfSections);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.NumberOfSections));

	printf("[%08X] - TimeDateStamp[%dbyte]\t\t: %08X\n", offset, sizeof(inh64->FileHeader.TimeDateStamp), inh64->FileHeader.TimeDateStamp);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.TimeDateStamp));

	printf("[%08X] - PointerToSymbolTable[%dbyte]\t: %08X\n", offset, sizeof(inh64->FileHeader.PointerToSymbolTable), inh64->FileHeader.PointerToSymbolTable);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.PointerToSymbolTable));

	printf("[%08X] - NumberOfSymbols[%dbyte]\t\t: %08X\n", offset, sizeof(inh64->FileHeader.NumberOfSymbols), inh64->FileHeader.NumberOfSymbols);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.NumberOfSymbols));

	printf("[%08X] - SizeOfOptionalHeader[%dbyte]\t: %04X\n", offset, sizeof(inh64->FileHeader.SizeOfOptionalHeader), inh64->FileHeader.SizeOfOptionalHeader);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.SizeOfOptionalHeader));

	printf("[%08X] - Characteristics[%dbyte]\t\t: %04X\n", offset, sizeof(inh64->FileHeader.Characteristics), inh64->FileHeader.Characteristics);
	offset = get_file_offset(fp, sizeof(inh64->FileHeader.Characteristics));
	printf("--------------------------------------------------------\n\n");

	printf("--------------- (OptionalHeader 64) ---------------\n");
	printf("[%08X] - Magic[%dbyte]\t\t\t: %04X\n", offset, sizeof(inh64->OptionalHeader.Magic), inh64->OptionalHeader.Magic);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.Magic));

	printf("[%08X] - MajorLinkerVersion[%dbyte]\t\t: %02X\n", offset, sizeof(inh64->OptionalHeader.MajorLinkerVersion), inh64->OptionalHeader.MajorLinkerVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MajorLinkerVersion));

	printf("[%08X] - MinorLinkerVersion[%dbyte]\t\t: %02X\n", offset, sizeof(inh64->OptionalHeader.MinorLinkerVersion), inh64->OptionalHeader.MinorLinkerVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MinorLinkerVersion));

	printf("[%08X] - SizeofCode[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfCode), inh64->OptionalHeader.SizeOfCode);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfCode));

	printf("[%08X] - SizeofInitializedData[%dbyte]\t: %08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfInitializedData), inh64->OptionalHeader.SizeOfInitializedData);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfInitializedData));

	printf("[%08X] - SizeOfUninitializedData[%dbyte]\t: %08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfUninitializedData), inh64->OptionalHeader.SizeOfUninitializedData);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfUninitializedData));

	printf("[%08X] - AddressOfEntryPoint[%dbyte]\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.AddressOfEntryPoint), inh64->OptionalHeader.AddressOfEntryPoint);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.AddressOfEntryPoint));

	printf("[%08X] - BaseOfCode[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.BaseOfCode), inh64->OptionalHeader.BaseOfCode);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.BaseOfCode));


	printf("[%08X] - ImageBase[%dbyte]\t\t\t: %016llX\n", offset, sizeof(inh64->OptionalHeader.ImageBase), inh64->OptionalHeader.ImageBase);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.ImageBase));

	printf("[%08X] - SectionAlignment[%dbyte]\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.SectionAlignment), inh64->OptionalHeader.SectionAlignment);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SectionAlignment));

	printf("[%08X] - FileAlignment[%dbyte]\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.FileAlignment), inh64->OptionalHeader.FileAlignment);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.FileAlignment));

	printf("[%08X] - MajorOperatingSystemVersion[%dbyte]\t: %04X\n", offset, sizeof(inh64->OptionalHeader.MajorOperatingSystemVersion), inh64->OptionalHeader.MajorOperatingSystemVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MajorOperatingSystemVersion));

	printf("[%08X] - MinorOperatingSystemVersion[%dbyte]\t: %04X\n", offset, sizeof(inh64->OptionalHeader.MinorOperatingSystemVersion), inh64->OptionalHeader.MinorOperatingSystemVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MinorOperatingSystemVersion));

	printf("[%08X] - MajorImageVersion[%dbyte]\t\t: %04X\n", offset, sizeof(inh64->OptionalHeader.MajorImageVersion), inh64->OptionalHeader.MajorImageVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MajorImageVersion));

	printf("[%08X] - MinorImageVersion[%dbyte]\t\t: %04X\n", offset, sizeof(inh64->OptionalHeader.MinorImageVersion), inh64->OptionalHeader.MinorImageVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MinorImageVersion));

	printf("[%08X] - MajorSubsystemVersion[%dbyte]\t: %04X\n", offset, sizeof(inh64->OptionalHeader.MajorSubsystemVersion), inh64->OptionalHeader.MajorSubsystemVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MajorSubsystemVersion));

	printf("[%08X] - MinorSubsystemVersion[%dbyte]\t: %04X\n", offset, sizeof(inh64->OptionalHeader.MinorSubsystemVersion), inh64->OptionalHeader.MinorSubsystemVersion);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.MinorSubsystemVersion));

	printf("[%08X] - Win32VersionValue[%dbyte]\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.Win32VersionValue), inh64->OptionalHeader.Win32VersionValue);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.Win32VersionValue));

	printf("[%08X] - SizeOfImage[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfImage), inh64->OptionalHeader.SizeOfImage);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfImage));

	printf("[%08X] - SizeOfHeaders[%dbyte]\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.SizeOfHeaders), inh64->OptionalHeader.SizeOfHeaders);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfHeaders));

	printf("[%08X] - CheckSum[%d]\t\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.CheckSum), inh64->OptionalHeader.CheckSum);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.CheckSum));

	printf("[%08X] - Subsystem[%dbyte]\t\t\t: %04X\n", offset, sizeof(inh64->OptionalHeader.Subsystem), inh64->OptionalHeader.Subsystem);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.Subsystem));

	printf("[%08X] - DllCharacteristics[%dbyte]\t\t: %04X\n", offset, sizeof(inh64->OptionalHeader.DllCharacteristics), inh64->OptionalHeader.DllCharacteristics);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.DllCharacteristics));

	printf("[%08X] - SizeOfStackReserve[%dbyte]\t\t: %016llX\n", offset, sizeof(inh64->OptionalHeader.SizeOfStackReserve), inh64->OptionalHeader.SizeOfStackReserve);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfStackReserve));

	printf("[%08X] - SizeOfStackCommit[%dbyte]\t\t: %016llX\n", offset, sizeof(inh64->OptionalHeader.SizeOfStackCommit), inh64->OptionalHeader.SizeOfStackCommit);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfStackCommit));

	printf("[%08X] - SizeOfHeapReserve[%dbyte]\t\t: %016llX\n", offset, sizeof(inh64->OptionalHeader.SizeOfHeapReserve), inh64->OptionalHeader.SizeOfHeapReserve);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfHeapReserve));

	printf("[%08X] - SizeOfHeapCommit[%dbyte]\t\t: %016llX\n", offset, sizeof(inh64->OptionalHeader.SizeOfHeapCommit), inh64->OptionalHeader.SizeOfHeapCommit);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.SizeOfHeapCommit));

	printf("[%08X] - LoaderFlags[%dbyte]\t\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.LoaderFlags), inh64->OptionalHeader.LoaderFlags);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.LoaderFlags));

	printf("[%08X] - NumberOfRvaAndSizes[%dbyte]\t\t: %08X\n", offset, sizeof(inh64->OptionalHeader.NumberOfRvaAndSizes), inh64->OptionalHeader.NumberOfRvaAndSizes);
	offset = get_file_offset(fp, sizeof(inh64->OptionalHeader.NumberOfRvaAndSizes));

	/*
	for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++)
	{
		printf("- DataDirectory[%d].VirtualAddress[%dbyte]\t:%08X\n", i, sizeof(inh64->OptionalHeader.DataDirectory[i].VirtualAddress), inh64->OptionalHeader.DataDirectory[i].VirtualAddress);
		printf("- DataDirectory[%d].Size[%dbyte]\t\t\t:%08X\n", i, sizeof(inh64->OptionalHeader.DataDirectory[i].Size), inh64->OptionalHeader.DataDirectory[i].Size);
	}
	*/

	print_inh64_datadirectory(fp, inh64);
	printf("----------------------------------------------\n\n");
	printf("==============================================\n\n");
}
