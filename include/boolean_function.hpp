// Copyright 2019 Your Name <your_email>

#ifndef INCLUDE_BOOLEAN_FUNCTION_
#define INCLUDE_BOOLEAN_FUNCTION_

#include <string>
#include <vector>
using std::initializer_list;
using std::string;
using std::vector;

class boolean_function {
 public:
 private:
  vector<bool> data;

  using value_type = bool;
  using size_type = size_t;
  using reference = std::vector<value_type>::reference;
  using const_reference = std::vector<value_type>::const_reference;

  using iterator = typename std::vector<value_type>::iterator;
  using const_iterator = typename std::vector<value_type>::const_iterator;

 public:
  // Cоздаёт функцию от dimension переменных, равную переменной Xn
  // Пример 1 (n = 0, dimension = 3):
  // вернёт "01010101"
  // пример 2 (n = 1, dimension = 4):
  // вернёт "0011001100110011"
  static boolean_function var(size_t n, size_t dimension) {
    boolean_function b;
    dimension = 1 << dimension;
    b.data.resize(dimension);
    size_t t = 0;
    bool k = false;
    for (size_t i = 0; i < dimension; ++i) {
      b.data[i] = k;
      if (t == n) {
        t = 0;
        k = !k;
      } else {
        t++;
      }
    }
    return b;
  }
  // Тождественный ноль "от dimension переменных"
  static boolean_function zero(size_t dimension) {
    boolean_function zer;
    dimension = 1 << dimension;
    zer.data.resize(dimension);
    for (size_t i = 0; i < dimension; ++i) {
      zer.data[i] = false;
    }
    return zer;
  }

  // Тождественная единица "от dimension переменных"
  static boolean_function one(size_t dimension) {
    boolean_function on;
    dimension = 1 << dimension;
    on.data.resize(dimension);
    for (size_t i = 0; i < dimension; ++i) {
      on.data[i] = true;
    }
    return on;
  }

  static boolean_function from_anf(std::vector<value_type> v) {
    boolean_function a(v);
    for (size_t i = 0; i < v.size(); ++i) {
      a.data[i] = v[0];
      for (size_t j = 0; j < v.size() - i; ++j) {
        v[j] = v[j] ^ v[j + 1];
      }
    }
    return a;
  }

  boolean_function() { data.resize(0); }

  // Создаёт функцию тождественного нуля от n переменных
  explicit boolean_function(size_t n) {
    n = 1 << n;
    data.resize(n);
    for (size_t i = 0; i < n; ++i) {
      data[i] = false;
    }
  }

  // Создаёт функцию от n переменных. Вектор значений представляется числом
  // value Пример: пусть value = 14, т.е. 0...00001110 в двоичной системе а n =
  // 3 тогда АНФ boolean_function будет равна x + y + xy + zx + zy + zyx
  boolean_function(size_t value, size_type n) {
    n = 1 << n;
    data.resize(n);
    for (size_t i = 0; i < n; ++i) {
      if (static_cast<int>(value) >= (1 << (n - i - 1))) {
        data[i] = true;
        value -= 1 << (n - i - 1);
      }
    }
  }

  // Создаёт функцию из строки, содержащей вектор значений
  // пусть table = "01110000"
  // тогда АНФ boolean_function будет равна x + y + xy + zx + zy + zyx
  explicit boolean_function(const string& table) {
    size_t n = table.size();
    data.resize(n);
    for (size_t i = 0; i < n; ++i) {
      if (table[i] == '1')
        data[i] = true;
      else
        data[i] = false;
    }
  }

  // Создаёт функцию из вектора значений
  // пусть table = {true, false, false, true};
  // тогда АНФ boolean_function будет равна x + y + 1
  explicit boolean_function(const vector<value_type>& table) {
    size_t n = table.size();
    data.resize(n);
    for (size_t i = 0; i < n; i++) {
      data[i] = static_cast<bool>(table[i]);
    }
  }
  explicit boolean_function(const initializer_list<bool> vars) {
    size_t n = vars.size(), t = 0;
    data.resize(n);
    for (auto i : vars) {
      data[t] = i;
      t++;
    }
  }

  boolean_function(const boolean_function& table) {
    size_t n = table.data.size();
    data.resize(n);
    for (size_t i = 0; i < n; ++i) {
      data[i] = table.data[i];
    }
  }

  boolean_function& operator=(const boolean_function& rhs) {
    size_t n = rhs.data.size();
    data.resize(n);
    for (size_t i = 0; i < n; ++i) {
      data[i] = rhs.data[i];
    }
    return *this;
  }

  // Сложение по модулю 2
  // если разное количество переменных - исключение
  boolean_function& operator+=(const boolean_function& rhs) {
    if (data.size() != rhs.data.size()) throw std::invalid_argument("Error");
    for (size_t i = 0; i < data.size(); ++i) {
      data[i] = (data[i] + rhs.data[i]) % 2;
    }
    return *this;
  }

