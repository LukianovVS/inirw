#include <iostream>

#include "inirw.h"


using std::cout, std::cin, std::endl;


template <typename T>
T myfunc(const std::string & s){
  std::stringstream ss(s);
  T t;
  ss >> t;
  return t;
}



int main(int argc, char* argv[])
{
//  inirw::IniRW config;
//
//    config["sec"] = {{"a", "1"}, {"b", "2"}};

//  inirw::Section sec = config["first"];
//
//  sec = {{"a", "1"}, {"b", "2"}};

//  config.view();

//

//  std::stringstream ss;
//
//  int a = 10;
//
//  cout << "a = "; cin >> a;
//  ss << a;
//  std::string s;
//  ss >> s;
//  cout << "s = " << s << endl;

//  int a;
//  a = myfunc<int>("10");
//  cout << "a = " << a << endl;
//  return 0;
  inirw::Field f;

  f["name"] = "1";

  f = "10";
  cout << f << endl;

  f = 12;
  cout << f << endl;

  int i = f.get<int>();
  cout << "i+10 = " << i+10 << endl;



  return 0;
}
