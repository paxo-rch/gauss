#ifndef LIST_HPP
#define LIST_HPP

#define VERTICAL 0
#define HORIZONTAL 1

class List : public Gauss
{
    public:
    List(int16_t x, int16_t y, int16_t width, int16_t height, bool direction = HORIZONTAL);
    byte getType(){return LIST_TYPE;}

    void setElementsHeight(int16_t height);
    void setElementsWidth(int16_t width);
    void add(Gauss* element);
    void draw(){}

    private:
    int16_t elementsWidth, elementsHeight;
    int elementNumber;
    bool direction;
};

List::List(int16_t x, int16_t y, int16_t width, int16_t height, bool direction)
{
    init(x, y, width, height);
    setRadius(0);
    setMarginX(0);
    setMarginY(0);
    setPaddingX(0);
    setMarginY(0);
    setBorderSize(0);
    this->direction = direction;
    this->elementsWidth=0;
    this->elementsHeight=0;
    this->elementNumber=0;
    this->direction=VERTICAL;
}

#include "list.cpp"

#endif