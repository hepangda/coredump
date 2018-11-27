#include <cstring>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum TokenType : int {
  kTTInvalid = 0,

  // reserved
  kTTRProgram,
  kTTRBegin,
  kTTREnd,
  kTTRInteger,
  kTTRWhile,
  kTTRVar,
  kTTRIf,
  kTTRThen,
  kTTRElse,
  kTTRProcedure,
  kTTRDo,
  kTTRConst,

  // symbols
  kTTSEqual,
  kTTSLess,
  kTTSLesseq,
  kTTSMore,
  kTTSMoreeq,
  kTTSNoteq,
  kTTSPlus,
  kTTSMulti,
  kTTSMinus,
  kTTSComma,
  kTTSColon,
  kTTSSemi,

  // other
  kTTXIdentifier,
  kTTXNumber,
};

std::unordered_map<std::string, TokenType> pascal_tokens;

void initMap() {
  pascal_tokens.emplace("const", kTTRConst);
  pascal_tokens.emplace("program", kTTRProgram);
  pascal_tokens.emplace("begin", kTTRBegin);
  pascal_tokens.emplace("end", kTTREnd);
  pascal_tokens.emplace("var", kTTRVar);
  pascal_tokens.emplace("integer", kTTRInteger);
  pascal_tokens.emplace("while", kTTRWhile);
  pascal_tokens.emplace("if", kTTRIf);
  pascal_tokens.emplace("then", kTTRThen);
  pascal_tokens.emplace("else", kTTRElse);
  pascal_tokens.emplace("do", kTTRDo);
  pascal_tokens.emplace("procedure", kTTRProcedure);
  pascal_tokens.emplace("=", kTTSEqual);
  pascal_tokens.emplace("<", kTTSLess);
  pascal_tokens.emplace("<=", kTTSLesseq);
  pascal_tokens.emplace(">", kTTSMore);
  pascal_tokens.emplace(">=", kTTSMoreeq);
  pascal_tokens.emplace("<>", kTTSNoteq);
  pascal_tokens.emplace("+", kTTSPlus);
  pascal_tokens.emplace("*", kTTSMulti);
  pascal_tokens.emplace("-", kTTSMinus);
  pascal_tokens.emplace(",", kTTSComma);
  pascal_tokens.emplace(":", kTTSColon);
  pascal_tokens.emplace(";", kTTSSemi);
}

class Token {
 public:
  explicit Token(TokenType type) : type_(type), value_(0) {}
  Token(TokenType type, int value) : type_(type), value_(value) {}

  TokenType type() { return type_; }
  int value() { return value_; }
  void set_type(TokenType type) { type_ = type; }
  void set_value(int value) { value_ = value; }

 private:
  TokenType type_;
  int value_;
};


class CharBuffer {
 public:
  explicit CharBuffer(int size)
      : size_(size), buf_(new char[size]), readptr_(0) {}

  char *peek() const { return buf_.get() + readptr_; }
  void take(char **ptr) const { *ptr = buf_.get() + (readptr_++); }

  void clearState() const { readptr_ = 0; }
  bool willEmpty() const { return readable_ <= readptr_ + 1; }
  void assign(const char *str) {
    readable_ = strlen(str);
    strcpy(buf_.get(), str);
  }

private:
  std::unique_ptr<char> buf_;
  int size_;
  int readable_;
  mutable int readptr_;
};

std::vector<std::string> identfiers;
std::vector<std::string> numbers;
std::vector<Token> results;

