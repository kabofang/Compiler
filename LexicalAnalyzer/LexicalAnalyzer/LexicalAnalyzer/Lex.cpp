#include <stdio.h>
//#include <stdlib.h>
#include "Lex.h"
#include "func.h"
char* pCur;
bool LexAnalysis(const char* src, const char* dyd, const char* err) {
	FILE* fp, *fpErr;
	if (!(fp = fopen(src, "r"))) {
		fprintf(stderr, "Error:Source file open fail!\n");
		return false;
	}
	fseek(fp, 0L, SEEK_END);
	int FileSize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* pSrc;
	pSrc = (char*)malloc(FileSize+1);
	fread(pSrc, 1, FileSize,fp);
	int LineNum = 0;
	for (int i = 0; i < FileSize; i++) {
		if (pSrc[i] == '\n')
			++LineNum;
	}
	pSrc[FileSize- LineNum] = EOF;

	pCur = pSrc;
	fclose(fp);
	fp = fopen(dyd, "w");
	fpErr = fopen(err, "w");
	while (1) {
		Token Temp = AnalysisOneToken(fpErr);
		if (Temp.type != EOF) {
			GenFile(fp,Temp);
			fflush(fp);
			continue;
		}
		GenFile(fp, Temp);
		break;
	}
	fclose(fp);
	fclose(fpErr);
	/*map<string,int>::iterator it;
	for (it = ReverseMap.begin(); it != ReverseMap.end(); it++)
		delete (string*)(&(it->first));*/
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