  // Конъюнкция
  // если разное количество переменных - исключение
  boolean_function& operator*=(const boolean_function& rhs) {
    if (data.size() != rhs.data.size()) throw std::invalid_argument("Error");
    for (size_t i = 0; i < data.size(); ++i) {
      data[i] = data[i] & rhs.data[i];
    }
    return *this;
  }

  // Дизъюнкция
  // если разное количество переменных - исключение
  boolean_function& operator|=(const boolean_function& rhs) {
    if (data.size() != rhs.data.size()) throw std::invalid_argument("Error");
    for (size_t i = 0; i < data.size(); ++i) {
      data[i] = data[i] | rhs.data[i];
    }
    return *this;
  }

  // Побитовое отрицание
  boolean_function operator~() {
    for (auto&& i : data) {
      i = !i;
    }
    return *this;
  }

  // true если функции равны
  // иначе false
  bool operator==(const boolean_function& rhs) const {
    if (data.size() != rhs.data.size()) return false;
    for (size_t i = 0; i < data.size(); ++i) {
      if (data[i] != rhs.data[i]) return false;
    }
    return true;
  }

  // true если одна функция сравнима и больше или равна rhs
  // false если сравнима и меньше rhs
  // logic_error если фунции не сравнимы
  bool operator==(const vector<value_type>& rhs) const {
    if (data.size() != rhs.size()) return false;
    for (size_t i = 0; i < data.size(); ++i) {
      if (data[i] != rhs[i]) return false;
    }
    return true;
  }

  size_t weight() {
    size_t n = 0;
    for (size_t i = 0; i < size(); ++i) {
      if (data[i] == 1) n++;
    }
    return n;
  }
  cbool operator>=(const boolean_function& rhs) const {
    if (size() != rhs.size()) throw std::logic_error("Error");
    if (weight() > rhs.weight()) {
      for (size_t i = 0; i < size(); ++i) {
        if (data[i] == 0 && rhs.data[i] == 1) throw std::logic_error("Error");
      }
    } else {
      for (size_t i = 0; i < size(); ++i) {
        if (data[i] == 1 && rhs.data[i] == 0) throw std::logic_error("Error");
      }
    }
    for (size_t i = 0; i < size(); ++i) {
      if (data[i] < rhs.data[i]) return false;
    }
    return true;
  }

  reference operator[](size_type ind) { return data[ind]; }

  const_reference operator[](size_type ind) const { return data[ind]; }

  reference at(size_type ind) { return data.at(ind); }

  const_reference at(size_type ind) const { return data.at(ind); }
  iterator begin() { return data.begin(); }

  iterator end() { return data.end(); }

  const_iterator cbegin() const { return data.begin(); }

  const_iterator cend() const { return data.end(); }
  // Длина столбца функции
  size_type size() const noexcept { return data.size(); }

  // Количество переменных
  size_type dimension() const noexcept {
    size_t n = data.size();
    size_t i = 0;
    while (n >> i != 1) i++;
    return i;
  }

  // Возвращает значение функции на наборе значений переменных
  // пусть boolean_function задает функцию f(x, y, z)
  // тогда функция вернет f(vars[0], vars[1], vars[2])
  bool operator()(const vector<bool>& vars) const {
    size_t n = 0;
    for (size_t i = 0; i < vars.size(); ++i) {
      n += vars[vars.size() - i - 1] << i;
    }
    return data[n];
  }

  bool operator()(const initializer_list<bool> vars) const {
    size_t n = 0, k = vars.size() - 1;
    for (auto i : vars) {
      n += i << k;
      k--;
    }
    return data[n];
  }

  // T(x1, x2, ..., xN) - текущая функция
  // operator вернет новую функцию, которая равна композиции G = T(fs[0], fs[1],
  // ..., fs[N-1])
  boolean_function operator()(const vector<boolean_function>& fs) const {
    boolean_function a(*this), b(a.dimension());
    vector<bool> k;
    for (size_t i = 0; i < a.size(); ++i) {
      k.clear();
      for (size_t j = 0; j < fs.size(); ++j) {
        k.push_back(fs[j].data[i]);
      }
      b[i] = a(k);
    }
    return b;
  }

  boolean_function operator()(
      const initializer_list<boolean_function> vars) const {
    boolean_function a(*this), b(a.dimension());
    vector<bool> k;
    for (size_t i = 0; i < a.size(); ++i) {
      k.clear();
      for (auto j : vars) {
        k.push_back(j.data[i]);
      }
      b[i] = a(k);
    }
    return b;
  }

