#ifndef NEOSCREEN_h
#define NEOSCREEN_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

typedef enum {
  NeoScreen_Lines_Rows,
  NeoScreen_Lines_Cols
} NeoScreen_Lines_Type;

typedef enum {
  NeoScreen_Direction_Normal,
  NeoScreen_Direction_Reverse
} NeoScreen_Direction;

struct NeoScreenLine
{
  uint32_t lineOffset;
  uint32_t lineLength;
  uint32_t stripOffset;
  NeoScreen_Direction direction;
};


class NeoScreen {
private:
  uint32_t _width;
  uint32_t _height;
  uint16_t *_pixelMap;
  Adafruit_NeoPixel *_strip;
public:
  NeoScreen(Adafruit_NeoPixel &strip);
  
  bool begin(NeoScreen_Lines_Type lineType, NeoScreenLine *lines, size_t numLines);

  void render(uint32_t **frame);
  void setPixel(uint32_t x, uint32_t y, uint32_t col);

  uint32_t getWidth() {
    return _width;
  }

  uint32_t getHeight() {
    return _height;
  }
};

#endif
