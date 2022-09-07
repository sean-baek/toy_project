#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tchar.h>
#include <errno.h>
#include <ctype.h>
#include <Windows.h>

int get_file_content(FILE* fp, u_char** buf);
int get_file_size(FILE* fp);
void print_dos_header(IMAGE_DOS_HEADER* idh);
void print_nt_header32(IMAGE_NT_HEADERS32* inh32);
void print_nt_header64(IMAGE_NT_HEADERS64* inh64);
void print_section_header(IMAGE_SECTION_HEADER* ish, WORD section_num);
void print_inh32_datadirectory(IMAGE_NT_HEADERS32* inh32);
void print_inh64_datadirectory(IMAGE_NT_HEADERS32* inh64);
