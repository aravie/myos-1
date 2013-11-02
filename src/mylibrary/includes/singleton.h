
#ifndef _SINGLETON_H
#define _SINGLETON_H

template<class T>
class Singleton
{
public:
	static T* getInstance()
	{
		static T obj;
		return &obj;
	}

private:
};

#endif
