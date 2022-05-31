#include <iostream>
#include <string>
#include <math.h>

using namespace std;

string toBase10(string number, int base)
{
	double result = 0;
	int fraction = -1; // обозначает индекс символа точка или запятая в строке с числом
	if (number.find('.') != string::npos)
		fraction = number.find('.');
	else if (number.find(',') != string::npos)
		fraction = number.find(',');

	for (int i = 0; i < number.length(); i++)
	{
		int power;
		if (fraction == -1)
			power = number.length() - i - 1;
		else
		{
			if (i < fraction)
				power = fraction - i - 1;
			else if (i > fraction)
				power = fraction - i;
			else
				continue; // i==fraction, то есть number[i] это точка или запятая, значит нам не нужно ничего делать на этой итерации цикла
		}
		cout << "Число в 10 с.с. += " << stoi(number.substr(i, 1), nullptr, base) << " * " << base << '^' << power << endl;
		result += stoi(number.substr(i, 1), nullptr, base) * pow(base, power);
	}
	return to_string(result);
}

string fromBase10(string number, int base)
{
	string result;
	int fraction = -1;
	if (number.find('.') != string::npos)
		fraction = number.find('.');
	else if (number.find(',') != string::npos)
		fraction = number.find(',');
	string integerPart = number.substr(0, fraction);	 // целая часть
	string fractionPart = '0' + number.substr(fraction); // дробная часть, дописываем ноль в начале

	// Перевод целой части в целевую с.с.
	char *newSymbolBuffer = new char[2]; // Нужен для работы _itoa_s, туда в виде строки будут записываться отдельные цифры целевой с.с. в процессе перевода
	int buffer = stoi(integerPart);
	cout << "Перевод целой части числа из 10 с.с. в " << base << " с.с." << endl;
	int lastBuffer = 0; // Нужно только чтобы выводить в консоль процесс вычислений, используется в строке 60
	while (buffer >= base)
	{
		_itoa(buffer % base, newSymbolBuffer, base);
		cout << "Записываем в начало строки цифру " << newSymbolBuffer << ", которая является остатком от деления " << buffer << " на основание " << base << endl;
		result.insert(0, newSymbolBuffer);
		lastBuffer = buffer;
		buffer = buffer / base;
	}
	_itoa(buffer, newSymbolBuffer, base);
	cout << "Записываем в начало строки цифру " << newSymbolBuffer << ", которая является результатом деления без остатка числа " << lastBuffer << " на основание  " << base << endl;
	result.insert(0, newSymbolBuffer);

	// Перевод дробной части в целевую с.с.
	double doubleFractionPart = stod(fractionPart);
	if (doubleFractionPart == 0) // Если дробная часть пустая то мы не пытаемся её перевести
		return result;
	result.append(".");
	// Длина строки с целой частью числа в целевой с.с., нужна чтобы отсчитывать сколько чисел после запятой мы перевели, чтобы ограничивать точность перевода дробной части
	int wholePartResSize = result.length();
	cout << "Перевод дробной части числа из 10 с.с. в " << base << " с.с." << endl;
	while (result.length() - wholePartResSize < 5 && doubleFractionPart > 0)
	{
		double lastDoubleFractionPart = doubleFractionPart; // Нужно только чтобы выводить в консоль процесс вычислений, используется в строке 75
		doubleFractionPart = doubleFractionPart * base;
		_itoa((int)doubleFractionPart, newSymbolBuffer, base);
		cout << "Добавляем в дробную часть цифру " << newSymbolBuffer << " которая получилась после отброса дробной части от произведения " << doubleFractionPart << " * " << base << endl;
		result.append(newSymbolBuffer);
		double notUsed; // Эта переменная не используется, нужна для работы modf
		doubleFractionPart = modf(doubleFractionPart, &notUsed);
	}
	delete[] newSymbolBuffer;

	return result;
}

// Проверяет правильность введённой строки с числом
bool isNumberInputCorrect(string number, int base)
{
	if (number.length() == 0)
		return false;
	for (int i = 0; i < number.length(); i++)
	{
		// Только символы ,.-0123456789ABCDEFabcdef
		if (base > 10) // Если основание системы счисления больше 10 то допустимы буквы которые проверяются по формуле дальше
		{
			if (number[i] >= '0' && number[i] <= '9' || number[i] >= 'A' && number[i] <= ('A' + base - 10 - 1) || number[i] >= 'a' && number[i] <= ('a' + base - 10 - 1))
				continue;
		}
		else if (number[i] >= '0' && number[i] <= ('0' + base - 1)) // Если основание 10 или меньше то допустимы только цифры
			continue;
		if (number[i] == ',' || number[i] == '.' || number[i] == '-')
			continue;
		return false;
	}
	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	int base1 = 0;
	string number;
	int base2 = 0;
	while (base1 < 2 || base1 > 16) // // Основание не может быть меньше 2 и никому не нужно основание больше 16
	{
		cout << "Введите основание с.с. в которой находится ваше число: ";
		cin >> base1;
	}
	while (!isNumberInputCorrect(number, base1))
	{
		cout << "Введите число: ";
		cin >> number;
	}
	while (base2 < 2 || base2 > 16)
	{
		cout << "Введите основание с.с. в которую вы хотите первести ваше число: ";
		cin >> base2;
	}
	if (number[0] != '-')
	{
		cout << fromBase10(toBase10(number, base1), base2) << endl;
	}
	else
	{
		number.erase(0, 1);
		number = fromBase10(toBase10(number, base1), base2);
		number.insert(0, "-");
		cout << number << endl;
	}
	return 0;
}
