#pragma once
#include <cstdarg>

// ������� � ���������� ������ ���������� - ��������� C, �����������!
int sum(size_t n, ...)
{
	va_list args;			// ������ ����������
	va_start(args, n);		// ������������� ������ ����������
	int result{};
	while (n--)
	{
		auto next_element = va_arg(args, int);		// ��������� ���������� ��������� �� va_list � ����� int
		result += next_element;
	}
	va_end(args);		// �������� ������ ���������� ����� ��������
	return result;
}

// ����������� ������ ��������� ��������
template <typename T>
T sum(T x)
{
	return x;
}

// ����������� ����������� ������
template <typename T, typename ... Args>
T sum(T x, Args ... args)
{
	return x + sum(args...);	// ���������� ����������� �����
}