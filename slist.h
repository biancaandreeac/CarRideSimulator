// Copyright 2019 Bianca Ciuche
#ifndef SLIST_H_
#define SLIST_H_

#include <list>
#include <utility>
#include <string>

template<typename T>
class Slist {
 public:
  std::list<std::pair <T, std::string> > v;
  void add_elem(std::pair<T, std::string> elem);
  void update_elem(std::pair<T, std::string> elem);
  void print_n(int n, std::ofstream& fout);
  void print_precision(int n, std::ofstream& fout);
};

// add an element to the list
template<typename T>
void Slist<T>::add_elem(std::pair<T, std::string> elem) {
  if (v.empty()) {
    v.push_back(elem);
    return;
  }

  typename std::list<std::pair <T, std::string> > :: iterator i;
  for (i = v.begin(); i != v.end(); ++i) {
    if (i->first == elem.first) {
      // if there are more elements with the same value
      // arrange them after name
      if (i->second > elem.second) {
          v.insert(i, elem);
          return;
      }
    } else if (i->first < elem.first) {
      v.insert(i, elem);
      return;
    }
  }

  v.push_back(elem);
}

// update the position and value of an existing element
template<typename T>
void Slist<T>::update_elem(std::pair<T, std::string> elem) {
  typename std::list<std::pair <T, std::string> > :: iterator i;
  for (i = v.begin(); i != v.end(); ++i) {
    if (i->second == elem.second) {
        v.erase(i);
        break;
    }
  }

  add_elem(elem);
}

// print first n elements with a precision
template<typename T>
void Slist<T>::print_precision(int n, std::ofstream& fout) {
    int count = 0;
    for (auto i = v.begin(); i != v.end(); ++i) {
        count++;
        fout << i->second << ":";
        fout << std::setprecision(3) << std::fixed << i->first << " ";
        if (count == n) {
            break;
        }
    }
    fout << "\n";
}

// print first n elements without precision
template<typename T>
void Slist<T>::print_n(int n, std::ofstream& fout) {
    int count = 0;
    for (auto i = v.begin(); i != v.end(); ++i) {
        count++;
        fout << i->second << ":";
        fout << i->first << " ";
        if (count == n) {
            break;
        }
    }
    fout << "\n";
}

#endif  // SLIST_H_
