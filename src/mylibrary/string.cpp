
#include "string.h"
#include "types.h"
//#include "debug.h"

namespace MyLibrary
{

	String::String()
	{
		Array();
	}

	String::String(const String& s)
	{
		Array();
		resize(s.vsize);
		for(int i=0; i<vsize; i++)
			vdata[i] = s.vdata[i];
	}


	String::String(const char *str)
	{
		Array();
		int length = 0;
		char *cursor = (char*)str;
		while(*cursor != '\0')
		{
			length++;
			cursor++;
		}
		resize(length+1);

		// on copie la chaine ac le symbole \0 dans le string
		for(int i=0; i<vsize; i++)
			vdata[i] = str[i];

		vdata[vsize] = '\0';
	}


	String& String::operator=(const char* str)
	{
		int length = 0;
		char *cursor = (char*)str;
		while(*cursor != '\0')
		{
			length++;
			cursor++;
		}
		resize(length+1);

		const int end=vsize;
		// on copie la chaine ac le symbole \0 dans le string
		for(int i=0; i<end; i++)
			vdata[i] = str[i];

		vdata[end-1] = '\0';

		return *this;
	}

	String& String::operator=(const String& str)
	{
		resize(str.vsize);
		for(int i=0; i<vsize; i++)
			vdata[i] = str.vdata[i];

		return *this;
	}


	String& String::operator+=(const String& s)
	{
		// On calcule la taille que va prendre la chaine en mémoire
		// On compte d'abord le nombre de caractere
		int length_without_eos = vsize + s.vsize - 2;
		char *tab = new char[vsize];
		for(int i=0; i<vsize; i++)
			tab[i] = vdata[i];

		const int vsize_before = vsize;

		// Taille de la concaténation des chaines + caractere eos '\0'
		resize(length_without_eos + 1);

		// On recopie dans la première partie la chaine du premier string
		for(int i=0; i<vsize_before-1; i++)
		{
			vdata[i] = tab[i];
		}

		delete tab;

		const int vsize_now = vsize;
		const int vsize_s = s.vsize;
		// Puis on recopie celle du second
		for(int i=0; i<vsize_s; i++)
		{
			vdata[vsize_before-1 + i] = s.vdata[i];
		}

		vdata[vsize_now-1] = 0;
		return *this;
	}

	String& String::operator+=(const char *s)
	{
		String str(s);
		*this += str;
		return *this;
	}


	String operator+(const String& s1, const String& s2)
	{
		String str = s1;
		str += s2;
		return str;
	}

	String operator+(const char* s1, const String& s2)
	{
		String st1 = s1;
		st1 += s2;
		return st1;
	}

	String operator+(const String& s1, const char* s2)
	{
		String st1 = s1;
		st1 += s2;
		return st1;
	}

	String String::fromUtf8(const char *s)
	{
		String str(s);
		//str.resize(10);
		//str.data()[0] = 0;
		return str;
	}

	String String::fromNumber(unsigned int nb, const int base)
	{
		if(!(base<=16 && base>=2))
			return String("");

		u8 digit;
		char digits_tab[] = "0123456789ABCDEF";
		String str;

		if(nb == 0)
		{
			str = "0";
			return str;
		}

		char tab[sizeof(int)*8];
		int max_digit = 0;

		for(int i=sizeof(int)*8-1; i>=0; i--)
		{
			digit = nb % base;
			nb -= digit;
			nb /= base;
			tab[i] = digits_tab[digit];
			if(tab[i] != '0')
				max_digit = sizeof(int)*8-i;
		}

		str.resize(max_digit+1);
		for(int i=0; i<max_digit; i++)
			str.vdata[i] = tab[sizeof(int)*8-max_digit+i];

		str.vdata[max_digit] = 0;
		return str;
	}


	bool String::operator==(const String& s) const
	{
		if(vsize != s.vsize)
			return false;

		for(int i=0; i<vsize; i++)
		{
			if(vdata[i] != s.vdata[i])
				return false;
		}

		return true;
	}

};


