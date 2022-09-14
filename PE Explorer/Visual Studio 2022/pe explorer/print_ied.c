#include "header.h"

void print_image_export_directory(FILE* fp, u_char** buf, IMAGE_EXPORT_DIRECTORY* ied)
{
	int raw;
	IMAGE_EXPORT_DIRECTORY* pied = (IMAGE_EXPORT_DIRECTORY*)ied;

	
	printf("%X\n", ied->Name);
	raw = rva_to_raw_dword(fp, buf, ied->Name);
	printf("[%08X] - %s\n", raw, *buf + raw);
	
}