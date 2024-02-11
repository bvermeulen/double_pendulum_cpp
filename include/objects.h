#ifndef OBJECTS_H
#define OBJECTS_H
#include <wx/wx.h>

class BasicDrawPane;

class CircleObject
{
public:
	int x, y, lineWidth;
	float radius;
	BasicDrawPane *bdpPointer;
	const wxBrush *brushFillColorPointer;
	CircleObject(BasicDrawPane &_bdp, int _x, int _y, float _radius, const wxBrush *_brushFillColorPointer, int _lineWidth);
	CircleObject();
	void draw();
	void update(int x, int y);
	bool mouseHover(int x, int y);
};

#endif // OBJECTS_H