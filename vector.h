#ifndef VECTOR_H
#define VECTOR_H

#include<initializer_list>
#include<iostream>
#include<stdexcept>

template <typename T>
class Vector {
public:
  class ConstIterator;
  class Iterator;
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = Vector::Iterator;
  using const_iterator = Vector::ConstIterator;
private:
	// Instanzvariablen
  size_t sz;
  size_t max_sz;
  pointer values;
  
  void allocate(size_t new_max_sz){
  	if(new_max_sz < this->sz){
  		return;
  	}
  	pointer help{new value_type[new_max_sz]};
  	for(size_t i{0}; i < this->sz; ++i){
  		help[i] = values[i];
  	}
  	delete[] values;
  	max_sz = new_max_sz;
  	values = help;
  }
public:
  // Methoden
  Vector() : sz{0}, max_sz{5}, values{new value_type[max_sz]} { //Liefert einen leeren Vector. DONE
  }
  Vector(const Vector& copy) : sz{copy.sz}, max_sz{copy.max_sz}, values{new value_type[max_sz]} {  //Liefert einen Vector mit demselben Inhalt wie der Parameter. DONE
    for(size_t i{0}; i < copy.sz; ++i) {
      this->values[i] = copy.values[i];
    }
  }
  Vector(size_t n) : sz{0}, max_sz{n}, values{new value_type[max_sz]} {  //Erzeugt einen leeren Vector mit Platz für n Elemente. DONE
    this->allocate(n);
  }
  Vector(std::initializer_list<value_type> l) : sz{0}, max_sz{l.size()}, values{new value_type[max_sz]} {  //Erzeugt einen Vector mit dem spezifizierten Inhalt.
  	for(auto it{l.begin()}; it != l.end(); ++it) {
      this->push_back(*it);
    }
  }
  Vector& operator=(Vector other){
  	std::swap(this->sz, other.sz);
  	std::swap(this->max_sz,other.max_sz);
  	std::swap(this->values, other.values);
  	return *this;
  }
  ~Vector() {
    delete[] values;
  }

  class Iterator { // iterator absichern, dass man nicht über die grenzen des arrays kommen kann. ist erlaubt und hilfreich für den test
    public:
      using value_type = Vector::value_type;
      using reference = Vector::reference;
      using pointer = Vector::pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
      // Instanzvariablen
      Vector *container;
      pointer ptr; //Zeigt auf das entsprechende Element im Vector.
    public:
      // Methoden
      Iterator() : container {nullptr}, ptr{nullptr} {
      }

      Iterator(Vector* v, pointer new_ptr) : container{v}, ptr{new_ptr} { //Erzeugt einen Iterator, bei dem die Instanzvariable ptr auf den Wert des Parameters gesetzt wird.
      }

      Iterator(pointer p) : container{nullptr}, ptr{p} {
      }

      reference operator*() const { //*it liefert Element
        if(container == nullptr) {
          throw std::runtime_error{"Iterator out of range"};
        }
        if(ptr < container->values || ptr >= container->values + container->sz) {
          throw std::runtime_error{"Iterator out of range"};
        }
        return *ptr;
      }

      pointer operator->() const { //in vector<Person> it->get_Name()
        if(container == nullptr) {
          throw std::runtime_error{"Iterator out of range"};
        }
        if(ptr < container->values || ptr >= container->values + container->sz) {
          throw std::runtime_error{"Iterator out of range"};
        }
        return ptr;
      }

      friend bool operator==(const Vector::Iterator& lop, const Vector::Iterator& rop) {
        return lop.ptr == rop.ptr;
      }

      friend bool operator!=(const Vector::Iterator& lop, const Vector::Iterator& rop) {
        return !(lop == rop);
      }

      Iterator& operator++() { //prefix ++it
        if(container == nullptr) {
          return *this;
        }
        if(ptr >= container->values && ptr < container->values + container->sz) {
          ++ptr;
        }
        return *this;
      }

      Iterator operator++(int) { //postfix it++
        Iterator old{*this};
        operator++();
        return old;
      }

      operator ConstIterator() const { //Vector::Iterator it{v.begin()}; --> Vector::ConstIterator cit{static_cast<ConstIterator>(it)}
        return ConstIterator{container, ptr};
      }
  };

  class ConstIterator {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::const_reference;
      using pointer = Vector::const_pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
      // Instanzvariablen
      const Vector *container;
      pointer ptr;
    public:
      // Methoden
      ConstIterator() : container{nullptr}, ptr{nullptr} {
      }

      ConstIterator(const Vector* v, pointer new_ptr) : container{v}, ptr{new_ptr} { //Erzeugt einen Iterator, bei dem die Instanzvariable ptr auf den Wert des Parameters gesetzt wird.
      }

      ConstIterator(pointer p) : container{nullptr}, ptr{p} {
      }

      reference operator*() const {
        if(container == nullptr) {
          throw std::runtime_error{"Iterator out of range"};
        }
        if(ptr < container->values || ptr >= container->values + container->sz) {
          throw std::runtime_error{"Iterator out of range"};
        }
        return *ptr;
      }

