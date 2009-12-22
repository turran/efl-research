#include "ttf.h"

/* TODO we need to add this to eina */
#define f26p6 uint32_t

typedef struct _ttf_instr
{
	char op;
} ttf_instr;

typedef struct _ttf_instr_vm
{
	char auto_flip;
	f26p6 cvtci;
	int delta;
	int delta_shift;
	/* dual projection vector */
	/* freedom vector */
	char instrctrl;
	char loop;
	f26p6 mindist;
	/* projection vector */
	int rst;
	int rp[3];
	char scanctrl;
	int single_width_cut_in;
	f26p6 single_width_cut_value;
	int zp[3];

} ttf_instr_vm;

void ttf_instr_vm_default(ttf_instr_vm *vm)
{
	vm->auto_flip = 1;
	vm->cvtci = 17/16;
	vm->delta = 9;
	vm->delat_shift = 3;
	vm->instrctrl = 0;
	vm->loop = 1;
	vm->mindist = 1;
	vm->rst = 1;
	vm->rp[0] = 0;
	vm->rp[1] = 0;
	vm->rp[2] = 0;
	vm->scanctrl = 0;
	vm->single_width_cut_in = 0;
	vm->single_width_value = 0;
	vm->zp[0] = 1;
	vm->zp[1] = 1;
	vm->zp[2] = 1;
}

/* The instruction set */
void ttf_instr_parse(Font *f, char *ptr, int ilength)
{
	char *end = ptr + ilength;

	while (ptr < end)
	{
		switch (*ptr)
		{
			default:
			break;
		}
		ptr++;
	}
}
