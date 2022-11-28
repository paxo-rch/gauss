#ifndef BOX_HPP
#define BOX_HPP

class Box : public Gauss
{
    public:
    byte getType(){return BOX_TYPE;}
    Box(int16_t x, int16_t y, int16_t width, int16_t height);
    void draw();
    void free(){}
};

Box::Box(int16_t x, int16_t y, int16_t width, int16_t height)
{
    init(x, y, width, height);
    setRadius(0);
    setTheme(THEME_CONTRAST);
    setMarginX(0);
    setMarginY(0);
    setPaddingX(0);
    setPaddingY(0);
    setMarginY(0);
    setBorderSize(0);
    setBackgroundColor(COLOR_LIGHT);
    enabledBackground=false;
}

void Box::draw()
{
    if(enabledBackground)
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), radius, getBorderSize(), getBackgroundColor(), getBorderColor());
}

#endif