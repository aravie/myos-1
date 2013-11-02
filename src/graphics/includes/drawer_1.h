
#ifndef _DRAWER_H
#define _DRAWER_H

#include "singleton.h"
#include "vesa.h"
#include "cursor.h"
#include "vector2d.h"
#include "color.h"
#include "string.h"

using namespace MyLibrary;


#define CHAR_WIDTH 9
#define CHAR_HEIGHT 9


class Drawer : public Singleton<Drawer>
{
	friend class Singleton<Drawer>;
public:
	//enum VerticalAlignement{ LEFT, CENTER, RIGHT };


	void setScreenResolution(int xres, int yres);
	void setDrawPixelPrimitive(void (*dP)(u16, u16, u8, u8, u8));

	void drawPixel(int x, int y, char red=0xFF, char green=0xFF, char blue=0xFF);
	void drawPixel(const Vector2D<int> &v, const Color& c = Color(0xFF,0xFF, 0xFF));

	void drawLine(const Vector2D<int> &v1, const Vector2D<int> &v2, const Color& c = Color(0xFF, 0xFF, 0xFF));

	void drawRectangle(const Vector2D<int> &v1, const Vector2D<int> &v2, const Color& c = Color(0xFF, 0xFF, 0xFF));

	void clearScreen();

	void putChar(u16 x, u16 y, u8 car);
	void print(u16 x, u16 y, const String &str);

	void putChar(u8 car);
	void print(const String &str);

private:
	Drawer();

	void (*drawPxl)(u16 x, u16 y, u8 red, u8 green, u8 blue);

	Cursor cursor;
	int XResolution;
	int YResolution;
};

#endif

#define drawer Drawer::getInstance()
