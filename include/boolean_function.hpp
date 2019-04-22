// Copyright 2019 Your Name <your_email>

#ifndef INCLUDE_BOOLEAN_FUNCTION_
#define INCLUDE_BOOLEAN_FUNCTION_

#include <string>
#include <vector>

class boolean_function
{
public:
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
	static boolean_function var(size_t n, size_t dimension);

	// Тождественный ноль "от dimension переменных"
	static boolean_function zero(size_t dimension);

	// Тождественная единица "от dimension переменных"
    static boolean_function one(size_t dimension);

	static boolean_function from_anf(std::vector<value_type> v);

	boolean_function();

	// Создаёт функцию тождественного нуля от n переменных
	boolean_function(size_t n);

	// Создаёт функцию от n переменных. Вектор значений представляется числом value
	// Пример:
	// пусть value = 14, т.е. 0...00001110 в двоичной системе
	// а n = 3
	// тогда АНФ boolean_function будет равна x + y + xy + zx + zy + zyx
	boolean_function(unsigned long long value, size_type n);

	// Создаёт функцию из строки, содержащей вектор значений
	// пусть table = "01110000"
	// тогда АНФ boolean_function будет равна x + y + xy + zx + zy + zyx
	boolean_function(const std::string& table);

	// Создаёт функцию из вектора значений
	// пусть table = {true, false, false, true};
	// тогда АНФ boolean_function будет равна x + y + 1
	boolean_function(const std::vector<value_type>& table);
	boolean_function(const std::initializer_list<bool> vars);

	boolean_function(const boolean_function& table);

	boolean_function& operator=(const boolean_function& rhs);

	// Сложение по модулю 2
	// если разное количество переменных - исключение
	boolean_function& operator += (const boolean_function& rhs);

	// Конъюнкция
	// если разное количество переменных - исключение
	boolean_function& operator *= (const boolean_function& rhs);

	// Дизъюнкция
	// если разное количество переменных - исключение
	boolean_function& operator |= (const boolean_function& rhs);

	// Побитовое отрицание
	boolean_function operator ~ ();

	// true если функции равны
	// иначе false
	bool operator == (const boolean_function& rhs) const;

	// true если одна функция сравнима и больше или равна rhs
	// false если сравнима и меньше rhs
	// logic_error если фунции не сравнимы
	bool operator >= (const boolean_function& rhs) const;

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
	// operator вернет новую функцию, которая равна композиции G = T(fs[0], fs[1], ..., fs[N-1])
	boolean_function operator()(const std::vector<boolean_function>& fs) const;
	boolean_function operator()(const std::initializer_list<boolean_function> vars) const;

	bool is_monotone() const; // монотонная
	bool is_selfdual() const; // самодвойственная
	bool is_linear() const; // линейная
	bool is_T1() const; // сохраняет единицу
	bool is_T0() const; // сохраняет ноль
	bool is_balanced() const; // равновесная
	bool is_symmetric() const; // симметричная
	size_t weight() const;

	bool is_functionally_complete_system() const;

	// Возвращает АНФ функции
	std::vector<value_type> anf() const;
};

// Пусть boolean_function задаётся вектором значений "01110000"
// тогда get_polynom вернет строку "x0 + x1 + x0x1 + x0x2 + x1x2 + x0x1x2"
std::string get_polynom(const boolean_function&);

boolean_function operator + (const boolean_function& a, const boolean_function& b);

boolean_function operator * (const boolean_function& a, const boolean_function& b);

boolean_function operator | (const boolean_function& a, const boolean_function& b);

bool operator != (const boolean_function& a, const boolean_function& b);

bool is_functionally_complete_system(const std::vector<boolean_function>& fs);
bool is_functionally_complete_system(const std::initializer_list<boolean_function> vars);

#endif // INCLUDE_BOOLEAN_FUNCTION_
