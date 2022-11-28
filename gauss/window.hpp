#ifndef WINDOW_HPP
#define WINDOW_HPP

LGFX_Sprite background(&tft);

class Window : public Gauss
{
    public:
    byte getType(){return WINDOW_TYPE;}
    Window(String name = "");
    void draw();        // draw objet
    bool isFocuced();   // return true if is touched
    void clear(color_t color) {}
    void free(){ delete windowbar; }
    void setTitle(String title) { this->name = title; draw(); }

    void drawAll(bool draw_ = true);
    void reload();

    void EnableScroolVertical(bool enable);
    void ActiveScroolHorizontal(bool enable);

    bool background_img = false;

    String name;
    Gauss* windowbar = nullptr;

    private:
    bool scroolHorizontal = false;
    bool scroolVertical = false;
    int fps = 0;
};

byte updateBarWindow();

#endif