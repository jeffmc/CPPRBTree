#include "types.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Read in a line from the console.
void consolein(char* cstr, std::streamsize n) {
	std::cin.getline(cstr, n);
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore();
	}
	#ifdef CONSOLEINDBG
	printf("\"%s\"\n",cstr);
	#endif
}

void median_insert(BST<int>& bst, const int* arr, const int count);
int compint(const void* a, const void* b) {
	return *(int*)a - *(int*)b;
}

// Load from space-delimited file of numbers.
void loadfile(BST<int>& bst) {
	char filename[32] = { '\0' };
	FILE* file = nullptr;
	while (file == nullptr) {
		printf("Filename: ");
		consolein(filename, 32);
		file = fopen(filename, "r");
		if (file == nullptr) {
			printf("Couldn't open \"%s\"!\n", filename);
		}
	}
	printf("File opened successfully!\n");
	char numbuf[32] = { '\0' };
	size_t bufpos = 0;
	int c;
	int count = 0;
	int capacity = 128;
	int* nums = (int*)malloc(capacity * sizeof(int));
	while (true) {
		c = fgetc(file); // Grab next character from file
		if (isdigit(c) || c == '+' || c == '-') { // Allow these characters as defined by atoi(const char*) spec.
			assert(bufpos < 32);
			numbuf[bufpos] = c; // Add character and move null-terminator
			numbuf[++bufpos] = '\0';
		}
		else {
			if (strlen(numbuf) > 0) {
				int num = atoi(numbuf);
				if (count >= capacity) {
					capacity *= 2;
					nums = (int*)realloc(nums,capacity*sizeof(int));
				}
				nums[count] = num;
				++count;
				numbuf[0] = '\0';
				bufpos = 0;
			}
		}
		if (c == EOF) break;
	}
	assert(strlen(numbuf) == 0);
	
	qsort(nums,count,sizeof(int),compint);
	median_insert(bst,nums,count);
	printf("Read %i numbers!\n", count);
	free(nums);
}

// Manual number entry via console.
void cmdinput(BST<int>& bst) {
	const int increment = 100;
	int capacity = increment;
	int count = 0;
	int* nums = (int*)malloc(sizeof(int)*capacity);
	char buf[32] = { '\0' };
	while (true) {
		printf("Enter a number (or \"STOP\"): ");
		consolein(buf, 32);
		if (strlen(buf)==0) {
			printf("Enter STOP if you wish to stop inputting numbers!\n");
			continue;
		}
		if (strcmp(buf,"STOP") == 0) break;
		int num = atoi(buf);
		if (count >= capacity) {
			capacity += increment;
			nums = (int*)realloc(nums,sizeof(int)*capacity);
		}
		nums[count] = num;
		++count;
	}
	qsort(nums,count,sizeof(int),compint);
	median_insert(bst,nums,count);
	printf("Added %i numbers!\n", count);
	free(nums);
}

// Takes sorted array, inserts recursive medians with the goal of making a height-balanced BST.
void median_insert(BST<int>& bst, const int* arr, const int count) {
	struct Block { int pos, len; };
	struct Node { Block b; Node* n; };

	Node* head = new Node{Block{0,count},nullptr};
	Node* tail = head;

	while (head != nullptr) {
		// Pop head
		const Block b = head->b;
		// printf("On block: %i, %i\n", b.pos, b.len);
		if (head == tail) {
			delete head;
			head = nullptr;
			tail = nullptr;
		}
		else {
			Node* oh = head;
			head = head->n;
			delete oh;
		}

		int rel_med_idx = b.len / 2;
		int abs_med_idx = b.pos + rel_med_idx;

		printf("%i ", arr[abs_med_idx]);
		bst.insert(arr[abs_med_idx]);
		
		if (rel_med_idx > 0) {
			Node* n = new Node{Block{b.pos,rel_med_idx},nullptr};
			if (tail) {
				tail->n = n;
				tail = tail->n;
			}
			else {
				head = n;
				tail = n;
			}
		}
		int rblk_sz = rel_med_idx;
		if (b.len%2==0) --rblk_sz;
		if (rblk_sz > 0) {
			Node* n = new Node{Block{abs_med_idx+1,rblk_sz},nullptr};
			if (tail) {
				tail->n = n;
				tail = tail->n;
			}
			else {
				head = n;
				tail = n;
			}
		}
	}
	printf("\n");
}

