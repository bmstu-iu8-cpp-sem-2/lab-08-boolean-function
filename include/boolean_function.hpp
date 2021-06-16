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
    if (data.size() != rhs.data.size()) throw std::invalid_argument("Fuck");
    for (size_t i = 0; i < data.size(); ++i) {
      data[i] = (data[i] + rhs.data[i]) % 2;
    }
    return *this;
  }

  // Конъюнкция
  // если разное количество переменных - исключение
  boolean_function& operator*=(const boolean_function& rhs);

  // Дизъюнкция
  // если разное количество переменных - исключение
  boolean_function& operator|=(const boolean_function& rhs);

  // Побитовое отрицание
  boolean_function operator~();

  // true если функции равны
  // иначе false
  bool operator==(const boolean_function& rhs) const;

  // true если одна функция сравнима и больше или равна rhs
  // false если сравнима и меньше rhs
  // logic_error если фунции не сравнимы
  bool operator>=(const boolean_function& rhs) const;

  reference operator[](size_type ind);
  const_reference operator[](size_type ind) const;

  reference at(size_type ind);
  const_reference at(size_type ind) const;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  // Длина столбца функции
  size_type size() const throw();

  // Количество переменных
  size_type dimension() const throw();

  // Возвращает значение функции на наборе значений переменных
  // пусть boolean_function задает функцию f(x, y, z)
  // тогда функция вернет f(vars[0], vars[1], vars[2])
  bool operator()(const std::vector<bool>& vars) const;
  bool operator()(const std::initializer_list<bool> vars) const;

  // T(x1, x2, ..., xN) - текущая функция
  // operator вернет новую функцию, которая равна композиции G = T(fs[0], fs[1],
  // ..., fs[N-1])
  boolean_function operator()(const std::vector<boolean_function>& fs) const;
  boolean_function operator()(
      const std::initializer_list<boolean_function> vars) const;

  bool is_monotone() const;   // монотонная
  bool is_selfdual() const;   // самодвойственная
  bool is_linear() const;     // линейная
  bool is_T1() const;         // сохраняет единицу
  bool is_T0() const;         // сохраняет ноль
  bool is_balanced() const;   // равновесная
  bool is_symmetric() const;  // симметричная
  size_t weight() const;

  bool is_functionally_complete_system() const;

  // Возвращает АНФ функции
  std::vector<value_type> anf() const;
};

// Пусть boolean_function задаётся вектором значений "01110000"
// тогда get_polynom вернет строку "x0 + x1 + x0x1 + x0x2 + x1x2 + x0x1x2"
std::string get_polynom(const boolean_function&);

boolean_function operator+(const boolean_function& a,
                           const boolean_function& b);

boolean_function operator*(const boolean_function& a,
                           const boolean_function& b);

boolean_function operator|(const boolean_function& a,
                           const boolean_function& b);

bool operator!=(const boolean_function& a, const boolean_function& b);

bool is_functionally_complete_system(const std::vector<boolean_function>& fs);
bool is_functionally_complete_system(
    const std::initializer_list<boolean_function> vars);

#endif  // INCLUDE_BOOLEAN_FUNCTION_
