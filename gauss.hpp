/* GUI SYSTEM */

#ifndef GAUSS_HPP
#define GAUSS_HPP

#include "../extern/extern.hpp" // graphics library
#include "color.hpp"
#include <vector>
#include "touchManager.hpp"

class App;
typedef byte Alignment;
typedef void event;

enum GAUSS_TYPE
{
    BOX_TYPE,
    BUTTON_TYPE,
    IMAGE_TYPE,
    KEYBOARD_TYPE,
    LABEL_TYPE,
    WINDOW_TYPE,
    SWITCH_TYPE,
    LIST_TYPE,
    WINDOW_BAR_TYPE
};

#define CONTROL_BAR_SIZE 25
#define WIDTH 320
#define HEIGHT 480
#define LONG_PRESS_TIME 500

LGFX_Sprite tft(&tft_root);

void init_gauss()
{
    
}

void notifications_update();

bool reload_afterunlocked = false;

class Gauss // widget system
{
    public:
    virtual ~Gauss();
    void init(int16_t x, int16_t y, int16_t width, int16_t height);

    virtual void draw() = 0;        // draw objet
    virtual bool update();      // update objet
    virtual bool isFocuced();   // return true if is touched
    virtual void clear(color_t color){}; // clear
    virtual void free(){}            // delete[] dynamics allocations before destructor
    virtual byte getType() = 0;
    virtual bool isTouched();

    virtual void drawAll(bool draw_ = true);         // draw all childs and itself
    void renderAll();
    bool updateAll();       // update all children and itself 

    void addChild(Gauss *child);    // add child
    void removechildren();        // remove all children
    void setParent(Gauss *parent);  // set parent

    virtual void reload();
    void reloadWidget();

    int16_t getX();            // get relative x
    int16_t getY();            // get relative y
    int16_t getWidth();        // get width
    int16_t getHeight();       // get height

    virtual int16_t getAbsoluteX();    // get absolute position on the screen
    virtual int16_t getAbsoluteY();

    virtual int16_t getRelativeX();    // get sprite position relative to parent sprite
    virtual int16_t getRelativeY();

    virtual int16_t getMarginX();      // get margin x
    virtual int16_t getMarginY();      // get margin y

    virtual int16_t getPaddingX();     // get padding x
    virtual int16_t getPaddingY();     // get padding y

    Gauss* getParent();
    Gauss* getMaster();

    color_t getColor();         // get color
    color_t getBackgroundColor();   // get background color
    color_t getBorderColor();   // get border color

    int16_t getBorderSize();   // get border size
    Alignment getAlignment(); // get alignment
    bool isEnabled(){return this->enabled;}
    bool getRadius(){ return this->radius;}

    int16_t getLowestY();
    int16_t getHighestY();

    int16_t getLowestX();
    int16_t getHighestX();

    void setX(int16_t x); // set relative x
    void setY(int16_t y); // set relative y
    void setWidth(int16_t width);   // set width
    void setHeight(int16_t height); // set height

    void setMarginX(int16_t marginX);  // set margin x
    void setMarginY(int16_t marginY);  // set margin y

    void setPaddingX(int16_t paddingX);   // set padding x
    void setPaddingY(int16_t paddingY);   // set padding y

    void setColor(color_t color);       // colors
    void setBackgroundColor(color_t backgroundColor);

    void setBorderColor(color_t borderColor);
    void setBorderSize(int16_t borderSize); // set border size

    void setTheme(byte theme);
    void setHorizontalAlignment(Alignment alignment); // set alignment
    void setVerticalAlignment(Alignment alignment); // set alignment
    void setRadius(int16_t radius); // set radius

    void setAutoSize(bool autoSize){this->autoSize=autoSize;}

    void setSpeed(bool speed){this->speed=speed;}

    void enable(){this->enabled=true; drawAll();}
    void disable(){this->enabled=false;}

    virtual void EventOnClick();
    virtual void EventOnLongClick();
    virtual void EventOnReleased();
    virtual bool EventOnScroll();

    bool lockedSlide = false;

    void (*onclick)(App *app, Gauss* object, void* data) = nullptr;
    void (*onlongclick)(App *app, Gauss* object, void* data) = nullptr;
    void (*onreleased)(App *app, Gauss* object, void* data) = nullptr;
    void (*onscroll)(App *app, Gauss* object, void* data) = nullptr;

    App* appCallback = nullptr;
    void* dataCallback = nullptr;

    bool verticalSlide=false;
    bool horizontalSlide=false;

    byte statePress = 0; // 0=no 1=yes 2=wait realesed
    bool enabledBackground = true;

    LGFX_Sprite l_tft;
    bool rendered = false;

    protected:
    int16_t x, y = 0;                      // position
    int16_t width, height = 0;             // sizes
    int16_t marginX, marginY = 0;          // margin
    int16_t paddingX, paddingY = 0;        // padding

    color_t color = theme_color[DEFAULT_THEME][0];
    color_t backgroundColor = theme_color[DEFAULT_THEME][1];
    color_t borderColor = theme_color[DEFAULT_THEME][2];

    int16_t borderSize = DEFAULT_BORDER_SIZE;
    Alignment H_alignment = DEFAULT_H_ALIGNEMENT;
    Alignment V_alignment = DEFAULT_V_ALIGNEMENT;
    int16_t radius = DEFAULT_RADIUS;

    unsigned int timerPress = 0;

    bool autoSize = true;
    bool speed = false;
    bool enabled = true;

    int16_t inercie = 0;

    std::vector<Gauss *> children;
    Gauss *parent = nullptr;

};

Gauss *upFromDrawAll = nullptr;
Gauss *mainWindow = nullptr;
bool drawing = false;


#include "gauss.cpp"

#include "gauss/additional.hpp"

#include "gauss/box.hpp"
#include "gauss/label.hpp"
#include "gauss/image.hpp"
#include "gauss/window.hpp"
#include "gauss/button.hpp"
#include "gauss/keyboard.hpp"
#include "gauss/switch.hpp"
#include "gauss/notification.hpp"
#include "gauss/list.hpp"

#endif /* GAUSS_HPP */