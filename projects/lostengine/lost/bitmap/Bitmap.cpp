#include "lost/common/Data.h"
#include "lost/bitmap/Bitmap.h"
#include "stb_image.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include <sstream>
#include <math.h>
#include "lost/profiler/Blackbox.h"

  
namespace lost
{
namespace bitmap
{

#define bb_bitmap_key "lost.bitmap.Bitmap"

void Bitmap::reset()
{
  data = NULL;
  loaded = false;
  width = 0;
  height = 0;
  premultiplied = false;
  format = COMPONENTS_UNDEFINED;
}

Bitmap::Bitmap()
{
  reset();
  BB_INC(bb_bitmap_key);
}

Bitmap::Bitmap(uint32_t inWidth,
       uint32_t inHeight,
       Components format)
{
  reset();
  init(inWidth, inHeight, format);
  BB_INC(bb_bitmap_key);
}

Bitmap::Bitmap(uint32_t inWidth,
               uint32_t inHeight,
               Components destComponents,
               Components srcComponents,
               uint8_t* data)
{
  reset();
  init(inWidth, inHeight, destComponents, srcComponents, data);
  BB_INC(bb_bitmap_key);
}

Bitmap::Bitmap(const common::DataPtr& inData)
{
  reset();
  init(inData);
  BB_INC(bb_bitmap_key);
}

Bitmap::~Bitmap()
{
  destroy();
  BB_DEC(bb_bitmap_key);
}

void Bitmap::destroy()
{
  if(data && loaded)
  {
    stbi_image_free(data);
  }
  else if(data && !loaded)
  {
    delete [] data;
  }
  reset();
}

void Bitmap::init(uint32_t inWidth,
          uint32_t inHeight,
          Components inFormat)
{
  destroy();
  // create target memory
  uint32_t destBytesPerPixel = bytesPerPixelFromComponents(inFormat);
  uint32_t destSizeInBytes = destBytesPerPixel * inWidth * inHeight;
  data = new uint8_t[destSizeInBytes];
  loaded = false; // prevent stb_image from freeing

  width = inWidth;
  height = inHeight;
  format = inFormat;
}


void Bitmap::init(uint32_t inWidth,
                  uint32_t inHeight,
                  Components destComponents,
                  Components srcComponents,
                  uint8_t* srcData)
{
  // basic initialisation of target area, size and flags
  init(inWidth, inHeight, destComponents);
  uint32_t destBytesPerPixel = bytesPerPixelFromComponents(format);

  // setup stc values for loop
  uint32_t srcBytesPerPixel = bytesPerPixelFromComponents(srcComponents);
  uint8_t* destWriter = data;
  uint8_t* srcReader = srcData;
  uint32_t numPixels = inWidth*inHeight;

  // copy pixels
  for(uint32_t currentPixel=0; currentPixel<numPixels; ++currentPixel)
  {
    copyPixel(destWriter, destComponents, srcReader, srcComponents);
    destWriter+=destBytesPerPixel;
    srcReader+=srcBytesPerPixel;
  }
}

// FIXME: not endian safe, we need to fix this for big endian
// FIXME: not sure if this is even correct byte order for little endian
//
// BIG MESS ALERT!!!!!
//
// the whole conversion process from one component type to another should really be programmable, but I can't be bothered right now.
// this thing was really only meant to convert alpha to rgba. This was tweaked to work for the fonts, everything else is an
// untested bonus. Maybe we should really resort to using boost::gil, but this should be enough for now.
//
void Bitmap::copyPixel(uint8_t* dest,
                Components destComponents,
                uint8_t* src,
                Components srcComponents)
{
  switch(destComponents)
  {
    case COMPONENTS_RGBA:
      switch(srcComponents)
      {
        case COMPONENTS_RGBA:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];dest[3]=src[3];break;
        case COMPONENTS_RGB:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];dest[3]=255;break;
        case COMPONENTS_ALPHA:dest[0]=src[0];dest[1]=src[0];dest[2]=src[0];dest[3]=src[0];break;
        default:
          THROW_RTE("can't copy pixel from source with components: " << srcComponents);
      }
      break;
    case COMPONENTS_RGB:
      switch(srcComponents)
      {
        case COMPONENTS_RGBA:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];break;
        case COMPONENTS_RGB:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];break;
        case COMPONENTS_ALPHA:dest[0]=src[0];dest[1]=src[0];dest[2]=src[0];break;
        default:
          THROW_RTE("can't copy pixel from source with components: " << srcComponents);
      }
      break;
    case COMPONENTS_ALPHA:
      switch(srcComponents)
      {
        case COMPONENTS_RGBA:dest[0]=src[3];break;
        case COMPONENTS_RGB:dest[0]=255;break;
        case COMPONENTS_ALPHA:dest[0]=src[0];break;
        default:
          THROW_RTE("can't copy pixel from source with components: " << srcComponents);
      }
      break;
    default:
      THROW_RTE("can't copy pixel to destination with components: " << destComponents);
  }
}

