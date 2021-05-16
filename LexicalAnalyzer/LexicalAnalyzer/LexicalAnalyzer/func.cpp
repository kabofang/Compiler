#include "func.h"
using namespace std;
map<string,int> ReverseMap;
map<int, string> ReverseIdxMap;
Token AnalysisOneToken(FILE*fpErr) {
	static int IsBuild;
	static int LineNum;
	if (!IsBuild) {
		LineNum = 1;
		IsBuild = 1;
		BuildMap();
	}
	JmpBlank();
	char CurChar = GetNextChar();

	char token[VAR_LEN+1];
	memset(token, 0,VAR_LEN + 1);
	int Len;
	int IsError = 0;
	Token ret;
	int Num;
	switch (CurChar) {
	CASE_ALPHA
		AddChar(token,CurChar);
		Len = 1;
		while (1) {
			CurChar = GetNextChar();
			if (++Len > VAR_LEN) {
				Error(LineNum,ERR_TOLONG,NULL,fpErr);
				ProcToNext();
				ret = BuildToken(ERROR, "ERROR");
				return ret;
			}
			if (IsReserveChar(CurChar))
				break;
			AddChar(token, CurChar);
			if (!isalnum(CurChar)) {
				IsError = 1;
				Error(LineNum, ERR_INVCHAR,(void*)CurChar,fpErr);
			}
		}
		--pCur;

		if (IsError) {
			ret = BuildToken(ERROR, "ERROR");
			return ret;
		}

		if (Num = IsReserve(token))
			ret = BuildToken(Num, token);
		else 
			ret = BuildToken(VAR, token);
		return ret;
	CASE_NUM
		AddChar(token, CurChar);
		Len = 1;
		while (1) {
		CurChar = GetNextChar();
		if (++Len > VAR_LEN) {
			Error(LineNum, ERR_TOLONG, NULL, fpErr);
			ProcToNext();
			ret = BuildToken(ERROR, "ERROR");
			return ret;
		}
		if (IsReserveChar(CurChar))
			break;
		AddChar(token, CurChar);
		if (!isdigit(CurChar)) {
			IsError = 1;
			Error(LineNum, ERR_INVCHAR, (void*)CurChar,fpErr);
			ProcToNext();
			ret = BuildToken(ERROR, "ERROR");
			return ret;
		}
	}
	--pCur;

	if (IsError) {
		ret = BuildToken(ERROR, "");
		return ret;
	}
	ret = BuildToken(CONST, token);
	return ret;

	case '\n':
		LineNum++;
		ret = BuildToken(EOLN, "EOLN");
		return ret;

	case '(':
	case ')':
	case '*':
	case '-':
	case '=':
	case ';':
		AddChar(token, CurChar);
		ret = BuildToken(ReverseMap[token], "EOLN");
		return ret;

	case EOF:
		ret = BuildToken(EOF, "EOF");
		return ret;

	case '<':
		AddChar(token, CurChar);
		CurChar = GetNextChar();
		if ('=' == CurChar || '>' == CurChar)
			AddChar(token, CurChar);
		else
			--pCur;
		ret = BuildToken(ReverseMap[token], token);
		return ret;

	case '>':
		AddChar(token, CurChar);
		CurChar = GetNextChar();
		if ('=' == CurChar)
			AddChar(token, CurChar);
		else
			--pCur;
		ret = BuildToken(ReverseMap[token], token);
		return ret;

	case ':':
		if ('=' != GetNextChar()) {
			Error(LineNum, ERR_INVCOLON, (void*)"", fpErr);
			ret = BuildToken(ERROR, "ERROR");
			--pCur;
			return ret;
		}
		ret = BuildToken(ASS, ":=");
		return ret;
	}

}//!strcmp(token,"write")
void JmpBlank() {
	while (*pCur++ == ' ')
		continue;
	--pCur;
}
char GetNextChar() {
	return *pCur++;
}

void AddChar(char*Src, char ch) {
	char temp[2] = { ch,'\0' };
	strcat(Src, temp);
	return;
}

int IsReserve(char* str) {
	string temp = str;
	if (ReverseMap.find(temp) == ReverseMap.end())
		return 0;
	return ReverseMap[temp];
}


int IsReserveChar(char ch) {
	switch (ch) {
	case '>':
	case '<':
	case '=':
	case ':':
	case '(':
	case ')':
	case '-':
	case '*':
	case ';':
	case ' ':
	case '\n':
	case EOF:
		return true;
	}
	return false;
}

void ProcToNext() {
	int ch;
	while (1) {
		ch = GetNextChar();
		if (ch == ' ' || IsReserveChar(ch))
			break;
	}
	--pCur;
}

void ToNextLine() {
	while ('\n' != GetNextChar())
		continue;
}

Token BuildToken(int Num,const char* str) {
	Token temp;
	temp.type = Num;
	memset(temp.str, 0, VAR_LEN);
	memmove(temp.str, str, strlen(str));
	return temp;
}


void Error(int LineNum, int Errno, void* pArg, FILE* fpErr) {
	switch (Errno) {
	case ERR_INVCOLON:
		fprintf(fpErr, "Error %d , Line %d : %s\n", Errno, LineNum, "Colon mismatch");
		break;
	case ERR_INVCHAR:
		fprintf(fpErr, "Error %d , Line %d : %s\n", Errno, LineNum, "Illegal character");
		break;
	case ERR_TOLONG:
		fprintf(fpErr, "Error %d , Line %d : %s\n", Errno, LineNum, "Identifier is too long");
		break;
	}
}

void BuildMap() {
	string* p;
	p = new string;
	*p = "begin";
	ReverseMap[*p] = BEGIN;

	p = new string;
	*p = "end";
	ReverseMap[*p] = END;

	p = new string;
	*p = "integer";
	ReverseMap[*p] = INTEGER;

	p = new string;
	*p = "if";
	ReverseMap[*p] = IF;

	p = new string;
	*p = "then";
	ReverseMap[*p] = THEN;

	p = new string;
	*p = "else";
	ReverseMap[*p] = ELSE;

	p = new string;
	*p = "function";
	ReverseMap[*p] = FUNCTION;

	p = new string;
	*p = "read";
	ReverseMap[*p] = READ;

	p = new string;
	*p = "write";
	ReverseMap[*p] = WRITE;

	p = new string;
	//var
	//const
	p = new string;
	*p = "==";
	ReverseMap[*p] = EQ;

	p = new string;
	*p = "!=";
	ReverseMap[*p] = NEQ;

	p = new string;
	*p = "<=";
	ReverseMap[*p] = LET;

	p = new string;
	*p = "<";
	ReverseMap[*p] = LT;

	p = new string;
	*p = ">=";
	ReverseMap[*p] = GET;

	p = new string;
	*p = ">";
	ReverseMap[*p] = GT;

	p = new string;
	*p = "-";
	ReverseMap[*p] = SUB;

	p = new string;
	*p = "*";
	ReverseMap[*p] = MULT;

	p = new string;
	*p = ":=";
	ReverseMap[*p] = ASS;

	p = new string;
	*p = "(";
	ReverseMap[*p] = LB;

	p = new string;
	*p = ")";
	ReverseMap[*p] = RB;

	p = new string;
	*p = ";";
	ReverseMap[*p] = SEM;

	p = new string;
	*p = "\n";
	ReverseMap[*p] = EOLN;

}