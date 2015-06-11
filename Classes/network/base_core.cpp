#include "base_core.h"

unsigned int bit32_extract(unsigned int value, int pos, int size)
{
	return (((value >> pos) & MASK(size)));
}
