#include <iostream>
#include <fstream>

#include "inirw.h"

using std::cout, std::cin, std::endl;

void simple_demo(){
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
  ini += {"section_3", {{"key", "val"}}};

  cout << ini;
}

void filestream_demo(){
  std::ifstream f_in("./files_ini/simple.ini");
  std::ofstream f_out("./files_ini/out.ini");
  inirw::IniRW cfg;

  f_in >> cfg;

  f_out << cfg;


}



int main(int argc, char* argv[])
{
  filestream_demo();

//  inirw::Field field;
//  cout << field;
//  cout << "field is:\r\n"; cin >> field;
//  cout<<field;





  return 0;
}
