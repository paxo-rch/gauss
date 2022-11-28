#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <SD.h>

#define BUFFPIXEL 20

#define BMP_FILE 1
#define PNG_FILE 2
#define JPG_FILE 3

uint16_t read16(fs::File &f)
{
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}

uint32_t read32(fs::File &f)
{
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}

bool parsePng(String filename, int16_t* width, int16_t* height)
{
    while(SPI_busy)
        delay(1);
    
    SPI_busy=true;

    File file = SD.open(filename);

    if (!file)
    {
        SPI_busy = false;
        return false;
    }

    byte table[32];

    for (int i = 0; file.available() && i < 32; i++)
    {
        table[i] = file.read();
    }

    *width=table[16]*256*256*256+table[17]*256*256+table[18]*256+table[19];
    *height=table[20]*256*256*256+table[21]*256*256+table[22]*256+table[23];

    file.close();

    SPI_busy = false;
    return true;
}

bool parseBmp(String filename, int16_t* width, int16_t* height)
{
    while(SPI_busy)
        delay(1);
    
    SPI_busy=true;

    File file = SD.open(filename);

    if (!file)
    {
        SPI_busy = false;
        return false;
    }

    byte table[32];

    for (int i = 0; file.available() && i < 32; i++)
    {
        table[i] = file.read();
    }

    *width=table[15]*256*256*256+table[16]*256*256+table[17]*256+table[18];
    *height=table[19]*256*256*256+table[20]*256*256+table[21]*256+table[22];

    file.close();

    SPI_busy = false;
    return true;
}

bool parseJpg(String filename, int16_t *width, int16_t *height )
{
    while(SPI_busy)
        delay(1);
    
    SPI_busy=true;

    File file = SD.open(filename);

    if (!file)
    {
        SPI_busy = false;
        return false;
    }

    file.read();
    file.read();

    for(;;) {
        int marker;
        while((marker = file.read()) != 0xFF);
        while((marker = file.read()) == 0xFF);

        if (marker ==  0xC0 || marker == 0xC2) {
        file.read();
        file.read();
        file.read();
        *height = (file.read() << 8) + file.read();
        *width = (file.read() << 8) + file.read();
        return true;
        }
    }

    file.close();

    SPI_busy = false;
    return true;
}

class Image : public Gauss
{
    public:
    byte getType(){return IMAGE_TYPE;}

    Image(String filename, int16_t x, int16_t y, int16_t resizew = -1, int16_t resizeh = -1);
    ~Image();

    void draw();
    void free(){ img.deleteSprite(); }
    
    void setAlpha(uint16_t alpha) { this->alpha = alpha; isAlphaEnabled=1; }
    void load();
    void close(){ img.deleteSprite(); }

    byte getImageType(String filename);

    int getWidthOfImage(String filename);
    int getHeightOfImage(String filename);

    private:
    String filename = "";

    int16_t resizew, resizeh;
    bool resize = false;

    uint16_t alpha = 0x0000;
    bool isAlphaEnabled = 0;

    LGFX_Sprite img;
};

byte Image::getImageType(String filename)
{
    if(filename.indexOf(".bmp") != -1)
        return BMP_FILE;
    if(filename.indexOf(".png") != -1)
        return PNG_FILE;
    if(filename.indexOf(".jpg") != -1 || filename.indexOf(".jpeg") != -1)
        return JPG_FILE;
    return 0;
}

int Image::getWidthOfImage(String filename)
{
    switch(getImageType(filename))
    {
        case BMP_FILE:
            parseBmp(filename, &this->height, &this->width);
        break;
        case PNG_FILE:
            parsePng(filename, &this->height, &this->width);
        break;
        case JPG_FILE:
            parseJpg(filename, &this->height, &this->width);
        break;
    };
    return this->width;
}

int Image::getHeightOfImage(String filename)
{
    return this->height;
}

Image::~Image()
{
    img.deleteSprite();
}

