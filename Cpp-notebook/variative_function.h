#pragma once
#include <cstdarg>

// функция с переменным числом аргументов - пережиток C, небезопасно!
int sum(size_t n, ...)
{
	va_list args;			// список аргументов
	va_start(args, n);		// инициализация списка аргументов
	int result{};
	while (n--)
	{
		auto next_element = va_arg(args, int);		// получение следующего аргумента из va_list с типом int
		result += next_element;
	}
	va_end(args);		// закрытие списка аргументов после итераций
	return result;
}

// Вариативный шаблон последней итерации
template <typename T>
T sum(T x)
{
	return x;
}

// Вариативный рекурсивный шаблон
template <typename T, typename ... Args>
T sum(T x, Args ... args)
{
	return x + sum(args...);	// используем рекурсивный вызов
}