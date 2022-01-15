/*
 * CSc103 Project 7: Towers of Hanoi
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References: cplusplus, runestone academy, youtube, Jeff, Piazza
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 4
 */

// TODO: write the program.
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <getopt.h> // to parse long arguments.
#include <cstdlib> // for atoi function

//function to print step of the game
void printSteps(short start, short end){
	cout << start << " " << end << endl;
}

//recursive function that carries out the steps of the tower game
void towerSteps(short height, short start, short end, short mid){
	if (height <= 1){ //base case of tower height 1 disk or less
		printSteps(start,end);
		return;
	} 
	else{
		towerSteps(height - 1, start, mid, end);
		printSteps(start, end);
		towerSteps(height - 1, mid, end, start);
	}

}

/* Here's a skeleton main function for processing the arguments. */
int main(int argc, char *argv[]) {
	// define long options
	static struct option long_opts[] = {
		{"start",        required_argument, 0, 's'},
		{"end",          required_argument, 0, 'e'},
		{"num-disks",    required_argument, 0, 'n'},
		{0,0,0,0} // this denotes the end of our options.
	};
	// now process the options:
	char c; // to hold the option
	int opt_index = 0;
	short n=0,start=0,end=0; /* to store inputs to the towers function. */
	while ((c = getopt_long(argc, argv, "s:e:n:", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 's': // process option s
				start = atoi(optarg);
				break;
			case 'e': // process option e
				end = atoi(optarg);
				break;
			case 'n': // process option n
				n = atoi(optarg);
				break;
			case '?': // this will catch unknown options.
				// here is where you would yell at the user.
				// although, getopt will already print an error message.
				return 1;
		}
	}
	/* TODO: now that you have the options and arguments,
	 * solve the puzzle. */

	//mid is the middle/placeholder/intermediate peg 
	short mid = 6 - (start + end); // 6 = sum of the rod #'s, 1 + 2 + 3 = 6. 
	towerSteps(n, start, end, mid); //call on recursive function
	return 0;
}
