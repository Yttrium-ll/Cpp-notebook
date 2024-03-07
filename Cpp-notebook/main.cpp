#include <iostream>
#include <fstream>
#include <string>
#include <utility>			// ��� move
#include <limits>			// ���������� �������� �������� �����
#include <memory>			// ��� ����� ����������
#include <optional>
#include <any>
#include <variant>
#include <complex>
#include <random>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <iomanip>
#include <filesystem>

#include "dummy.h"
#include "hierarchy.h"
#include "variative_function.h"

using std::cout, std::endl, std::string;		// ���������� ��������� �������� �� namespace
namespace fs = std::filesystem;					// ��������� ������������ ����

void get_copy(dummy d)		// ��������� �������� �� �������� (�����������)
{
	cout << "From function copy: " << d.report() << endl;
}

void get_reference(dummy& d)	// ��������� �������� �� ������ (lvalue)
{
	cout << "From function reference: " << d.report() << endl;
}

void get_dummy(dummy&& d)	// ��������� �������� �� ������ (rvalue)
{
	dummy func_d(move(d));		// ���������� ������ d � func_d
	cout << "From function move: " << func_d.report() << endl;
}

std::optional<dummy> make_them_know(int x)
{
	if (x == 42) return dummy(42, 1, "Savant");
	else return std::nullopt;
}

// [Lospinozo ����� 1 ����� 9]
// decltype � auto-����� ����
template <typename X, typename Y>
auto add(X x, Y y) -> decltype(x + y)		// -> ��������� �� ��������� ����, ����������� ������������ ���
											// decltype ���������� ��� ���������
{
	return x + y;
}

template <typename T>
void print_vector(const std::vector<T>& vec, string message)
{
	cout << message << ": ";
	for (auto& item : vec)
		cout << item << " ";
	cout << endl;
}

template <typename T>
void print_set(const std::set<T>& s, string message)
{
	cout << message << ": ";
	for (auto& item : s)
		cout << item << " ";
	cout << endl;
}

void print_dummies_vector(const std::vector<dummy>& vec, string message)
{
	cout << message << ":" << endl;;
	for (auto& item : vec)
		cout << "\t" << item.report() << endl;
	cout << endl;

}

