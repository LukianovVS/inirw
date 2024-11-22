#include <iostream>
#include "inirw.h"

using std::cout, std::cin, std::endl;

int main(int argc, char* argv[])
{
  cout << "DEMO Field" << endl;
  inirw::Field field("name", "value");
  cout << field;
  field = 123;
  cout << field << endl;

  cout << "DEMO Section" << endl;
  inirw::Section section("AbC");
  section = {{"A", "1"}, {"B", "2"}, {"C", "3"}};
  cout << section << endl;

  cout << "Demo inifile" << endl;

  inirw::IniRW ini;
  ini = {
    { "section_1", {{"A", "1"}, {"B", "2"}, {"C", "3"}}},
    { "section_2", {{"D", "4"}, {"E", "5"}, {"F", "6"}}},
  };

  cout << ini;

  return 0;
}
