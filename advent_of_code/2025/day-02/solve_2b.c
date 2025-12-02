#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

long long check_repeat(long long num);
long long check_range(long long num1, long long num2);
int check_repeat_len(char *str, int len);


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
	for (int i = 1; i <= len/2; i++) {
		if (check_repeat_len(str, i) == 1) return num;
	}
	return 0;
}

int check_repeat_len(char *str, int len) {
	if (strlen(str)%len != 0) return 0;
	char base[len+1];
	char compare[len+1];
	strncpy(base, str, len);
	base[len] = '\0';
	compare[len] = '\0';

	for (int i = len; i < strlen(str); i+=len) {
		strncpy(compare, str+i, len);
		if (strcmp(base, compare) != 0) return 0;
	}

	return 1;
}