class Tokenizer {
 public:
  void run(const CharBuffer &buf) {
    enum {
      kBegin,
      kWord,
      kSymbol,
      kSpace,
      kNumber,
      kError,
      kEnd,
    } state = kBegin;

#define isLetter ((*tch >= 'a' && *tch <= 'z') || (*tch >= 'A' && *tch <= 'Z'))
#define isNumber (*tch >= '0' && *tch <= '9')
#define isSymbol (*tch == '>' || *tch == '<' || *tch == '=' || *tch == ';' || *tch == ':' || *tch == ',' || *tch == '+' || *tch =='-' || *tch == '*' || *tch == '/')
#define isWhitespace (*tch == ' ' || *tch == '\n' || *tch == '\t')
#define SetStartpos (startpos = tch)
#define Deal(type) {need_deal = true; next_type = type;}
#define SetType(type)(token_type = type)
#define stemap(cond, new_state, other) if ((cond)) { state = new_state; other ; break; }
#define reject() state = (buf.willEmpty() ? kEnd : kError); break; 

    char *startpos = nullptr;
    bool need_deal = false;
    enum {
      kTypeNothing,
      kTypeIdentifier,
      kTypeNumber,
      kTypeSymbol
    } token_type = kTypeNothing, next_type = kTypeNothing;

    for (char *lch = nullptr; state != kEnd; buf.take(&lch)) {
      char *tch = buf.peek();
      switch (state) {
        case kBegin:
        case kSpace:
        stemap(isLetter, kWord, { 
          SetStartpos; 
          SetType(kTypeIdentifier);
        });

        stemap(isNumber, kNumber, { 
          SetStartpos; 
          SetType(kTypeNumber); 
        });

        stemap(isSymbol, kSymbol, { 
          SetStartpos; 
          SetType(kTypeSymbol);
        });

        stemap(isWhitespace, kBegin, {});
        reject();
      case kWord:
        stemap(isLetter, kWord, {});
        stemap(isNumber, kWord, {});
        stemap(isSymbol, kSymbol, Deal(kTypeSymbol));
        stemap(isWhitespace, kSpace, Deal(kTypeNothing));
        reject();
      case kSymbol:
        stemap(isLetter, kWord, Deal(kTypeIdentifier));
        stemap(isNumber, kNumber, Deal(kTypeNumber));
        stemap(isSymbol, kSymbol, Deal(kTypeSymbol));
        stemap(isWhitespace, kSpace, Deal(kTypeNothing));
        reject();
      case kNumber:
        stemap(isLetter, kError, {});
        stemap(isNumber, kNumber, {});
        stemap(isSymbol, kSymbol, Deal(kTypeSymbol));
        stemap(isWhitespace, kSpace, Deal(kTypeNothing));
        reject();
      case kError:
        stemap(isWhitespace, kBegin, results.emplace_back(kTTInvalid));
        reject();
      case kEnd:
        break;
      }

      if (need_deal) {
        need_deal = false;
        std::string token_string(startpos, std::distance(startpos, lch) + 1);

        if (pascal_tokens.find(token_string) != pascal_tokens.end()) {
          results.emplace_back(pascal_tokens[token_string]);
        } else if (token_type == kTypeIdentifier) {
          identfiers.emplace_back(token_string);
          results.emplace_back(kTTXIdentifier, identfiers.size() - 1);
        } else if (token_type == kTypeNumber) {
          numbers.emplace_back(token_string);
          results.emplace_back(kTTXNumber, numbers.size() - 1);
        }
        token_type = next_type;
        SetStartpos;
      }
    }

#undef isLetter
#undef isNumber
#undef isSymbol
#undef isWhitespace
#undef SetStartpos
#undef SetFlag
#undef SetType
#undef stemap
#undef reject
  }
};

int main(int argc, char *argv[])
{
  initMap();
  CharBuffer buf(2000);
  buf.assign(
R"(program pro;
const a := 10;
var b,c;
procedure p;
begin
  c:=b+a;
end;
begin
  while b<>0 do
  begin
  end;
end;
)");

  Tokenizer app;
  app.run(buf);

  const char *typestr[] = {
    "invalid", "program", "begin", "end",
    "integer", "while", "var", "if", 
    "then", "else", "procedure", "do",
    "const", "=", "<", "<=", ">", ">=",
    "<>", "+", "*", "-", ",", ":", ";",
  };
  for (auto &i: results) {
    if (i.type() == kTTXIdentifier) {
      printf("\033[33m[idn] \"%s\"\033[0m\n", identfiers[i.value()].c_str());
    } else if (i.type() == kTTXNumber) {
      printf("\033[34m[num] %s\033[0m\n", numbers[i.value()].c_str());
    } else if (i.type() == kTTInvalid) {
      printf("\033[35m[inv] invalid\033[0m\n");
    } else {
      printf("\033[36m[key] %s\033[0m\n", typestr[i.type()]);
    }
  }

  puts("");
  puts("============================");
  puts("id\t value");
  puts("============================");
  for (int i = 0; i < identfiers.size(); i++) {
    printf("%d\t %s\n", i, identfiers[i].c_str());
  }
  return 0;
}
