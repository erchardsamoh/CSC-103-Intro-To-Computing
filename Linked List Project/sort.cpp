/*
 * CSc103 Project 4: Sorting with lists
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References: Jeff, Syed, Geeksforgeeks, stackoverflow, l4.pdf, piazza, youtube.
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 10~
 */

#include <iostream>
#include <getopt.h> // to parse long arguments.
#include <string>
using namespace std;

/* doubly linked list node: */
struct node {
  string data;
  node* prev;
  node* next;
  node(string s="", node* p=NULL, node* n=NULL) : data(s),prev(p),next(n) {}
};

//global node pointers for head and tail for doubly linked list intialized to NULL.
node* head = NULL;
node* tail = NULL;

//*NOTE TO SELF* (probably won't make sense to anyone else)
//view the linked list as a linear line (i guess?) going from left to right. head is leftmost. tail is rightmost.
//node*& L is wherever the linked list begins. changes according to the function that it is passed through as an argument.
//example would be how regularPrint would take node*& L as the head, since it needs to iterate in foward fashion, while in reversePrint node*& is the tail, as it needs to iterate in backwards fashion.

//below are all the functions used for the program
//function to insert element to tail of the list
//by reference because we want change to reflect inside main.
void pushbackList(string words, node*& L = tail) {
  if (tail == NULL) {
    return;
  }
  node* current = new node(words, L); //argument inside node (data, prev, next) here words = data, L = prev
  L->next = current;
  L = L->next;
}

//function to insert element to head of the list.
void pushfrontList(string words, node*& L = head) {
  if (head == NULL) {
    return;
  }
  node* current = new node(words, NULL, L); //argument inside node (data, prev, next) here words = data, NULL = head (since we are pushing at the front.), and L = next
  L->prev = current;
  L = L->prev;
}

//function to insert element between two nodes (left node, right node)
void split(string words, node*& left, node*& right) {
  node* current = new node(words, left, right); //left --> prev, right --> next (think of list linearly!)
  left->next = current;
  right->prev = current;
}

//function to check for duplicates within given strings
bool duplicateCheck(string words, node*& L = head) {
  for (node* i = L; i != NULL; i = i->next) { //iterates through ith index of list looking for strings that match the data stored in the lists. if duplicates exist, then return false.
    if (words == i->data) { 
      return false; //means duplicates exist
    }
  }
  return true; //return true if no duplicates found
}

//function to print doubly linked list in forward order
void regularPrint(node* L = head) {
  for (node* i = L; i != NULL; i = i->next) { //iterate through linked list forwards (l4.pdf)
    cout << i->data << endl;
  }
}

//function to print doubly linked list in reverse order
void reversePrint(node* L = tail) {
  for (node* i = L; i != NULL; i = i->prev) { //iterate through linked list backwards
    cout << i->data << endl;
  }
}

int main(int argc, char *argv[]) {
  /* define long options */
  static int unique=0, reverse=0;
  static struct option long_opts[] = {
    {"unique",   no_argument,       &unique,   'u'},
    {"reverse",  no_argument,       &reverse,  'r'},
    {0,0,0,0} // this denotes the end of our options.
  };
  /* process options */
  char c; /* holds an option */
  int opt_index = 0;
  while ((c = getopt_long(argc, argv, "ur", long_opts, &opt_index)) != -1) {
    switch (c) {
      case 'u':
        unique = 1;
        break;
      case 'r':
        reverse = 1;
        break;
      case '?': /* this will catch unknown options. */
        return 1;
    }
  }

  /* NOTE: at this point, variables 'reverse' and 'unique' have been set
   * according to the command words.  */
  /* TODO: finish writing this.  Maybe use while(getline(cin,words)) or
   * similar to read all the lines from stdin. */

  string words;
  //if condition for input, create a new node for the corresponding inputs. this code always runs when there is input given.
  if (getline(cin,words)) {
    head = new node(words); //new node created, head assigned its value
    tail = head; //tail assigned with head's value.
  }

  //insertion sort occurs within this while loop.
  //what is being compared, is the lexographical order i assume, as things are printed out in lexigraphical order. 
  while (getline(cin,words)) {
    if (words > tail->data) { //if input > elements stored in list, add the input at the end (pushback). 
      pushbackList(words); 
    }
    else if (words < head->data) { //if input < elements stored in list, add the input at the front of list (pushfront).
      pushfrontList(words);
    } 
    else if (unique == 1 && !(duplicateCheck(words, head))) { //if duplicatecheck is not true and unique is true, meaning no duplicates in input.
      continue;
    } 
    else { //if there are duplicates this code runs:
      node* i; //node pointer i
      for (i = head; words >= i->data; i = i->next){ 
        if (i == tail) 
          break; //reached end of list
      }
      if (i == head) {
        pushfrontList(words); //insert new node at front, since i == head (head is at front of list)
      }
      else {
        split(words, i->prev, i); //insert new node between
      }
    }
  }
  //finally, print the list!
  if (reverse == 1) { //if reverse is given, run reversePrint
    reversePrint();
  }
  else{ //if not just regularPrint runs.
    regularPrint(); 
  } //prints the output depending on value of reverse (-r). if -r given in stdin, reverse == 1, and if not, reverse == 0.
  return 0;
}

