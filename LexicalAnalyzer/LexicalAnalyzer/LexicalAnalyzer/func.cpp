#include "func.h"
using namespace std;
map<string,int> ReverseMap;
map<int, string> ReverseIdxMap;
Token AnalysisOneToken() {
	static int IsBuild;
	if (!IsBuild) {
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
				Error(ERR_TOLONG,NULL);
				ProcToNext();
				return Token{ ERROR };
			}
			if (IsReserveChar(CurChar))
				break;
			AddChar(token, CurChar);
			if (!isalnum(CurChar)) {
				IsError = 1;
				Error(ERR_INVVAR,(void*)CurChar);
			}
		}
		--pCur;

		if (IsError)
			return Token{ ERROR };

		if (Num = IsReserve(token)) 
			ret = { Num };
		else 
			ret = { VAR };
		memset(ret.str, 0, VAR_LEN);
		memmove(ret.str, token, strlen(token));
		return ret;
	CASE_NUM
		AddChar(token, CurChar);
		Len = 1;
		while (1) {
		CurChar = GetNextChar();
		if (IsReserveChar(CurChar))
			break;
		AddChar(token, CurChar);
		if (!isdigit(CurChar)) {
			IsError = 1;
			Error(ERR_INVVAR, (void*)CurChar);
			ProcToNext();
			return Token{ ERROR };
		}
	}
	--pCur;

	if (IsError)
		return Token{ ERROR };
	ret = { CONST };
	memset(ret.str, 0, VAR_LEN);
	memmove(ret.str, token, strlen(token));
	return ret;

	case EOF:
		ret = { EOF };
		memset(ret.str, 0, VAR_LEN);
		memmove(ret.str, "EOF", strlen("EOF"));
		return ret;
	case ';':
		ret = { SEM };
		memset(ret.str, 0, VAR_LEN);
		memmove(ret.str, ";", strlen(";"));
		ToNextLine();
		return ret;
	case ':':
		if ('=' != GetNextChar())
			Error(ERR_INVCHR, (void*)"");
		ret = { ASS };
		memset(ret.str, 0, VAR_LEN);
		memmove(ret.str, ":=", strlen(":="));
		return ret;
	}

}
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

void Error(int x,void* pArg) {

}

int IsReserveChar(char ch) {
	switch (ch) {
	case '>':
	case '<':
	case '=':
	case ':':
	case '(':
	case '-':
	case '*':
	case ';':
	case ' ':
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