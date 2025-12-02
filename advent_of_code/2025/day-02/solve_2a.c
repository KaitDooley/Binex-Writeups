#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

long long check_repeat(long long num);
long long check_range(long long num1, long long num2);


int main() {
        FILE* fp = fopen("input_2.txt", "r");
        if (fp == NULL) {
                printf("Error opening file.\n");
                exit(EXIT_FAILURE);
        }
		long long total = 0;
        char line[BUFSIZ];
        fgets(line, sizeof(line), fp);
			
		char *token = strtok(line, ",");
		while (token) {
    		long long a, b;
    		sscanf(token, "%lld-%lld", &a, &b);
    		total += check_range(a, b);
    		token = strtok(NULL, ",");
		}
        printf("%lld\n", total);
		fclose(fp);
        return EXIT_SUCCESS;
}

long long check_range(long long num1, long long num2) {
	long long total = 0;

	for (long long i = num1; i <= num2; i++) {
		total += check_repeat(i);
	}

	return total;
}

long long check_repeat(long long num) {
	char str[16];
	sprintf(str, "%lld", num);
	int len = strlen(str);
	if (len%2 != 0) return 0;
	for (int i = 0; i < len/2; i++) {
		if (str[i] != str[i+(len/2)]) return 0;
	}
	return num;
}
