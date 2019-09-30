// Copyright 2019 Bianca Ciuche
#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <list>
#include <iterator>
#include <vector>
#include <string>
#include <limits>

// Hash Function
int string_hash(std::string str) {
  unsigned int hash = 5381;

  for (std::string ::iterator it = str.begin(); it != str.end(); ++it) {
    hash = ((hash << 5) + hash) + *it;
  }

  return hash % std::numeric_limits<int> :: max();
}

// node info
template <typename Tkey, typename Tvalue>
struct info {
  Tkey key;
  Tvalue value;
};

// Hashtable
template <typename Tkey, typename Tvalue>
class Hashtable {
 private:
  std::list< struct info <Tkey, Tvalue> > *H;
  int size;
  int capacity;
  int (*hash)(Tkey);

 public:
  explicit Hashtable(int capacity = 1000 , int (*h)(Tkey) = string_hash) {
    this->capacity = capacity;
    hash = h;
    size = 0;
    H = new std::list< struct info <Tkey, Tvalue> > [capacity];
  }

  // Destructor
  ~Hashtable() {
    for (int i = 0; i < capacity; ++i) {
      if (!H[i].empty()) {
        H[i].clear();
      }
    }
    if (H)
      delete[] H;
  }

  void put_capacity(int capacity) {
    this->capacity = capacity;
  }

  void resize() {
    std::list< struct info <Tkey, Tvalue> > *H2;
    capacity *= 2;
    H2 = new std::list< struct info <Tkey, Tvalue> > [capacity/2];
    for (int i = 0; i < capacity/2; ++i) {
      while (H[i].begin() != H[i].end()) {
        H2[i].push_back(H[i].front);
        H[i].pop_front();
      }
    }
    delete[] H;
    H = new std::list< struct info <Tkey, Tvalue> > [capacity];
    for (int i = 0; i < capacity/2; ++i) {
      while (H2[i].begin() != H2[i].end()) {
        put(H2[i].front(), H2[i].front());
        H2[i].pop_front();
      }
    }
    delete[] H2;
  }

  // get hashtable keys
  std::vector<Tkey> get_keys() {
    std::vector<Tkey> l;
    for (int i = 0;  i < capacity; ++i) {
      for (auto j = H[i].begin(); j != H[i].end(); ++j) {
        l.push_back(j->key);
      }
    }
    return l;
  }

  // add/update an element
  void put(Tkey key, Tvalue value) {
    int index = hash(key) % capacity;
    bool switch_flag = false;

    for (auto it = H[index].begin(); it != H[index].end(); ++it) {
      // if the key exists, update its value
      if (it->key == key) {
        it->value = value;
        switch_flag = true;
      }
    }
    if (!switch_flag) {
      // if the key wasn't found add it
        struct info<Tkey, Tvalue> a;
        a.value = value;
        a.key = key;
        H[index].push_back(a);
        size++;
    }
  }

  // remove an elem from the hashtable
  void remove(Tkey key) {
    int index = hash(key) % capacity;

    typename std::list <struct info<Tkey, Tvalue> > :: iterator it;
    for (it = H[index].begin(); it != H[index].end(); ++it) {
      if (it->key == key) {
        break;
      }
    }
    if (it != H[index].end()) {
      it = H[index].erase(it);
    }
    size--;
  }

  Tvalue get(Tkey key) {
    int index = hash(key) % capacity;
    for (auto it = H[index].begin(); it != H[index].end(); ++it) {
      if (it->key == key) {
        return (it->value);
      }
    }
    return Tvalue();
  }

  // removes all elem
  void clear() {
    size = 0;
    for (int i = 0; i < capacity; ++i) {
      if (!H[i].empty()) {
        H[i].clear();
      }
    }
  }

  // verifies if the key exists
  bool has_key(Tkey key) {
    int index = hash(key) % capacity;
    for (auto it = H[index].begin(); it != H[index].end(); ++it) {
      if (it->key == key) {
        return true;
      }
    }
    return false;
  }

  int get_size() {
    return size;
  }

  int get_capacity() {
    return capacity;
  }
};

#endif  // HASHTABLE_H_
