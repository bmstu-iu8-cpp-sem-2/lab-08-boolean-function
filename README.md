# Лабораторная работа 8

## Задание
* Реализовать класс булевой функции от произвольного количества переменных;
* Реализовать модульные тесты, используя Gtest;
* Асимптотическая сложность нахождения АНФ должна быть N*logN;

Интерфейс класса из файла `boolean_function.hpp`:

```cpp
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
	static boolean_function var(size_t n, size_t dimension);
	static boolean_function zero(size_t dimension);
	static boolean_function one(size_t dimension);
	static boolean_function from_anf(std::vector<value_type> v);

	boolean_function();
	boolean_function(size_t n);
	boolean_function(unsigned long long value, size_type n);
	boolean_function(const std::string& table);
	boolean_function(const std::vector<value_type>& table);
	boolean_function(const std::initializer_list<bool> vars);
	boolean_function(const boolean_function& table);
	boolean_function& operator=(const boolean_function& rhs);

	boolean_function& operator += (const boolean_function& rhs);
	boolean_function& operator *= (const boolean_function& rhs);
	boolean_function& operator |= (const boolean_function& rhs);
	boolean_function operator ~ ();

	bool operator == (const boolean_function&) const;
	bool operator >= (const boolean_function& rhs) const;

	reference operator[](size_type ind);
	const_reference operator[](size_type ind) const;

	reference at(size_type ind);
	const_reference at(size_type ind) const;

	iterator begin();
	iterator end();

	const_iterator cbegin() const;
	const_iterator cend() const;

	size_type size() const throw();
	size_type dimension() const throw();

	bool operator()(const std::vector<bool>& vars) const;
	bool operator()(const std::initializer_list<bool> vars) const;

	boolean_function operator()(const std::vector<boolean_function>& fs) const;
	boolean_function operator()(const std::initializer_list<boolean_function> vars) const;

	bool is_monotone() const;
	bool is_selfdual() const;
	bool is_linear() const;
	bool is_T1() const;
	bool is_T0() const;
	bool is_balanced() const;
	bool is_symmetric() const;
	size_t weight() const;

	bool is_functionally_complete_system() const;
	std::vector<value_type> anf() const;
};

std::string get_polynom(const boolean_function&);
boolean_function operator + (const boolean_function& a, const boolean_function& b);
boolean_function operator * (const boolean_function& a, const boolean_function& b);
boolean_function operator | (const boolean_function& a, const boolean_function& b);
bool operator != (const boolean_function& a, const boolean_function& b);

bool is_functionally_complete_system(const std::vector<boolean_function>& fs);
bool is_functionally_complete_system(const std::initializer_list<boolean_function> vars);
```
Описание методов с примерами можно найти в файле `boolean_function.hpp`.

## Литература
* Алгоритм быстрого поиска [АНФ](http://dha.spb.ru/PDF/AlgebraicNormalForm.pdf);
