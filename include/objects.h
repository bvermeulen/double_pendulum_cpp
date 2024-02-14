#ifndef OBJECTS_H
#define OBJECTS_H
#include <wx/wx.h>

class DrawingPane;

class CircleObject
{
private:
	int x, y;
	int lineWidth;
	float radius;
	const wxBrush *brushFillColorPointer;
	const wxColor *colorPointer;
	DrawingPane *bdpPointer;

public:
	CircleObject(
		DrawingPane &_bdp,
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
	const wxColor *colorPointer;
	DrawingPane *bdpPointer;

public:
	LineObject(
		DrawingPane &_bdp,
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

class TracerObject
{
	private:
		wxVector<wxPoint> tracerVector;
		int lineWidth;
		const wxColor *colorPointer;
		DrawingPane *bdpPointer;

	public:
		TracerObject(
			DrawingPane &_bdp,
			int _lineWidth,
			const wxColor *_colorPointer);
		TracerObject();
		void draw();
		void update(float x, float y);

};

#endif // OBJECTS_H