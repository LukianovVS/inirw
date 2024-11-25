#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <sstream>


namespace inirw{


using Field_t = std::pair<std::string, std::string>;
using Section_t = std::pair<std::string, std::initializer_list<Field_t>>;


enum class TypeLine{
  EmptyL,
  SectionL,
  FieldL,

};

class StringIni: public std::string {

public:

  void split_field(std::string &key, std::string& val, const std::string& delimiter) const{
    size_t pos = this->find(delimiter);
    // TODO check Exception
    key = this->substr(0, pos);
    val = this->substr(pos+delimiter.length());
  }


  TypeLine detectTypeLine() const{
    if(this->length() == 0)
      return TypeLine::EmptyL;
    else if((*this)[0] == '[')
      return TypeLine::SectionL;
    else if(this->find("=") != this->npos)
      return TypeLine::FieldL;
    // ERROR
    return TypeLine::EmptyL;

  }


  std::string getSectionName() const{
    std::size_t pos_beg = this->find("[") + 1;
    std::size_t pos_end = this->find("]");
    return this->substr(pos_beg, pos_end - pos_beg);
  }

  void getKeyVal(std::string& key, std::string& val) const{
    split_field(key, val, "=");
  }


};


template <typename T> class MapOrder{
protected:
  std::vector<std::string> m_order;
  std::map<std::string, T> m_data;

public:

  T& addEmpty(const std::string& key){
    m_order.push_back(key);
    return m_data[key] = T(key);
  }



  bool is_key_exist(const std::string& key){
    auto it = m_data.find(key);
    if(it == m_data.end())
       return false;
    else
      return true;
  }

  void clear(){
    // TODO DTOR, check alloc memory
    m_order.clear();
    m_data.clear();
  }

  void erase(const std::string& key){
    for (std::vector<std::string>::iterator it = m_order.begin(); it != m_order.end(); ++it){
      if(*it == key){
        m_order.erase(it);
        m_data.erase(key);
      }
    }
  }

};

//! it is a single line of the file, contains a pair: name and value
//! <n_name>=<m_val>
class Field{
  std::string m_name;
  std::string m_val;

  void split(std::string &key, std::string& val, std::string& line, const std::string& delimiter){
    size_t pos = line.find(delimiter);
    // TODO check Exception
    key = line.substr(0, pos);
    val = line.substr(pos+delimiter.length());
  }

public:

  Field() = default;
  Field(std::string name) : m_name(name) {}
  Field(std::string name, std::string val) : m_name(name), m_val(val) {}

  ~Field() = default;

  //! set name of field
//  Field& operator[](const std::string& name){
//    m_name = name;
//    return *this;
//  }
  //! set name and value
  Field& operator=(const Field_t& nameval){
    m_name = nameval.first;
    m_val  = nameval.second;
    return *this;
  }

  //! set value of field
  template <typename T>
  Field& operator=(const T& val){
    std::stringstream ss;
    ss << val;
    ss >> m_val;
    return *this;
  }

  Field& operator=(const std::string& val){
    m_val = val;
    return *this;
  }


  //! get value
  template <class ValOut>
  ValOut get() const{
    ValOut tmp;
    std::stringstream ss(m_val);
    ss >> tmp;
    return tmp;
  }


//  template <class ValOut>
//  ValOut operator()() const{
//    ValOut tmp;
//    std::stringstream ss(m_val);
//    ss >> tmp;
//    return tmp;
//  }



  friend std::ostream& operator<<(std::ostream& os, const Field& obj);
  friend std::istream& operator>>(std::istream& is,  Field& obj);
};


std::ostream& operator<<(std::ostream& os, const Field& obj){
  return os << obj.m_name << "=" << obj.m_val << std::endl;
}

std::istream& operator>>(std::istream& is, Field& obj){
  std::string line;
  is >> line;
  obj.split(obj.m_name, obj.m_val, line, "=");
  return is;
}

//-----------------------------------------------------------------------------------
// Section
//-----------------------------------------------------------------------------------


class Section : private MapOrder<Field>
{

  std::string m_name="";

  void updateData(const std::string& key, const std::string& val){
    if(is_key_exist(key)){
      erase(key);
      Field& t = addEmpty(key);
      t = Field_t(key, val);
    }
    else{
      Field& t = m_data[key];
      t = Field_t(key, val);
    }
  }

public:

  Section() = default;
  Section(std::string section_name) : m_name(section_name) {}

  ~Section() = default;


  Section& operator=(std::initializer_list<Field_t> lst){
      clear();
      for(auto& m: lst){
        (*this)+=m;
      }
      return *this;
  }

  Section& operator+=(Field_t nameval){
    updateData(nameval.first, nameval.second);
    return *this;
  }

  Section& operator+=(const StringIni& line){
    std::string key, val;
    line.getKeyVal(key, val);
    updateData(key, val);
    return *this;
  }




  friend std::ostream& operator<<(std::ostream& os, const Section& obj);
//  friend std::istream& operator>>(std::istream& is, Section& obj);

};

std::ostream& operator<<(std::ostream& os, const Section& obj){
  os << "[" << obj.m_name << "]" << std::endl;
  for(auto& [key, val]: obj.m_data){
    os << val;
  }
  return os;
}





class IniRW: protected MapOrder<Section> {


  public:
    IniRW() = default;
    IniRW(std::initializer_list<Section_t> section_lst){
      (*this) = section_lst;
    }


    ~IniRW() = default;

    IniRW& operator=(std::initializer_list<Section_t> section_lst){
      clear(); // TODO: double deletion...
      for(auto section_cur: section_lst)
        (*this)+=section_cur;
      return *this;
    }

    IniRW& operator+=(Section_t section_add){
      const std::string& section_name = section_add.first;
      erase(section_name);
      Section& section_new = addEmpty(section_name);
      section_new = section_add.second;
      return *this;
    }



    friend std::ostream& operator<<(std::ostream& os, IniRW& obj);
    friend std::istream& operator>>(std::istream& is, IniRW& obj);

};

std::ostream& operator<<(std::ostream& os, IniRW& obj){
  for(auto& section_name: obj.m_order){
    os << obj.m_data[section_name];
  }
  return os;
}

std::istream& operator>>(std::istream& is, IniRW& obj){
  StringIni line;
  Section* section = nullptr;
  size_t line_num = 0;
  while(getline(is, line)){
    ++line_num;
    TypeLine typeLine = line.detectTypeLine();
    switch(typeLine){
      case TypeLine::SectionL:{
        std::string section_name = line.getSectionName();
        section = &obj.addEmpty(section_name);
        break;
      }
      case TypeLine::FieldL:{
        if(!section){
          std::cerr << "Error parser. Not define Section. Line number " << line_num << std::endl;
          return is; // TODO Exception;
        }
        (*section) += line;
        break;
      }
      case TypeLine::EmptyL: continue;
    }
  }
  return is;
}


}

