#include "atmons.h"
#include "string.h"
#include "stdlib.h"
xcb_atom_t WM_STATE;

xcb_atom_t getatom(char *name)
{
    xcb_intern_atom_cookie_t atom_cookie;
    xcb_atom_t atom;
    xcb_intern_atom_reply_t *rep;

    atom_cookie = xcb_intern_atom(connection, 0, strlen(name), name);
    rep = xcb_intern_atom_reply(connection, atom_cookie, NULL);
    if (NULL != rep)
    {
        atom = rep->atom;
        free(rep);
        return atom;
    }

    return 0;
}
void initAtoms(){
    WM_STATE = getatom("WM_STATE");
}

