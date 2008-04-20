#ifndef TRANSFORMER_H_
#define TRANSFORMER_H_

#define MATRIX_XX 0
#define MATRIX_XY 1
#define MATRIX_XZ 2
#define MATRIX_YX 3
#define MATRIX_YY 4
#define MATRIX_YZ 5
#define MATRIX_ZX 6
#define MATRIX_ZY 7
#define MATRIX_ZZ 8
#define MATRIX_SIZE 9

#define ENESIM_TRANSFORMATIONS 3

typedef enum
{
	ENESIM_TRANSFORMATION_IDENTITY = (1 << 0),
	ENESIM_TRANSFORMATION_AFFINE = (1 << 1),
	ENESIM_TRANSFORMATION_PROJECTIVE = (1 << 2),
} Enesim_Transformer_Type;

struct _Enesim_Transformation
{
	float matrix[MATRIX_SIZE];
	Enesim_Transformer_Type type;
	int quality; // TODO fix this
};

typedef struct _Enesim_Transformer
{
	
} Enesim_Transformer;

/* identity[quality][xscale][yscale]
 * affine[quality][xscale][yscale]
 * projective[quality][xscale][yscale]
 */

//typedef void (*Enesim_Transformer_Func)()

#endif /*TRANSFORMER_H_*/
