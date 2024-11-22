#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <sstream>


namespace inirw{


using Field_t = std::pair<std::string, std::string>;
using Section_t = std::pair<std::string, std::initializer_list<Field_t>>;


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
  return is >> obj.m_val;
}

//-----------------------------------------------------------------------------------
// Section
//-----------------------------------------------------------------------------------


class Section : private MapOrder<Field>
{

  std::string m_name="";

  void updateData(const std::string& key, const std::string& val){
    if(is_key_exist(key)){
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

  Section& operator-=(const std::string& name){
    erase(name);
    return *this;
  }




  friend std::ostream& operator<<(std::ostream& os, const Section& obj);

};


std::ostream& operator<<(std::ostream& os, const Section& obj){
  os << "[" << obj.m_name << "]" << std::endl;
  for(auto& [key, val]: obj.m_data){
//    std::cout << "k = " << key << "; v = "<< val << std::endl;
    os << val;
  }
  return os;
}




class IniRW: protected MapOrder<Section> {


  public:
    IniRW() = default;

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

    friend std::ostream& operator<<(std::ostream& os,  IniRW& obj);


};

std::ostream& operator<<(std::ostream& os, IniRW& obj){
  for(auto& section_name: obj.m_order){
    os << obj.m_data[section_name];
  }
  return os;
}


}

