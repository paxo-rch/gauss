#ifndef Keyboard_HPP
#define Keyboard_HPP

#define KEY_ERASE 127 // ascii delete char

#define COLOR_PRESS 0x9F1D
#define COLOR_NOT_PRESS 0x29A6
#define COLOR_BACKGROUND 0xFFFF

class Keyboard : public Gauss
{
public:
    byte getType(){return KEYBOARD_TYPE;}
    Keyboard();
    ~Keyboard();

    void clear(color_t color) { tft.fillRect(0, 320, 322, 200, color); }
    void free(){}
    bool update();

    char getKey();

    void draw();

    bool isEnd();

    String getString();

    void setString(String newString);

    void setMode(byte mode){ this->mode = mode;}

    void link(Label* label)
    {
        Label* oldlabel = this->label;
        if(oldlabel!=nullptr)
            oldlabel->linked=false;
        this->label = label;
        label->linked=true;
        if(oldlabel!=nullptr)
            oldlabel->drawAll();
        label->drawAll();
    }

private:
    char* alphabet[4][3] =
    {
        {
            {"azertyuiop"},
            {"qsdfghjklm"},
            {"  wxcvbn  "}
        },
        {
            {"AZERTYUIOP"},
            {"QSDFGHJKLM"},
            {"  WXCVBN  "}
        },
        {
            {"[]{}#%^*+="},
            {"-\\|~<>*+=/"},
            {"   .,?!'  "}
        },
        {
            {"1234567890"},
            {"-/:;()$&@\""},
            {"   .,?!'  "}
        }
    };

    Label *space_label;
    Label *return_label;

    Box* box_maj;
    Box* box_type;
    Box* box_erase;

    Label* label = nullptr;

    byte mode = 0;
    char lastChar = 0;
    unsigned int timer_last_char = 0;
    byte blocked = 0; // 0=no, 1=used for the first time, 2 = repeat
};

#endif /* Keyboard_HPP */