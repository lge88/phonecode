#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::transform;

struct Options {
  Options();
  string wordsFile, phoneFile;
  bool reverse;
};

Options::Options() {
  wordsFile = "words.txt";
  phoneFile = "phone.txt";
  reverse = false;
}

void _parseOptions(std::vector<string> args, Options* optionsPtr) {
  if (args.empty()) return;

  string arg = args.back();
  args.pop_back();

  if (arg == "-p" || arg == "--phone-file") {
    optionsPtr->phoneFile = args.back();
    args.pop_back();
  } else if (arg == "-w" || arg == "--words-file") {
    optionsPtr->wordsFile = args.back();
    args.pop_back();
  } else if (arg == "-r" || arg == "--reverse") {
    optionsPtr->reverse = true;
  } else {
    cout << "Unknow flag " << arg << "." << endl;
    exit(1);
  }

  _parseOptions(args, optionsPtr);
}

Options parseOptions(int argc, char* argv[]) {
  Options options;
  std::vector<string> args;
  while (argc-- > 1) {
    args.push_back(string(argv[argc]));
  }
  _parseOptions(args, &options);
  return options;
}

class Translator {
 public:
  string phoneFile, wordsFile;

  // Letter to digit map
  std::map<char, char> L2D;
  bool l2dInitialized;

  // Digits to words map
  std::map<string, std::vector<string> > D2W;
  bool d2wInitialized;

  std::vector<string> buildWordsList();
  void initL2D();
  void initD2W();
  const std::map<char, char>& getL2D();
  const std::map<string, std::vector<string> >& getD2W();
  char letterToDigit(char letter);
  string encode(string word);
  std::vector<std::vector<string> > decode(string digits);

 public:
  Translator(string phoneFile, string wordsFile);
  string translate(string digits);
  string reverseTranslate(string sentences);
};

typedef string (Translator::*t_string_transform_func)(string);

// Debugging:

// std::ostream& operator<<(std::ostream& os, const Options& options) {
//   os << "wordsFile: " << options.wordsFile << endl;
//   os << "phoneFile: " << options.phoneFile << endl;
//   os << "reverse: " << options.reverse;
//   return os;
// }

// std::ostream& operator<<(std::ostream& os, const std::vector<string>& vs) {
//   os << "[";
//   for (int i = 0, len = vs.size(); i < len; ++i) {
//     if (i > 0) os << ", ";
//     os << vs[i];
//   }
//   os << "]";
//   return os;
// }

// std::ostream& operator<<(std::ostream& os, const std::map<char, char>& m) {
//   std::map<char, char>::const_iterator it = m.begin();
//   std::map<char, char>::const_iterator en = m.end();
//   while (it != en) {
//     os << it->first << " -> " << it->second << endl;
//     ++it;
//   }
//   return os;
// }

// std::ostream& operator<<(std::ostream& os, const std::map<string, std::vector<string> >& m) {
//   std::map<string, std::vector<string> >::const_iterator it = m.begin();
//   std::map<string, std::vector<string> >::const_iterator en = m.end();
//   while (it != en) {
//     os << it->first << " -> " << it->second << endl;
//     ++it;
//   }
//   return os;
// }

Translator::Translator(string pFile, string wFile) :
    phoneFile(pFile), wordsFile(wFile),
    l2dInitialized(false), d2wInitialized(false) {}

std::vector<string> Translator::buildWordsList() {
  std::vector<string> out;
  string word;
  std::ifstream infile(wordsFile);
  while (infile >> word) out.push_back(word);
  return out;
}

const std::map<char, char>& Translator::getL2D() {
  if (!l2dInitialized) initL2D();
  return L2D;
}

const std::map<string, std::vector<string> >& Translator::getD2W() {
  if (!d2wInitialized) initD2W();
  return D2W;
}

void split(const string& s, char delim, std::vector<string>& elems) {
  std::istringstream is(s);
  string item;
  while (std::getline(is, item, delim)) elems.push_back(item);
}

std::vector<string> split(const string& s, char delim) {
  std::vector<string> elems;
  split(s, delim, elems);
  return elems;
}

void Translator::initL2D() {
  std::ifstream ifs(phoneFile);
  string line;
  while (ifs >> line) {
    std::vector<string> parts = split(line, ',');
    string digit = parts[0], letters = parts[1];
    for (int i = 0, len = letters.size(); i < len; ++i) {
      char letter = letters[i];
      L2D[letter] = digit.empty() ? '-' : digit[0];
    }
  }
  l2dInitialized = true;
}

char Translator::letterToDigit(char letter) {
  letter = ::toupper(letter);
  const std::map<char, char>& l2d = getL2D();
  return l2d.find(letter) == l2d.end() ? '-' : l2d.at(letter);
}

string Translator::encode(string word) {
  string digits;
  for (int i = 0, len = word.size(); i < len; ++i) {
    char letter = word[i];
    char digit = letterToDigit(letter);
    if (digit != '-') digits.push_back(digit);
  }
  return digits;
}

string Translator::reverseTranslate(string word) {
  return word + ": " + encode(word);
}

std::vector<std::vector<string> > Translator::decode(string digits) {
  if (digits.empty())
    return std::vector<std::vector<string> >(1, std::vector<string>());

  const std::map<string, std::vector<string> >& d2w = getD2W();
  std::vector<std::vector<string> > solutions = std::vector<std::vector<string> >();

  for (int i = 1, len = digits.size(); i <= len; ++i) {
    string prefix = digits.substr(0, i);
    string rest = digits.substr(i);

    if (d2w.find(prefix) != d2w.end()) {
      const std::vector<string>& firstWords = d2w.at(prefix);
      std::vector<std::vector<string> > partialSolutions = decode(rest);
      for (int i = 0, lenI = firstWords.size(); i < lenI; ++i) {
        string firstWord = firstWords[i];
        for (int j = 0, lenJ = partialSolutions.size(); j < lenJ; ++j) {
          std::vector<string> partialSolution = partialSolutions[j];
          std::vector<string> solution;
          solution.push_back(firstWord);
          solution.insert(
              solution.end(),
              partialSolution.begin(),
              partialSolution.end());
          solutions.push_back(solution);
        }
      }
    }
  }
  return solutions;
}

string Translator::translate(string digits) {
  std::vector<std::vector<string> > solutions = decode(digits);
  std::stringstream ss;

  for (int i = 0, len = solutions.size(); i < len; ++i) {
    if (i > 0) ss << "\n";
    ss << digits << ": ";

    const std::vector<string>& words = solutions[i];
    for (int i = 0, len = words.size(); i < len; ++i) {
      if (i > 0) ss << " ";
      ss << words[i];
    }
  }

  return ss.str();
}

void Translator::initD2W() {
  std::vector<string> wl = buildWordsList();
  for (int i = 0, len = wl.size(); i < len; ++i) {
    string word = wl[i];
    string digits = encode(word);
    D2W[digits].push_back(word);
  }
  d2wInitialized = true;
}

int main(int argc, char* argv[]) {
  Options options = parseOptions(argc, argv);
  Translator translator(options.phoneFile, options.wordsFile);

  t_string_transform_func transformLine;
  if (options.reverse) {
    transformLine = &Translator::reverseTranslate;
  } else {
    transformLine = &Translator::translate;
  }

  string line;
  while (std::getline(cin, line, '\n'))
    cout << (translator.*transformLine)(line) << endl;
}