      pointer operator->() const { //in vector<Person> it->get_Name()
        if(container == nullptr) {
          throw std::runtime_error{"Iterator out of range"};
        }
        if(ptr < container->values || ptr >= container->values + container->sz) {
          throw std::runtime_error{"Iterator out of range"};
        }
        return ptr;
      }

      friend bool operator==(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
        return lop.ptr == rop.ptr;
      }

      friend bool operator!=(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
        return !(lop == rop);
      }

      ConstIterator& operator++() { //prefix ++it
        if(container == nullptr) {
          return *this;
        }
        if(ptr >= container->values && ptr < container->values + container->sz) {
          ++ptr;
        }
        return *this;
      }

      ConstIterator operator++(int) { //postfix it++
        ConstIterator old{*this};
        operator++();
        return old;
      }
      friend difference_type operator-(const ConstIterator& lop, const ConstIterator& rop) {
        return lop.ptr - rop.ptr;
      }
  };

  size_t size() const { //Retourniert die Anzahl der aktuell gespeicherten Elemente. DONE
  	return this->sz;
  }

  bool empty() const {  //Retourniert true, wenn der Vector leer ist, false sonst. DONE
    if (this->sz == 0) {
      return true;
    } else {
      return false;
    }
  }

  void clear() {  //Löscht alle Elemente aus dem Vector. DONE
  	this->sz = 0;
  }

  void reserve(size_t n) {  //Wenn die Kapazität des Vectors nicht schon zumindest n ist, wird dieser entsprechend vergrößert. DONE
    if(this->max_sz < n) {
      this->allocate(n);
    }
  }

  void shrink_to_fit() {  //Kapazität des Vectors wird auf das erforderliche Mindestmaß (Anzah der aktuell gespeicherten Elemente) reduziert. DONE
  	this->allocate(this->sz);
    this->max_sz = this->sz;
  }

  void push_back(value_type x) {  //Eine Kopie von x wird zum Vector als letztes Element (am Ende) hinzugefügt. DONE
  	if(sz >= max_sz) {
  		allocate(2*max_sz+1);
  	}
  	values[sz] = x;
  	++sz;
  }

  void pop_back() { //Entfernt das letzte Element aus dem Vector. Wirft eine Exception vom Typ std::runtime_error, falls der Vector leer war. DONE
  	if(this->sz == 0) {
      throw std::runtime_error{"Vector is empty, nothing to delete!"};
    } else {
      --sz;
    }
  }

  value_type& operator[](size_t index) {  //Retourniert eine Referenz auf das Element an der spezifizierten Position (index). Ist der Index nicht im erlaubten Bereich,
  	if(index >= this->sz) {               //muss eine Exception vom Typ std::runtime_error geworfen werden. DONE
      throw std::runtime_error{"Index out of range!"};
    } else {
      return this->values[index];
    }
  }

  const value_type& operator[](size_t index) const {  //Retourniert eine Referenz auf das Element an der spezifizierten Position (index). Ist der Index nicht im erlaubten Bereich,
  	if(index >= this->sz) {                           //muss eine Exception vom Typ std::runtime_error geworfen werden. DONE
      throw std::runtime_error{"Index out of range!"};
    } else {
      return this->values[index];
    }
  }

  size_t capacity() const { //Retourniert die aktuelle Kapazität des Vectors. DONE
  	return this->max_sz;
  }

  friend std::ostream& operator<<(std::ostream& o, const Vector& v) { //[Element1, Element2, Element3] DONE
  	o << "[";
    bool first{true};
    for(size_t i{0}; i < v.sz; ++i) {
      if(first) {
        o << v.values[i];
        first = false;
      } else {
        o << ", " << v.values[i];
      }
    }
  	return o << "]";
  }

  Iterator begin() {
    return Iterator{this, values};
  }

  Iterator end() {
    return Iterator{this, values+sz};
  }

  ConstIterator begin() const {
    return ConstIterator{this, values};
  }

  ConstIterator end() const {
    return ConstIterator{this, values+sz};
  }
  
  Iterator insert(ConstIterator pos, const_reference val) {
    auto diff{pos - begin()};
    if(diff < 0 || static_cast<size_type>(diff) > sz) {
      throw std::runtime_error{"Iterator out of bounds"};
    }
    size_type current{static_cast<size_type>(diff)};
    if(sz >= max_sz) {
      reserve(max_sz*2+1);
    }
    for(auto i{sz}; i-->current;) {
      values[i+1] = values[i];
    }
    values[current] = val;
    ++sz;
    return Iterator{this, values+current};
  }

  Iterator erase(ConstIterator pos) {
    auto diff = pos-begin();
    if(diff < 0 || static_cast<size_type>(diff) > sz) {
      throw std::runtime_error{"Iterator out of bounds"};
    }
    size_type current{static_cast<size_type>(diff)};
    for(auto i{current}; i < sz-1; ++i) {
      values[i] = values[i+1];
    }
    --sz;
    return Iterator{this, values+current};
  }
};
#endif