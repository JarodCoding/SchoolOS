#ifndef WINDOW
#define WINDOW


#include <xcb/xcb_util.h>
#include "Structures.h"
#include "Data.h"

using namespace std;


class Window
{
    public:
        Window(xcb_window_t window);
        virtual ~Window();
        void Raise();
        void RaiseOrLower();
    protected:
    private:
    xcb_window_t id;                // Window ID
    struct sizepos mode0;            // Size and Position in Mode 0
    struct sizepos mode1;            // Size and Position in Mode 1
    bool mode;                       // The Mode of the Window. Switched with double click.
    uint16_t min_width, min_height; // Hints from application
    uint16_t max_width, max_height;
    int32_t width_inc, height_inc;
    int32_t base_width, base_height;


};

#endif

