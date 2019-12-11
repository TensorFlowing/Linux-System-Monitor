#include <string>
#include <iomanip>
#include "format.h"
#include <chrono>

using namespace std::chrono;
using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long long_elpasedSec) {

  seconds elpasedSec = seconds(long_elpasedSec);
  hours   HH = duration_cast<hours>(elpasedSec);
  minutes MM = duration_cast<minutes>(elpasedSec % hours(1));
  seconds SS = duration_cast<seconds>(elpasedSec % minutes(1));

  std::stringstream ss;
  // Format the output: HH/MM/SS each outputs 2 digits with zero-padding
  ss << std::setw(2) << std::setfill('0') << HH.count() << ":";
  ss << std::setw(2) << std::setfill('0') << MM.count() << ":";
  ss << std::setw(2) << std::setfill('0') << SS.count() << "";
  string output = ss.str();
  
  return output;
}