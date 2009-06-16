#ifndef SELECTIONDISPLAY_H
#define SELECTIONDISPLAY_H

#include "lost/rg/Node.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/rg/Draw.h"

struct SelectionDisplay
{
  SelectionDisplay(lost::font::TrueTypeFontPtr inFont);
  ~SelectionDisplay();
    
  // size of titles and descriptions must be same or it will throw
  // titles and descriptions must not be empty, but can be empty strings
  // the number of selections will be derived from the vector sizes, so if you want 5 
  // sections 1-5, add 5 strings to titles and descriptions
  void build(const lost::math::Vec2& screenSize,
             const std::vector<std::string>& titles,
             const std::vector<std::string>& descriptions); 
  
  // index must be zero based, no error checking performed
  void highlight(int selection); // highlights the new selection, lowlights all others, sets title and description from arrays passed to build()

  void description(const std::string& str); // renders the given string and sets the render node acoorindingly
  void title(const std::string& str); // renders the given string and sets the render node acoorindingly
  void addLed(int i); // creates a led by adding background to vector and root, adding frame to root, rendering int and adding it to root

  lost::rg::NodePtr rootNode;
  lost::rg::DrawPtr titleNode;
  lost::rg::DrawPtr descriptionNode;
  
  std::vector<lost::rg::DrawPtr> ledBgNodes;
  
  std::vector<std::string> titles;
  std::vector<std::string> descriptions;
  lost::math::Vec2 screenSize;

  float numLeds;
  
  float ledSquareSize;
  float ledSquareFontSize;
  float ledSpacing;
  float ledFontSize;
  lost::common::Color offColor;
  lost::common::Color onColor;
  
  float titleFontSize;
  float titleWidth;
  float titleHeight;

  float descriptionFontSize;
  float descriptionWidth;
  float descriptionHeight;

  float displayHeight;
  float displayWidth;
  float displayMargin;
  
  lost::math::Rect displayRect;
  lost::math::Rect titleRect;
  lost::math::Rect descriptionRect;
  
  lost::font::TrueTypeFontPtr fnt;
};

#endif