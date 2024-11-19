#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <sstream>


namespace inirw{


template <typename T> class MapOrder{
  std::vector<std::string> m_order;
  std::map<std::string, T> m_data;

public:

  void addEmpty(const std::string& key){
   m_order.push_back(key);
   m_data[key] = T();
  }

  T& operator[](const std::string& key){
  auto it = m_data.find(key);
  if(it == m_data.end()){
    this->addEmpty(key);
  }

  return m_data[key];
}


};

//! it is a single line of the file, contains a pair: name and value
//! <n_name>=<m_val>
class Field{
  std::string m_name;
  std::string m_val;

public:

  Field() = default;
  Field(std::string name, std::string val) : m_name(name), m_val(val) {}

  ~Field() = default;

  //! set name of field
  Field& operator[](const std::string& name){
    m_name = name;
    return *this;
  }

  //! set value of field
  Field& operator=(const std::string& val){
    m_val = val;
    return *this;
  }

  //! get value
  template <typename T>
  T operator()() const{
    T tmp;
    std::stringstream ss(m_val);
    ss >> tmp;
    return tmp;
  }

  template <typename T>
  void operator()(T& val){
    std::stringstream ss(m_val);
    ss >> val;
  }

//  std::string operator()() const{
//    return m_val;
//  }



  friend std::ostream& operator<<(std::ostream& os, const Field& obj);
  friend std::istream& operator>>(std::istream& is,  Field& obj);
};

std::ostream& operator<<(std::ostream& os, const Field& obj){
  return os << obj.m_name << "=" << obj.m_val << std::endl;
}

std::istream& operator>>(std::istream& is, Field& obj){
  return is >> obj.m_val;
}


class Section : public MapOrder<Field>
{


public:

  Section() = default;
  ~Section() = default;

  Section& operator=(std::initializer_list<std::pair<std::string, std::string>> lst){
      for(auto& m: lst){
        std::cout << m.first << " - " << m.second << std::endl;
      }
      return *this;
  }

//  template<T>& operator[](std::string name){
//
//
//  }

};



class IniRW{

  std::vector<std::string> m_order;
  std::map<std::string, Section> m_section;

  void addSection(const std::string& section_name){
     m_order.push_back(section_name);
     m_section[section_name] = Section();
  }

  public:
    IniRW() = default;

    ~IniRW() = default;

//
//    const Section& operator[](std::string section_name) const {
//      return &m_section[section_name];
//    }

    Section& operator[](const std::string& section_name){
      auto it = m_section.find(section_name);
      if(it == m_section.end()){
        this->addSection(section_name);
      }

      return m_section[section_name];
    }




    void view(){
      for (const auto& [key, value] : m_section)
        std::cout << "[" << key << "]" << "; " << std::endl;

    }


};


}

