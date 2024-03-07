#include "dummy.h"

// int dummy::count = 0;		// ������������� ������������ ����� ������ (���� ���������� ��� ����� inline)

dummy::dummy() : x{ 0 }, y{ 0 }, name{ "dummy" }		// ������ �������������: ������������� ������ ��� ���� ������������
{
	num = count++;
	cout << report() << " constructed empty" << endl;
}

dummy::dummy(string iName) : x{ 0 }, y{ 0 }, name{ iName }
{
	num = count++;
	cout << report() << " constructed with name" << endl;
}

dummy::dummy(int ix, int iy, string iName) : x{ ix }, y{ iy }, name{ iName }
{
	num = count++;
	cout << report() << " fully constructed" << endl;
}

dummy::dummy(const dummy& other)
{
	x = other.x;
	y = other.y;
	name = other.name + "-copy";
	num = count++;
	cout << report() << " copied from: " << other.report() << endl;
}

dummy& dummy::operator=(const dummy& other)
{
	if (this != &other)
	{
		x = other.x;
		y = other.y;
		name = other.name + "-copy";
		num = count++;
		cout << report() << " copied from: " << other.report() << endl;
	}
	return *this;
}

dummy::dummy(dummy&& other) noexcept
{
	x = other.x;
	y = other.y;
	name = other.name + "-moved";
	num = other.num;

	// ������� other
	other.x = 0;
	other.y = 0;
	other.name = "deleted";
	other.num = -1;

	cout << report() << " moved" << endl;
}

dummy& dummy::operator=(dummy&& other) noexcept
{
	if (this != &other)
	{
		x = other.x;
		y = other.y;
		name = other.name + "-moved";
		num = other.num;

		// ������� other
		other.x = 0;
		other.y = 0;
		other.name = "deleted";
		other.num = -1;

		cout << report() << " moved" << endl;
	}
	return *this;
}

void dummy::operator()() const
{
	cout << report() << " reporing" << endl;
}

void dummy::operator()(string str) const
{
	cout << report() << " reporing - " << str << endl;
}

dummy::~dummy()
{
	cout << report() << " destructed" << endl;
}

int dummy::getX() const		// const-�����
{
	return x;
}

int dummy::getY() const		// const-�����
{
	return y;
}

const string dummy::getName() const		// const-�����, ������������ const �������� string
{
	return name;
}

void dummy::setX(int ix)
{
	x = ix;
}

void dummy::setY(int iy)
{
	y = iy;
}

void dummy::set(int ix, int iy)
{
	x = ix;
	y = iy;
}

int dummy::ratio() const	// ���������� ����������
{
	if (y == 0) throw std::logic_error("Division by zero");
	else return x / y;
}

result dummy::check_ratio() const
{
	int res = ratio();
	bool isCorrect = true;
	if (res * y != x) isCorrect = false;
	return { res, isCorrect };		// �������� ����������� � POD
}

void dummy::takeAction()
{
	cout << report() << " taking Action: ";
	if (action) action();		// ����� std::function ������������ ��� ������ (������� ���������� std::bad_function_call, ���� ����� ������)
}

string dummy::report() const
{
	return "Dummy " + name + " (" + to_string(x) + ", " + to_string(y) + "), " + to_string(num) + " in line:";
}
