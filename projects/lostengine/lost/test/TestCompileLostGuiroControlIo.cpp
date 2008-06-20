#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

// unfortunately, due to the way boost::serialization works, we have to put all control serialization
// headers into one cpp file. Failing to do so will result in duplicate symbols during link stage.
#include "lost/guiro/io/ImageIo.h"
#include "lost/guiro/io/LabelIo.h"
#include "lost/guiro/io/QuadIo.h"
#include "lost/guiro/io/ViewIo.h"
#include "lost/guiro/io/EditIo.h"

#include "UnitTest++.h"


TEST(compile_image_io)
{
}
