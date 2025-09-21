#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct node {
	char data;
	struct node *next;
} node;

int insert_node(node** head, int index, char letter);
int remove_node(node** head, int index);
void free_list(node* head);
void print_list(node* head);

int main() {
	node *head = NULL;
	
	FILE* fp = fopen("data.bin", "rb");
	if (fp == NULL) {
    		printf("Error opening file.\n");
    		exit(EXIT_FAILURE);
	}
	int size=0;

	uint8_t opcode;
	uint16_t index;
	uint8_t c;

	while (1) {
		// fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), fp);
		if (!fread(&opcode, sizeof(opcode), 1, fp)) break;
		if (opcode == 0) {
			if (!fread(&c, sizeof(c), 1, fp)) break;
			// append c
			size += insert_node(&head, size, c); 
		} else if (opcode == 1) {
			if (!fread(&c, sizeof(c), 1, fp)) break;
			// prepend c
			size += insert_node(&head, 0, c);
		} else if (opcode == 2) {
			if (!fread(&index, sizeof(index), 1, fp)) break;
			if (!fread(&c, sizeof(c), 1, fp)) break;
			// insert c at index
			if (index <= size) {
    				size += insert_node(&head, index, c);
			}
		} else if (opcode == 3) {
			// pop tail
			size -= remove_node(&head, size-1);
		} else if (opcode == 4) {
			// pop head
			size -= remove_node(&head, 0);
		} else if (opcode == 5) {
			// pop index
			if (!fread(&index, sizeof(index), 1, fp)) break;
			if (index < size) {
				size -= remove_node(&head, index);
			}
		} else {
			exit(EXIT_FAILURE);
		}
		
	}
	fclose(fp);
	print_list(head);
	free_list(head);
}

int insert_node(node** head, int index, char letter) {
	node* new = malloc(sizeof(node));
    	new->data = letter;
    	new->next = NULL;

    	if (index==0) {
		new->next = *head;
		*head = new;
		return 1;
	}
	node *curr = *head;
	for (int i=0; i < index - 1; i++) {
		curr = curr->next;
		if (curr == NULL) {
			free(new);
			return 0;
		}
	}

	new->next = curr->next;
	curr->next = new;
	return 1;
}

int remove_node(node** head, int index) {
	if (*head == NULL) return 0; // list is empty
	node *curr = *head;
	node *prev = NULL;
	if (index == 0){
		*head = curr->next;
		free(curr);
		return 1;
	}
	for (int i=0; i < index; i++) {
		prev = curr;
		curr = curr->next;
		if (curr == NULL) return 0; // out of bounds
	}
	
	prev->next = curr->next;
	free(curr);

	return 1;
}


void free_list(node* head) {
    while (head != NULL) {
        node* tmp = head;
        head = head->next;
        free(tmp);
    }
}

void print_list(node* head) {
	printf("flag{");
	while (head != NULL) {
       		printf("%c", head->data);
        	head = head->next;
    	}
	printf("}\n");
}
