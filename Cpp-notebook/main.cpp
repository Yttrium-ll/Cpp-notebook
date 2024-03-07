#include <iostream>
#include <fstream>
#include <string>
#include <utility>			// для move
#include <limits>			// предельные значения числовых типов
#include <memory>			// для умных указателей
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

using std::cout, std::endl, std::string;		// добавление отдельных символов из namespace
namespace fs = std::filesystem;					// псевдоним пространства имен

void get_copy(dummy d)		// принимаем аргумент по значению (копирование)
{
	cout << "From function copy: " << d.report() << endl;
}

void get_reference(dummy& d)	// принимаем аргумент по ссылке (lvalue)
{
	cout << "From function reference: " << d.report() << endl;
}

void get_dummy(dummy&& d)	// принимаем аргумент по ссылке (rvalue)
{
	dummy func_d(move(d));		// перемещаем объект d в func_d
	cout << "From function move: " << func_d.report() << endl;
}

std::optional<dummy> make_them_know(int x)
{
	if (x == 42) return dummy(42, 1, "Savant");
	else return std::nullopt;
}

// [Lospinozo Часть 1 Глава 9]
// decltype и auto-вывод типа
template <typename X, typename Y>
auto add(X x, Y y) -> decltype(x + y)		// -> указывает на выражение типа, вычисляющее возвращаемый тип
											// decltype возвращает тип выражения
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

	// [Lospinozo Часть 1 Глава 3 и 6]
	cout << endl << "Const and cast section" << endl;
	const dummy d;
	const dummy& pd = d;	// можно создать только const-ссылку на const-объект
	// d.setX(1);			// у const-объекта нельзя вызвать не-const метод
	// pd.setX(1);			// у ссылки на const-объект нельзя вызвать не-const метод
	cout << d.report() << " const dummy has x=" << d.getX() << " and y=" << d.getY() << endl;		// у const-объекта можно вызвать только const методы
	dummy& pdnc = const_cast<dummy&>(pd);		// pdnc теперь не константная ссылка, можно изменять объект, на которые она ссылается
	pdnc.setX(1);
	cout << d.report() << " changing unchangeble" << endl;		// const-объект изменен

	// [Habr Еще раз про приведение типов в языке C++...]
	// Си-стиль		(с числовыми типами в этом плане все ок, взяты просто для примера)
	int n = 42;
	double dbl = (int)n;	// явное приведение - не производит никаких проверок, кроме критических случаев невозможности преобразования
	double dbl1 = n;		// неявное приведение - не производит никаких проверок, кроме критических случаев невозможности преобразования
	int n1 = (int)dbl;
	string* ps = (string*)(new dummy("A string, not dummy"));		// почему бы и нет; использование такого указателя скорее всего приведет к segfault

	// const_cast - может убирать или добавлять модификаторы const и volatile у указателей и ссылок
	const int* cn = const_cast<const int*>(&n);
	int* pn = const_cast<int*>(cn);

	// reinterpret_cast - приведение к/из указателя: указ. -> указ.; указ. -> целое число; целое число -> указ.
	long address = 0x100042;
	int* pointerToAnswer = reinterpret_cast<int*>(address);		// если память по адресу не читается, то разыменование указателя приведет к segfault
	long addr = reinterpret_cast<long>(&n);

	// static_cast - более безопасный аналог Си-подобного преобразования
	double dbl2 = static_cast<double>(n);
	//string* ps1 = static_cast<string*>(new dummy());	// указатели разных классов не приводятся, если они не находятся в одной иерархии наследования

	// dynamic_cast - приведение указателей и ссылок классов в иерархии наследования
	king* pk = new king();
	heir* ph = new heir();

	// если приведение ошибочно, возвращается пустой указатель
	king* newKing = dynamic_cast<king*>(ph);		// допускается приведение вверх по иерархии
	if (!newKing) cout << "No king in kindom!" << endl;
	heir* abducted = dynamic_cast<heir*>(pk);		// не допускается приведение вниз по иерархии
	if (!abducted) cout << "Old king ran away!" << endl;

	dummy* pdk = dynamic_cast<dummy*>(pk);			// не допускается приведение в разных иерархиях наследования
	if (!pdk) cout << "King will never be a peasant!" << endl;
	//king* ps1 = dynamic_cast<king*>(pdk);			// не приводится на этапе компиляции, если приводимый класс не являются полиморфными

	cout << endl << "Dummy test" << endl;
	dummy d1(5, 2, "John");
	d1.setX(4);
	cout << d1.report() << " ratio: " << d1.ratio() << endl;

	// [Lospinozo Часть 1 Глава 4]
	cout << endl << "Exception section" << endl;
	d1.setY(0);
	// пример ловли исключений
	try
	{
		cout << d1.report() << " ratio: " << d1.ratio() << endl;
	}
	catch (const std::exception & err)
	{
		cout << err.what() << endl;
	}

	// [Lospinozo Часть 1 Глава 8]
	cout << endl << "Structure unpacking section" << endl;
	d1.set(3, 2);
	cout << d1.report();
	// пример структурированной привязки
	auto [res, exact] = d1.check_ratio();		// распаковка POD
	if (exact) cout << " ratio: " << res << endl;
	else cout << " Exact ratio is not integer" << endl;

	cout << endl << "if init section" << endl;
	if (dummy d2(42, 4, "Jack"); d2.getX() == 42)		// блок инициализации в if
		cout << d2.report() << " has answer to The Main Question of The Universe and All" << std::endl;
	else cout << d2.report() << " has no answer" << endl;
	// d2.getY();		// область видимости ограничена блоком if

	// [Lospinozo Часть 1 Глава 4]
	cout << endl << "Copy and move section" << endl;
	get_copy(d1);
	get_reference(d1);
	get_dummy(move(d1));
	cout << "Previos place of moved: " << d1.report() << endl;

	// [Lospinozo Часть 1 Глава 7]
	cout << endl << "Max numeric values section" << endl;
	cout << "Max short: " << std::numeric_limits<short>::max() << endl;
	cout << "Max int: " << std::numeric_limits<int>::max() << endl;
	cout << "Max float: " << std::numeric_limits<float>::max() << endl;
	cout << "Max double: " << std::numeric_limits<double>::max() << endl;

	// [Lospinozo Часть 1 Глава 9]
	cout << endl << "Variative function section" << endl;
	cout << "The answer is " << sum<int>(2, 4, 6, 8, 10, 12) << endl;

	cout << endl << "Callable object section" << endl;
	d();					// вызываем объект
	dummy d0("Robert");		// конструктор отличается от оператора вызова наличием объявления типа
	d0("Hi");				// вызываем объект
	dummy df("Semi-function");
	d0.action = df;			// std::function можно присвоить callable объект (происходит копирование)
	d0.takeAction();

	cout << endl << "Lambda functions section" << endl;
	auto lambdaFunc = []() {cout << "Hi from lambda!" << endl; };							// лямбды могут быть присвоены либо указателям на функции, либо переменной auto
	auto lambdaCapture = [d]() {cout << "Captured by copy - " << d.report() << endl; };		// переменная d находится в лямбда-захвате - копируется в объект лямбда-функции
	lambdaFunc();
	d0.action = lambdaCapture;
	d0.takeAction();
	d0.action = [&d]() {cout << "Captured by ref - " << d.report() << endl; };		// захват переменной d по ссылке
	d0.takeAction();
	d0.distance = [](int x1, int y1) { return  x1 + y1; };							// можно присвоить лямбду напрямую указателю
	d0.distance = [](int x1, int y1) -> int { return  x1 / y1; };					// -> int - явное указание типа возвращаемого значения
	auto templateLambda = [](auto x) {cout << "auto report: " << x; };				// использование auto как типа аргумента превращает лямбду в шаблон
	d0.makeCustomReport(templateLambda, "some string");			// второй аргумент шаблонированной функции передается лямбде
	d0.makeCustomReport(templateLambda, 42);

	// [Lospinozo Часть 2 Глава 11]
	cout << endl << "Smart pointers section" << endl;
	// уникальный указатель - запрет на копирование
	std::unique_ptr<dummy> upd(new dummy("Captain Jack Sparrow"));		// передаем указатель в конструктор умного указателя
	cout << upd->report() << " accessed from unique pointer" << endl;
	using uniqueDummyPtr = std::unique_ptr<dummy>;		// можно использовать псевдоним
	{
		// upd1 существует только в этой области и будет автоматически уничтожен указателем при выходе
		uniqueDummyPtr upd1 = std::make_unique<dummy>("Arthur The King");	// make_unique конструирует объект с аргументами, переданными ему
	}
	//uniqueDummyPtr cupd = upd;			// уникальный указатель нельзя скопировать
	uniqueDummyPtr mupd(std::move(upd));	// но его можно перенести
	cout << mupd->report() << " accessed from moved unique pointer" << endl;
	//cout << upd->report() << endl;				// вызовет исключение - объекта у этого указателя уже нет

	// общий указатель - можно перемещать и копировать, последний указатель уничтожает объект
	std::shared_ptr<dummy> spd = std::make_shared<dummy>("Monica");
	cout << spd->report() << " accessed from shared pointer" << endl;

	using dummyPtr = std::shared_ptr<dummy>;
	{
		dummyPtr spd1 = spd;		// копируем общий указатель
		cout << spd1->report() << " accessed from other pointer" << endl;
		// объект не уничтожен, так как на него еще указывает spd1
	}

	// слабый указатель - временное владение объектом
	std::weak_ptr<dummy> wpd(spd);		// слабый указатель можно инициализировать только общим указателем
	//wpd->report();					// через слабый указатель нельзя обратиться к объекту
	std::shared_ptr<dummy> ptr = wpd.lock();		// функция lock дает доступ к объекту через общий указатель, если объект еще существует
	cout << ptr->report() << " accessed from weak pointer" << endl;

	using weakDummyPtr = std::weak_ptr<dummy>;
	{
		dummyPtr sd = std::make_shared<dummy>("Orlando");		// sd существует только здесь и будет уничтожен при выходе из области
		wpd = sd;
	}
	ptr = wpd.lock();		// ptr == nullptr, так как объект, на который указывает слабый указатель, уничтожен
	if (!ptr)
		cout << "Dummy, which weak pointer points to, expired" << endl;

	// [Lospinozo Часть 2 Глава 12]
	cout << endl << "Optional section" << endl;
	std::optional<dummy> opt = make_them_know(42);		// optional реализует значение, которое может отсутствовать
	dummy savant = opt.value();							// доступ к объекту; исключение, если объекта нет
	cout << savant.report() << " You give the right answer!" << endl;
	std::optional<dummy> opt2 = make_them_know(41);
	if (!opt2) cout << "Wrong answer!" << endl;			// отсутсвие объекта можно проверить простым приведением к bool

	cout << endl << "Pair and unpacking section" << endl;
	std::pair<int, string> theKnowlenge(42, "Answer");
	auto& [num, str] = theKnowlenge;		// распаковываем pair с помощью структурированной привязки
	cout << str << ": " << num << endl;

	cout << endl << "Tuple section" << endl;
	// tuple - кортеж, обобщение pair на произвольное количество разнородных элементов
	std::tuple<string, string, string> word("The Oak", "The Crown", "The Crow");
	// Для доступа используется std::get<i>, где i - индекс элемента
	cout << std::get<0>(word) << " there stands with heavy " << std::get<1>(word) << " and " << std::get<2>(word) << " upon them" << endl;

	std::tuple knowledge(true, "Answer", 42);		// можно использовать автовывод типов в шаблоне
	if (std::get<bool>(knowledge))					// если нет повторяющихся по типу элементов, то get можно передавать желаемый тип
		cout << std::get<const char*>(knowledge) << ": " << std::get<int>(knowledge) << endl;
	auto& [flag, str1, num1] = knowledge;			// распаковываем tuple с помощью структурированной привязки
	if (flag)
		cout << str1 << ": " << num1 << endl;

	cout << endl << "Any section" << endl;
	std::any shapeshifter;							// класс, хранящий отдельные значения любого типа
	shapeshifter.emplace<int>(42);					// метод emplace конструирует в any объект указанного типа
	int num2 = std::any_cast<int>(shapeshifter);	// приведение к исходному типу
	cout << "Shapeshifter turned to int " << num2 << endl;
	try
	{
		float fl = std::any_cast<float>(shapeshifter);		// в случае несовпадения типов генерируется исключение
	}
	catch (const std::bad_any_cast & err)
	{
		cout << "Shapeshifter refused to turn into float: " << err.what() << endl;
	}
	std::any shapeshifter2 = std::make_any<dummy>("The Doppelganger Junior");		// можно создать any в одну строчку
	shapeshifter.emplace<dummy>("The Doppelganger");
	cout << "Shapeshifter now in " << shapeshifter.type().name() << " shape!" << endl;
	cout << std::any_cast<dummy>(shapeshifter).report() << " shifting the shape" << endl;
	try
	{
		int num2 = std::any_cast<int>(shapeshifter);		// старое значение int уничтожено
	}
	catch (const std::bad_any_cast & err)
	{
		cout << "Shapeshifter refused to turn into int: " << err.what() << endl;
	}

	cout << endl << "Variant section" << endl;
	std::variant<int, float, dummy> lawfulShapeshifter;		// класс, хранящий отдельные значения любого типа, которые указываются заранее
	lawfulShapeshifter.emplace<int>(42);					// метод emplace конструирует в variant объект указанного типа
	cout << "LawfulShapeshifter turned to int " << std::get<int>(lawfulShapeshifter) << endl;		// приведение к исходному типу
	lawfulShapeshifter.emplace<float>(4.2f);
	cout << "LawfulShapeshifter turned to float " << std::get<1>(lawfulShapeshifter) << endl;		// можно использовать индекс типа в перечислении типов в шаблоне
	lawfulShapeshifter.emplace<dummy>(4, 2, "The Doppelganger Senior");
	if (lawfulShapeshifter.index() == 2)					// доступ к индексу текущего типа в перечислении типов в шаблоне
	{
		dummy lsspd = std::get<dummy>(lawfulShapeshifter);
		cout << lsspd.report() << " shifting the shape" << endl;
	}
	try
	{
		int num2 = std::get<int>(lawfulShapeshifter);		// неправильное приведение генерирует исключение
	}
	catch (const std::bad_variant_access & err)
	{
		cout << "LawfulShapeshifter refused to turn into int: " << err.what() << endl;
	}

	cout << endl << "Complex number section" << endl;
	std::complex a(0.5, 1.0);
	cout << "We have complex number " << std::real(a) << " + " << std::imag(a) << "*i" << endl;
	// complex с помощью внешниех функций поддерживает все мат. операции над комплексными числами (см. справку)

	cout << endl << "Pseudorandom section" << endl;
	std::mt19937_64 oracle(91586);			// генератор псевдослучайных чисел - вихрь Мерсенна
											// инициализируется зерном, которое задает константную при этом зерне псевдослучайную последовательность
	if (auto n = oracle(); n == 8346843996631475880) cout << "I guessed first: " << n << endl;		// если известно зерно, последовательность детерминирована
	if (auto n = oracle(); n == 2237671392849523263) cout << "I guessed second: " << n << endl;
	if (auto n = oracle(); n == 7333164488732543658) cout << "I guessed third: " << n << endl;

	std::uniform_int_distribution uniform(0, 10);				// равномерное распределение целых чисел от 0 до 10
	cout << "10 random numbers from 0 to 10:" << endl;
	for (size_t i = 0; i < 10; i++)
		cout << uniform(oracle) << " ";							// получаем число вызовом объекта с передачей генератора псевдослучайных чисел
	cout << endl << "It was 6 9 10 1 1 7 9 9 2 10" << endl;		// последовательность детерминирована
	// в stdlib random представлены наиболее часто используемые распределения (равномерное, нормальное, хи-квадрат, Коши, Фишера, Стьюдента, Бернулли, биномиальное, Пуассона и др.)

	// [Lospinozo Часть 2 Глава 13]
	cout << endl << "Containers section" << endl;
	std::array<int, 10> static_array;		// массив постоянной длины
	std::array<int, 3> arr{ 0, 10, 42 };	// фигурная инициализация
	for (size_t i = 0; i < 10; i++)			// цикл для массива на основе индекса
		static_array[i] = i * i;
	for (auto it = static_array.begin(); it != static_array.end(); it++)		// цикл для массива на основе итераторов
		cout << *it << " ";
	cout << endl;
	/*
	методы begin() и end() у линейных контейнеров STL устроены так:
	begin() возвращает итератор на 1й элемент, end() - на фиктивный элемент после последнего
	 begin()	 end()
	 |			 |
	[-][-][-][-][ ]
	 1  2  3  4
	*/
	for (int& i : arr)		// цикл для массива с использованием выражения диапазона
		cout << i << " ";
	cout << endl;

	std::vector<float> vec;								// массив переменной длины
	if (vec.empty()) cout << "Vec is empty" << endl;	// empty определяет пустоту массива
	std::vector<float> vec1{ 3, 4, 42.1 };				// фигурная инициализация
	std::vector<float> vec2(vec1.begin(), vec1.end());	// копирование из диапазона, ограниченного 2мя итераторами

	vec.push_back(42.1);						// добавление нового элемента
	print_vector(vec, "Vec after push_back");

	print_vector(vec2, "Vec2 before assign");
	vec2.assign({ 0.1, 0.2, 0.3 });				// замена содержимого новым списком инициализации
	print_vector(vec2, "Vec2 after assign");

	print_vector(vec1, "Vec1 before insert");
	vec1.insert(vec1.begin() + 1, 33);			// вставка на позицию перед элементом, на который указывает переданный итератор
	print_vector(vec1, "Vec1 after insert");

	// [Lospinozo Часть 2 Глава 14]
	cout << "Vec1 has size " << vec1.size() << " and length " << std::distance(vec1.begin(), vec1.end()) << endl;		// distance вычисляет расстояние между двумя непрерывными итераторами
	auto iterFour = std::find(vec1.begin(), vec1.end(), 4);																// find ищет элемент в области между указанными итераторами (функция универсальна)
	cout << "In vec1 4 has index " << std::distance(vec1.begin(), iterFour) << endl;									// можно использовать для определения индекса элемента, если есть итератор, указывающий на него

	std::vector<float> reversedVec1{ vec1.rbegin(), vec1.rend() };		// итераторы rbegin и rend предоставляют инверсивный диапазон
	print_vector(reversedVec1, "Reversed Vec1");
	/*
	методы rbegin() и rend() у линейных контейнеров STL устроены так:
	rend() возвращает итератор на 1й элемент, rbegin() - на фиктивный элемент после последнего
	увеличение реверсивного итератора - это перемещение влево от rbegin к rend
	 rend()		 rbegin()
	 |			 |
	[-][-][-][-][ ]
	 1  2  3  4
	*/
	std::vector<float> reversedVec2;
	for (auto iter = vec2.rbegin(); iter < vec2.rend(); iter++)			// ввиду того, что для реверсивного оператора увеличение - это движение справа налево, синтаксис перебора не меняется
		reversedVec2.push_back(*iter);
	print_vector(reversedVec2, "Reversed Vec2");

	// [Lospinozo Часть 2 Глава 13]
	std::vector dummiesLine{ dummy("First"), dummy("Second"), dummy("Third") };
	print_dummies_vector(dummiesLine, "DummiesLine before emplace");
	dummiesLine.emplace_back("Forth");								// emplace_back конструирует объект с переданными аргументами и вставляет его в конец
	dummiesLine.emplace(dummiesLine.begin() + 1, "New Second");		// emplace конструирует объект с переданными аргументами и вставляет его перед элементом, на который указывает итератор
	print_dummies_vector(dummiesLine, "DummiesLine after emplace");

	std::set<int> s{ 1, 1, 2, 3, 5 };	// set (множество) - ассоциативный контейнер отсортированных уникальных ключей (модель данных - дерево)
	print_set(s, "Set s");				// вторая 1 исчезла, т.к. значения уникальные
	auto secIter = s.find(2);			// find ищет значение и возвращает итератор, указывающий на него
	auto sixIter = s.find(6);			// поиск по упорядоченным элементам происходит быстро
	if (*secIter == 2) cout << "Found element 2" << endl;
	if (sixIter == s.end()) cout << "6 is not found" << endl;

	std::multiset<int> ms{ 1, 1, 2, 3, 5 };		// multiset - аналог set, допускающие неуникальные ключи
	cout << "In multiset " << ms.count(1) << " times present element 1" << endl;

	// map - ассоциативный контейнер, хранящий пары ключ-значение; доступ к значению осуществляется по ключу (модель данных - дерево)
	std::map<string, int> cyberspace{ {"Neuromancer", 1984}, {"Count Zero", 1986}, {"Mona Lisa Overdrive", 1988} };	// фигурная инициализация парами ключ-значение
	cout << "First novel of Gibson's Cyberspace was published in " << cyberspace["Neuromancer"] << endl;			// для доступа можно использовать [], передавая ключ
	cout << "Final Countdown was published in " << cyberspace["Final Countdown"] << endl;							// но если элемента нет, [] создаст его по умолчанию
	try
	{
		cout << "Second novel of Gibson's Cyberspace was published in " << cyberspace.at("Count Zero") << endl;		// at также предоставляет доступ к значению по ключу
		cout << "Never existing novel of Gibson was published in";
		cout << " " << cyberspace.at("Not A Final Countdown") << endl;			// однако, at выбрасывает исключение, если такого ключа нет
	}
	catch (const std::out_of_range & err)
	{
		cout << "... Well, it was never published: " << err.what() << endl;
	}
	cyberspace.insert({ "Burning Chrome", 1982 });		// добавление элемента
	cyberspace.erase("Final Countdown");				// удаление элемента (также можно передать итератор)

	// [Lospinozo Часть 2 Глава 15]
	cout << endl << "String section" << endl;
	string owlbear("owlbear");
	string owl(owlbear, 0, 3);		// создание подстроки: исходная строка, индекс начала, длина
	string bear(owlbear, 3);		// создание подстроки от данной позиции до конца: исходная строка, индекс начала
	cout << owlbear << " = " << owl << " + " << bear << endl;
	string ans = to_string(42);		// перевод числа в строку
	int iAns = std::stoi(ans);
	string raw(R"(Literal \t with spec symbols \0 \n \hello"\world"\)");		// R"()" заставляет игнорировать спец. символы
	cout << "Raw string: " << raw << endl;
	// все остальные методы для манипулирования строками см. в [Lospinozo Часть 2 Глава 15]

	// [Lospinozo Часть 2 Глава 16]
	cout << endl << "Streams section" << endl;
	// средства манипулирования форматом вывода у потока
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
	cout << "Contents: " << intro.rdbuf() << endl;										// rdbuf передает буфер целиком
	intro.seekg(0);																		// seekg переводит каретку на указанную позицию
	cout << "Contents after seekg(0): " << intro.rdbuf() << endl;
	intro.seekg(-4, std::ios_base::end);												// seekg с двумя аргументами - смещение и точка отсчета
	cout << "tellg() after seekg(-4, ios_base::end): " << intro.tellg() << endl;		// tellg дает текущую позицию каретки
	cout << "Contents after seekg(-4, ios_base::end): " << intro.rdbuf() << endl;

	// [Lospinozo Часть 2 Глава 17]
	cout << endl << "Filesystem section" << endl;
	const std::filesystem::path kernel32{ R"(C:\Windows\System32\kernel32.dll)" };		// платформонезависимое представление пути
	cout << "Root name: " << kernel32.root_name()					// получение различных частей (декомпозиция) пути
		<< "\nRoot directory: " << kernel32.root_directory()
		<< "\nRoot path: " << kernel32.root_path()
		<< "\nRelative path: " << kernel32.relative_path()
		<< "\nParent path: " << kernel32.parent_path()
		<< "\nFilename: " << kernel32.filename()
		<< "\nStem: " << kernel32.stem()
		<< "\nExtension: " << kernel32.extension()
		<< endl << endl;

	fs::path path{ R"(C:/Windows/System32/kernel32.dll)" };		// модификация пути
	cout << path << endl;
	path.make_preferred();										// изменение разделителей на предпочтительные для ОС
	cout << path << endl;
	path.replace_extension(".txt");								// замена расширения
	cout << path << endl;
	path.replace_filename("win32kfull.sys");					// замена имени файла
	cout << path << endl;
	path.remove_filename();										// удаление имени файла
	cout << path << endl;
	path = path / "REAMDE";										// конкатенация путей
	cout << path << endl;
	path.clear();
	cout << "Is empty: " << std::boolalpha << path.empty() << endl << endl;

	const auto temp_path = fs::temp_directory_path();						// системная директория для временных файлов
	try
	{
		const auto relative = fs::relative(temp_path);							// получить относительный путь относительно текущей директории
		cout << std::boolalpha
			<< "Temporary directory path: " << temp_path
			<< "\nTemporary directory absolute: " << temp_path.is_absolute()	// проверка, является ли путь абсолютным
			<< "\nCurrent path: " << fs::current_path()							// текущая директория
			<< "\nTemporary directory's relative path: " << relative
			<< "\nRelative directory absolute: " << relative.is_absolute()
			<< "\nChanging current directory to temp.";
		fs::current_path(temp_path);											// установка текущей директории
		cout << "\nCurrent directory: " << fs::current_path() << endl;
	}
	catch (const std::exception & e)
	{
		std::cerr << "Error: " << e.what();				// эти функции могут выбросить ошибку, если ОС не поддерживает операцию
	}

	cout << endl << std::boolalpha << "Path: " << kernel32 << endl;
	cout << "Is directory: " << fs::is_directory(kernel32) << endl;			// указывает ли путь на директорию
	cout << "Is regular file: " << fs::is_regular_file(kernel32) << endl;	// является ли файл обычным
	cout << "Is empty: " << fs::is_empty(kernel32) << endl;					// является ли пустым файлом или каталогом

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
	// про манипуляции с файловой системой см. [Lospinozo Часть 2 Глава 17]

	// обход директории
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