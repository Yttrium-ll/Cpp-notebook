#pragma once
#include <iostream>
#include <string>
#include <stdexcept>		// ��� �������-����������
#include <functional>		// ������ �� �������

using std::string, std::to_string, std::cout, std::endl, std::move; 	// ���������� ��������� �������� �� namespace

struct result		// POD-������ ��� �������� �������
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
	dummy(const dummy& other);							// ����������� �����������
	dummy& operator=(const dummy& other);
	//dummy& operator=(const dummy& other) = delete;	// ������ ������������� ��������� ������������ (����� ����� ��������� ����������� �����������)
	dummy(dummy&& other) noexcept;						// ����������� ����������� (noexept ������ �������������)
	dummy& operator=(dummy&& other) noexcept;
	//dummy& operator=(dummy&& other) = delete;			// ������ ������������� ��������� ����������� (����� ����� ��������� ����������� �����������)
	void operator()() const;
	void operator()(string str) const;
	~dummy();

	int getX() const;						// const-�����
	int getY() const;						// const-�����
	const std::string getName() const;		// const-�����, ������������ const �������� string

	void setX(int ix);
	void setY(int iy);
	void set(int ix, int iy);

	int ratio() const;			// ���������� ����������
	result check_ratio() const;	// ���������� � ���������, � ������

	void takeAction();
	string report() const;

	template <typename fn, typename T>
	void makeCustomReport(fn func, T x);					// ����� �������� ��������� �������, ����� �������� � ������ ��������� �� �����
	
	std::function<void()> action = nullptr;					// ������ �� �������, ������������ void � �� ����������� ����������
	std::function<int(int, int)> distance = nullptr;		// ������ �� �������, ������������ int � ����������� ��� ��������� 2 int

private:
	int x;
	int y;
	string name;
	// static int count;			// ����������� ���� ������ - ���� �� ���� (���������������� � .cpp)
	static inline int count = 0;	// ����������� ���� ������ - ����� ���� ������������������ � ���������� ������ ��� ������� ����� inline
	int num;
};

template<typename fn, typename T>
inline void dummy::makeCustomReport(fn func, T x)		// ��������������� ������� ������� ���������� � .h
{
	cout << report() << " ";
	func(x);
	cout << endl;
}
