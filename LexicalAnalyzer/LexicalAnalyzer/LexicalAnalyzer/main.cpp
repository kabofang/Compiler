#include <stdio.h>
#include "Lex.h"
int main() {
	if (!LexAnalysis("src.pas", "src.dyd", "src.err")) {
		fprintf(stderr, "Error:lex analysis fail!\n");
		return 0;
	}
	return 0;
}