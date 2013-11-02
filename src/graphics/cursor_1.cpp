
#include "cursor.h"



Cursor::Cursor()
{
	currentX = 0;
	currentY = 0;
	this->limitX = 0;
	this->limitY = 0;
}


Cursor::Cursor(int limitx, int limity)
{
	currentX = 0;
	currentY = 0;
	this->limitX = limitx;
	this->limitY = limity;
}

void Cursor::setLimits(int limx, int limy)
{
	limitX = limx;
	limitY = limy;
	return;
}

void Cursor::setPosition(const int x, const int y)
{
	currentX = x;
	currentY = y;

	if(currentX > limitX)
	{
		currentY+=currentX/limitX;
		currentX %= limitX ;
	}

	if(currentY > limitY)
	{
		currentY=0;
	}

	return;
}


void Cursor::operator+=(int nb)
{
	currentX+=nb;

	if(currentX > limitX)
	{
		currentY+=currentX/limitX;
		currentX %= limitX ;
	}

	if(currentY > limitY)
	{
		currentY=0;
	}

	return;
}

Cursor& Cursor::operator++()
{
	currentX++;

	if(currentX >= limitX)
	{
		currentY++;
		currentX = 0;
	}

	if(currentY >= limitY)
	{
		currentY=0;
	}

	return *this;
}


void Cursor::operator++(int n)
{
	currentX+=1;

	if(currentX >= limitX)
	{
		currentY+=1;
		currentX = 0;
	}

	if(currentY >= limitY)
	{
		currentY=0;
	}
	return;
}

void Cursor::endLine()
{
	currentY++;
	currentX = 0;

	if(currentY > limitY)
		currentY=0;

	return;
}

