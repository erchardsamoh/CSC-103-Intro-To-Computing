/*
 * CSc103 Project 3: Game of Life
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References: cpluscplus.com, stackoverflow.com, rosettacode, cppreference, geeksforgeeks, tutorialspoint, my friend Jeff from UMICH. csc103 piazza
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 30-40
 */

#include <cstdio>
#include <stdlib.h> // for exit();
#include <getopt.h> // to parse long arguments.
#include <unistd.h> // sleep
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using namespace std;

static const char* usage =
"Usage: %s [OPTIONS]...\n"
"Text-based version of Conway's game of life.\n\n"
"   --seed,-s     FILE     read start state from FILE.\n"
"   --world,-w    FILE     store current world in FILE.\n"
"   --fast-fw,-f  NUM      evolve system for NUM generations and quit.\n"
"   --help,-h              show this message and exit.\n";

size_t max_gen = 0; /* if > 0, fast forward to this generation. */
string wfilename = "/tmp/gol-world-current"; /* write state here */
FILE* fworld = 0; /* handle to file wfilename. */
string initfilename = "/tmp/gol-world-current"; /* read initial state from here. */

vector<vector<bool> > world;
vector<vector<bool> > future;

int nbrCount(int i, int j, const vector<vector<bool> >& g); //will check the surrounding cells of a cell for a neighbor and add to a count.
void update(); //takes the count of neighbors and applies rules of GOL to world
int initFromFile(const string& fname); //open/initalize the file going to be used
void mainLoop(); //main loop consisting of update and nbrCount functions of GOL
void dumpState(FILE* f); //the function where the world gets written to a file and dumped into corresponding location
void printGen(); //prints generation out onto terminal, utilized when max_gen > 0

/* NOTE: you can use a *boolean* as an index into the following array
 * to translate from bool to the right characters: */
 //char text[3] = ".O"; <--- not sure what this is for still :(

int main(int argc, char* argv[]) {
	// define long options
	static struct option long_opts[] = {
		{"seed",    required_argument, 0, 's'},
		{"world",   required_argument, 0, 'w'},
		{"fast-fw", required_argument, 0, 'f'},
		{"help",    no_argument,       0, 'h'},
		{0,0,0,0}
	};
	// process options:
	char c;
	int opt_index = 0;
	while ((c = getopt_long(argc, argv, "hs:w:f:", long_opts, &opt_index)) != -1) {
		switch (c) {
		case 'h':
			printf(usage, argv[0]);
			return 0;
		case 's':
			initfilename = optarg;
			break;
		case 'w':
			wfilename = optarg;
			break;
		case 'f':
			max_gen = atoi(optarg);
			break;
		case '?':
			printf(usage, argv[0]);
			return 1;
		}
	}
	/* NOTE: at this point wfilename initfilename and max_gen
	 * are all set according to the command line: */
	 //printf("input file:  %s\n",initfilename.c_str());
	 //printf("output file: %s\n",wfilename.c_str());
	 //printf("fast forward to generation: %lu\n",max_gen);
	 /* TODO: comment out 3 lines above once you see what's in them. */
	 /* NOTE: if wfilename == "-" you should write to stdout, not a file
	  * named "-"! */

	  /* If you wrote the initFromFile function, call it here: */
	initFromFile(initfilename);
	mainLoop();
	return 0;
}

int initFromFile(const string& fname) {
	FILE* f = fopen(fname.c_str(), "rb"); //arguments in fopen are function fname.c_str() which acquires filename of file being read, and "rb" which indicates the mode, which is open file for reading
	int readChar; //to read characters inside file
	vector<bool> row; //row of the world

	if (!f) {
		exit(1);
	}

	//determines the values of the world based on characters within the file
	while ((readChar = getc(f)) != EOF) { // getc is a f
		if (readChar == '\n') {
			world.push_back(row);
			row.clear();
		}
		else if (readChar == '.') {
			row.push_back(false); //dead cell --> put false value into vector
		}
		else if (readChar == 'O') {
			row.push_back(true); //alive cell --> put true value into vector
		}
	}
	rewind(f); //moves file position indicator to beginning of file stream
	return 0;
}

