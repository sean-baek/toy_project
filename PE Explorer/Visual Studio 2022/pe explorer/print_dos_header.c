#include "header.h"

void print_dos_header(FILE* fp, IMAGE_DOS_HEADER* idh)
{

	printf("=============== [Dos Header] ===============\n\n");
	// 파일의 처음으로 이동하여 offset 가져오기
	offset = set_file_offset(fp, 0);
	printf("[%08X] - e_magic[%dbyte]\t: %04X\n", offset, sizeof(idh->e_magic), idh->e_magic);
	
	offset = get_file_offset(fp, sizeof(idh->e_magic));
	printf("[%08X] - e_cblp[%dbyte]\t: %04X\n", offset, sizeof(idh->e_cblp), toupper(idh->e_cblp));
	
	offset = get_file_offset(fp, sizeof(idh->e_cblp));
	printf("[%08X] - e_cp[%dbyte]\t: %04X\n", offset, sizeof(idh->e_cp), idh->e_cp);
	
	offset = get_file_offset(fp, sizeof(idh->e_cp));
	printf("[%08X] - e_crlc[%dbyte]\t: %04X\n", offset, sizeof(idh->e_crlc), idh->e_crlc);
	
	offset = get_file_offset(fp, sizeof(idh->e_crlc));
	printf("[%08X] - e_cparhdr[%dbyte]\t: %04X\n", offset, sizeof(idh->e_cparhdr), idh->e_cparhdr);
	
	offset = get_file_offset(fp, sizeof(idh->e_cparhdr));
	printf("[%08X] - e_minalloc[%dbyte]\t: %04X\n", offset, sizeof(idh->e_minalloc), idh->e_minalloc);
	
	offset = get_file_offset(fp, sizeof(idh->e_minalloc));
	printf("[%08X] - e_maxalloc[%dbyte]\t: %04X\n", offset, sizeof(idh->e_maxalloc), idh->e_maxalloc);
	
	offset = get_file_offset(fp, sizeof(idh->e_maxalloc));
	printf("[%08X] - e_ss[%dbyte]\t: %04X\n", offset, sizeof(idh->e_ss), idh->e_ss);
	
	offset = get_file_offset(fp, sizeof(idh->e_ss));
	printf("[%08X] - e_sp[%dbyte]\t: %04X\n", offset, sizeof(idh->e_sp), idh->e_sp);
	
	offset = get_file_offset(fp, sizeof(idh->e_sp));
	printf("[%08X] - e_csum[%dbyte]\t: %04X\n", offset, sizeof(idh->e_csum), idh->e_csum);
	
	offset = get_file_offset(fp, sizeof(idh->e_csum));
	printf("[%08X] - e_ip[%dbyte]\t: %04X\n", offset, sizeof(idh->e_ip), idh->e_ip);
	
	offset = get_file_offset(fp, sizeof(idh->e_ip));
	printf("[%08X] - e_cs[%dbyte]\t: %04X\n", offset, sizeof(idh->e_cs), idh->e_cs);
	
	offset = get_file_offset(fp, sizeof(idh->e_cs));
	printf("[%08X] - e_lfarlc[%dbyte]\t: %04X\n", offset, sizeof(idh->e_lfarlc), idh->e_lfarlc);
	
	offset = get_file_offset(fp, sizeof(idh->e_lfarlc));
	printf("[%08X] - e_ovno[%dbyte]\t: %04X\n", offset, sizeof(idh->e_ovno), idh->e_ovno);
	
	offset = get_file_offset(fp, sizeof(idh->e_ovno));
	for (int i = 0; i < 4; i++)
	{
		printf("[%08X] - e_res[%d][%dbyte]\t: %04X\n", offset, i, sizeof(idh->e_res[i]), idh->e_res[i]);
		offset = get_file_offset(fp, sizeof(idh->e_res[i]));
	}
	printf("[%08X] - e_oemid[%dbyte]\t: %04X\n", offset, sizeof(idh->e_oemid), idh->e_oemid);
	
	offset = get_file_offset(fp, sizeof(idh->e_oemid));
	printf("[%08X] - e_oeminfo[%dbyte]\t: %04X\n", offset, sizeof(idh->e_oeminfo), idh->e_oeminfo);
	
	offset = get_file_offset(fp, sizeof(idh->e_oeminfo));
	for (int i = 0; i < 10; i++)
	{
		printf("[%08X] - e_res2[%d][%dbyte]\t: %04X\n", offset, i, sizeof(idh->e_res2[i]), idh->e_res2[i]);
		offset = get_file_offset(fp, sizeof(idh->e_res2[i]));
	}
	printf("[%08X] - e_lfanew[%dbyte]\t: %08X\n", offset, sizeof(idh->e_lfanew), idh->e_lfanew);

	offset = get_file_offset(fp, sizeof(idh->e_lfanew));

	printf("\n============================================\n\n");
}