#include <vector>
#include <sstream>
using namespace std;

// Some useful string parsing functions for getting values out of config files
namespace StringParsing {
  // Returns true if the 'line' begins with 'startingStraing', case SENSITIVE
  bool startsWith(string line, string StartingString);

  // Splits 'str' using 'delim'
  vector<string> &split(string s, char delim, vector<string> &elems);

  // Returns a copy of the specified string with all the single and double
  // quotes removed.
  //
  // string str = "We don't want your \"upgrades\" anymore.";
  // stripQuotationMarks(str);
  //   => "We dont want your upgrades anymore"
  //             ^           ^       ^
  //
  string stripQuotationMarks(string s);
}
