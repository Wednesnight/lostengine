#include <iostream>

// spirit::real_p (double) assignment to float
#pragma warning(disable:4244)

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/bind.hpp>

#include "lost/obj/Parser.h"
#include "lost/obj/Scene.h"
#include "lost/math/Vec4.h"
#include "lost/common/SpiritHelpers.h"

using namespace std;
using namespace boost;
using namespace boost::spirit;
using namespace lost::math;
using namespace lost::common::spirithelpers;

namespace lost
{
  namespace obj
  {
    namespace parser
    {

      bool parse(const std::string& data, Scene& newscene)
      {
          Vec4 vertex;
          Vec4 normal;
          Vec4 texcoord;
          Vertex curvert;
          obj::Face curface;
          
          rule<> comment_p = ch_p('#') >> *(anychar_p-eol_p) >> eol_p;
          rule<> empty_p = *(space_p-eol_p) >> eol_p;
          rule<> vertex_p =       ch_p('v') 
                              >> +space_p >> real_p[assign_a(vertex.x)] 
                              >> +space_p >> real_p[assign_a(vertex.y)]
                              >> +space_p >> real_p[assign_a(vertex.z)]
                              >> !(+space_p >> real_p[assign_a(vertex.w)])
                              >> *(space_p-eol_p) >> eol_p;
          rule<> normal_p =       str_p("vn") 
                              >> +space_p >> real_p[assign_a(normal.x)] 
                              >> +space_p >> real_p[assign_a(normal.y)]
                              >> +space_p >> real_p[assign_a(normal.z)]
                              >> *(space_p-eol_p) >> eol_p;
          rule<> texcoord_p =       str_p("vt") 
                              >> +space_p >> real_p[assign_a(texcoord.x)] 
                              >> +space_p >> real_p[assign_a(texcoord.y)]
                              >> !(+space_p >> real_p[assign_a(texcoord.z)])
                              >> *(space_p-eol_p) >> eol_p;
          rule<> group_p = ch_p('g') >> *(anychar_p - eol_p) >> eol_p;
          rule<> smoothgroup_p = ch_p('s') >> *(anychar_p - eol_p) >> eol_p;
          rule<> materiallib_p = str_p("mtllib") >> *(anychar_p - eol_p) >> eol_p;
          rule<> object_p = str_p("o") >> *(anychar_p - eol_p) >> eol_p;
          rule<> usematerial_p = str_p("usemtl") >> *(anychar_p - eol_p) >> eol_p;
          rule<> facevert_p =    int_p[assign_a(curvert.vertex_index)]
                              >> !(   '/'
                                   >>  !(int_p[assign_a(curvert.texcoord_index)])
                                   >>  !(   '/'
                                         >> !(int_p[assign_a(curvert.normal_index)])
                                        )
                                  );                   
          rule<> face_p = ch_p('f') >> +space_p
                                    >> list_p(facevert_p[push_back(curface.vertices, curvert)] // add vertex to face if successfully parsed
                                                        [bind(&Vertex::clear, &curvert)], // and clear it afterwards
                                              +space_p)
                                    >> *(space_p-eol_p) >> eol_p;                                                       
          rule<> unknown_p = (+alpha_p >> *(anychar_p-eol_p))[print("unknown entity: ")] >> eol_p;
          rule<> objfile_p = *(comment_p
                               |empty_p
                               |vertex_p[push_back(newscene.vertices, vertex)]
                                        [bind(&Vec4::clear, &vertex)]
                               |normal_p[push_back(newscene.normals, normal)]
                                        [bind(&Vec4::clear, &normal)]
                               |texcoord_p[push_back(newscene.texcoords, texcoord)]
                                        [bind(&Vec4::clear, &texcoord)]
                               |group_p
                               |face_p[push_back(newscene.faces, curface)]
                                      [bind(&Face::clear, &curface)]
                               |smoothgroup_p
                               |materiallib_p
                               |usematerial_p
                               |object_p
                               |unknown_p);
          
          BOOST_SPIRIT_DEBUG_NODE(comment_p);
          BOOST_SPIRIT_DEBUG_NODE(empty_p);
          BOOST_SPIRIT_DEBUG_NODE(vertex_p);
          BOOST_SPIRIT_DEBUG_NODE(unknown_p);
          BOOST_SPIRIT_DEBUG_NODE(normal_p);
          BOOST_SPIRIT_DEBUG_NODE(group_p);
          BOOST_SPIRIT_DEBUG_NODE(face_p);
          BOOST_SPIRIT_DEBUG_NODE(facevert_p);
          BOOST_SPIRIT_DEBUG_NODE(objfile_p);
              
          return boost::spirit::parse(data.c_str(), objfile_p).full;
      }

    } // parser
  } // obj
}
