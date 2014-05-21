#include "Tile.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "stdint.h"
#include <sstream>
#include "Desktop.h"
#include "Data.h"
#include "atmons.h"
using namespace std;
Tile::Tile()
{

}

Tile::~Tile()
{
    //dtor
}
void Tile::load(string data){
    vector< string> res;
    using namespace boost::algorithm;
    split(res, data, is_any_of(";"));
    using namespace std;
    position.x = atoi(res.at(0).c_str());
    position.y = atoi(res.at(1).c_str());
    position.width = atoi(res.at(2).c_str());
    position.height = atoi(res.at(3).c_str());
    TileIndex = atoi(res.at(4).c_str());


}


string Tile::getData(){
ostringstream s;
s <<position.x<<";"<<position.y<<";"<<position.width<<";"<<position.height<<";"<<TileIndex;
return s.str();
}
void Tile::setIndex(int index){
TileIndex = index;
}
int Tile::getIndex(){
return TileIndex;
}
void Tile::setLocalX(uint16_t x){
position.x = x;
const static uint32_t values[] = { x };
xcb_configure_window (connection, runtime, XCB_CONFIG_WINDOW_X, values);
}
void Tile::setLocalY(uint16_t y){
position.y = y;
const static uint32_t values[] = { y };
xcb_configure_window (connection, runtime, XCB_CONFIG_WINDOW_Y, values);
}
void Tile::setLocalWidth(uint16_t width){
position.width = width;
const static uint32_t values[] = { width };
xcb_configure_window (connection, runtime, XCB_CONFIG_WINDOW_WIDTH, values);
}
void Tile::setLocalHeight(uint16_t height){
position.height = height;
const static uint32_t values[] = { height};
xcb_configure_window (connection, runtime, XCB_CONFIG_WINDOW_HEIGHT, values);
}
uint16_t Tile::setX(uint16_t x){

    setLocalX((*desktop).resizeX(getIndex(),x));
    xcb_configure_notify_event_t ce;
    ce.response_type = XCB_CONFIGURE_NOTIFY;
    ce.event = runtime;
    ce.window = runtime;
    ce.x = getX();
    ce.above_sibling = XCB_NONE;
    ce.override_redirect = false;
    xcb_send_event(connection, false, runtime, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *) &ce);

    xcb_flush (connection);
}
uint16_t Tile::setY(uint16_t y){
    setLocalY((*desktop).resizeY(getIndex(),y));
    xcb_configure_notify_event_t ce;
        ce.response_type = XCB_CONFIGURE_NOTIFY;
        ce.event = runtime;
        ce.window = runtime;
        ce.y = getY();
        ce.above_sibling = XCB_NONE;
        ce.override_redirect = false;
        xcb_send_event(connection, false, runtime, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *) &ce);

    xcb_flush (connection);
}
uint16_t Tile::setWidth(uint16_t width){
        setLocalWidth((*desktop).resizeWidth(getIndex(),width));
        xcb_configure_notify_event_t ce;
        ce.response_type = XCB_CONFIGURE_NOTIFY;
        ce.event = runtime;
        ce.window = runtime;
        ce.width = getWidth();
        ce.above_sibling = XCB_NONE;
        ce.override_redirect = false;
        xcb_send_event(connection, false, runtime, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *) &ce);

        xcb_flush (connection);
}
uint16_t Tile::setHeight(uint16_t height){
        setLocalHeight((*desktop).resizeHeight(getIndex(),height));
        xcb_configure_notify_event_t ce;
        ce.response_type = XCB_CONFIGURE_NOTIFY;
        ce.event = runtime;
        ce.window = runtime;
        ce.height = getHeight();
        ce.above_sibling = XCB_NONE;
        ce.override_redirect = false;
        xcb_send_event(connection, false, runtime, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *) &ce);
        xcb_flush (connection);
}
uint16_t Tile::getX(){
    return position.x;
}
uint16_t Tile::getY(){
    return position.y;
}
uint16_t Tile::getWidth(){
    return position.width;
}
uint16_t Tile::getHeight(){
    return position.height;
}
void Tile::setHidden(bool hidden){

    uint32_t state;
    if(hidden){
        xcb_unmap_window (connection,runtime);
        state = XCB_ICCCM_WM_STATE_WITHDRAWN;
    }else{
        xcb_map_window (connection,runtime);
        state = XCB_ICCCM_WM_STATE_NORMAL;
    }

    uint32_t data[] = { state, XCB_NONE };
    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, runtime,
                        WM_STATE, WM_STATE, 32, 2, data);


    (*this).hidden = hidden;

}
bool Tile::isHidden(){
    return hidden;
}
void Tile::focus()
{
    xcb_client_message_event_t ev;

    /* Initialize all of event's fields first */
    p_clear(&ev, 1);

    ev.response_type = XCB_CLIENT_MESSAGE;
    ev.window = win;
    ev.format = 32;
    ev.data.data32[1] = globalconf.timestamp;
    ev.type = WM_PROTOCOLS;
    ev.data.data32[0] = WM_TAKE_FOCUS;

    xcb_send_event(globalconf.connection, false, win,
                   XCB_EVENT_MASK_NO_EVENT, (char *) &ev);
    xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, runtime,
                        XCB_CURRENT_TIME);
}

