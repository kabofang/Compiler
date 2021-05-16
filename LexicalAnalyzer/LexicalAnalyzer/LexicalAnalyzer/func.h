#pragma once
#include "funcdefine.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <map>
#include <string>
using namespace std;
typedef struct Token {
	int type;
	char str[16];
};
extern char* pCur;
extern map<string, int> ReverseMap;
extern map<int, string> ReverseIdxMap;
Token AnalysisOneToken(FILE*fp);
void JmpBlank();
char GetNextChar();
void AddChar(char*, char);
int IsReserve(char* str);
void Error(int LineNum,int Errno, void* pArg,FILE* fpErr);
int IsReserveChar(char);
void ProcToNext();
void BuildMap();
void ToNextLine();
Token BuildToken(int Num, const char* str);