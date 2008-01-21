#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/* path
 * Core.attrib, Conditional.attrib, External.attrib, Style.attrib, transform, d, pathLength, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, Marker.attrib
 */
struct _ESVG_Path
{
	 
};

/* rect
 * Core.attrib, Conditional.attrib, Style.attrib, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, x, y, width, height, rx, ry, transform 
 */
struct _ESVG_Rect
{
	
};
/* line
 * Core.attrib, Conditional.attrib, Style.attrib, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, x1, y1, x2, y2, transform
 */
struct _ESVG_Line
{
	
};

/* circle
 * Core.attrib, Conditional.attrib, Style.attrib, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, cx, cy, r, transform
 */ 
struct _ESVG_Circle
{
	
};
/* ellipse
 * Core.attrib, Conditional.attrib, Style.attrib, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, cx, cy, rx, ry, transform
 */ 
struct _ESVG_Ellipse
{
	
};
/* polyline
 * Core.attrib, Conditional.attrib, Style.attrib, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, points, transform
 */
struct _ESVG_Polyline
{
	
};
/* polygon
 * Core.attrib, Conditional.attrib, Style.attrib, GraphicalEvents.attrib, Paint.attrib, Paint.attrib, Opacity.attrib, Graphics.attrib, Cursor.attrib, Filter.attrib, Mask.attrib, GraphicalEvents.attrib, Clip.attrib, points, transform
 */
struct _ESVG_Polygon
{
	
};

struct _ESVG_Shape
{
	/* state */
	/* attributes */
	/* implementation */
	Ekeko_Object *object;	
};
