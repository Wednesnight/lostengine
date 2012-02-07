#
# Copyright (c) 2011 Tony Kostanjsek, Timo Boll
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
# following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
{

  'variables': {

    # defines
    'SLUB_STRING_INCLUDE' : '<EASTL/string.h>',
    'SLUB_STRING_TYPE'    : 'eastl::string',
    'SLUB_LIST_INCLUDE'   : '<EASTL/list.h>',
    'SLUB_LIST_TYPE'      : 'eastl::list',
    'SLUB_MAP_INCLUDE'    : '<EASTL/map.h>',
    'SLUB_MAP_TYPE'       : 'eastl::map',

    # dependencies
    'boost_path': '../../thirdparty/boost/boost_1_44_0',
    'box2d_path': '../../thirdparty/box2d/box2d_2_1_2',
    'eastl_path': '../../thirdparty/eastl/fe4d920',
    'freetype2_path': '../../thirdparty/freetype2/freetype_2_4_3',
    'giflib_path': '../../thirdparty/giflib/giflib_4_1_6',
    'glee_path': '../../thirdparty/glee/glee_5_21',
    'md5_path': '../../thirdparty/md5/',
    'msinttypes_path': '../../thirdparty/msinttypes',
    'lua_path': '../../thirdparty/lua/lua_5_1_4',
    'openal_path': '../../thirdparty/OpenAL/OpenAL_1_1',
    'opengl_path': '../../thirdparty/OpenGL',
    'slub_path': '../../thirdparty/slub',
    'stb_image_path': '../../thirdparty/stb/stb_image_1_18',
    'stb_vorbis_path': '../../thirdparty/stb/stb_vorbis_0_99996',
    'utfcpp_path': '../../thirdparty/utfcpp/utfcpp_2_3_1',
    'tinythread_path': '../../thirdparty/tinythread/1_0/source',
    'unittest++_path': '../../thirdparty/UnitTest++',
    'zlib_path': '../../thirdparty/zlib/zlib_1_2_3',
    'zziplib_path': '../../thirdparty/zziplib/zziplib-0.13.58',

  },

  'target_defaults': {

    'defines': [
      'SLUB_STRING_INCLUDE=<(SLUB_STRING_INCLUDE)',
      'SLUB_STRING_TYPE=<(SLUB_STRING_TYPE)',
      'SLUB_LIST_INCLUDE=<(SLUB_LIST_INCLUDE)',
      'SLUB_LIST_TYPE=<(SLUB_LIST_TYPE)',
      'SLUB_MAP_INCLUDE=<(SLUB_MAP_INCLUDE)',
      'SLUB_MAP_TYPE=<(SLUB_MAP_TYPE)',
    ],

  },

}
