#include <iostream>

#include "inirw.h"


using std::cout, std::cin, std::endl;


#include <vector>
#include <map>

//struct C{
//  std::string name;
//  std::string val;
//  std::string tmp_name = "";
//};
//
//
//struct B{
//  std::string name;
//  std::vector<C> v;
//
//  C& operator[](std::string name){
//      for(C& c: v){
//        if(c.name == name)
//          return c;
//      }
//
//    }
//
//    B& operator=(std::string name, std::string val){
//      C c={name, val};
//      v.push_back(c);
//    }
//
//};
//
//struct A{
//    std::vector<B> v;
//
//    B& operator[](std::string name){
//      for(B& b: v){
//        if(b.name == name)
//          return b;
//      }
//    }
//
//
//
//};


namespace nsp{

class Mc{
  int a = 10;
public:
  friend std::ostream& operator<<(std::ostream& out, const Mc& obj);
};

std::ostream& operator<<(std::ostream& out, const Mc& obj){
  return out << "a = " << obj.a << endl;
}
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
  inirw::Field f;

  f["name"] = "1";

  cout << "val = ";
  cin >> f;

  cout << f << endl;

  std::string qwe = f();

  cout << "qwe = " << qwe << endl;


  return 0;
}
