#include "Window.h"
#include "xcb/xcb.h"
#include "xcb_icccm.h"
using namespace std;


Window::Window(xcb_window_t window)
{
    id = window;

}

Window::~Window()
{


}
void Window::Raise()
{
    uint32_t values[] = { XCB_STACK_MODE_ABOVE };



    xcb_configure_window(connection, id,
                         XCB_CONFIG_WINDOW_STACK_MODE,
                         values);
    xcb_flush(connection);
}

/*
 * Set window client to either top or bottom of stack depending on
 * where it is now.
 */
void Window::RaiseOrLower()
{
    uint32_t values[] = { XCB_STACK_MODE_OPPOSITE };

    xcb_configure_window(connection, id,
                         XCB_CONFIG_WINDOW_STACK_MODE,
                         values);


    xcb_flush(connection);
}
