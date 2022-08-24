/*
 * GPS.c
 *
 *  Created on: Aug 15, 2022
 *      Author: Patryk
 */
#include <stdlib.h>
typedef struct{
	float lat;
	float lon;
}Coords;
char** splitCSV(char * data)
{
	int i=0;
	int j=0;
	int k = 0;
	char **array = (char**)malloc(sizeof(char*)*13);
	while(data[i]!='\0' || k<13){

		char * column = (char*)malloc(15);
		for(j = 0;data[i]!=','&&data[i]!='\0';i++,j++)
		{
			column[j]=data[i];
		}
		i++;
		column[j]='\0';
		array[k]=column;
		k++;
	}


	return array;
}
Coords getCoords(char**array){
	Coords coord = {};
	if(strcmp(array[0], "$GPRMC") == 0){
		coord.lat = atof(array[3]);
		if(strcmp(array[4], "S") == 0){
			coord.lat *= -1;
		}
			coord.lon = atof(array[5]);
		}
	return coord;
}

int isDataValid(char**array){
	if(strcmp(array[0], "$GPRMC") == 0){
		if(strcmp(array[3],"A") == 0){
			return 1;
		}else{
			return 0;
		}
	}
	return -1;
}



