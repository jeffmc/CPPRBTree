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
				// printf("Read: %i\n", num);
				count++;
				bst.insert(num); // Push number to heap and reset integer building buffer.
				numbuf[0] = '\0';
				bufpos = 0;
			}
		}
		if (c == EOF) break;
	}
	assert(strlen(numbuf) == 0);
	printf("Read %i numbers!\n", count);
}

// Manual number entry via console.
void cmdinput(BST<int>& bst) {
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
		bst.insert(num);
		printf("Added %i\n", num);
	}
}


// Randomly generate numbers and add to heap.
void randomGen(BST<int>& bst, int count) {
	srand(time(NULL));

	printf("Generating and adding %i numbers...\n", count);
	for (int i=0;i<count;i++) {
		int r = rand() % 1000;
		bst.insert(r);
	}
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
    
    char* npref = new char[strlen(prefix)+5];
    strcpy(npref, prefix);
    strcat(npref, isLeft ? "┃  " : "   ");

    // enter the next tree level - left and right branch
    print_bnode(node->left, npref, node->right == nullptr ? false : true);
    print_bnode(node->right, npref, false);

    delete[] npref; // TODO: FIX THIS CRASH!
}

template <typename T>
void print_bst(BST<T>& bst) {
    assert(false);
    printf("Unspec template!\n");
}
template <>
void print_bst(BST<int>& bst) {
    if (bst.head) 
    {
        print_bnode(bst.head,"",false);
    }
    else {
        printf("Empty tree!\n");
    }
}

int main() {
	BST<int> bst;
	const char* const HELP_STR = "Commands:\n"
		"	HELP - displays this list of commands.\n"
		"	MANUAL - type in numbers manually to the heap.\n"
		"	FILE - type in a filename to load numbers from.\n"
		"	RANDOM - generates 100 random integers to be added.\n"
		"	CLEAR - remove all numbers from heap without printing.\n"
		"	TREE - visualize the heap as a tree in the console.\n"
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
			randomGen(bst,100);
		}
		else if (strcmp(buf,"CLEAR") == 0) {
			bst.clear();
			printf("Cleared heap!\n");
		}
		else if (strcmp(buf,"TREE") == 0) {
			printf("\n TREE: \n");
			print_bst(bst);
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
