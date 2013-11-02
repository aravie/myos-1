
#include "color.h"

namespace MyLibrary
{

	Color::Color()
	{
		red = 0;
		green = 0;
		blue = 0;
	}


	Color::Color(int red, int green, int blue, int a)
	{
		this->red =red;
		this->green = green;
		this->blue = blue;
	}

	Color Color::fromRgb(int red, int green, int blue, int a)
	{
		return Color(red, green, blue, a);
	}

}; // namespace
