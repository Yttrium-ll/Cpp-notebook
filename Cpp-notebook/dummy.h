#pragma once
#include <iostream>
#include <string>
#include <stdexcept>		// для классов-исключений
#include <functional>		// ссылки на функции

using std::string, std::to_string, std::cout, std::endl, std::move; 	// добавление отдельных символов из namespace

struct result		// POD-объект для проверки статуса
{
	int res;
	bool success;
};

class dummy
{
public:
	dummy();
	dummy(string iName);
	dummy(int ix, int iy, string iName);
	dummy(const dummy& other);							// конструктор копирования
	dummy& operator=(const dummy& other);
	//dummy& operator=(const dummy& other) = delete;	// запрет использования оператора присваивания (нужно также запрещать конструктор копирования)
	dummy(dummy&& other) noexcept;						// конструктор перемещения (noexept строго рекомендуется)
	dummy& operator=(dummy&& other) noexcept;
	//dummy& operator=(dummy&& other) = delete;			// запрет использования оператора перемещения (нужно также запрещать конструктор перемещения)
	void operator()() const;
	void operator()(string str) const;
	~dummy();

	int getX() const;						// const-метод
	int getY() const;						// const-метод
	const std::string getName() const;		// const-метод, возвращающий const значение string

	void setX(int ix);
	void setY(int iy);
	void set(int ix, int iy);

	int ratio() const;			// генерирует исключение
	result check_ratio() const;	// возвращает и результат, и статус

	void takeAction();
	string report() const;

	template <typename fn, typename T>
	void makeCustomReport(fn func, T x);					// можем объявить шаблонную функцию, чтобы работать с любыми функциями на входе
	
	std::function<void()> action = nullptr;					// ссылка на функцию, возвращающую void и не принимающую аргументов
	std::function<int(int, int)> distance = nullptr;		// ссылка на функцию, возвращающую int и принимающую как аргументы 2 int

private:
	int x;
	int y;
	string name;
	// static int count;			// статический член класса - один на всех (инициализируется в .cpp)
	static inline int count = 0;	// статический член класса - может быть проинициализирован в объявлении класса при наличии слова inline
	int num;
};

template<typename fn, typename T>
inline void dummy::makeCustomReport(fn func, T x)		// шаблонированные функции обязаны находиться в .h
{
	cout << report() << " ";
	func(x);
	cout << endl;
}
