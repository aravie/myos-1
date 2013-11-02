
#ifndef _CURSOR_H
#define _CURSOR_H

class Cursor
{
public:
	Cursor();
	Cursor(int x, int y);

	Cursor& operator++();
	void operator++(int n);
	void operator+=(int nb);

	void setLimits(int limx, int limy);
	void setPosition(const int x, const int y);

	int getY() const { return currentY; }
	int getX() const { return currentX; }

	int getLimitY() const { return limitY; }
	int getLimitX() const { return limitX; }

	void endLine();

private:
	int currentX, currentY, limitX, limitY;
};

#endif
