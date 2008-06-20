#include "lost/guiro/control/View.h"
#include "UnitTest++.h"

using namespace lost::guiro;
using namespace lost::guiro::control;
using namespace lost;
using namespace std;


SUITE(ViewTest)
{
  TEST(construction)
  {
    boost::shared_ptr<View> view(new View("root"));
    CHECK(view);
  }
  
  TEST(viewpath_traversal)
  {

    boost::shared_ptr<View> ui(new View("ui"));
    boost::shared_ptr<View> rootview(new View("root"));
    boost::shared_ptr<View> childview(new View("child"));
    boost::shared_ptr<View> childchildview(new View("childchild"));
    
    rootview->add(childview);
    childview->add(childchildview);
    ui->add(rootview);
    CHECK((*rootview)["child"] == childview);
    CHECK((*rootview)["child/childchild"] == childchildview);
    CHECK((*ui)["root/child/childchild"] == childchildview);
    CHECK_THROW((*rootview)[""], std::exception);
    CHECK_THROW((*rootview)["child/missing"], std::exception);
    DOUT("chello");
  }
}
