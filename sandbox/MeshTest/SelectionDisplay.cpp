#include "SelectionDisplay.h"
#include "lost/mesh/Rect.h"
#include "lost/mesh/Quad.h"
#include "lost/math/Vec3.h"
#include "lost/math/Matrix.h"
#include "lost/camera/Camera2D.h"
#include "lost/rg/Camera.h"
#include "lost/rg/DepthTest.h"

using namespace std;
using namespace lost;
using namespace lost::camera;
using namespace lost::math;
using namespace lost::mesh;
using namespace lost::font;
using namespace lost::common;

SelectionDisplay::SelectionDisplay(lost::font::TrueTypeFontPtr inFont)
{
  fnt = inFont;
  offColor = blackColor;
  onColor = redColor;
}

SelectionDisplay::~SelectionDisplay()
{
}

void SelectionDisplay::description(const std::string& str)
{
  MeshPtr txt = fnt->render(str, descriptionFontSize);
  txt->modelTransform = MatrixTranslation(Vec3(descriptionRect.x, descriptionRect.y, 0));
  descriptionNode->mesh = txt;
}

void SelectionDisplay::title(const std::string& str)
{
  MeshPtr txt = fnt->render(str, titleFontSize);
  txt->modelTransform = MatrixTranslation(Vec3(titleRect.x, titleRect.y, 0));
  titleNode->mesh = txt;
}

void SelectionDisplay::addLed(int i)
{
  float cornerRadius = 7;

  // background
  Rect current(displayRect.x+displayMargin+titleWidth+(i*(ledSpacing+ledSquareSize)),
               displayRect.y+displayMargin+ledSpacing+descriptionHeight,
               ledSquareSize,
               ledSquareSize);
  MeshPtr m = mesh::FilledRoundedRect2D::create(current,cornerRadius);
  m->material->color = offColor;
  rg::NodePtr n = rg::Draw::create(m);
  ledBgNodes.push_back(static_pointer_cast<rg::Draw>(n));
  rootNode->add(n);

  // white border
  rootNode->add(rg::Draw::create(mesh::RoundedRect2D::create(current,cornerRadius)));
  
  // number
  ostringstream os;
  os << i+1;
  string num = os.str(); 
  RenderedTextPtr txt = fnt->render(num, ledFontSize);
  Vec3 translation(current.x+((current.width-txt->size.width)/2),
                   current.y+((current.height-txt->size.height)/2),
                   0);
  txt->modelTransform = MatrixTranslation(translation);
  rootNode->add(rg::Draw::create(txt));
}

void SelectionDisplay::build(const lost::math::Vec2& sz,
                             const std::vector<std::string>& inTitles,
                             const std::vector<std::string>& inDescriptions)
{
  titles = inTitles;
  descriptions = inDescriptions;
  screenSize = sz;

  if(inTitles.size() != inDescriptions.size())
    throw runtime_error("number of titles and descriptions must match");

  if(inTitles.size() == 0)
    throw runtime_error("number of entries must be larger than 0");

  numLeds = titles.size();

  ledSquareSize = 30;
  ledSquareFontSize = 20; // canbe fairly large since we only render one number per square
  ledSpacing = 10; // spacing between leds and title
  ledFontSize = 20;

  titleFontSize = 18;
  titleWidth = 100;
  titleHeight = ledSquareSize;

  descriptionFontSize = 11;
  float minDescriptionWidth = 300;
  descriptionWidth = std::max((numLeds*(ledSquareSize+ledSpacing))+titleWidth, minDescriptionWidth);
  descriptionHeight = descriptionFontSize;

  displayMargin = 10;

  float height = (ledSquareSize+ledSpacing+descriptionHeight+2*displayMargin);
  float width = (displayMargin*2)+descriptionWidth;
  displayRect = Rect((screenSize.width-width)/2, // centered horizontally on screen
                     screenSize.height-height,  // aligned to top edge
                     width,
                     height);

  titleRect = Rect(displayRect.x+displayMargin,
                   displayRect.y+displayMargin+2*ledSpacing+descriptionHeight,
                   titleWidth,
                   titleHeight);

  descriptionRect = Rect(displayRect.x+displayMargin,
                         displayRect.y+displayMargin,
                         descriptionWidth,
                         descriptionHeight);

  titleNode = static_pointer_cast<rg::Draw>(rg::Draw::create(mesh::Quad2D::create())); // create a dummy quad // FIXME: how do we create NULL MeshPtr ?
  title("Testtitle");

  descriptionNode = static_pointer_cast<rg::Draw>(rg::Draw::create(mesh::Quad2D::create())); // create a dummy quad // FIXME: how do we create NULL MeshPtr ?
  description("a longer test description for the current test scene");

  // create root node
  rootNode = rg::Node::create();
  CameraPtr camera = Camera2D::create(Rect(0,0,screenSize.width,screenSize.height));
  rootNode->add(rg::Camera::create(camera));
  rootNode->add(rg::DepthTest::create(false));
  // translucent black background
  MeshPtr m = Quad2D::create(displayRect);
  m->material->color = Color(0,0,0,.6);
  m->material->blend = true;
  rootNode->add(rg::Draw::create(m));
  // white border on black
  rootNode->add(rg::Draw::create(Rect2D::create(displayRect)));
  
  rootNode->add(titleNode);
  rootNode->add(descriptionNode);
  
  for(int i=0; i<titles.size(); ++i)
  {
    addLed(i);
  }
  
}
  
void SelectionDisplay::highlight(int selection)
{
  title(titles[selection]);
  description(descriptions[selection]);
  for(int i=0; i<ledBgNodes.size(); ++i)
  {
    if(i==selection)
      ledBgNodes[i]->mesh->material->color = onColor;
    else
      ledBgNodes[i]->mesh->material->color = offColor;    
  }
}