uint32_t Bitmap::bytesPerPixelFromComponents(Components components)
{
  uint32_t result = 0;
  switch(components)
  {
    case COMPONENTS_ALPHA:result = 1;break;
    case COMPONENTS_RGB:result = 3;break;
    case COMPONENTS_RGBA:result = 4;break;
    default:
      THROW_RTE("can't determine bytes per pixel from components: " << components);
  }
  return result;
}

void Bitmap::init(const common::DataPtr& inData)
{
  destroy();
//  DOUT("init image from memory: " << inFile->location);
  int bytesPerPixel, w, h;
  data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(inData->bytes.get()), inData->size, &w, &h, &bytesPerPixel, 0);
  width = w;
  height = h;
  if(data == NULL)
  {
    // FIXME: stbi_failure_reason is not threadsafe
    throw std::runtime_error("couldn't init image from memory: "+ std::string(stbi_failure_reason()));
  }

  switch(bytesPerPixel)
  {
    case 3:format = COMPONENTS_RGB;break;
    case 4:format = COMPONENTS_RGBA;break;
    default:
      THROW_RTE("couldn't init image, don't know what to do with bytesPerPixel: " << bytesPerPixel);
  }
  loaded = true;
}

void Bitmap::clear(const lost::common::Color& inColor)
{
  uint32_t bpp = bytesPerPixelFromComponents(format);
  switch(bpp)
  {
    case 1:clearA(inColor);break;
    case 3:clearRGB(inColor);break;
    case 4:clearRGBA(inColor);break;
    default:
      THROW_RTE("couldn't clear image with bpp: " << bpp);
  }
}

void Bitmap::clearA(const lost::common::Color& inColor)
{
  uint8_t v = static_cast<uint8_t>(inColor.a()*255.0f);
  uint32_t numpixels = width * height;
  for(uint32_t i=0; i<numpixels; ++i)
    data[i] = v;
}

void Bitmap::clearRGB(const lost::common::Color& inColor)
{
  uint32_t numpixels = width * height;
  uint32_t bpp = 3;
  uint8_t* d = data;
  uint8_t r = static_cast<uint8_t>(inColor.r()*255.0f);
  uint8_t g = static_cast<uint8_t>(inColor.g()*255.0f);
  uint8_t b = static_cast<uint8_t>(inColor.b()*255.0f);
  uint32_t bytesize = numpixels * bpp;

  for(uint32_t i=0; i<bytesize; i+=bpp)
  {
    d[i+0] = r;
    d[i+1] = g;
    d[i+2] = b;
  }
}

void Bitmap::clearRGBA(const lost::common::Color& inColor)
{
  uint32_t numpixels = width * height;
  uint32_t bpp = 4;
  uint8_t* d = data;
  uint8_t r = static_cast<uint8_t>(inColor.r()*255.0f);
  uint8_t g = static_cast<uint8_t>(inColor.g()*255.0f);
  uint8_t b = static_cast<uint8_t>(inColor.b()*255.0f);
  uint8_t a = static_cast<uint8_t>(inColor.a()*255.0f);
  uint32_t bytesize = numpixels * bpp;

  for(uint32_t i=0; i<bytesize; i+=bpp)
  {
    d[i+0] = r;
    d[i+1] = g;
    d[i+2] = b;
    d[i+3] = a;
  }
}

