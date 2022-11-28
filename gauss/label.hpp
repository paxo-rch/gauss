#ifndef Label_HPP
#define Label_HPP

const GFXfont *allFronts[3][4][4] = {
{
    {&FreeSans9pt7b, &FreeSans12pt7b, &FreeSans18pt7b, &FreeSans24pt7b},
    {&FreeSansOblique9pt7b, &FreeSansOblique12pt7b, &FreeSansOblique18pt7b, &FreeSansOblique24pt7b},
    {&FreeSansBold9pt7b, &FreeSansBold12pt7b, &FreeSansBold18pt7b, &FreeSansBold24pt7b},
    {&FreeSansBoldOblique9pt7b, &FreeSansBoldOblique12pt7b, &FreeSansBoldOblique18pt7b, &FreeSansBoldOblique24pt7b}
},
{
    {&FreeMono9pt7b, &FreeMono12pt7b, &FreeMono18pt7b, &FreeMono24pt7b},
    {&FreeMonoOblique9pt7b, &FreeMonoOblique12pt7b, &FreeMonoOblique18pt7b, &FreeMonoOblique24pt7b},
    {&FreeMonoBold9pt7b, &FreeMonoBold12pt7b, &FreeMonoBold18pt7b, &FreeMonoBold24pt7b},
    {&FreeMonoBoldOblique9pt7b, &FreeMonoBoldOblique12pt7b, &FreeMonoBoldOblique18pt7b, &FreeMonoBoldOblique24pt7b}
},
{
    {&FreeSerif9pt7b, &FreeSerif12pt7b, &FreeSerif18pt7b, &FreeSerif24pt7b},
    {&FreeSerifItalic9pt7b, &FreeSerifItalic12pt7b, &FreeSerifItalic18pt7b, &FreeSerifItalic24pt7b},
    {&FreeSerifBold9pt7b, &FreeSerifBold12pt7b, &FreeSerifBold18pt7b, &FreeSerifBold24pt7b},
    {&FreeSerifBoldItalic9pt7b, &FreeSerifBoldItalic12pt7b, &FreeSerifBoldItalic18pt7b, &FreeSerifBoldItalic24pt7b}
}};

// /paxo/system/fronts/roboto.vlw

String allFrontsNames[] = {"sans", "mono", "serif"};

class Label : public Gauss
{
public:
    byte getType(){return LABEL_TYPE;}
    Label(int16_t x, int16_t y, int16_t width, int16_t height, String text = "");

    void draw();
    void free(){}

    void setText(String text) { this->text = text; reloadWidget(); }
    String getText() { return this->text; reloadWidget(); }
    void addText(String toAdd) { this->text += toAdd; reloadWidget(); }

    int getHeightOfText();
    int getWidthOfText();
    
    void enableBackground(bool state) { this->enabledBackground = state; reloadWidget(); }

    void setTextColor(color_t textColor) { this->textColor = textColor; reloadWidget(); }
    color_t getTextColor() { return this->textColor; reloadWidget(); }
    
    byte fontSize = 24;
    String frontName = "sans";
    bool bold = false;
    bool oblique = false;
    bool linked = false;

private:
    void drawText(String line, int counter, LGFX_Sprite* sprite);
    void generateFont();

    const GFXfont *front; 

    String text = "";
    color_t textColor = COLOR_BLACK;

    uint16_t numberOfLines = 0;
    uint16_t textPadding = 0; // screen value
    uint16_t heightOfText = 0;  // screen value
    uint16_t spaceBetweenLines = 5; // screen value
    uint16_t spriteHeight = 0; // buffer value
    float factor = 1; // true value * factor = buffer value
};

#include "label.cpp"

#endif