  // монотонная
  bool is_monotone() const {
    boolean_function a(*this);
    for (size_t i = 0; i < a.size(); ++i) {
      for (size_t j = i + 1; j < a.size(); ++j) {
        if (a[i] > a[j]) {
          int t = 0, k = 0;
          boolean_function z(i, dimension()), y(j, dimension());
          while (z.data[i] > y.data[i]) {
            t++;
          }
          while (z.data[j] < y.data[j]) {
            k++;
          }
          if (i != size() && j != size()) return false;
        }
      }
    }
    return true;
  }
  // самодвойственная
  bool is_selfdual() const {
    for (size_t i = 0; i < data.size() / 2; ++i) {
      if (data[i] == data[data.size() - i]) return false;
    }
    return true;
  }
  // линейная
  bool is_linear() const {
    if (!is_balanced()) return false;
    size_t n = 0;
    for (size_t i = 0; i < size(); ++i) {
      if (data[i] == 1 && i != n) return false;
      if (n <= i) n = n << 1;
    }
    return true;
  }
  bool is_T1() const { return data[data.size()] == 1; }

  bool is_T0() const { return data[0] == 0; }

  bool is_balanced() const { return (data.size() / 2) == weight(); }

  bool is_symmetric() const {
    for (size_t i = 0; i < data.size() / 2; ++i) {
      if (data[i] != data[data.size() - i]) return false;
    }
    return true;
  }

  size_t weight() const {
    size_t t = 0;
    for (bool i : data) {
      if (i) t++;
    }
    return t;
  }

  bool is_functionally_complete_system() const {
    if (is_T0()) return false;
    if (is_T1()) return false;
    if (is_linear()) return false;
    if (is_selfdual()) return false;
    if (is_monotone()) return false;
    return true;
  }

  // Возвращает АНФ функции
  vector<value_type> anf() const {
    boolean_function b(*this);
    std::vector<value_type> a(size());
    size_t n = size();
    for (size_t i = 0; i < n; ++i) {
      a[i] = b.data[0];
      for (size_t j = 0; j < n - i; ++j) {
        b.data[j] = b.data[j] ^ b.data[j + 1];
      }
    }
    return a;
  }
};

// Пусть boolean_function задаётся вектором значений "01110000"
// тогда get_polynom вернет строку "x0 + x1 + x0x1 + x0x2 + x1x2 + x0x1x2"
string get_polynom(const boolean_function& a) {
  vector<bool> k = a.anf();
  string st;
  for (size_t i = 0; i < a.size(); ++i) {
    if (a[i] == 1) {
      size_t t = i;

      for (size_t j = 0; j < a.dimension(); ++j) {
        if (static_cast<int>(t) >= (1 << (a.dimension() - j))) {
          st += "x";
          st += a.dimension() - j;
          t -= 1 << (a.dimension() - j);
        }
      }
      st += " + ";
    }
  }
  return st;
}

boolean_function operator+(const boolean_function& a,
                           const boolean_function& b) {
  boolean_function c = a;
  c += b;
  return c;
}

boolean_function operator*(const boolean_function& a,
                           const boolean_function& b) {
  boolean_function c = a;
  c *= b;
  return c;
}

boolean_function operator|(const boolean_function& a,
                           const boolean_function& b) {
  boolean_function c = a;
  c |= b;
  return c;
}

bool operator!=(const boolean_function& a, const boolean_function& b) {
  return !(a == b);
}

bool is_functionally_complete_system(const vector<boolean_function>& fs) {
  size_t n = 0;
  for (const auto& f : fs) {
    if (f.is_T0()) n++;
  }
  if (n == fs.size()) return false;
  n = 0;
  for (const auto& f : fs) {
    if (f.is_T1()) n++;
  }
  if (n == fs.size()) return false;
  n = 0;
  for (const auto& f : fs) {
    if (f.is_selfdual()) n++;
  }
  if (n == fs.size()) return false;
  n = 0;
  for (const auto& f : fs) {
    if (f.is_monotone()) n++;
  }
  if (n == fs.size()) return false;
  n = 0;
  for (const auto& f : fs) {
    if (f.is_linear()) n++;
  }
  if (n == fs.size()) return false;
  return true;
}

bool is_functionally_complete_system(
    const initializer_list<boolean_function> vars) {
  size_t n = 0;
  for (const auto& f : vars) {
    if (f.is_T0()) n++;
  }
  if (n == vars.size()) return false;
  n = 0;
  for (const auto& f : vars) {
    if (f.is_T1()) n++;
  }
  if (n == vars.size()) return false;
  n = 0;
  for (const auto& f : vars) {
    if (f.is_selfdual()) n++;
  }
  if (n == vars.size()) return false;
  n = 0;
  for (const auto& f : vars) {
    if (f.is_monotone()) n++;
  }
  if (n == vars.size()) return false;
  n = 0;
  for (const auto& f : vars) {
    if (f.is_linear()) n++;
  }
  if (n == vars.size()) return false;
  return true;
}

#endif  // INCLUDE_BOOLEAN_FUNCTION_HPP_
