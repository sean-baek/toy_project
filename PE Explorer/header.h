#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <Windows.h>

int offset = 0;

int get_file_content(FILE* fp, u_char** buf);
int get_file_size(FILE* fp);
int get_file_offset(FILE* fp, int size);
void print_dos_header(FILE* fp, IMAGE_DOS_HEADER* idh);
void print_nt_header32(FILE* fp, IMAGE_NT_HEADERS32* inh32);
void print_nt_header64(FILE* fp, IMAGE_NT_HEADERS64* inh64);
void print_section_header(FILE* fp, IMAGE_SECTION_HEADER* ish, WORD section_num);
void print_inh32_datadirectory(FILE* fp, IMAGE_NT_HEADERS32* inh32);
void print_inh64_datadirectory(FILE* fp, IMAGE_NT_HEADERS64* inh64);
int rva_to_raw(FILE* fp, u_char** binary_buf, DWORD rva_value);
