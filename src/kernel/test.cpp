

#include "heap.h"
#include "vga.h"
#include "linked_list.h"
#include "stack.h"
#include "queue.h"
#include "drawer.h"
#include "vector.h"
#include "vector2d.h"
#include "color.h"


void testGraphic()
{
	//Vector<int> v1(2);
		//Vector<int> v2(2);
		Vector2D<int> v4(200,200);
		Vector2D<int> v5(200,400);
		Vector2D<int> v6(400,400);
		Vector2D<int> v7(200,600), v8(100,100);
		Vector2D<int> v9(300,400), v10(600,10);

		Vector2D<int> v11(800,100), v12(950,350);

		drawer->drawPixel(v4+v6);
		drawer->drawPixel(v5+v8);

		drawer->drawLine(v4, v5, Color::fromRgb(0xFF, 0x00, 0x00));
		drawer->drawLine(v5, v6,  Color::fromRgb(0xFF, 0x00, 0xFF));
		drawer->drawLine(v8, v7, Color::fromRgb(0x00, 0xFF, 0x00));
		drawer->drawLine(v9, v10, Color::fromRgb(0x00, 0x00, 0xFF));
		//drawer->drawLine(v9, v7, Color::fromRgb(0x00, 0x00, 0xFF));
		//drawer->drawLine(v4, v12, Color::fromRgb(0x00, 0xFF, 0x00));
		drawer->drawRectangle(v12,v11, Color::fromRgb(0xFF, 0xFF, 0));
		//drawer->drawLine(v9, v5, Color::fromRgb(0x00, 0xFF, 0x00));
}


