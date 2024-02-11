#ifndef OBJECTS_H
#define OBJECTS_H
#include <wx/wx.h>

class BasicDrawPane;

class CircleObject
{
private:
	int x, y;
	int lineWidth;
	float radius;
	BasicDrawPane *bdpPointer;
	const wxBrush *brushFillColorPointer;
	const wxColor *colorPointer;

public:
	CircleObject(
		BasicDrawPane &_bdp,
		int _x,
		int _y,
		float _radius,
		const wxBrush *_brushFillColorPointer,
		int _lineWidth,
		const wxColor *_colorPointer);
	CircleObject();
	void draw();
	void update(int x, int y);
	bool mouseHover(int x, int y);
};


class LineObject
{
private:
	int x1, y1, x2, y2;
	int lineWidth;
	BasicDrawPane *bdpPointer;
	const wxColor *colorPointer;

public:
	LineObject(
		BasicDrawPane &_bdp,
		int _x1,
		int _y1,
		int _x2,
		int _y2,
		int _lineWidth,
		const wxColor *_colorPointer);
	LineObject();
	void draw();
	void update(int x1, int y1, int x2, int y2);

};

#endif // OBJECTS_H