void Bitmap::write(const string inFullPathname)
{
  if(!stbi_write_tga(inFullPathname.c_str(), width, height, bytesPerPixelFromComponents(format), data))
  {
    throw std::runtime_error("screenshot save failed");
  }
}

void Bitmap::flip()
{
  unsigned long pixelSizeBytes = bytesPerPixelFromComponents(format);
  // flip vertically because OpenGL returns it the other way round
  unsigned long lineInBytes = width * pixelSizeBytes;
  unsigned long halfHeight = height / 2; // deliberately round down if height is odd
  unsigned char* dataBytes = data;
  for(unsigned long bottomLine=0; bottomLine<halfHeight; ++bottomLine)
  {
    unsigned long topLine = (height - 1) - bottomLine;
    for(unsigned long bi=0; bi<lineInBytes; ++bi)
    {
      unsigned long topLineByte = width*topLine*pixelSizeBytes+bi;
      unsigned long bottomLineByte = width*bottomLine*pixelSizeBytes+bi;
      unsigned char b = dataBytes[topLineByte];
      dataBytes[topLineByte] = dataBytes[bottomLineByte];
      dataBytes[bottomLineByte] = b;
    }
  }
}

uint8_t* Bitmap::pixelPointer(uint32_t x, uint32_t y)
{
  uint32_t bpp = bytesPerPixelFromComponents(format);
  uint8_t* target = data+((x+(y*width))*bpp);
  return target;
}


void Bitmap::pixel(uint32_t x, uint32_t y, const common::Color& inColor)
{
  assert((x<width) && (y<height));
  uint8_t* target = pixelPointer(x,y);
  switch(format)
  {
    case COMPONENTS_ALPHA:target[0] =inColor.au8();break;
    case COMPONENTS_RGB:
      target[0]=inColor.ru8();
      target[1]=inColor.gu8();
      target[2]=inColor.bu8();
      break;
    case COMPONENTS_RGBA:
      target[0]=inColor.ru8();
      target[1]=inColor.gu8();
      target[2]=inColor.bu8();
      target[3]=inColor.au8();
      break;
    default:
      break;
  }
}

/** reads a pixel from the given coordinates and returns it as a Color. */
  common::Color Bitmap::pixel(uint32_t x, uint32_t y)
{
  uint8_t* target = pixelPointer(x,y);
  common::Color result;
  switch(format)
  {
    case COMPONENTS_ALPHA:
      result.r(1);
      result.g(1);
      result.b(1);
      result.au8(target[0]);break;
    case COMPONENTS_RGB:
      result.ru8(target[0]);
      result.gu8(target[1]);
      result.bu8(target[2]);
      result.a(1);
      break;
    case COMPONENTS_RGBA:
      result.ru8(target[0]);
      result.gu8(target[1]);
      result.bu8(target[2]);
      result.au8(target[3]);
      break;
    default:
      break;
  }
  return result;
}


lost::shared_ptr<Bitmap> Bitmap::rotCW()
{
  shared_ptr<Bitmap> result(new Bitmap(height, width, COMPONENTS_RGBA));
  for(uint32_t y=0; y<height; ++y)
  {
    for(uint32_t x=0; x<width; ++x)
    {
      result->pixel(y, x, pixel((width-1)-x,y));
    }
  }
  return result;
}

void Bitmap::hline(uint32_t y, uint32_t xl, uint32_t xr, const common::Color& inColor)
{
  for(uint32_t x=xl; x<xr; ++x)
  {
    pixel(x,y,inColor);
  }
}

void Bitmap::vline(uint32_t x, uint32_t yb, uint32_t yt, const common::Color& inColor)
{
  for(uint32_t y=yb; y<yt; ++y)
  {
    pixel(x,y,inColor);
  }
}

