#ifndef Keyboard_CPP
#define Keyboard_CPP

Keyboard::Keyboard()
{
    init(15, 310, 290, 157);
    setY(300);
    //setMarginX(15);
    
    Image* maj = new Image("/paxo/system/keyboard/maj_"+String(mode)+".bmp", 8, 9);   // majuscule
    Image* type = new Image("/paxo/system/keyboard/type_"+String(mode)+".bmp", 0, 2);   // type
    Image* erase = new Image("/paxo/system/keyboard/delete.bmp", 5, 10);   // erase char

    box_maj = new Box(0, 73, 28, 36);
    box_type = new Box(0, 110, 28, 36);
    box_erase = new Box(261, 73, 28, 36);

    box_maj->setBorderSize(0);
    box_type->setBorderSize(0);
    box_erase->setBorderSize(0);

    space_label = new Label(88, 115, 112, 37);
    return_label = new Label(237, 115, 57, 37);

    space_label->setRadius(0);
    return_label->setRadius(0);

    space_label->fontSize = 17;
    return_label->fontSize = 17;

    space_label->setHorizontalAlignment(CENTER_ALIGNMENT);
    return_label->setHorizontalAlignment(CENTER_ALIGNMENT);

    space_label->setText("space");
    return_label->setText("return");

    space_label->setVerticalAlignment(CENTER_ALIGNMENT);
    return_label->setVerticalAlignment(CENTER_ALIGNMENT);

    box_maj->addChild(maj);
    box_type->addChild(type);
    box_erase->addChild(erase);

    this->addChild(box_maj);
    this->addChild(box_type);
    this->addChild(box_erase);
    this->addChild(space_label);
    this->addChild(return_label);
}

Keyboard::~Keyboard()
{
    removechildren();
}

void Keyboard::draw()
{
    l_tft.fillRect(0, getAbsoluteY(), 320, 200, COLOR_BACKGROUND);
    l_tft.setTextColor(COLOR_NOT_PRESS);

    for (byte j = 0; j < 3; j++)
    {
        for (byte i = 0; i < 10; i++)
        {
            if(alphabet[mode][j][i]!=' ')
                l_tft.drawCentreString(String(alphabet[mode][j][i]), 15 + i * 29, 7 + j * 37, 4);
        }
    }
}

char Keyboard::getKey()
{
    if (isFocuced())
    {
        for (byte j = 0; j < 3; j++)
        {
            for (byte i = 0; i < 10; i++)
            {
                if(Touched(getAbsoluteX() + i * 29, getAbsoluteY() + j * 37, 29, 37))
                {
                    if(alphabet[mode][j][i]==' ')
                        break;
                    tft_root.setTextColor(COLOR_PRESS);
                    tft_root.drawCentreString(String(alphabet[mode][j][i]), getAbsoluteX() + 15 + i * 29, getAbsoluteY() + 7 + j * 37, 4);
                    while(touch.isTouch());
                    tft_root.setTextColor(COLOR_NOT_PRESS);
                    tft_root.drawCentreString(String(alphabet[mode][j][i]), getAbsoluteX() + 15 + i * 29, getAbsoluteY() + 7 + j * 37, 4);
                    
                    lastChar=alphabet[mode][j][i];
                    timer_last_char=millis();
                    return alphabet[mode][j][i];
                }
            }
        }
    }
    if(return_label->isTouched())
    {
        while(touch.isTouch());
        lastChar='\n';
        timer_last_char=millis();
        return '\n';
    }
    if(box_maj->isTouched()) // maj
    {
        switch(mode)
        {
            case 0:
                mode=1;
            break;
            case 1:
                mode=0;
            break;
            case 2:
                mode=3;
            break;
            case 3:
                mode=2;
            break;
        }
        while(touch.isTouch());
        reloadWidget();
        this->drawAll();
    }

    if(box_type->isTouched()) // mode
    {
        switch(mode)
        {
            case 0:
                mode=3;
            break;
            case 1:
                mode=3;
            break;
            case 2:
                mode=0;
            break;
            case 3:
                mode=0;
            break;
        }
        while(touch.isTouch());
        reloadWidget();
        this->drawAll();
    }

    if(box_erase->isTouched())
    {
        if(lastChar==KEY_ERASE)
        {
            delay(100);
        }
        lastChar=KEY_ERASE;
        timer_last_char=millis();
        return KEY_ERASE;
    }
    if(space_label->isTouched())
    {
        while(touch.isTouch());
        return ' ';
    }
    return 0;
}

bool Keyboard::update()
{
    if(label==nullptr)
        return false;
    char chr = getKey();

    if(chr!=0)
    {
        if(chr==KEY_ERASE)
            label->setText(label->getText().substring(0, label->getText().length()-1));
        else
            label->setText(label->getText() + String(chr));
        label->drawAll();
        delay(10);
        return true;
    }
}

String Keyboard::getString()
{
    return "";
}

void Keyboard::setString(String newString)
{
}

#endif /* Keyboard_CPP */