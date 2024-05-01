#pragma once

// logID - ID сообщения
// quantityBytes - количество байт в сообщении
// numberByte - номер нужного байта
// numberBit - номер нужного бита
// SetQuantityBytes(int quan) - установить количество значимых байт
// GetQuantityBytes() const - получить фактическое количество байт в логе
// SetNumberByte(int num) - установить номер нужного байта
// GetNumberByte() const - получить номер нужного байта

class Log
{
private:
	unsigned int arr[8]{};
	int logID;
	int quantityBytes;
	int numberByte;
	int numberBit;
public:
	Log(int id = 0, int quantity = 0, int num = 0);
	~Log();

	int GetArrSize() const;
	void SetID(int id);
	int GetID() const;
	void SetQuantityBytes(int quan);
	int GetQuantityBytes() const;
	void SetNumberByte(int num);
	int GetNumberByte() const;
	void SetNumberBit(int num);
	int GetNumberBit() const;

	unsigned int& operator[](unsigned int num);
};

