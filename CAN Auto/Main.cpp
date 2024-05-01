#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::ios_base;
using std::sort;

#include "Log.h"

void PrintBinary(int num);

int main()
{
	vector<Log> vec;

	string str;
	int res = 0; // результат содержимого байта
	bool flagID; // флаг для ID
	int count; // счётчик группы разрядов по 4 бита
	int byteNumber; // количество байт в сообщении
	bool match; // флаг, что есть совпадение байта с маской

	ifstream file;
	file.open("01010_ЦЗ.log");
	if (file.is_open())
	{
		cout << "File is open: " << endl;
		while (!file.eof())
		{
			getline(file, str); // получаю строку из файла и переписываю в str
			
			const char* pstr = str.c_str();

			flagID = false;
			count = 0;
			byteNumber = 0;
			match = false;
			Log log;

			while (*pstr) // перебираю строку
			{
				pstr++;
				if (*pstr == 'x') // считываю 16-ричное число
				{
					count = 0;
					pstr++;
					while (*pstr != ' ') // считываю 16-ричное число до разделителя
					{
						int tmp = 0;
						if (*pstr == '0') { tmp = 0; }
						else if (*pstr == '1') { tmp = 1; }
						else if (*pstr == '2') { tmp = 2; }
						else if (*pstr == '3') { tmp = 3; }
						else if (*pstr == '4') { tmp = 4; }
						else if (*pstr == '5') { tmp = 5; }
						else if (*pstr == '6') { tmp = 6; }
						else if (*pstr == '7') { tmp = 7; }
						else if (*pstr == '8') { tmp = 8; }
						else if (*pstr == '9') { tmp = 9; }
						else if (*pstr == 'A' || *pstr == 'a') { tmp = 10; }
						else if (*pstr == 'B' || *pstr == 'b') { tmp = 11; }
						else if (*pstr == 'C' || *pstr == 'c') { tmp = 12; }
						else if (*pstr == 'D' || *pstr == 'd') { tmp = 13; }
						else if (*pstr == 'E' || *pstr == 'e') { tmp = 14; }
						else if (*pstr == 'F' || *pstr == 'f') { tmp = 15; }

						if (count == 0)
						{
							res = tmp;
						}
						else
						{
							res = (res << 4) | tmp;
						}

						count++;
						pstr++;
					}

					if (!flagID)
					{
						log.SetID(res);
						flagID = true;
					}
					else
					{
						log[byteNumber] = res;
						byteNumber++;
					}
					res = 0;
				}
			}
			
			log.SetQuantityBytes(byteNumber);
			vec.push_back(log);
			
		}
	}
	else
	{
		cout << "File is not founded" << endl;
	}
	file.close();

	// сортирую исходный вектор:
	sort(vec.begin(), vec.end(),
		[](const auto& a, const auto& b)
		{
			return a.GetID() < b.GetID();
		});

	//// Предварительный вывод исходного отсортированного вектора
	//cout << "Vector: " << endl;
	//for (int i = 0; i < vec.size(); i++)
	//{
	//	cout << std::dec << i << " ";
	//	cout << "ID = " << std::hex << vec[i].GetID() << "; ";
	//	for (int j = 0; j < vec[i].GetQuantityBytes(); j++)
	//	{
	//		//cout << std::dec << vec[i][j] << " ";
	//		PrintBinary(vec[i][j]);
	//		cout << " ";
	//	}
	//	cout << endl;
	//}
	//cout << "\n" << endl;

	cout << "Origin vec size =  " << std::dec << vec.size() << endl;

	vector<Log> resultVec; // результирующий вектор для хранения найденных логов

	vector<Log>::iterator itb = vec.begin(), ite = vec.end();
	int countAllElem = 0;
	while (itb != ite) // прохожу исходный вектор
	{
		static int index = 0;
		int countSameId = 1;

		while (vec[index].GetID() == vec[index + 1].GetID()) // считаю количество логов с одинаковым ID
		{
			countSameId++;
			index++;
			itb++;
			countAllElem++;
			if (index == vec.size() - 1) // последний ID равен предыдущему ID
			{
				break;
			}
		}
		cout << std::hex << vec[index].GetID() << "=" << std::dec << countSameId << "; ";

		if (countSameId > 1) // если есть несколько логов подряд с одинаковым ID
		{
			unsigned int mask = 0x80;
			int countChange = 0; // сколько раз менялся бит
			bool addIndex = false; // добавляем логи с этим ID в результирующий вектор или нет
			int numByte;
			int numBit;
			for (int k = 0; k < vec[index].GetQuantityBytes(); k++) // проход по байтам массива лога
			{
				for (int b = 7; b >= 0; b--) // проход по битам байта
				{
					for (int i = index + 1 - countSameId; i <= index - 1; i++) // проход по логам с одинаковым ID
					{
						for (int j = i; j <= index - 1; j++) // проход по логам с одинаковым ID
						{
							int res1 = vec[j][k] & mask;
							int res2 = vec[j + 1][k] & mask;
							if ((res1) != (res2))
							{
								countChange++;
							}
						}
					}
					if (countChange >= (countSameId / 2)) // биты часто менялись
					{
						addIndex = true;
						numBit = b;
						break;
					}
					mask = mask >> 1;
				}
				if (addIndex)
				{
					numByte = k;
					break;
				}
			}

			if (addIndex)
			{
				for (int i = index + 1 - countSameId; i <= index; i++)
				{
					vec[i].SetNumberByte(numByte);
					vec[i].SetNumberBit(numBit);
					resultVec.push_back(vec[i]);
				}
			}
			
		}

		index++;
		itb++;
		countAllElem++;
		if (index == vec.size() - 1) // ID последнего лога вектора не равен предыдущему ID
		{
			cout << std::hex << vec[index].GetID() << "=" << std::dec << 1 << "; ";
			countAllElem++;
			break;
		}
	}
	cout << endl;
	cout << "countAllElem  =  " << countAllElem << endl;


	cout << "Result Vector size =  " << std::dec << resultVec.size() << endl;

	cout << "Result Vector: " << endl;
	for (int i = 0; i < resultVec.size(); i++)
	{
		cout << std::dec << i << " ";
		cout << "ID = " << std::hex << resultVec[i].GetID() << "; ";
		for (int j = 0; j < resultVec[i].GetQuantityBytes(); j++)
		{
			//cout << std::dec << resultVec[i][j] << " ";
			PrintBinary(resultVec[i][j]);
			cout << " ";
		}
		cout << " Byte = " << resultVec[i].GetNumberByte() << " Bit = " << resultVec[i].GetNumberBit();
		cout << endl;
	}
	cout << "\n" << endl;
	



	return 0;
}


void PrintBinary(int num)
{
	unsigned int maska = 0x80;
	int ost;
	int k = 0;
	int ar = 8;
	while (ar > 0)
	{
		ost = num & maska;
		if (ost == 0)
		{
			cout << ost << "";
		}
		else
		{
			cout << 1 << "";
		}
		maska = maska >> 1;
		ar--;
	}
}