#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "defns.h"
#include <string.h>

using namespace std;

ifstream safeopen(const string &fname) {
        ifstream f;
        try {
                f.open(fname, std::ifstream::in);
        } catch (const std::ios_base::failure &e) {
                cerr << e.code().message() << endl;
                exit(1); // nonzero here means to the O/S that the application has failed
        }
        return f;
}

void copyTree(struct tree &left, struct tree &right) {
  left.symbol = right.symbol;
  left.freq = right.freq;
  left.root = right.root;
}

bool isLessThan(struct tree &left, struct tree &right) {
	if (left.freq < right.freq)
		return true;
	if (left.freq > right.freq)
		return false;
	if (left.symbol <= right.symbol)
		return true;
	return false;
}

void merge(struct tree array[], int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    auto *leftArray = new struct tree[subArrayOne],
         *rightArray = new struct tree[subArrayTwo];

    for (auto i = 0; i < subArrayOne; i++) 
        copyTree(leftArray[i], array[left + i]);
    for (auto j = 0; j < subArrayTwo; j++)
        copyTree(rightArray[j], array[mid + 1 + j]);

    auto indexOfSubArrayOne = 0,
        indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (isLessThan(leftArray[indexOfSubArrayOne], rightArray[indexOfSubArrayTwo])) {
            copyTree(array[indexOfMergedArray], leftArray[indexOfSubArrayOne]);
            indexOfSubArrayOne++;
        }
        else {
            copyTree(array[indexOfMergedArray], rightArray[indexOfSubArrayTwo]);
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    while (indexOfSubArrayOne < subArrayOne) {
        copyTree(array[indexOfMergedArray], leftArray[indexOfSubArrayOne]);
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    while (indexOfSubArrayTwo < subArrayTwo) {
        copyTree(array[indexOfMergedArray], rightArray[indexOfSubArrayTwo]);
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void mergeSort(struct tree array[], int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively
    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

// Insertion sort algo.
void insertionSort(struct tree arr[], int n) {
    for (int i = 1; i < n; i++) {
	struct tree structInsert = arr[i];
        int j = i - 1;
	while (j >= 0 && isLessThan(structInsert, arr[j])) {
	    arr[j + 1].symbol = arr[j].symbol;
	    arr[j + 1].freq = arr[j].freq;
	    arr[j + 1].root = arr[j].root;
            j = j - 1;
        }
        arr[j + 1].symbol = structInsert.symbol;
	arr[j + 1].freq = structInsert.freq;
	arr[j + 1].root = structInsert.root;
    }
}

// Insert the new node at correct index in array.
void insertAtCorrect(struct tree arr[], int n) {
        struct tree structInsert = arr[n-1];
        int key = arr[n-1].freq;
        int j = n - 1 - 1;
        while (j >= 0 && arr[j].freq > key) {
            arr[j + 1].symbol = arr[j].symbol;
            arr[j + 1].freq = arr[j].freq;
            arr[j + 1].root = arr[j].root;
            j = j - 1;
        }
        arr[j + 1].symbol = structInsert.symbol;
        arr[j + 1].freq = structInsert.freq;
        arr[j + 1].root = structInsert.root;
}

// A print utility for debugging.
void printStruct(struct tree arr[], int n) {
  for (int i = 0; i<n; i++) {
	  cout<<arr[i].symbol<<"\t"<<arr[i].freq<<"\n";
  }
}

// Constructing binary tree in bottom up manner.
void constructTree(struct tree Alpha[], int alphas) {
  unsigned threshold_alpha = alphas - 1;
  unsigned max_alphas = alphas;
  while (threshold_alpha--) {
    // Since the input is already sorted and the new node is inserted at correct
    // position at the end, 0-th and 1-th index will always be the minimum.
    struct tree tL = Alpha[0];
    struct tree tR = Alpha[1];
    // Create new node.
    struct symbol* tNew = (struct symbol*)malloc(sizeof(struct symbol));
    tNew->parent = NULL;
    // Set the left and right child.
    tNew->left = tL.root;
    tL.root->parent = tNew;
    tNew->right = tR.root;
    tR.root->parent = tNew;
    tNew->freq = tL.freq + tR.freq;

    // Shift other elements 2 steps back.
    int i = 2;
    for (; i < alphas; i++) {
      if (Alpha[i].freq == -1)
              break;
      Alpha[i-2].freq = Alpha[i].freq;
      Alpha[i-2].root = Alpha[i].root;
      Alpha[i-2].symbol = Alpha[i].symbol;
      Alpha[i].freq = -1;
    }

    // Insert the new node into the Alpha array at the end.
    Alpha[max_alphas-2].freq = tNew->freq;
    Alpha[max_alphas-2].root = tNew;
    max_alphas--;
    // Insert the new node at correct index.
    insertAtCorrect(Alpha, max_alphas);
  }
}

// Find decoding by traversing the root and increment 'i' for every branch.
void findDecoding(struct symbol* root, string &str, int &i, char &ans) {
  if (!root)
	  return;
  // If reached leaf node, use the root->symbol. We decode the stream so far.
  if (!root->left && !root->right) {
    ans = root->symbol;
    return;
  }
  // If '0' go to left branch.
  if (str[i] == '0')
    findDecoding(root->left, str, ++i, ans);
  else // If '1' go to right branch.
    findDecoding(root->right, str, ++i, ans);
}

int main(int argc, char **argv) {
  
  if (argc != 3) {
    cerr<<"Usage: decode pre-file.txt insertion/merge\n";
    exit(1);
  }

  ifstream pre_file_fs = safeopen(argv[1]);

  // Initialize Symbols table.
  struct symbol Symbols[NSYMBOLS];
  for (int i = 0; i<NSYMBOLS; i++) {
    Symbols[i].symbol = (char)i;
    Symbols[i].freq = 0;
    Symbols[i].parent = NULL;
    Symbols[i].left = NULL;
    Symbols[i].right = NULL;
    Symbols[i].encoding[0] = '\0';
  }

  // Read in from the input file passed by command line parameter (the one generated by preprocess exe).
  string input, line;
  unsigned index, freq;
  int alphas, non_alphas;
  alphas = non_alphas = 0;
  while (1) {
    if (pre_file_fs.eof())
      break;
    pre_file_fs>>index>>freq;
    if (Symbols[index].freq != 0)
      break;
    Symbols[index].freq = freq;
    if ((index>=65 && index<=90) || (index>=97 && index<=122)) {
      ++alphas;
    } else {
      ++non_alphas;
    }
  }

  // Initialize Alpha and NonAlpha arrays.
  struct tree *Alpha = new struct tree[alphas];
  struct tree *NonAlpha = new struct tree[non_alphas];
  unsigned alpha_index = 0, non_alpha_index = 0;
  for (unsigned i = 0; i<NSYMBOLS; i++) {
    if (!Symbols[i].freq)
      continue;
    // Comparing ASCII character value to decide whether it is Alpha or not.
    if ((i>=65 && i<=90) || (i>=97 && i<=122)) {
      Alpha[alpha_index].index = i;
      Alpha[alpha_index].freq = Symbols[i].freq;
      Alpha[alpha_index].symbol = Symbols[i].symbol;
      Alpha[alpha_index++].root = &Symbols[i];
    } else {
      NonAlpha[non_alpha_index].index = i;
      NonAlpha[non_alpha_index].freq = Symbols[i].freq;
      NonAlpha[non_alpha_index].symbol = Symbols[i].symbol;
      NonAlpha[non_alpha_index++].root = &Symbols[i];
    }
  }

  // Sort Alpha and NonAlpha based on frequency of characters.
  if (strcmp(argv[2], "insertion") == 0) {
    insertionSort(Alpha, alphas);
    insertionSort(NonAlpha, non_alphas);
  } else if (strcmp(argv[2], "merge") == 0) {
    mergeSort(Alpha, 0, alphas-1);
    mergeSort(NonAlpha, 0, non_alphas-1);
  }

  // Construct binary tree by bottom up manner for Alpha and NonAlpha.
  constructTree(Alpha, alphas);
  constructTree(NonAlpha, non_alphas);

  struct symbol* Root = (struct symbol*)malloc(sizeof(struct symbol));
  Root->left = Alpha[0].root;
  Root->right = NonAlpha[0].root;

  // Until this point we followed steps 1-6 of encoding.
  // Now we'll start off by reading the input and traversing root to decode.

  // Read full 'input' by reading each 'line' from stdin.
  int N;
  cin >> N;
  N+=3;
  line = "";
  while(N--) {
    getline(cin, line);
    //cout << line << "------\n" ;
    line = "";
  }

  input = "";
  string output = "";
  while(getline(cin, line)) {
  //cout << line << "------";
    if (line == "\n") {
	    output += '\n';
    } else {
    //line = line.substr(0,line.size()-3);
	//cout<<"vbdjbvujv";
      for (int i = 0, n = line.size(); i<n; ) {
	char c;
	findDecoding(Root, line, i, c);
	output += c;
      }
    }
    line = "";
  }
cout<<output;
  

  return 0;
}