int main()
{
	cout << "Hello world" << endl;

	// [Lospinozo ����� 1 ����� 3 � 6]
	cout << endl << "Const and cast section" << endl;
	const dummy d;
	const dummy& pd = d;	// ����� ������� ������ const-������ �� const-������
	// d.setX(1);			// � const-������� ������ ������� ��-const �����
	// pd.setX(1);			// � ������ �� const-������ ������ ������� ��-const �����
	cout << d.report() << " const dummy has x=" << d.getX() << " and y=" << d.getY() << endl;		// � const-������� ����� ������� ������ const ������
	dummy& pdnc = const_cast<dummy&>(pd);		// pdnc ������ �� ����������� ������, ����� �������� ������, �� ������� ��� ���������
	pdnc.setX(1);
	cout << d.report() << " changing unchangeble" << endl;		// const-������ �������

	// [Habr ��� ��� ��� ���������� ����� � ����� C++...]
	// ��-�����		(� ��������� ������ � ���� ����� ��� ��, ����� ������ ��� �������)
	int n = 42;
	double dbl = (int)n;	// ����� ���������� - �� ���������� ������� ��������, ����� ����������� ������� ������������� ��������������
	double dbl1 = n;		// ������� ���������� - �� ���������� ������� ��������, ����� ����������� ������� ������������� ��������������
	int n1 = (int)dbl;
	string* ps = (string*)(new dummy("A string, not dummy"));		// ������ �� � ���; ������������� ������ ��������� ������ ����� �������� � segfault

	// const_cast - ����� ������� ��� ��������� ������������ const � volatile � ���������� � ������
	const int* cn = const_cast<const int*>(&n);
	int* pn = const_cast<int*>(cn);

	// reinterpret_cast - ���������� �/�� ���������: ����. -> ����.; ����. -> ����� �����; ����� ����� -> ����.
	long address = 0x100042;
	int* pointerToAnswer = reinterpret_cast<int*>(address);		// ���� ������ �� ������ �� ��������, �� ������������� ��������� �������� � segfault
	long addr = reinterpret_cast<long>(&n);

	// static_cast - ����� ���������� ������ ��-��������� ��������������
	double dbl2 = static_cast<double>(n);
	//string* ps1 = static_cast<string*>(new dummy());	// ��������� ������ ������� �� ����������, ���� ��� �� ��������� � ����� �������� ������������

	// dynamic_cast - ���������� ���������� � ������ ������� � �������� ������������
	king* pk = new king();
	heir* ph = new heir();

	// ���� ���������� ��������, ������������ ������ ���������
	king* newKing = dynamic_cast<king*>(ph);		// ����������� ���������� ����� �� ��������
	if (!newKing) cout << "No king in kindom!" << endl;
	heir* abducted = dynamic_cast<heir*>(pk);		// �� ����������� ���������� ���� �� ��������
	if (!abducted) cout << "Old king ran away!" << endl;

	dummy* pdk = dynamic_cast<dummy*>(pk);			// �� ����������� ���������� � ������ ��������� ������������
	if (!pdk) cout << "King will never be a peasant!" << endl;
	//king* ps1 = dynamic_cast<king*>(pdk);			// �� ���������� �� ����� ����������, ���� ���������� ����� �� �������� ������������

	cout << endl << "Dummy test" << endl;
	dummy d1(5, 2, "John");
	d1.setX(4);
	cout << d1.report() << " ratio: " << d1.ratio() << endl;

	// [Lospinozo ����� 1 ����� 4]
	cout << endl << "Exception section" << endl;
	d1.setY(0);
	// ������ ����� ����������
	try
	{
		cout << d1.report() << " ratio: " << d1.ratio() << endl;
	}
	catch (const std::exception & err)
	{
		cout << err.what() << endl;
	}

	// [Lospinozo ����� 1 ����� 8]
	cout << endl << "Structure unpacking section" << endl;
	d1.set(3, 2);
	cout << d1.report();
	// ������ ����������������� ��������
	auto [res, exact] = d1.check_ratio();		// ���������� POD
	if (exact) cout << " ratio: " << res << endl;
	else cout << " Exact ratio is not integer" << endl;

	cout << endl << "if init section" << endl;
	if (dummy d2(42, 4, "Jack"); d2.getX() == 42)		// ���� ������������� � if
		cout << d2.report() << " has answer to The Main Question of The Universe and All" << std::endl;
	else cout << d2.report() << " has no answer" << endl;
	// d2.getY();		// ������� ��������� ���������� ������ if

	// [Lospinozo ����� 1 ����� 4]
	cout << endl << "Copy and move section" << endl;
	get_copy(d1);
	get_reference(d1);
	get_dummy(move(d1));
	cout << "Previos place of moved: " << d1.report() << endl;

	// [Lospinozo ����� 1 ����� 7]
	cout << endl << "Max numeric values section" << endl;
	cout << "Max short: " << std::numeric_limits<short>::max() << endl;
	cout << "Max int: " << std::numeric_limits<int>::max() << endl;
	cout << "Max float: " << std::numeric_limits<float>::max() << endl;
	cout << "Max double: " << std::numeric_limits<double>::max() << endl;

	// [Lospinozo ����� 1 ����� 9]
	cout << endl << "Variative function section" << endl;
	cout << "The answer is " << sum<int>(2, 4, 6, 8, 10, 12) << endl;

	cout << endl << "Callable object section" << endl;
	d();					// �������� ������
	dummy d0("Robert");		// ����������� ���������� �� ��������� ������ �������� ���������� ����
	d0("Hi");				// �������� ������
	dummy df("Semi-function");
	d0.action = df;			// std::function ����� ��������� callable ������ (���������� �����������)
	d0.takeAction();

	cout << endl << "Lambda functions section" << endl;
	auto lambdaFunc = []() {cout << "Hi from lambda!" << endl; };							// ������ ����� ���� ��������� ���� ���������� �� �������, ���� ���������� auto
	auto lambdaCapture = [d]() {cout << "Captured by copy - " << d.report() << endl; };		// ���������� d ��������� � ������-������� - ���������� � ������ ������-�������
	lambdaFunc();
	d0.action = lambdaCapture;
	d0.takeAction();
	d0.action = [&d]() {cout << "Captured by ref - " << d.report() << endl; };		// ������ ���������� d �� ������
	d0.takeAction();
	d0.distance = [](int x1, int y1) { return  x1 + y1; };							// ����� ��������� ������ �������� ���������
	d0.distance = [](int x1, int y1) -> int { return  x1 / y1; };					// -> int - ����� �������� ���� ������������� ��������
	auto templateLambda = [](auto x) {cout << "auto report: " << x; };				// ������������� auto ��� ���� ��������� ���������� ������ � ������
	d0.makeCustomReport(templateLambda, "some string");			// ������ �������� ��������������� ������� ���������� ������
	d0.makeCustomReport(templateLambda, 42);

	// [Lospinozo ����� 2 ����� 11]
	cout << endl << "Smart pointers section" << endl;
	// ���������� ��������� - ������ �� �����������
	std::unique_ptr<dummy> upd(new dummy("Captain Jack Sparrow"));		// �������� ��������� � ����������� ������ ���������
	cout << upd->report() << " accessed from unique pointer" << endl;
	using uniqueDummyPtr = std::unique_ptr<dummy>;		// ����� ������������ ���������
	{
		// upd1 ���������� ������ � ���� ������� � ����� ������������� ��������� ���������� ��� ������
		uniqueDummyPtr upd1 = std::make_unique<dummy>("Arthur The King");	// make_unique ������������ ������ � �����������, ����������� ���
	}
	//uniqueDummyPtr cupd = upd;			// ���������� ��������� ������ �����������
	uniqueDummyPtr mupd(std::move(upd));	// �� ��� ����� ���������
	cout << mupd->report() << " accessed from moved unique pointer" << endl;
	//cout << upd->report() << endl;				// ������� ���������� - ������� � ����� ��������� ��� ���

	// ����� ��������� - ����� ���������� � ����������, ��������� ��������� ���������� ������
	std::shared_ptr<dummy> spd = std::make_shared<dummy>("Monica");
	cout << spd->report() << " accessed from shared pointer" << endl;

	using dummyPtr = std::shared_ptr<dummy>;
	{
		dummyPtr spd1 = spd;		// �������� ����� ���������
		cout << spd1->report() << " accessed from other pointer" << endl;
		// ������ �� ���������, ��� ��� �� ���� ��� ��������� spd1
	}

	// ������ ��������� - ��������� �������� ��������
	std::weak_ptr<dummy> wpd(spd);		// ������ ��������� ����� ���������������� ������ ����� ����������
	//wpd->report();					// ����� ������ ��������� ������ ���������� � �������
	std::shared_ptr<dummy> ptr = wpd.lock();		// ������� lock ���� ������ � ������� ����� ����� ���������, ���� ������ ��� ����������
	cout << ptr->report() << " accessed from weak pointer" << endl;

	using weakDummyPtr = std::weak_ptr<dummy>;
	{
		dummyPtr sd = std::make_shared<dummy>("Orlando");		// sd ���������� ������ ����� � ����� ��������� ��� ������ �� �������
		wpd = sd;
	}
	ptr = wpd.lock();		// ptr == nullptr, ��� ��� ������, �� ������� ��������� ������ ���������, ���������
	if (!ptr)
		cout << "Dummy, which weak pointer points to, expired" << endl;

	// [Lospinozo ����� 2 ����� 12]
	cout << endl << "Optional section" << endl;
	std::optional<dummy> opt = make_them_know(42);		// optional ��������� ��������, ������� ����� �������������
	dummy savant = opt.value();							// ������ � �������; ����������, ���� ������� ���
	cout << savant.report() << " You give the right answer!" << endl;
	std::optional<dummy> opt2 = make_them_know(41);
	if (!opt2) cout << "Wrong answer!" << endl;			// ��������� ������� ����� ��������� ������� ����������� � bool

	cout << endl << "Pair and unpacking section" << endl;
	std::pair<int, string> theKnowlenge(42, "Answer");
	auto& [num, str] = theKnowlenge;		// ������������� pair � ������� ����������������� ��������
	cout << str << ": " << num << endl;

	cout << endl << "Tuple section" << endl;
	// tuple - ������, ��������� pair �� ������������ ���������� ����������� ���������
	std::tuple<string, string, string> word("The Oak", "The Crown", "The Crow");
	// ��� ������� ������������ std::get<i>, ��� i - ������ ��������
	cout << std::get<0>(word) << " there stands with heavy " << std::get<1>(word) << " and " << std::get<2>(word) << " upon them" << endl;

	std::tuple knowledge(true, "Answer", 42);		// ����� ������������ ��������� ����� � �������
	if (std::get<bool>(knowledge))					// ���� ��� ������������� �� ���� ���������, �� get ����� ���������� �������� ���
		cout << std::get<const char*>(knowledge) << ": " << std::get<int>(knowledge) << endl;
	auto& [flag, str1, num1] = knowledge;			// ������������� tuple � ������� ����������������� ��������
	if (flag)
		cout << str1 << ": " << num1 << endl;

	cout << endl << "Any section" << endl;
	std::any shapeshifter;							// �����, �������� ��������� �������� ������ ����
	shapeshifter.emplace<int>(42);					// ����� emplace ������������ � any ������ ���������� ����
	int num2 = std::any_cast<int>(shapeshifter);	// ���������� � ��������� ����
	cout << "Shapeshifter turned to int " << num2 << endl;
	try
	{
		float fl = std::any_cast<float>(shapeshifter);		// � ������ ������������ ����� ������������ ����������
	}
	catch (const std::bad_any_cast & err)
	{
		cout << "Shapeshifter refused to turn into float: " << err.what() << endl;
	}
	std::any shapeshifter2 = std::make_any<dummy>("The Doppelganger Junior");		// ����� ������� any � ���� �������
	shapeshifter.emplace<dummy>("The Doppelganger");
	cout << "Shapeshifter now in " << shapeshifter.type().name() << " shape!" << endl;
	cout << std::any_cast<dummy>(shapeshifter).report() << " shifting the shape" << endl;
	try
	{
		int num2 = std::any_cast<int>(shapeshifter);		// ������ �������� int ����������
	}
	catch (const std::bad_any_cast & err)
	{
		cout << "Shapeshifter refused to turn into int: " << err.what() << endl;
	}

	cout << endl << "Variant section" << endl;
	std::variant<int, float, dummy> lawfulShapeshifter;		// �����, �������� ��������� �������� ������ ����, ������� ����������� �������
	lawfulShapeshifter.emplace<int>(42);					// ����� emplace ������������ � variant ������ ���������� ����
	cout << "LawfulShapeshifter turned to int " << std::get<int>(lawfulShapeshifter) << endl;		// ���������� � ��������� ����
	lawfulShapeshifter.emplace<float>(4.2f);
	cout << "LawfulShapeshifter turned to float " << std::get<1>(lawfulShapeshifter) << endl;		// ����� ������������ ������ ���� � ������������ ����� � �������
	lawfulShapeshifter.emplace<dummy>(4, 2, "The Doppelganger Senior");
	if (lawfulShapeshifter.index() == 2)					// ������ � ������� �������� ���� � ������������ ����� � �������
	{
		dummy lsspd = std::get<dummy>(lawfulShapeshifter);
		cout << lsspd.report() << " shifting the shape" << endl;
	}
	try
	{
		int num2 = std::get<int>(lawfulShapeshifter);		// ������������ ���������� ���������� ����������
	}
	catch (const std::bad_variant_access & err)
	{
		cout << "LawfulShapeshifter refused to turn into int: " << err.what() << endl;
	}

	cout << endl << "Complex number section" << endl;
	std::complex a(0.5, 1.0);
	cout << "We have complex number " << std::real(a) << " + " << std::imag(a) << "*i" << endl;
	// complex � ������� �������� ������� ������������ ��� ���. �������� ��� ������������ ������� (��. �������)

	cout << endl << "Pseudorandom section" << endl;
	std::mt19937_64 oracle(91586);			// ��������� ��������������� ����� - ����� ��������
											// ���������������� ������, ������� ������ ����������� ��� ���� ����� ��������������� ������������������
	if (auto n = oracle(); n == 8346843996631475880) cout << "I guessed first: " << n << endl;		// ���� �������� �����, ������������������ ���������������
	if (auto n = oracle(); n == 2237671392849523263) cout << "I guessed second: " << n << endl;
	if (auto n = oracle(); n == 7333164488732543658) cout << "I guessed third: " << n << endl;

	std::uniform_int_distribution uniform(0, 10);				// ����������� ������������� ����� ����� �� 0 �� 10
	cout << "10 random numbers from 0 to 10:" << endl;
	for (size_t i = 0; i < 10; i++)
		cout << uniform(oracle) << " ";							// �������� ����� ������� ������� � ��������� ���������� ��������������� �����
	cout << endl << "It was 6 9 10 1 1 7 9 9 2 10" << endl;		// ������������������ ���������������
	// � stdlib random ������������ �������� ����� ������������ ������������� (�����������, ����������, ��-�������, ����, ������, ���������, ��������, ������������, �������� � ��.)

	// [Lospinozo ����� 2 ����� 13]
	cout << endl << "Containers section" << endl;
	std::array<int, 10> static_array;		// ������ ���������� �����
	std::array<int, 3> arr{ 0, 10, 42 };	// �������� �������������
	for (size_t i = 0; i < 10; i++)			// ���� ��� ������� �� ������ �������
		static_array[i] = i * i;
	for (auto it = static_array.begin(); it != static_array.end(); it++)		// ���� ��� ������� �� ������ ����������
		cout << *it << " ";
	cout << endl;
	/*
	������ begin() � end() � �������� ����������� STL �������� ���:
	begin() ���������� �������� �� 1� �������, end() - �� ��������� ������� ����� ����������
	 begin()	 end()
	 |			 |
	[-][-][-][-][ ]
	 1  2  3  4
	*/
	for (int& i : arr)		// ���� ��� ������� � �������������� ��������� ���������
		cout << i << " ";
	cout << endl;

	std::vector<float> vec;								// ������ ���������� �����
	if (vec.empty()) cout << "Vec is empty" << endl;	// empty ���������� ������� �������
	std::vector<float> vec1{ 3, 4, 42.1 };				// �������� �������������
	std::vector<float> vec2(vec1.begin(), vec1.end());	// ����������� �� ���������, ������������� 2�� �����������

	vec.push_back(42.1);						// ���������� ������ ��������
	print_vector(vec, "Vec after push_back");

	print_vector(vec2, "Vec2 before assign");
	vec2.assign({ 0.1, 0.2, 0.3 });				// ������ ����������� ����� ������� �������������
	print_vector(vec2, "Vec2 after assign");

	print_vector(vec1, "Vec1 before insert");
	vec1.insert(vec1.begin() + 1, 33);			// ������� �� ������� ����� ���������, �� ������� ��������� ���������� ��������
	print_vector(vec1, "Vec1 after insert");

	// [Lospinozo ����� 2 ����� 14]
	cout << "Vec1 has size " << vec1.size() << " and length " << std::distance(vec1.begin(), vec1.end()) << endl;		// distance ��������� ���������� ����� ����� ������������ �����������
	auto iterFour = std::find(vec1.begin(), vec1.end(), 4);																// find ���� ������� � ������� ����� ���������� ����������� (������� ������������)
	cout << "In vec1 4 has index " << std::distance(vec1.begin(), iterFour) << endl;									// ����� ������������ ��� ����������� ������� ��������, ���� ���� ��������, ����������� �� ����

	std::vector<float> reversedVec1{ vec1.rbegin(), vec1.rend() };		// ��������� rbegin � rend ������������� ����������� ��������
	print_vector(reversedVec1, "Reversed Vec1");
	/*
	������ rbegin() � rend() � �������� ����������� STL �������� ���:
	rend() ���������� �������� �� 1� �������, rbegin() - �� ��������� ������� ����� ����������
	���������� ������������ ��������� - ��� ����������� ����� �� rbegin � rend
	 rend()		 rbegin()
	 |			 |
	[-][-][-][-][ ]
	 1  2  3  4
	*/
	std::vector<float> reversedVec2;
	for (auto iter = vec2.rbegin(); iter < vec2.rend(); iter++)			// ����� ����, ��� ��� ������������ ��������� ���������� - ��� �������� ������ ������, ��������� �������� �� ��������
		reversedVec2.push_back(*iter);
	print_vector(reversedVec2, "Reversed Vec2");

	// [Lospinozo ����� 2 ����� 13]
	std::vector dummiesLine{ dummy("First"), dummy("Second"), dummy("Third") };
	print_dummies_vector(dummiesLine, "DummiesLine before emplace");
	dummiesLine.emplace_back("Forth");								// emplace_back ������������ ������ � ����������� ����������� � ��������� ��� � �����
	dummiesLine.emplace(dummiesLine.begin() + 1, "New Second");		// emplace ������������ ������ � ����������� ����������� � ��������� ��� ����� ���������, �� ������� ��������� ��������
	print_dummies_vector(dummiesLine, "DummiesLine after emplace");

	std::set<int> s{ 1, 1, 2, 3, 5 };	// set (���������) - ������������� ��������� ��������������� ���������� ������ (������ ������ - ������)
	print_set(s, "Set s");				// ������ 1 �������, �.�. �������� ����������
	auto secIter = s.find(2);			// find ���� �������� � ���������� ��������, ����������� �� ����
	auto sixIter = s.find(6);			// ����� �� ������������� ��������� ���������� ������
	if (*secIter == 2) cout << "Found element 2" << endl;
	if (sixIter == s.end()) cout << "6 is not found" << endl;

	std::multiset<int> ms{ 1, 1, 2, 3, 5 };		// multiset - ������ set, ����������� ������������ �����
	cout << "In multiset " << ms.count(1) << " times present element 1" << endl;

	// map - ������������� ���������, �������� ���� ����-��������; ������ � �������� �������������� �� ����� (������ ������ - ������)
	std::map<string, int> cyberspace{ {"Neuromancer", 1984}, {"Count Zero", 1986}, {"Mona Lisa Overdrive", 1988} };	// �������� ������������� ������ ����-��������
	cout << "First novel of Gibson's Cyberspace was published in " << cyberspace["Neuromancer"] << endl;			// ��� ������� ����� ������������ [], ��������� ����
	cout << "Final Countdown was published in " << cyberspace["Final Countdown"] << endl;							// �� ���� �������� ���, [] ������� ��� �� ���������
	try
	{
		cout << "Second novel of Gibson's Cyberspace was published in " << cyberspace.at("Count Zero") << endl;		// at ����� ������������� ������ � �������� �� �����
		cout << "Never existing novel of Gibson was published in";
		cout << " " << cyberspace.at("Not A Final Countdown") << endl;			// ������, at ����������� ����������, ���� ������ ����� ���
	}
	catch (const std::out_of_range & err)
	{
		cout << "... Well, it was never published: " << err.what() << endl;
	}
	cyberspace.insert({ "Burning Chrome", 1982 });		// ���������� ��������
	cyberspace.erase("Final Countdown");				// �������� �������� (����� ����� �������� ��������)

	// [Lospinozo ����� 2 ����� 15]
	cout << endl << "String section" << endl;
	string owlbear("owlbear");
	string owl(owlbear, 0, 3);		// �������� ���������: �������� ������, ������ ������, �����
	string bear(owlbear, 3);		// �������� ��������� �� ������ ������� �� �����: �������� ������, ������ ������
	cout << owlbear << " = " << owl << " + " << bear << endl;
	string ans = to_string(42);		// ������� ����� � ������
	int iAns = std::stoi(ans);
	string raw(R"(Literal \t with spec symbols \0 \n \hello"\world"\)");		// R"()" ���������� ������������ ����. �������
	cout << "Raw string: " << raw << endl;
	// ��� ��������� ������ ��� ��������������� �������� ��. � [Lospinozo ����� 2 ����� 15]

	// [Lospinozo ����� 2 ����� 16]
	cout << endl << "Streams section" << endl;
	// �������� ��������������� �������� ������ � ������
	cout << "Gotham needs its " << std::boolalpha << true << " hero.";
	cout << "\nMark it " << std::noboolalpha << false << "!";
	cout << "\nThere are " << 69 << "," << std::oct << 105 << " leaves in here.";
	cout << "\nYabba " << std::hex << 3669732608 << "!";
	cout << "\nAvogadro's number: " << std::scientific << 6.0221415e-23;
	cout << "\nthe Hogwarts platform: " << std::fixed << std::setprecision(2) << 9.750123;
	cout << "\nAlways eliminate " << 3735929054;
	cout << std::setw(4) << "\n"
		<< 0x1 << "\n"
		<< 0x10 << "\n"
		<< 0x100 << "\n"
		<< 0x1000 << endl;

	auto intro = std::ifstream("introspection.txt");
	cout << "Contents: " << intro.rdbuf() << endl;										// rdbuf �������� ����� �������
	intro.seekg(0);																		// seekg ��������� ������� �� ��������� �������
	cout << "Contents after seekg(0): " << intro.rdbuf() << endl;
	intro.seekg(-4, std::ios_base::end);												// seekg � ����� ����������� - �������� � ����� �������
	cout << "tellg() after seekg(-4, ios_base::end): " << intro.tellg() << endl;		// tellg ���� ������� ������� �������
	cout << "Contents after seekg(-4, ios_base::end): " << intro.rdbuf() << endl;

	// [Lospinozo ����� 2 ����� 17]
	cout << endl << "Filesystem section" << endl;
	const std::filesystem::path kernel32{ R"(C:\Windows\System32\kernel32.dll)" };		// �������������������� ������������� ����
	cout << "Root name: " << kernel32.root_name()					// ��������� ��������� ������ (������������) ����
		<< "\nRoot directory: " << kernel32.root_directory()
		<< "\nRoot path: " << kernel32.root_path()
		<< "\nRelative path: " << kernel32.relative_path()
		<< "\nParent path: " << kernel32.parent_path()
		<< "\nFilename: " << kernel32.filename()
		<< "\nStem: " << kernel32.stem()
		<< "\nExtension: " << kernel32.extension()
		<< endl << endl;

	fs::path path{ R"(C:/Windows/System32/kernel32.dll)" };		// ����������� ����
	cout << path << endl;
	path.make_preferred();										// ��������� ������������ �� ���������������� ��� ��
	cout << path << endl;
	path.replace_extension(".txt");								// ������ ����������
	cout << path << endl;
	path.replace_filename("win32kfull.sys");					// ������ ����� �����
	cout << path << endl;
	path.remove_filename();										// �������� ����� �����
	cout << path << endl;
	path = path / "REAMDE";										// ������������ �����
	cout << path << endl;
	path.clear();
	cout << "Is empty: " << std::boolalpha << path.empty() << endl << endl;

	const auto temp_path = fs::temp_directory_path();						// ��������� ���������� ��� ��������� ������
	try
	{
		const auto relative = fs::relative(temp_path);							// �������� ������������� ���� ������������ ������� ����������
		cout << std::boolalpha
			<< "Temporary directory path: " << temp_path
			<< "\nTemporary directory absolute: " << temp_path.is_absolute()	// ��������, �������� �� ���� ����������
			<< "\nCurrent path: " << fs::current_path()							// ������� ����������
			<< "\nTemporary directory's relative path: " << relative
			<< "\nRelative directory absolute: " << relative.is_absolute()
			<< "\nChanging current directory to temp.";
		fs::current_path(temp_path);											// ��������� ������� ����������
		cout << "\nCurrent directory: " << fs::current_path() << endl;
	}
	catch (const std::exception & e)
	{
		std::cerr << "Error: " << e.what();				// ��� ������� ����� ��������� ������, ���� �� �� ������������ ��������
	}

	cout << endl << std::boolalpha << "Path: " << kernel32 << endl;
	cout << "Is directory: " << fs::is_directory(kernel32) << endl;			// ��������� �� ���� �� ����������
	cout << "Is regular file: " << fs::is_regular_file(kernel32) << endl;	// �������� �� ���� �������
	cout << "Is empty: " << fs::is_empty(kernel32) << endl;					// �������� �� ������ ������ ��� ���������

	cout << endl << "Is kernel32 equal to path: " << std::boolalpha << fs::equivalent(kernel32, temp_path) << endl;
	cout << "Does kernel32 exists: " << fs::exists(kernel32) << endl;
	cout << "Size: " << std::dec << fs::file_size(kernel32) << endl;
	auto time = fs::last_write_time(kernel32);
	cout << "Kernel was changed " << std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count()
		<< " since epoch" << endl;
	auto space = fs::space(kernel32);
	cout << "Space available: " << space.available << endl;
	cout << "Space capacity: " << space.capacity << endl;
	cout << "Space free: " << space.free << endl;
	// ��� ����������� � �������� �������� ��. [Lospinozo ����� 2 ����� 17]

	// ����� ����������
	const fs::path sys_path{ R"(C:\\Windows\\)" };
	cout << endl << "Size Last Write Name\n";
	cout << "------------ ----------- ------------\n";
	for (const auto& entry : fs::directory_iterator{ sys_path })
	{
		try
		{
			if (entry.is_directory())
				cout << " *";
			else
				cout << std::setw(12) << entry.file_size();
			const auto lw_time = std::chrono::duration_cast<std::chrono::seconds>(entry.last_write_time().time_since_epoch());
			cout << std::setw(12) << lw_time.count() << " " << entry.path().filename().string() << endl;
		}
		catch (const std::exception & e)
		{
			cout << "Error accessing " << entry.path().string() << ": " << e.what() << endl;
		}
	}

	cout << endl << "Bye world" << endl;
	return 0;
}