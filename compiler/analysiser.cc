#include <cstdio>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <string>
#include <cstring>
/*
LL(1)

syntax:
  E -> TA
  A -> +TA | e
  T -> FB
  B -> *FB | e
  F -> (E) | i
*/

class CharSet: public std::set<char> {
public:
  bool contains(char ch) {
    return (this->find(ch) != this->cend());
  }
};

class CharStack : public std::list<char> {
public:
  const char peek() {
    return back();
  }

  void push(char x) {
    emplace_back(x);
  }

  const char pop() {
    char r = peek();
    pop_back();
    return r;
  }

  const char *print() {
    for (auto i: *this) {
      putchar(i);
    }
    return "";
  }
};

struct ProdFirst {
  char terminator;
  char nonterminator;

  ProdFirst(char terminator, char nonterminator):
    terminator(terminator), nonterminator(nonterminator) {}

  bool operator<(const ProdFirst &rhs) const {
    return terminator * 10000 + nonterminator < rhs.terminator * 10000 + rhs.nonterminator;
  }
};

class ProductionMap : public std::map<ProdFirst, const char *> {
public:
  bool contains(ProdFirst pf) {
    return (find(pf) != cend());
  }
};

class Analysiser {
public:
  Analysiser() {
    terminator_.emplace('i'); terminator_.emplace('+'); terminator_.emplace('*');
    terminator_.emplace('('); terminator_.emplace(')');
    nonterminator_.emplace('E'); nonterminator_.emplace('A'); nonterminator_.emplace('T');
    nonterminator_.emplace('B'); nonterminator_.emplace('F');
    prods_.emplace(ProdFirst('E', 'i'), "TA");
    prods_.emplace(ProdFirst('E', '('), "TA");
    prods_.emplace(ProdFirst('A', '+'), "+TA");
    prods_.emplace(ProdFirst('A', ')'), "e");
    prods_.emplace(ProdFirst('A', '#'), "e");
    prods_.emplace(ProdFirst('T', 'i'), "FB");
    prods_.emplace(ProdFirst('T', '('), "FB");
    prods_.emplace(ProdFirst('B', '+'), "e");
    prods_.emplace(ProdFirst('B', '*'), "*FB");
    prods_.emplace(ProdFirst('B', ')'), "e");
    prods_.emplace(ProdFirst('B', '#'), "e");
    prods_.emplace(ProdFirst('F', 'i'), "i");
    prods_.emplace(ProdFirst('F', '('), "(E)");
  }

  int runAndPrint(const char *oristr) {
    CharStack symbols;
    symbols.push('#');
    symbols.push(kBeginSymbol());

    int cursor = 0, length = strlen(oristr);
    char st = oristr[cursor];

    printf("Step\t Symbol-Stack\t Input-String\t Production\n");
    printf("0\t ");
    symbols.print();
    printf("\t\t ");
    puts(oristr + cursor);
    for (int i = 1; cursor < length; i++) {
      char x = symbols.pop();

      if (terminator_.contains(x)) {
        if (x == st) {
          st = oristr[++cursor];
          printf("%d\t ", i);
          symbols.print();
          printf("\t\t %s\n", oristr + cursor);
        }
        else return 1;
      } else if (x == '#') {
        if (x == st) return 0;
        else return 2;
      } else if (prods_.contains(ProdFirst(x, st))) {
        std::string desc = prods_[ProdFirst(x, st)];
        if (desc != "#" && desc != "e") {
          std::for_each(desc.crbegin(), desc.crend(), [&symbols] (char x) {
            symbols.push(x);
          });
        }
        printf("%d\t ", i);
        symbols.print();
        printf("\t\t %s\t\t %c->%s\n", oristr + cursor, x, desc.c_str());
      } else {
        return 3;
      }
    }
    return 4;
  }
private:
  const char kBeginSymbol() { return 'E'; }
  CharSet terminator_;
  CharSet nonterminator_;
  ProductionMap prods_;
};

int main(int argc, char *argv[]) {
  char buffer[300];
  Analysiser app;
  printf("Input: ");
  scanf("%s", buffer);
  if (app.runAndPrint(buffer) == 0) {
    puts("Succeed.");
  } else {
    puts("Failed.");
  }

  return 0;
}
