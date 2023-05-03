// Jeff McMillan 3-27-23 Red-Black Tree C++
// This assignment is to implement a red-black tree in C++, 
// in addition to a CLI for viewing and manipulating the contents of the tree.

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

// Load from space-delimited file of numbers.
void loadfile(RBTree<int>& rbt) {
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
	int c; // current char
	int read_count = 0, inserted_count = 0;
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
				++read_count;
				if (rbt.insert(num)) ++inserted_count;

				numbuf[0] = '\0';
				bufpos = 0;
			}
		}
		if (c == EOF) break;
	}
	assert(strlen(numbuf) == 0);
	
	printf("Read %i numbers, inserted %i!\n", read_count, inserted_count);
}

// Manual number entry via console.
void cmdinput(RBTree<int>& rbt) {
	int inserted_count = 0;
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
		if (rbt.insert(num)) {
			++inserted_count;
		}
		else {
			printf("Tree already contains %i, try another\n", num);
		}
	}
	printf("Added %i numbers!\n", inserted_count);
}

// Randomly generate numbers and add to heap.
void randomGen(RBTree<int>& rbt, int count, int max) {
	srand(time(NULL));

	DBGTRACE();
	int inserted_count = 0;
	printf("Generating %i numbers...\n", count);
	for (int i=0;i<count;i++) {
		DBGLOG("Generating a number");
		if (rbt.insert(rand() % max)) ++inserted_count;
	}
	printf("Inserted %i numbers...\n", inserted_count);
}

// https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
void print_rbnode(RBNode<int> *node, const char* prefix, bool isLeft) {
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
    print_rbnode(node->left, npref, node->right == nullptr ? false : true);
    print_rbnode(node->right, npref, false);

    delete[] npref;
}

void print_rbt(const RBTree<int>& rbt) {
	print_rbnode(rbt.head,"",false);
}

void search_rbt(const RBTree<int>& rbt) {
	char buf[32] = { '\0' };
	printf("Number to search for: ");
	consolein(buf,32);
	int key = atoi(buf);
	printf("Searching for %i...\n", key);
	assert(false);
	// bool found = bst.contains(key);
	// printf("%s %i\n", found ? "Found" : "Did not find", key); 
}
void delete_rbt(RBTree<int>& rbt) {
	char buf[32] = { '\0' };
	printf("Number to remove: ");
	consolein(buf,32);
	int key = atoi(buf);
	printf("Searching for %i...\n", key);
	assert(false);
	// bool removed = bst.remove(key);
	// printf("%s %i\n", removed ? "Removed" : "Did not find", key); 
}

int main() {
	RBTree<int> rbt;
	const char* const HELP_STR = "Commands:\n"
		"	HELP - displays this list of commands.\n"
		"	MANUAL - manually add numbers to the RBT.\n"
		"	FILE - load numbers from a specified file into the RBT.\n"
		"	RANDOM - generates 100 random integers to be added to the RBT.\n"
		"	CLEAR - remove all numbers from RBT without printing.\n"
		"	TREE - visualize the RBT as a tree in the console.\n"
		"	SEARCH - check if a specified number is in the RBT.\n"
		"	DELETE - remove a specified number from the RBT.\n"
        "	QUIT - end program.\n"
        "	EX - clear current and load default example RBT.\n";

	printf(HELP_STR);
	char buf[32] = { '\0' };
	while (true) {
		printf("> ");
		consolein(buf, 32);
		if (strcmp(buf,"HELP") == 0) {
			printf(HELP_STR);
		}
		else if (strcmp(buf,"MANUAL") == 0) {
			cmdinput(rbt);
		}
		else if (strcmp(buf,"FILE") == 0) {
			loadfile(rbt);
		}
		else if (strcmp(buf,"RANDOM") == 0) {
			randomGen(rbt,25,100);
		}
		else if (strcmp(buf,"CLEAR") == 0) {
			rbt.clear();
			printf("Cleared BST of all nodes!\n");
		}
		else if (strcmp(buf,"TREE") == 0) {
			printf("\n TREE: \n");
			print_rbt(rbt);
		}
		else if (strcmp(buf,"SEARCH") == 0) {
			search_rbt(rbt);
		}
		else if (strcmp(buf,"DELETE") == 0) {
			delete_rbt(rbt);
		}
		else if (strcmp(buf,"QUIT") == 0) {
			printf("Exiting!\n");
			break;
		}
		else if (strcmp(buf,"EX") == 0) {
			example_rbtree(rbt);
			printf("Example loaded!\n");
		}
		else {
			printf("\"%s\" is not a valid command!\n", buf);
		}
	}

	printf("Goodbye World!\n");
    return 0;
}
