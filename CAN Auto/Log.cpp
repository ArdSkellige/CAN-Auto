#include "Log.h"

#include <iostream>

using std::cout;
using std::endl;

Log::Log(int id, int quantity, int num) : logID(id), quantityBytes(quantity), numberByte(num)
{
}

Log::~Log()
{
}

int Log::GetArrSize() const
{
	return sizeof(arr) / sizeof(arr[0]);
}

void Log::SetID(int id)
{
	logID = id;
}

int Log::GetID() const
{
	return logID;
}

void Log::SetQuantityBytes(int quan)
{
	quantityBytes = quan;
}

int Log::GetQuantityBytes() const
{
	return quantityBytes;
}

void Log::SetNumberByte(int num)
{
	numberByte = num;
}

int Log::GetNumberByte() const
{
	return numberByte;
}

unsigned int& Log::operator[](unsigned int num)
{
	return arr[num];
}

void Log::SetNumberBit(int num)
{
	numberBit = num;
}

int Log::GetNumberBit() const
{
	return numberBit;
}