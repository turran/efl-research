/* FT2_DEBUG="ttgload:6" LD_LIBRARY_PATH=/home/jl/tmp/freetype-2.3.9/objs/.libs/ ./ft */

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library;
FT_Face face;

int main(void)
{
	FT_UInt index;
	char ch = 0x41;

	FT_Init_FreeType(&library);
	FT_New_Face(library, "/usr/share/fonts/truetype/freefont/FreeSans.ttf", 0,  &face);
	index = FT_Get_Char_Index(face, ch);
	FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);

	return 0;
}
