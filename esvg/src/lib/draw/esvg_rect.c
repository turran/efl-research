/* rect
 * Core.attrib, Conditional.attrib, Style.attrib, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, x, y, width, height, rx, ry, transform 
 */
struct _ESVG_Rect
{
	ESVG_Shape shape;
	ESVG_Coord x;
	ESVG_Coord y;
	ESVG_Length rx;
	ESVG_Length ry;
	ESVG_Length width;
	ESVG_Length height;
};
