#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int HH = seconds / 3600;
  int MM = (seconds - 3600 * HH) / 60;
  int SS = seconds % 60;

  std::stringstream ss;
  // Format the output: HH/MM/SS each outputs 2 digits with zero-padding
  ss << std::setw(2) << std::setfill('0') << HH << ":";
  ss << std::setw(2) << std::setfill('0') << MM << ":";
  ss << std::setw(2) << std::setfill('0') << SS << "";

  string output = ss.str();
  return output;
}