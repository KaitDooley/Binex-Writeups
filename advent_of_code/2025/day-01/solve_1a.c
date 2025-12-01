#include <stdio.h> 
#include <stdlib.h> 
int main() { 
	FILE* fp = fopen("input_1.txt", "r"); 
	if (fp == NULL) { 
		printf("Error opening file.\n"); 
		exit(EXIT_FAILURE); 
	} 
	int num = 50; // range 0-99 
	int move = 0; // current rotation 
	int count = 0; // number of times dial reads 0 
	char line[16]; 
	while (fgets(line, sizeof(line), fp)) { 
		move = atoi(line+1); 
		if (line[0] == 'L') move *= -1; 
		num += move % 100; 
		if (num > 99) num -= 100; 
		if (num < 0) num += 100; 
		if (num == 0) count++; 
	} 
	
	printf("%d\n", count); 
	return EXIT_SUCCESS; 
}
