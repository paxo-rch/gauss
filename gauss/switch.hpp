#ifndef SWITCH_BUTTON
#define SWITCH_BUTTON

class Switch : public Gauss
{
    public:
    byte getType(){return SWITCH_TYPE;}
    Switch(int16_t x, int16_t y, bool state);
    void draw();
    void clear(color_t color) { l_tft.fillRect(0, 0, getWidth(), getHeight(), color); }
    void free(){}

    void switchState(bool state);
    bool getState(){return state;}

    void EventOnClick();

    private:
    bool state = 0;
};

Switch::Switch(int16_t x, int16_t y, bool state)
{
    init(x, y, 60, 41);
    this->setRadius(20);
    this->setBackgroundColor(0x7BCF);
    this->setColor(COLOR_LIGHT);
    this->setBorderSize(0);
}

void Switch::draw()
{
    if(state==1)
    {
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), radius, 0, 0x2E43, 0x2E43);
        l_tft.fillCircle(0+getWidth()-20, 0 + 20, 18, COLOR_LIGHT);
    }else
    {
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), radius, 0, 0x7BCF, 0x7BCF);
        l_tft.fillCircle(0+20, 0 + 20, 18, COLOR_LIGHT);
    }
}

void Switch::switchState(bool state)
{
    this->state=state;
    draw();
}

void Switch::EventOnClick()
{   
    switchState(!state);
}

#endif