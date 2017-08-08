#ifndef DM_OUTPUT_H
#define DM_OUTPUT_H
#include <d3d9.h>

struct DM_Output
{
	LPDIRECT3DSURFACE9	Render;
	LPDIRECT3DSURFACE9	Depth;
};

#endif
