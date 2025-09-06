#include <stdlib.h>
#include <stdio.h>

#include "loader.h"

char* loadTextFile(char* dir){
	FILE *txtFile; txtFile = fopen(dir, "r");
	if(!txtFile){
		return NULL;
	}
	char *fullData;
	fullData = malloc(sizeof(char) * 4096);
	fullData[0] = '\0';
	char loadData[512];

	int lineCount = 1;
	for(int c = getc(txtFile); c != EOF; c = getc(txtFile))
		if(c == '\n') lineCount++;
	rewind(txtFile);
	
	for(int lineI=0; lineI < lineCount; lineI++){
		fgets(loadData, 512, txtFile);
		sprintf(fullData, "%s%s", fullData, loadData);
	}
	
	return fullData;
}