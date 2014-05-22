#include <Data.h>
#include <xcb/xcb_atom.h>


#ifndef ATMONS
#define ATMONS
extern xcb_atom_t WM_STATE;

void initAtoms();

#endif // ATMONS
