 /*----------------------------------------------
 * Author: Kabir Alvaro Hinduja-Obregon 
 * Date: 25/11/24
 * Description: Simple Binary Search Tree using 
 * recursion. Does not add duplicate entries. 
 ---------------------------------------------*/


#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct tree_node* find(const char* name, struct tree_node* root)
{
	if ( root == NULL || strcmp(name, root->data.name) == 0){
		return root;
	}

	if (strcmp(name, root->data.name) < 0){
		return find(name, root->left);
	}
	else{
		return find(name, root->right);
	}
}

struct tree_node* insert(const char* name, struct tree_node* root)
{
	if (root == NULL){
		struct tree_node* new_node = malloc(sizeof(struct tree_node));
		if (!new_node){
			perror("Failed to create new_node in memory");
			exit(EXIT_FAILURE);
		}
		new_node->left = NULL;
		new_node->right = NULL;
		strcpy(new_node->data.name, name);
		return new_node;
	}
	if (strcmp(name, root->data.name) > 0){
		root->right = insert(name, root->right);
	}
	else if (strcmp(name, root->data.name) < 0){
		root->left = insert(name, root->left);
	}

  return root;
}

void clear(struct tree_node* root)
{
	if (root == NULL){
		return;
	}
	clear(root->left);
	clear(root->right);
	free(root);
}

void print_help_func(struct tree_node* root, int step, char* side){
	if (root == NULL){
		return;
	}
	for (int i = 0; i < step; i++){
		printf("  ");
	}

	if (side[0] != '\0'){
		printf("%s: ", side);
	}
	printf("%s\n", root->data.name);

	print_help_func(root->left, step + 1, "l");
	print_help_func(root->right, step + 1, "r");
}

void print(struct tree_node* root)
{
    print_help_func(root, 0, "");
}



void printSorted(struct tree_node* root)
{
	if (root == NULL){
                return;
        }

        printSorted(root->left);
	printf("%s\n", root->data.name);
        printSorted(root->right);
}