// Randomly generate numbers and add to heap.
void randomGen(BST<int>& bst, int count, int max) {
	srand(time(NULL));

	int* nums = new int[count];

	printf("Generating and adding %i numbers...\n", count);
	for (int i=0;i<count;i++) {
		nums[i] = rand() % max;
	}
	qsort(nums,count,sizeof(int),compint);
	median_insert(bst,nums,count);
	delete[] nums;
}

// https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
void print_bnode(BNode<int> *node, const char* prefix, bool isLeft) {
    if( node == nullptr ) {
        return;
    }
    printf("%s", prefix);
    printf(isLeft ? "┣━━" : "┗━━" );

    // print the value of the node
    printf("%i\n", node->data);
    
    char* npref = new char[strlen(prefix)+6];
    strcpy(npref, prefix);
    strcat(npref, isLeft ? "┃  " : "   ");

    // enter the next tree level - left and right branch
    print_bnode(node->left, npref, node->right == nullptr ? false : true);
    print_bnode(node->right, npref, false);

    delete[] npref;
}

template <typename T>
void print_bst(const BST<T>& bst) {
    assert(false);
    printf("Unspec template!\n");
}
template <>
void print_bst(const BST<int>& bst) {
    if (bst.head) 
    {
        print_bnode(bst.head,"",false);
    }
    else {
        printf("Empty tree!\n");
    }
}

void search_bst(const BST<int>& bst) {
	char buf[32] = { '\0' };
	printf("Number to search for: ");
	consolein(buf,32);
	int key = atoi(buf);
	printf("Searching for %i...\n", key);
	bool found = bst.contains(key);
	printf("%s %i\n", found ? "Found" : "Did not find", key); 
}
void delete_bst(BST<int>& bst) {
	char buf[32] = { '\0' };
	printf("Number to remove: ");
	consolein(buf,32);
	int key = atoi(buf);
	printf("Searching for %i...\n", key);
	bool removed = bst.remove(key);
	printf("%s %i\n", removed ? "Removed" : "Did not find", key); 
}

int main() {
	BST<int> bst;
	const char* const HELP_STR = "Commands:\n"
		"	HELP - displays this list of commands.\n"
		"	MANUAL - manually add numbers to the BST.\n"
		"	FILE - load numbers from a specified file into the BST.\n"
		"	RANDOM - generates 100 random integers to be added to the BST.\n"
		"	CLEAR - remove all numbers from BST without printing.\n"
		"	TREE - visualize the BST as a tree in the console.\n"
		"	SEARCH - check if a specified number is in the BST.\n"
		"	DELETE - remove a specified number from the BST.\n"
        "	QUIT - end program.\n";

	printf(HELP_STR);

	char buf[32] = { '\0' };
	while (true) {
		printf("> ");
		consolein(buf, 32);
		if (strcmp(buf,"HELP") == 0) {
			printf(HELP_STR);
		}
		else if (strcmp(buf,"MANUAL") == 0) {
			cmdinput(bst);
		}
		else if (strcmp(buf,"FILE") == 0) {
			loadfile(bst);
		}
		else if (strcmp(buf,"RANDOM") == 0) {
			randomGen(bst,25,100);
		}
		else if (strcmp(buf,"CLEAR") == 0) {
			bst.clear();
			printf("Cleared BST of all nodes!\n");
		}
		else if (strcmp(buf,"TREE") == 0) {
			printf("\n TREE: \n");
			print_bst(bst);
		}
		else if (strcmp(buf,"SEARCH") == 0) {
			search_bst(bst);
		}
		else if (strcmp(buf,"DELETE") == 0) {
			delete_bst(bst);
		}
		else if (strcmp(buf,"QUIT") == 0) {
			printf("Exiting!\n");
			break;
		}
		else {
			printf("\"%s\" is not a valid command!\n", buf);
		}
	}

	printf("Goodbye World!\n");
    return 0;
}
