// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    this->MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    this->MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    this->MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
  return TBitField(BitField);
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return this->MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    this->BitField = s.BitField;
    this->MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if  (this->BitField == s.BitField && this->MaxPower ==s.MaxPower ) return 1;
    else return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (this->BitField == s.BitField && this->MaxPower == s.MaxPower) return 0;
    else return 1;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int maxsize = MaxPower;
    if (this->MaxPower < s.MaxPower) maxsize = s.MaxPower;
    TSet tmp(maxsize);
    tmp.BitField = this->BitField | s.BitField;
    return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet tmp(*this);
    tmp.InsElem(Elem);
    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(*this);
    tmp.DelElem(Elem);
    return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int maxsize = this->MaxPower;
    if (this->MaxPower < s.MaxPower) maxsize = s.MaxPower;
    TSet tmp(maxsize);
    tmp.BitField = this->BitField & s.BitField;
    return tmp;
}

TSet TSet::operator~(void) // дополнение
{
    return ~(this->BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    char tmp;
    for (size_t i = 0; i < s.GetMaxPower(); i++) {
        istr >> tmp;
        if (tmp < s.GetMaxPower() && s.IsMember(tmp) == 0)
            s.InsElem(tmp);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (size_t i = 0; i < s.GetMaxPower(); i++)
        if (s.IsMember(i)==1)
            ostr << '1';
        else ostr << '0';
    return ostr;
}
