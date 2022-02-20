#include <stdlib.h>
#include <iostream>
#include "defns.h"

using namespace std;

int main(void) {
  struct symbol Symbols[NSYMBOLS];
  for (unsigned i = 0; i<NSYMBOLS; i++) {
    Symbols[i].symbol = (char)i;
    Symbols[i].freq = 0;
    Symbols[i].parent = NULL;
    Symbols[i].left = NULL;
    Symbols[i].right = NULL;
    Symbols[i].encoding[0] = '\0';
  }
  // Read full 'input' by reading each 'line'.
  string input, line;
  do {
    line = "-";
    getline(cin, line);
    if (line != "-")
      input += line+"\n";
  } while(line != "-");

  // Preprocess.
  for (char c : input) {
    unsigned i = (int)c;
    Symbols[i].freq++;
    Symbols[i].symbol = c;
  }

  // Output non-zero frequency symbol data.
  for (unsigned i = 0; i<NSYMBOLS; i++) {
    if (!Symbols[i].freq)
      continue;
    cout<<i<<"\t"<<Symbols[i].freq<<"\n";
  }
  return 0;
}
