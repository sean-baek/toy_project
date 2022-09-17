#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <ctype.h>
#include <Windows.h>

extern int offset;

int get_file_content(FILE* fp, u_char** buf);
int get_file_size(FILE* fp);
int set_file_offset(FILE* fp, int offset);
int get_file_offset(FILE* fp, int offset);

int rva_to_raw(FILE* fp, u_char** binary_buf, DWORD rva_value);
int rva_to_raw_dword(FILE* fp, u_char** binary_buf, DWORD rva_value);
ULONGLONG rva_to_raw_ulonglong(FILE* fp, u_char** binary_buf, ULONGLONG rva_value);

void print_dos_header(FILE* fp, IMAGE_DOS_HEADER* idh);
// idh가 인자로 넘겨지는 이유 : offset 초기화를 위해
void print_nt_header32(FILE* fp, IMAGE_DOS_HEADER* idh, IMAGE_NT_HEADERS32* inh32);
void print_nt_header64(FILE* fp, IMAGE_DOS_HEADER* idh, IMAGE_NT_HEADERS64* inh64);
void print_inh32_datadirectory(FILE* fp, IMAGE_NT_HEADERS32* inh32);
void print_inh64_datadirectory(FILE* fp, IMAGE_NT_HEADERS64* inh64);
void print_section_header(FILE* fp, IMAGE_SECTION_HEADER* ish, WORD section_num);

void print_image_import_descriptor(FILE* fp, u_char** buf, IMAGE_IMPORT_DESCRIPTOR* iid, int size);
void print_image_export_directory(FILE* fp, u_char** buf, IMAGE_EXPORT_DIRECTORY* ied);

