#include "NeoScreen.h"

NeoScreen::NeoScreen(Adafruit_NeoPixel &strip) : 
  _width(0),
  _height(0),
  _pixelMap(NULL),
  _strip(&strip)
{
}

bool NeoScreen::begin(NeoScreen_Lines_Type lineType, NeoScreenLine *lines, size_t numLines)
{
  size_t maxPixelsInLine = 0;
  for(uint32_t line = 0; line < numLines; line++) {
    size_t virtualPixelsInLine = lines[line].lineOffset + lines[line].lineLength;
    if(virtualPixelsInLine > maxPixelsInLine) {
      maxPixelsInLine = virtualPixelsInLine;
    }
  }

  if(_pixelMap) {
    delete _pixelMap;
    _pixelMap = NULL;
  }

  _width = NeoScreen_Lines_Rows == lineType ? numLines : maxPixelsInLine;
  _height = NeoScreen_Lines_Rows == lineType ? maxPixelsInLine : numLines;

  _pixelMap = new uint16_t[_width * _height];
  if(_pixelMap)
  {
    for(uint32_t r=0; r < getHeight(); r++) 
    { // For each row...
      for(uint32_t p=0; p < getWidth(); p++)
      { // For each pixel of row...
        NeoScreenLine &line = lines[NeoScreen_Lines_Rows == lineType ? r : p];
        size_t lineOffset = NeoScreen_Lines_Rows == lineType ? p : r;

        uint16_t pixel = UINT16_MAX;
        
        if(line.lineOffset <= lineOffset && lineOffset < line.lineOffset + line.lineLength)
        {
          lineOffset -= line.lineOffset;
          pixel = (NeoScreen_Direction_Normal == line.direction) ?
                    line.stripOffset + lineOffset : 
                    line.stripOffset + (line.lineLength - 1 - lineOffset);
        }

        _pixelMap[p + (r * _width)] = pixel;
      }
    }

    for(uint32_t r=0; r < getHeight(); r++) 
    { // For each row...
      for(uint32_t p=0; p < getWidth(); p++)
      {
        uint16_t pixel = _pixelMap[p + (r * _width)];
        if(pixel != UINT16_MAX) {
          Serial.printf(" % 4d", pixel);
        } else {
          Serial.print(" XXXX");
        }
      }
      Serial.println("");
    }

    return true;
  }

  return false;
}

void NeoScreen::render(uint32_t **frame)
{
  for(uint32_t r=0; r < getHeight(); r++) { // For each row...
    for(uint32_t p=0; p < getWidth(); p++) { // For each pixel of row...
      setPixel(p, r, frame[r][p]);
    }
  }
}

void NeoScreen::setPixel(uint32_t x, uint32_t y, uint32_t col)
{
  uint16_t pixel = _pixelMap[x + (y * _width)];
  if(UINT16_MAX != pixel) {
    _strip->setPixelColor(pixel, col);
  }
}

