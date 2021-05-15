#include <stdio.h>
//#include <stdlib.h>
#include "Lex.h"
#include "func.h"
char* pCur;
bool LexAnalysis(const char* src, const char* dyd, const char* err) {
	FILE* fp;
	if (!(fp = fopen(src, "r"))) {
		fprintf(stderr, "Error:Source file open fail!\n");
		return false;
	}
	fseek(fp, 0L, SEEK_END);
	int FileSize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* pSrc;
	pSrc = (char*)malloc(FileSize+2);
	fread(pSrc, 1, FileSize,fp);
	if (pSrc[FileSize - 1] != '\n') {
		pSrc[FileSize] = '\n';
		pSrc[FileSize+1] = EOF;
	}
	else
		pSrc[FileSize] = EOF;
	pCur = pSrc;
	fclose(fp);
	fp = fopen(dyd, "w");
	while (1) {
		Token Temp = AnalysisOneToken();
		if (Temp.type != EOF) {
			GenFile(fp,Temp);
			fflush(fp);
			continue;
		}
		GenFile(fp, Temp);
		break;
	}
	fclose(fp);
	return true;
}

bool GenFile(FILE* fp, Token token) {
	const char* temp = token.str;
	int len = strlen(token.str);
	char* blank = (char*)malloc(VAR_LEN - len+1);
	memset(blank, ' ', VAR_LEN - len);
	blank[VAR_LEN - len] = 0;
	fprintf(fp, "%s%s %d\n", blank,token.str, token.type);
	return true;
}