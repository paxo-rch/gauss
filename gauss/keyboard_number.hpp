#ifndef KEYNOARD_NUMBER_HPP
#define KEYNOARD_NUMBER_HPP

class KeyboardNumber : public Keyboard
{
    public:
    KeyboardNumber(String code*);
    bool update();

    private:
    String code* = "";
};

KeyboardNumber::KeyboardNumber(String code*)
{
    this->code = code;
}

bool KeyboardNumber::update()
{
    
}

#endif