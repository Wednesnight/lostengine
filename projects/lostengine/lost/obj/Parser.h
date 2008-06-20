#ifndef _OBJ_PARSER_H_
#define _OBJ_PARSER_H_

#include <string>
#include "objlib/scene.h"

namespace obj
{

namespace parser
{    
    /** parses the textual data into
     * the supplied scene object.
     *
     * @return true if data was fully understood, false otherwise.
     */
    bool parse(const std::string& data,
               obj::scene& newscene);
}

}

#endif 