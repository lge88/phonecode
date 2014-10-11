#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::transform;

string upcase(string in) {
  string out(in);
  transform(in.begin(), in.end(), out.begin(), ::toupper);
  return out;
}

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

std::ostream& operator<<(std::ostream& os, const Options& options) {
  os << "wordsFile: " << options.wordsFile << endl;
  os << "phoneFile: " << options.phoneFile << endl;
  os << "reverse: " << options.reverse;
  return os;
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

int main(int argc, char* argv[]) {
  cout << "options:\n" << parseOptions(argc, argv) << endl;

  // string line;
  // while (cin >> line) {
  //   cout << upcase(line) << endl;
  // }
}
