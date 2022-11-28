#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP


class Calculator : public App
{
public:
    String getName() { return "calculator"; }
    String getLogo() { return "/paxo/apps/calculator/logo.bmp"; };

    void launch();

    void buildGui();

    appEvent addChar(App *app, Gauss* objectPrt, void* data)
    {
        String chr = reinterpret_cast<Label*>(objectPrt)->getText();
        Label* label = reinterpret_cast<Label*>(data);

        label->addText(chr);
        label->drawAll();
    }
};

#include "Calculator.cpp"


#endif