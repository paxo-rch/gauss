#ifndef WINDOW_CPP
#define WINDOW_CPP

class WindowBar : public Gauss
{
    public:
    WindowBar()
    {
        init(0, 0, 320, CONTROL_BAR_SIZE);
    }
    void draw()
    {
        l_tft.fillRect(0,0,320,CONTROL_BAR_SIZE, (speed) ? (ALPHA_8B) : (ALPHA_16B));

        LGFX_Sprite BI(&tft_root);
        byte level = battery.getLevel();
        BI.createSprite(35, 15);
        BI.fillSprite(0xFFFF);
        BI.drawRoundRectWithBorder(0,0,BI.width()-5,BI.height()-2, 4, 1, 0xFFFF, COLOR_BLACK);
        BI.fillRoundRect(3,3,(BI.width()-5-6)*level/100,BI.height()-2-6, 2, (level>20) ? COLOR_BLACK : COLOR_WARNING);
        BI.pushSprite(&l_tft, 320-BI.width(), 5);
        BI.deleteSprite();


        hour_label = new Label(0,3,60,18);
        addChild(hour_label);
        String hour = "";
        if(ACTUAL_HOUR<=9)
            hour+="0";
        hour+=ACTUAL_HOUR;
        hour+=":";
        if(ACTUAL_MINUTE<=9)
            hour+="0";
        hour+=ACTUAL_MINUTE;
        hour_label->setText(hour);
        hour_label->fontSize=17;
        hour_label->setBorderSize(0);
        hour_label->setRadius(0);
        hour_label->setBackgroundColor(0xFFFF);
        hour_label->drawAll();

        String logo_connection[] = {"reseau0.bmp", "reseau1.bmp", "reseau2.bmp", "reseau3.bmp", "reseau4.bmp"};
        Image* connection = new Image("/paxo/system/statebar/reseau"+String(getConnectionQuality())+".bmp", 65, 1);
        addChild(connection);
        connection->drawAll();

        removechildren();

        reloadWidget();
    }
    int16_t getRelativeX()
    {
        return 0;
    }
    int16_t getRelativeY()
    {
        return 0;
    }
    byte getType(){return WINDOW_BAR_TYPE;}

    private:
    Label* hour_label = nullptr;
    Image* connection = nullptr;
};

void Window::drawAll(bool draw_)
{
    if(!isEnabled())
        return;

    setPaddingY(25);

    if(upFromDrawAll==nullptr)
    {
        upFromDrawAll=this;
    }
    
    if(upFromDrawAll==this && is_cpu0_locked())
    {
        reload_afterunlocked=false;
        return;
    }
    
    if(!rendered)
    {
        rendered=true;
        l_tft.deleteSprite();
        l_tft.setPsram(true);
        l_tft.setColorDepth((speed) ? (8) : (16));
        l_tft.createSprite(this->getWidth(), this->getHeight());
        
        if(background_img)
        {
            if(background.getBuffer()==nullptr)
            {
                background.setPsram(true);
                background.createSprite(320, 480);
                background.drawPngFile(SD, "/paxo/usr/bcground.png", 0, 0);
            }
            background.pushSprite(&l_tft, 0, 0);
        }else
            if(upFromDrawAll==this)
                l_tft.fillSprite(0xFFFF);
            else
                l_tft.fillSprite((speed) ? (ALPHA_8B) : (ALPHA_16B));

        for (int i = 0; i < children.size(); i++)
        {
            if(children[i] != nullptr)
                children[i]->drawAll();
        }
        
        draw();
    }

    if(upFromDrawAll==this)
    {
        upFromDrawAll=nullptr;
        Screen_busy=true;
        l_tft.pushSprite(&tft_root, 0, 0);
        Screen_busy=false;
        drawing=false;
    }
    else
    {
        l_tft.pushSprite(&parent->l_tft, getAbsoluteX(), getAbsoluteY(), (speed) ? (256) : (1));
    }
}

void Window::reload()
{
    tft.fillRect(getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight(), getBackgroundColor());
    drawAll();
}

Window::Window(String name)
{
    init(0, 0, 320, 480);
    setMarginX(0);
    this->name = name;
    setPaddingX(0);
    setPaddingY(25);

    mainWindow=this;
    speed=false;

    windowbar = new WindowBar();
    windowbar->setParent(this);
}

void Window::draw()
{
    Serial.println("paddingY="+String(getPaddingY()));
    windowbar->drawAll();
}

bool Window::isFocuced()
{
    return touch.isTouch();
}

void Window::EnableScroolVertical(bool enable)
{
    this->verticalSlide=enable;
}

void Window::ActiveScroolHorizontal(bool enable)
{
    this->horizontalSlide=enable;
}

byte updateBarWindow()
{
    if(STANDBY_MODE::mode==STANDBY_MODE_ENABLE)
        return 0;
    if(mainWindow==nullptr)
        return 0;
    if(reinterpret_cast<Window*>(mainWindow)->name=="")
        return 0;
    if(reinterpret_cast<Window*>(mainWindow)->windowbar!=nullptr)
        return 0;

    drawing=true;

    if(xPortGetCoreID() == 1)
    {
        CPU0_locked = LOCKED_CORE_REQUEST;
        while (CPU0_locked!=LOCKED_CORE)
        {
            delay(2);
        }
    }

    reinterpret_cast<Window*>(mainWindow)->windowbar->drawAll();
    CPU0_locked = UNLOCKED;
}



#endif