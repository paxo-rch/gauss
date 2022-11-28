#ifndef CALCULATOR_CPP
#define CALCULATOR_CPP

#include "Calculator.hpp"
#include <tinyexpr.h> // mathematics library


void Calculator::launch()
{
    buildGui();
}

void Calculator::buildGui()
{
    Window window("");
    window.background_img=true;

    String keys[] = {"AC", "+/-", "%", "/", "7", "8", "9", "*", "4", "5", "6", "-", "1", "2", "3", "+", "0", " ", ".", "="};

    Box* calculContent = new Box(63, 44, 193, 81);
        Label* oldCalculLabel = new Label(0, 0, 193, 31, "");
            oldCalculLabel->fontSize=16;
            oldCalculLabel->setTextColor(COLOR_GREY);
            oldCalculLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
        Label* actualCalculLabel = new Label(0, 31, 193, 50, "");
            actualCalculLabel->fontSize=40;
            actualCalculLabel->setTextColor(COLOR_BLACK);
            actualCalculLabel->setHorizontalAlignment(CENTER_ALIGNMENT);
            actualCalculLabel->setRadius(0);
        calculContent->addChild(oldCalculLabel);
        calculContent->addChild(actualCalculLabel);

    Box* keyboard = new Box(20, 125, 280, 340);
        Label* AC_btn;
        Label* invert_btn;
        Label* equals_btn;
        for(int y = 0; y < 5; y++)
            for(int x = 0; x < 4; x++)
            {
                Label* nbt = new Label(72.5*x, 70*y, 62, 60, keys[x+y*4]);
                nbt->setRadius(30);
                nbt->fontSize=20;
                nbt->setTextColor(COLOR_BLACK);
                if(x==3 || (x==2 && y==0))
                    nbt->setBackgroundColor(COLOR_BEIGE);
                else
                    nbt->setBackgroundColor(COLOR_LIGHT);

                if(x==0 && y==4)
                    nbt->setWidth(135);

                if(x==0 && y==0)
                    AC_btn=nbt;
                if(x==1 && y==0)
                    invert_btn=nbt;
                if(x==3 && y==4)
                    equals_btn=nbt;
                
                nbt->onreleased = addChar;
                nbt->dataCallback = (void*) actualCalculLabel;

                if(keys[x+y*4]=="0")
                    x++;
                keyboard->addChild(nbt);
            }

    window.addChild(calculContent);
    window.addChild(keyboard);

    window.drawAll();

    while (1)
    {
        window.updateAll();

        if(home_button.pressed())
        {
            window.removechildren();
            return;
        }

        {
            int oldFont = actualCalculLabel->fontSize;
            while(actualCalculLabel->getWidthOfText()<160 && actualCalculLabel->fontSize <= 40)
            {
                actualCalculLabel->fontSize++;
            }
            while(actualCalculLabel->getWidthOfText()>160)
            {
                actualCalculLabel->fontSize--;
            }
            if(actualCalculLabel->fontSize != oldFont)
                actualCalculLabel->drawAll();
        }

        if(equals_btn->isTouched())
        {
            String str = actualCalculLabel->getText();
            if(str.length()!=0)
            {
                char * chr =(char *) malloc(str.length());
                strcpy(chr, str.c_str());
                str = String(te_interp(chr, 0));
                oldCalculLabel->setText(actualCalculLabel->getText());
                actualCalculLabel->setText(str);
                window.drawAll();
            }
        }
        if(AC_btn->isTouched())
        {
            actualCalculLabel->setText("");
            window.drawAll();
        }
    }
}

#endif