void Bitmap::draw(uint32_t x, uint32_t y, lost::shared_ptr<Bitmap> target)
{
  for(uint32_t cy=0; cy<height; ++cy)
  {
    for(uint32_t cx=0; cx<width; ++cx)
    {
      target->pixel(x+cx, y+cy, pixel(cx, cy));
    }
  }
}

void Bitmap::drawRectOutline(const lost::math::Rect& inRect, const common::Color& inColor)
{
  float minX = inRect.x;
  float minY = inRect.y;
  float maxX = inRect.maxX();
  float maxY = inRect.maxY();

  vline((uint32_t)minX, (uint32_t)minY, (uint32_t)maxY, inColor);
  vline((uint32_t)maxX, (uint32_t)minY, (uint32_t)maxY, inColor);
  hline((uint32_t)minY, (uint32_t)minX, (uint32_t)maxX, inColor);
  hline((uint32_t)maxY, (uint32_t)minX, (uint32_t)maxX, inColor);
}

void Bitmap::disc(float x, float y, float r)
{
  r = std::max(1.0f, r-1.0f);
  float w = (float)width;
  float h = (float)height;
  common::Color white(1,1,1,1);
  common::Color black(0,0,0,0);
  for(float cx=0; cx<w; cx+=1.0f)
  {
    for(float cy=0; cy<h; cy+=1.0f)
    {
      float dx = cx-x;
      float dy = cy-y;
      float cr = sqrtf(dx*dx+dy*dy);
      pixel((uint32_t)cx, (uint32_t)cy, (cr < r) ? white : black);
      float dr = cr-r;
      if(dr >= 0.0f && (dr < 1.0f))
      {
        float fract = 1.0f-(dr - floor(dr));
        pixel((uint32_t)cx, (uint32_t)cy, common::Color(1,1,1, fract));
      }
    }
  }
}

void Bitmap::ring(float x, float y, float r, float t)
{
  r = std::max(1.0f, r-1.0f);
  float w = (float)width;
  float h = (float)height;
  float lr = r - t;
  common::Color white(1,1,1,1);
  common::Color black(0,0,0,0);
  for(float cx=0; cx<w; cx+=1.0f)
  {
    for(float cy=0; cy<h; cy+=1.0f)
    {
      float dx = cx-x;
      float dy = cy-y;
      float cr = sqrtf(dx*dx+dy*dy);
      pixel((uint32_t)cx, (uint32_t)cy, ((cr < r) && (cr > lr)) ? white : black);

      // outer AA
      float dr = cr-r;
      if(dr >= 0.0f && (dr < 1.0f))
      {
        float fract = 1.0f-(dr - floor(dr));
        pixel((uint32_t)cx, (uint32_t)cy, common::Color(1,1,1,fract));
      }

      // inner AA
      dr = cr-lr;
      if(dr >= 0.0f && (dr < 1.0f))
      {
        float fract = (dr - floor(dr));
        pixel((uint32_t)cx, (uint32_t)cy, common::Color(1,1,1,fract));
      }
    }
  }
}

void Bitmap::filledRect(const common::Color& col, uint32_t posx, uint32_t posy, uint32_t sizew, uint32_t sizeh)
{
  uint32_t maxx = posx+sizew;
  uint32_t maxy = posy+sizeh;
  for(uint32_t x=posx; x<maxx; ++x)
  {
    for(uint32_t y=posy; y<maxy; ++y)
    {
      pixel(x,y,col);
    }
  }
}

void Bitmap::premultiplyAlpha()
{
  if(!premultiplied)
  {
    for(uint32_t x=0; x<width; ++x)
    {
      for(uint32_t y=0; y<height; ++y)
      {
        common::Color c = pixel(x,y);
        float a = c.fv[3];
        c.fv[0] = c.fv[0]*a;
        c.fv[1] = c.fv[1]*a;
        c.fv[2] = c.fv[2]*a;
        pixel(x,y,c);
      }
    }
    premultiplied = true;
  }
}


}
}
