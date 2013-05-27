#include <algorithm>
#include <string>
#include "StringParsing.h"
using namespace std;

namespace StringParsing {
  bool startsWith(string line, string startingString) {
    return startingString == line.substr(0, startingString.size());
  }

  vector<string> &split(string s, char delim, vector<string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }

    return elems;
  }

 string stripQuotationMarks(string s) {
    s.erase(remove( s.begin(), s.end(), '"' ), s.end());
    s.erase(remove( s.begin(), s.end(), '\'' ), s.end());

    return s;
  }
}
