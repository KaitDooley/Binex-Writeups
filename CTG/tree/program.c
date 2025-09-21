#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_NODES 64

/* Structure */

typedef struct node {
	uint16_t index;
	uint8_t c;
	uint16_t left;
	uint16_t right;
	bool valid;
} node;

/* Functions */

void dfs_print(uint16_t index, node* nodes);


int main() {
        FILE* fp = fopen("data.bin", "rb");
        if (fp == NULL) {
                printf("Error opening file.\n");
                return EXIT_FAILURE;
        }

	node nodes[MAX_NODES] = {0};
	for (int i = 0; i < MAX_NODES; i++) {
		nodes[i].valid = false;
	}
        uint16_t i, left, right;
	uint8_t letter;
	uint16_t root = 1;
	
	// read in nodes
        while (fread(&i, sizeof(i), 1, fp) == 1 && 
			fread(&letter, sizeof(letter), 1, fp) == 1 &&
			fread(&left, sizeof(left), 1, fp) == 1 &&
			fread(&right, sizeof(right), 1, fp) == 1) {
		nodes[i] = (node){ .index = i, .c = letter, .left = left, .right = right, .valid = true };
		printf("i=%u, left=%u, right=%u\n", i, left, right);
		if (root == 0xFFFF) root = i;
	}
	fclose(fp);
	
	// print nodes in order
	dfs_print(root, nodes);
	printf("\n");

	return EXIT_SUCCESS;
}


void dfs_print(uint16_t index, node* nodes) {
	if (index == 0xFFFF || nodes[index].valid == false) return; // base case
	printf("%c", nodes[index].c);
	dfs_print(nodes[index].left, nodes);
	dfs_print(nodes[index].right, nodes);
}
