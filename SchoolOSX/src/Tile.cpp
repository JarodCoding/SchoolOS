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
    destroy();

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
    name = res.at(4);


}


string Tile::getData(){
ostringstream s;
s <<position.x<<";"<<position.y<<";"<<position.width<<";"<<position.height<<";"<<name;
return s.str();
}
void Tile::setIndex(int index){
TileIndex = index;
}
int Tile::getIndex(){
return TileIndex;
}
void Tile::setLocalX(uint16_t x){
if(isHidden())return;
position.x = x;
const static uint32_t values[] = { x };
xcb_configure_window (connection, runtime, XCB_CONFIG_WINDOW_X, values);
 xcb_configure_notify_event_t ce;
    ce.response_type = XCB_CONFIGURE_NOTIFY;
    ce.event = runtime;
    ce.window = runtime;
    ce.x = getX();
    ce.above_sibling = XCB_NONE;
    ce.override_redirect = false;
    xcb_send_event(connection, false, runtime, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *) &ce);
}
void Tile::setLocalY(uint16_t y){
if(isHidden())return;

position.y = y;
const static uint32_t values[] = { y };
xcb_configure_window (connection, runtime, XCB_CONFIG_WINDOW_Y, values);
   xcb_configure_notify_event_t ce;
        ce.response_type = XCB_CONFIGURE_NOTIFY;
        ce.event = runtime;
        ce.window = runtime;
        ce.y = getY();
        ce.above_sibling = XCB_NONE;
        ce.override_redirect = false;
        xcb_send_event(connection, false, runtime, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *) &ce);
}
void Tile::setLocalWidth(uint16_t width){
if(isHidden())return;

position.width = width;
const static uint32_t values[] = { width };
xcb_configure_window (connection, runtime, XCB_CONFIG_WINDOW_WIDTH, values);
       xcb_configure_notify_event_t ce;
        ce.response_type = XCB_CONFIGURE_NOTIFY;
        ce.event = runtime;
        ce.window = runtime;
        ce.width = getWidth();
        ce.above_sibling = XCB_NONE;
        ce.override_redirect = false;
        xcb_send_event(connection, false, runtime, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *) &ce);

}
void Tile::setLocalHeight(uint16_t height){
if(isHidden())return;

position.height = height;
const static uint32_t values[] = { height};
xcb_configure_window (connection, runtime, XCB_CONFIG_WINDOW_HEIGHT, values);
  xcb_configure_notify_event_t ce;
        ce.response_type = XCB_CONFIGURE_NOTIFY;
        ce.event = runtime;
        ce.window = runtime;
        ce.height = getHeight();
        ce.above_sibling = XCB_NONE;
        ce.override_redirect = false;
        xcb_send_event(connection, false, runtime, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char *) &ce);
}
uint16_t Tile::setX(uint16_t x){
    if(isHidden())return -1;

    setLocalX((*desktop).resizeX(getIndex(),x));


    xcb_flush (connection);
}
uint16_t Tile::setY(uint16_t y){
    if(isHidden())return -1;

    setLocalY((*desktop).resizeY(getIndex(),y));


    xcb_flush (connection);
}
uint16_t Tile::setWidth(uint16_t width){
        if(isHidden())return -1;

        setLocalWidth((*desktop).resizeWidth(getIndex(),width));

        xcb_flush (connection);
}
uint16_t Tile::setHeight(uint16_t height){
        if(isHidden())return -1;

        setLocalHeight((*desktop).resizeHeight(getIndex(),height));

        xcb_flush (connection);
}
uint16_t Tile::getX(){
    if(isHidden())return -1;

    return position.x;
}
uint16_t Tile::getY(){
    if(isHidden())return -1;
    return position.y;
}
uint16_t Tile::getWidth(){
    if(isHidden())return -1;
    return position.width;
}
uint16_t Tile::getHeight(){
    if(isHidden())return -1;
    return position.height;
}
void Tile::setHidden(bool hidden){

    setHiddenLocal(hidden);

    xcb_flush (connection);

}
// To Improve Later: only send when relly nesc (don't send hide + unhide when hidden and unhidden make a set ,apply Method (one with and one without flush) )
void Tile::setHiddenLocal(bool hidden){
        if(!isAktive()&&hidden){
            destroy();
            return;
        }

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
    focusLocal();
        xcb_flush (connection);


}
void Tile::focusLocal()
{
    if(isHidden())return;

    if(!isAktive()){
// TODO (jarod#7#): Start Applikation
    }
    xcb_set_input_focus(connection, XCB_INPUT_FOCUS_POINTER_ROOT, runtime,
                        XCB_CURRENT_TIME);

}
bool Tile::isAktive(){
    return runtime != NULL;
}
bool Tile::compare(Tile tile){
    return tile.runtime==runtime||tile.name.compare(name)==0;
}
void Tile::destroy(){
    desktop->removeTile(TileIndex);
    xcb_flush(connection);

}
void Tile::destroyLocal(){
    if(isAktive()){
        xcb_unmap_window(connection,runtime);
        xcb_destroy_window(connection,runtime);
        runtime = NULL;
// TODO (jarod#7#): Stop Process
    }

}
void Tile::setRutime(xcb_window_t window){
    runtime = window;
}


