

#ifndef _STRING_H
#define _STRING_H

#include "array.h"


namespace MyLibrary
{

	class String : protected Array<char>
	{
	public:
		String();
		String(const String& s);
		String(const char *str);


		//bool isNull() const { return Array::isEmpty(); }
		//bool isEmpty1() const;

		inline int size() const { return vsize-1; }
		inline int length() const { return vsize-1; }
		inline int count() const { return vsize-1; }

		char *data() const { return vdata; }

		static String fromNumber(unsigned int nb, const int base = 10);
		static String fromUtf8(const char *s);

		String& operator=(const char* s);
		String& operator=(const String& s);
		String& operator+=(const String& s);
		String& operator+=(const char *s);

		bool operator==(const String& s) const;
		//bool operator!=(const String& s) const { return !operator==(s); }


	protected:
	};

	String operator+(const String& s1, const String& s2);
	String operator+(const char *s1, const String& s2);
	String operator+(const String& s2, const char *s1);


}

#endif


