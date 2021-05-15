#pragma once
#include "func.h"
extern char* pCur;
bool LexAnalysis(const char* src,const char* dyd,const char* err);
bool GenFile(FILE* fp,Token);