// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) throw exception("length is negative or equal to zero");
	this->BitLen = len;
	this->MemLen = len / (sizeof(int) * 8);//длина памяти, для 32 бит нужен 1 инт, для 64 бит нужно 2 инта и тд; выделяем длину памяти чтобы влезло количество бит
	if (this->MemLen * sizeof(TELEM) * 8 < len)
	{
		MemLen++;
	}
	this->pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++) this->pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	this->BitLen = bf.BitLen;
	this->MemLen = bf.MemLen;
	this->pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++) {
		this->pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n, получить позицию в памяти pmem
{
	//бит в элементе bitsinelem это sizeof(telem)*8
	//метод вернет индекс элемента в массиве pmem который соответствует позиции n
	return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const  // битовая маска для бита n
// получаем маску которая установит 1 на номер бита n
{
	return 1 << (n % ((sizeof(TELEM)*8)-1));
	//выражение в скобках - индекс бита в конкретном байте
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return this->BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= BitLen || n<0) throw exception("bit pos is out of range");
	else pMem[GetMemIndex(n)] |= GetMemMask(n); // применили маску, значит установили нужный бит в 1
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= BitLen || n<0) throw exception("bit pos is out of range");
	else pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen || n<0) throw exception("bit pos is out of range");
	TELEM res = pMem[GetMemIndex(n)] & GetMemMask(n);
		
	if (res!=0) return 1;
	else return 0;

}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	this->BitLen = bf.BitLen;
	if (this->MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		TELEM* tmp = new TELEM[bf.MemLen];
		delete[]pMem;
		pMem = tmp;
	}
	for (size_t i = 0; i < bf.MemLen; i++) {
		this->pMem[i] = bf.pMem[i];
	}

	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (this->BitLen != bf.BitLen) return 0;
	for (size_t i = 0; i < bf.MemLen; i++) {
		if (this->pMem[i] != bf.pMem[i]) return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int maxBitLen = BitLen;
	if (this->BitLen < bf.BitLen) maxBitLen = bf.BitLen;
	TBitField tmp(maxBitLen);

	for (size_t i = 0; i < min(MemLen, bf.MemLen); i++) {
		tmp.pMem[i] = this->pMem[i] | bf.pMem[i];
	}

	return tmp;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxBitLen = BitLen;
	if (this->BitLen < bf.BitLen) maxBitLen = bf.BitLen;
	TBitField tmp(maxBitLen);
	for (size_t i = 0; i < min(MemLen, bf.MemLen); i++) {
		tmp.pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(*this);
	for (size_t i = 0; i < BitLen; i++) {
		if (GetBit(i) == 1) tmp.ClrBit(i);
		else tmp.SetBit(i);
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char tmp;
	for (size_t i = 0; i < bf.GetLength(); i++)
	{
		istr >> tmp;
		if (tmp == '1') bf.SetBit(i);
		if (tmp == '0') bf.ClrBit(i);
	}
	return istr;

}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (size_t i = 0; i < bf.GetLength(); i++) {
		if (bf.GetBit(i) == 1) ostr << '1';
		else ostr << '0';
	}

	return ostr;
}
