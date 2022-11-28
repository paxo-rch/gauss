#ifndef LIST_CPP
#define LIST_CPP

#include "list.hpp"

void List::setElementsHeight(int16_t height)
{
    elementsHeight = height;
}

void List::setElementsWidth(int16_t width)
{
    elementsWidth = width;
}

void List::add(Gauss* element)
{
    if(element==nullptr)
        return;
    this->addChild(element);
    if(direction == VERTICAL)
    {
        element->setX(0);
        element->setY(elementNumber*elementsHeight);
        element->setWidth(this->getWidth());
        element->setHeight(elementsHeight);
    }
    else
    {
        element->setX(elementNumber*elementsWidth);
        element->setY(0);
        element->setWidth(elementsWidth);
        element->setHeight(this->getHeight());
    }

    this->elementNumber++;
}

#endif