//are these considered magic numbers? I want to try and make the world fit any type of grid dimension input.

//the following function will make the world a "torus." Cells on the edges will be neighbors to one another.
int torusMaker(int row, int col, vector<vector<bool> >& world) {
	int rowCount = world.size(); //this allows for our grid to be whatever the seed is.
	int colsCount = world[0].size();
	return world.at((row + rowCount) % rowCount).at((col + colsCount) % colsCount); //the .at() function will return an element that is located at the position given. incuded in the <vector> library.
}   /*modulus operator as explained in Piazza by Hameed: "forward index can be generalized this way [ i ] [ (j + 1) % row.size()]"
	 *However, instead of the +1, I add the number of rows and columns there are to make the torus for the entireity of the board.
	 */

	 //the following function checks the amount of neighbors around a certain cell in the world.
int nbrCount(int row, int col, vector<vector<bool> >& world) {
	int neighbors = 0; //using marty's method from recitation
	for (int c = -1; c <= 1; c++) { //checks for neighbors from left to right. (column wise)
		for (int r = -1; r <= 1; r++) { //checks for neighbrs from top to bottom. (row wise)
			if (r || c) {
				neighbors += torusMaker(row + r, col + c, world); //everytime a neighbor is found, add to the count.
			}
		}
	}
	return neighbors; //return the sum of neighbors around cell
}

//apply rules of GOL
void update() {
	vector<bool> middle; //placeholder vector that stores new updated version of grid, according to nbrCount functions
	for (size_t i = 0; i < world.size(); i++) {
		for (size_t j = 0; j < world[i].size(); j++) {
			if (nbrCount(i, j, world) < 2 || nbrCount(i, j, world) > 3) {
				middle.push_back(false);
			}
			else if (nbrCount(i, j, world) == 3) {
				middle.push_back(true);
			}
			else if (nbrCount(i, j, world) == 2) {
				middle.push_back(world[i][j]);
			}
		}
		future.push_back(middle); //middle/placeholder vector has updated world w/ rules applied. put all of middle's content into future vector.
		middle.clear(); //erases the contents of the middle vector.
	}
	world.clear(); //erases content of world, which held the previous generation.
	world = future; //world now has future vector's contents.
	future.clear(); //clear future vector contents.
}

void dumpState(FILE* f) {
	char c;
	//writes the alive/dead character based on its boolean value from the vector
	for (size_t i = 0; i < world.size(); i++) {
		for (size_t j = 0; j < world[i].size(); j++) {
			if (world[i][j]) {
				c = 'O'; //write "O" into output file if value exists (cell alive)
			}
			else {
				c = '.'; //write "." into output file if value does not exist (cell dead)
			}
			fwrite(&c, 1, 1, f);
		}
		c = '\n';
		fwrite(&c, 1, 1, f);
	}
	rewind(f);
}

void printGen() {
	for (size_t i = 0; i < world.size(); i++) {
		for (size_t j = 0; j < world[i].size(); j++) {
			if (world[i][j] == false) { //if cell is dead print "."
				cout << ".";
			}
			else if (world[i][j] == true) { //if cell is alive print "O"
				cout << "O";
			}
		}
		cout << "\n";
	}
}

void mainLoop() {
	/* update, write, sleep */
	fworld = fopen(wfilename.c_str(), "wb"); // wb = create file for writing
	while (max_gen == 0) { //if max_gen is = to 0, then just cont. update the world every 1 second 
		update();
		dumpState(fworld);
		sleep(1); //sleep function for the 1 second gap beteen generations being printed onto the terminal
	}
	for (size_t i = 0; i < max_gen; i++) { //if max_gen is given a certain number in the input, run the for loop for as many times (up to number)
		update();
		dumpState(fworld);
		if (i == max_gen - 1) { //print last generation only
			printGen();
		}
	}
}