Image::Image(String filename, int16_t x, int16_t y, int16_t resizew, int16_t resizeh)
{
    if(resizew!=-1 && resizeh!=-1) // resize image
    {
        init(x, y, getWidthOfImage(filename), getHeightOfImage(filename));
        this->resizew=resizew;
        this->resizeh=resizeh;
        resize=false;
    }
    else
    {
        init(x, y, getWidthOfImage(filename), getHeightOfImage(filename));
        resize=false;
    }    

    this->filename=filename;
}

void Image::draw()
{
    if(img.getBuffer()!=nullptr)
        if(isAlphaEnabled)
            img.pushSprite(&l_tft, 0, 0,alpha);
        else
            img.pushSprite(&l_tft, 0, 0);
    else
    {
        if(resize)
        {
            switch(getImageType(filename))
            {
                case BMP_FILE:
                    l_tft.drawBmpFile(SD, filename, 0, 0, getWidth(), getHeight(), 0, 0, resizew, resizeh);
                break;
                case PNG_FILE:
                    l_tft.drawPngFile(SD, filename, 0, 0, getWidth(), getHeight(), 0, 0, resizew, resizeh);
                break;
                case JPG_FILE:
                    l_tft.drawJpgFile(SD, filename, 0, 0, getWidth(), getHeight(), 0, 0, resizew, resizeh);
                break;
            };
        }else
        {
            switch(getImageType(filename))
            {
                case BMP_FILE:
                    l_tft.drawBmpFile(SD, filename, 0, 0);
                break;
                case PNG_FILE:
                    l_tft.drawPngFile(SD, filename, 0, 0);
                break;
                case JPG_FILE:
                    l_tft.drawJpgFile(SD, filename, 0, 0);
                break;
            };
        }
    }
}

void Image::load()
{
    Serial.println("loading image: " + filename);

    img.setPsram(true);
    img.setColorDepth((speed) ? (8) : (16));

    while(SPI_busy)
        delay(1);

    SPI_busy=true;

    if(resize)
    {
        img.createSprite(resizew, resizeh);
        img.fillSprite((speed) ? (ALPHA_8B) : (ALPHA_16B));

        switch(getImageType(filename))
        {
            case BMP_FILE:
                img.drawBmpFile(SD, filename, 0, 0, getWidth(), getHeight(), 0, 0, resizew, resizeh);
            break;
            case PNG_FILE:
                img.drawPngFile(SD, filename, 0, 0, getWidth(), getHeight(), 0, 0, resizew, resizeh);
            break;
            case JPG_FILE:
                img.drawJpgFile(SD, filename, 0, 0, getWidth(), getHeight(), 0, 0, resizew, resizeh);
            break;
        };

        setWidth(resizew);
        setHeight(resizeh);
    }else
    {
        img.createSprite(getWidth(), getHeight());
        img.fillSprite((speed) ? (ALPHA_8B) : (ALPHA_16B));

        switch(getImageType(filename))
        {
            case BMP_FILE:
                img.drawBmpFile(SD, filename, 0, 0);
            break;
            case PNG_FILE:
                img.drawPngFile(SD, filename, 0, 0);
            break;
            case JPG_FILE:
                img.drawJpgFile(SD, filename, 0, 0);
            break;
        };
    }
    
    SPI_busy=false;

    if(this->radius || this->borderSize)
    {
        LGFX_Sprite mask(&img);

        mask.setPsram(true);
        mask.setColorDepth((speed) ? (8) : (16));
        mask.createSprite(getWidth(), getHeight());
        mask.fillSprite((speed) ? (ALPHA_8B) : (ALPHA_16B));

        mask.drawRoundRectWithBorder(0, 0,getWidth(),getHeight(),this->radius, getBorderSize(), 0xFFFF, getBorderColor());
        mask.pushSprite(&img,0, 0, 0xFFFF);

        mask.deleteSprite();
        setAlpha((speed) ? (ALPHA_8B) : (ALPHA_16B));
        img.drawPngFile(SD, filename, 0, 0);
    }
}



#endif