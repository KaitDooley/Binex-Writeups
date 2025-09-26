#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


/* Functions */

bool is_prime(uint8_t tag);


int main() {
        FILE* fp = fopen("data.bin", "rb");
        if (fp == NULL) {
                printf("Error opening file.\n");
                return EXIT_FAILURE;
        }



        uint8_t tag, length;
	char string[10000] = {0};
	int i = 0;
	char *value;

        // read in values
        while (fread(&tag, sizeof(tag), 1, fp) == 1 && fread(&length, sizeof(length), 1, fp) == 1) {
		value = malloc(sizeof(char) * length);
		if (fread(value, length, 1, fp) != 1) {
			free(value);
			break;
		}
		if (is_prime(tag)) {
			for (int j = 0; j < length; j++) {
				string[i+j] = value[j];
			}
			i += length;       
			
		}
		free(value);
	}
	fclose(fp);
	string[i] = '\0';

	printf("flag{");
	// print 1st 8
	for (int j = 0; j < 8; j++) {
		printf("%c", string[j]);
	}
	// print middle 8
	for (int j = 0; j < 8; j++) {
		printf("%c", string[(i/2)-4+j]);
	}
	// print last 8
	for (int j = 0; j < 8; j++) {
		printf("%c", string[i+j-8]);
	}
	printf("}\n");
}


bool is_prime(uint8_t tag) {
	if (tag <= 2) return false;
	for (int i = 2; i < tag; i++) {
		if (tag % i == 0) return false;
	}
	return true;
}
