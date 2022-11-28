#ifndef Label_CPP
#define Label_CPP

#include "label.hpp"

#define CALIBRATION_X 2
#define CALIBRATION_Y 2

Label::Label(int16_t x, int16_t y, int16_t width, int16_t height, String text)
{
    setX(x);
    setY(y);
    setHeight(height);
    setWidth(width);
    setMarginX(0);
    setMarginY(0);
    setPaddingX(0);
    setPaddingY(0);
    setHorizontalAlignment(LEFT_ALIGNMENT);
    setTextColor(COLOR_BLACK);
    setRadius(0);
    setBorderSize(0);
    enableBackground(true);
    setBackgroundColor(COLOR_LIGHT);
    setBorderColor(COLOR_EXTRA_LIGHT);

    if(text!="")
    {
        setText(text);
        setMarginX(0);
        setMarginY(0);
        setPaddingX(0);
        setPaddingY(0);
        setRadius(0);
        if(getHeight()<40)
            setRadius(getHeight()/2 - 2);
        else
            setRadius(10);
        setBackgroundColor(COLOR_PRIMARY);
        setTextColor(COLOR_LIGHT);
        setVerticalAlignment(CENTER_ALIGNMENT);
        setHorizontalAlignment(CENTER_ALIGNMENT);
    }
}

void Label::generateFont()
{
    byte frontIndex = 0;

    for (int i = 0; i < 3; i++) 
    {
        if(allFrontsNames[i] == frontName)
            frontIndex = i;
    }

    for(int i = 0; i < 4; i++)
    {
        l_tft.setFreeFont(allFronts[frontIndex][bold*2+oblique][i]);
        if(l_tft.fontHeight()>fontSize)
        {
            front = allFronts[frontIndex][bold*2+oblique][i];
            break;
        }
    }

    if(l_tft.fontHeight()<fontSize)
    {
        Serial.println("Warning: the front used is too huge !");
        return;
    }
}

void Label::drawText(String line, int counter, LGFX_Sprite* sprite)
{
    while(line[line.length()-1]==' ')
        line=line.substring(0, line.length()-1);
    sprite->setFreeFont(front);
    sprite->setTextColor(textColor);
    //counter++;
    uint16_t Y = (l_tft.fontHeight()+(spaceBetweenLines*factor))*counter;

    switch(V_alignment)
    {
        case UP_ALIGNMENT:
        break;
        case DOWN_ALIGNMENT:
        Y=sprite->height()-(heightOfText*factor) + Y;
        break;
        case CENTER_ALIGNMENT:
        Y=(sprite->height()/2)-((heightOfText*factor)/2)+(textPadding*factor) + Y;
        break;
    }

    switch (H_alignment)
    {
    case LEFT_ALIGNMENT:
        sprite->setCursor(0, Y);
        break;
    case RIGHT_ALIGNMENT:
        sprite->setCursor(sprite->width() - l_tft.textWidth(line), Y);
        break;
    case CENTER_ALIGNMENT:
        sprite->setCursor(sprite->width()/2 - l_tft.textWidth(line)/2, Y);
        break;
    }

    sprite->print(line);
}

void Label::draw()
{
    if(!isEnabled())
        return;

    generateFont(); // select the best front for this size to use less ram
    getHeightOfText();

    if(enabledBackground || linked)
        l_tft.drawRoundRectWithBorder(0, 0, getWidth(), getHeight(), radius, getBorderSize(), (this->linked)?(COLOR_PRIMARY):(getBackgroundColor()), getBorderColor());

    l_tft.setFreeFont(front);

    int lineCounter = 0;
    int actualIndex = 0;
    int parserIndex = 0;
    String line = "";
    String txt = text+String((linked)?("|"):(""))+"  ";
    String world = "";

    factor = (float) l_tft.fontHeight()/fontSize;

    int16_t mX = getRadius()/2 + getBorderSize() + getPaddingX() +2, mY = getRadius()/2 + getBorderSize() + getPaddingY() +2;
    int16_t bufferX = getWidth()-(2*mX), bufferY = getHeight()-(2*mY);
    int16_t spriteX = bufferX*factor, spriteY = bufferY*factor;

    LGFX_Sprite sprite(&l_tft);

    sprite.setPsram(false);

    if(getWidth()*factor*getHeight()*factor>ESP.getMaxAllocHeap()-1000)
    {
        sprite.setPsram(true);
    }

    sprite.setColorDepth(8);
    sprite.createSprite(spriteX, spriteY);
    spriteHeight=sprite.height();

    sprite.fillSprite((enabledBackground) ? (getBackgroundColor()) : (0xFFFF));

    while(actualIndex < txt.length())
    {
        while(actualIndex < txt.length() && l_tft.textWidth(line+world) <= spriteX)
        {
            if(world.indexOf("\n")!=-1)
            {
                world=world.substring(0, world.length()-1);
                line+=world;
                parserIndex+=world.length()+1;
                world="";
                actualIndex=parserIndex;
                break;
            }

            if(world[world.length()-1] == ' ')
            {
                line+=world;
                parserIndex+=world.length();
                world="";
                actualIndex=parserIndex;
            }

            world+=txt[actualIndex];
            actualIndex++;
        }

        if(l_tft.textWidth(world) >= spriteX)
        {
            line=world.substring(0, world.length()-2);
            parserIndex+=world.length()-2;
            world="";
            actualIndex=parserIndex;
        }

        drawText(line, lineCounter, &sprite);

        line="";
        lineCounter++;
    }

    sprite.pushRotateZoomWithAA(&l_tft, getWidth()/2, getHeight()/2, 0, 1/factor, 1/factor, (enabledBackground) ? (getBackgroundColor()) : 0xFFFF);
}

int Label::getHeightOfText()
{
    generateFont(); // select the best front for this size to use less ram

    l_tft.setFreeFont(front);

    int lineCounter = 0;
    int actualIndex = 0;
    int parserIndex = 0;
    String line = "";
    String txt = text+String((linked)?("|"):(""))+"  ";
    String world = "";

    factor = (float) l_tft.fontHeight()/fontSize;

    int16_t mX = getRadius()/2 + getBorderSize() + getPaddingX() +2, mY = getRadius()/2 + getBorderSize() + getPaddingY() +2;
    int16_t bufferX = getWidth()-(2*mX), bufferY = getHeight()-(2*mY);
    int16_t spriteX = bufferX*factor, spriteY = bufferY*factor;

    while(actualIndex < txt.length())
    {
        while(actualIndex < txt.length() && l_tft.textWidth(line+world) <= spriteX)
        {
            if(world.indexOf("\n")!=-1)
            {
                world=world.substring(0, world.length()-1);
                line+=world;
                parserIndex+=world.length()+1;
                world="";
                actualIndex=parserIndex;
                break;
            }

            if(world[world.length()-1] == ' ')
            {
                line+=world;
                parserIndex+=world.length();
                world="";
                actualIndex=parserIndex;
            }

            world+=txt[actualIndex];
            actualIndex++;
        }

        if(l_tft.textWidth(world) >= spriteX)
        {
            line=world.substring(0, world.length()-2);
            parserIndex+=world.length()-2;
            world="";
            actualIndex=parserIndex;
        }

        line="";
        lineCounter++;
    }

    numberOfLines=lineCounter;
    heightOfText=(l_tft.fontHeight()/factor + spaceBetweenLines)*numberOfLines + mY*2;
    return heightOfText;
}

int Label::getWidthOfText()
{
    generateFont();
    factor = (float) l_tft.fontHeight()/fontSize;
    return l_tft.textWidth(text) / factor;
}

#endif