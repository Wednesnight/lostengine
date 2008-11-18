class Callable:
  def __init__(self, anycallable):
    self.__call__ = anycallable


##############################################################################
# check for boost libraries

class BoostCheck(object):

  def register_options(opts = None):
    from SCons.Script import PathOption
    if opts:
      opts.Add(PathOption('BOOST_PREFIX', 'Base path of boost installation; '
            'use this if auto-detection fails.', None))
  register_options = Callable(register_options)


  def check_for_boost(context, *args, **kw):
    test_file = """
#include <iostream>
#include <boost/version.hpp>

int main(int argc, char **argv)
{
  std::cout << (BOOST_VERSION / 100000) << "."
            << (BOOST_VERSION / 100 % 1000) << "."
            << BOOST_VERSION % 100
            << std::endl;
}
"""
    env = context.sconf.env

    lib_patterns = {}
    for libname in kw.get('LIBS', []):
      pattern = '%s(?P<LINK_NAME>boost_%s(-mt)?(-(?P<VERSION_MAJOR>[0-9]+)_(?P<VERSION_MINOR>[0-9]+))?)' % (env['LIBPREFIX'],
          libname)
      lib_patterns[libname] = [
        r'%s%s' % (pattern, env['LIBSUFFIX']),
        r'%s%s' % (pattern, env['SHLIBSUFFIX']),
      ]

    return env.check_for_lib(context, 'boost',
        header_only = True,
        test_file = test_file,
        suffix = '.cpp',
        prefix_var = 'BOOST_PREFIX',
        output_var = '%s_BOOST_VERSION' % env._custom_check_prefix,
        lib_patterns = lib_patterns,
        min_version = kw.get('min_version', ()),
        max_version = kw.get('max_version', ()))
  check_for_boost = Callable(check_for_boost)


  def init(self, testdict):
    if not self.has_key(testdict):
      self[testdict] = {}
    self[testdict]['BoostCheck'] = BoostCheck.check_for_boost
  init = Callable(init)



##############################################################################
# check for cppunit libraries

class CppUnitCheck(object):

  def register_options(opts = None):
    from SCons.Script import PathOption
    if opts:
      opts.Add(PathOption('CPPUNIT_PREFIX', 'Base path of cppunit installation;'
              ' use this if auto-detection fails.', None))
  register_options = Callable(register_options)


  def check_for_cppunit(context, *args, **kw):
    test_file = """
#include <iostream>
#include <cppunit/Portability.h>

int main(int argc, char **argv)
{
  std::cout << CPPUNIT_VERSION << std::endl;
}
"""
    return context.sconf.env.check_for_lib(context, 'cppunit',
        header_only = True,
        test_file = test_file,
        suffix = '.cpp',
        prefix_var = 'CPPUNIT_PREFIX',
        output_var = '%s_CPPUNIT_VERSION' % context.sconf.env._custom_check_prefix)
  check_for_cppunit = Callable(check_for_cppunit)


  def init(self, testdict):
    if not self.has_key(testdict):
      self[testdict] = {}
    self[testdict]['CppUnitCheck'] = CppUnitCheck.check_for_cppunit
  init = Callable(init)



##############################################################################
# check for fhtagn libraries

class FhtagnCheck(object):

  def register_options(opts = None):
    from SCons.Script import PathOption
    if opts:
      opts.Add(PathOption('FHTAGN_PREFIX', 'Base path of fhtagn installation; '
              'use this if auto-detection fails.', None))
  register_options = Callable(register_options)


  def check_for_fhtagn(context, *args, **kw):
    test_file = """
#include <iostream>
#include <fhtagn/version.h>

int main(int argc, char **argv)
{
  std::pair<uint16_t, uint16_t> version = fhtagn::version();
  std::cout << version.first << "." << version.second << std::endl;
}
"""
    return context.sconf.env.check_for_lib(context, 'fhtagn',
        test_file = test_file,
        suffix = '.cpp',
        prefix_var = 'FHTAGN_PREFIX',
        output_var = '%s_FHTAGN_VERSION' % context.sconf.env._custom_check_prefix)
  check_for_fhtagn = Callable(check_for_fhtagn)


  def init(self, testdict):
    if not self.has_key(testdict):
      self[testdict] = {}
    self[testdict]['FhtagnCheck'] = FhtagnCheck.check_for_fhtagn
  init = Callable(init)


##############################################################################
# check for cartograph

class CartographCheck(object):

  def register_options(opts = None):
    from SCons.Script import PathOption
    if opts:
      opts.Add(PathOption('CARTOGRAPH_PREFIX', 'Base path of cartograph '
            'installation; assumed to be a Framework.', None))
  register_options = Callable(register_options)


  def check_for_cartograph(context, *args, **kw):
    test_file = """
#include <iostream>
#include <cartograph/version.h>

int main(int argc, char **argv)
{
  std::pair<uint16_t, uint16_t> version = cartograph::version();
  std::cout << version.first << "." << version.second << std::endl;
}
"""
    return context.sconf.env.check_for_lib(context, 'cartograph',
        test_file = test_file,
        suffix = '.cpp',
        prefix_var = 'CARTOGRAPH_PREFIX',
        output_var = '%s_CARTOGRAPH_VERSION' % context.sconf.env._custom_check_prefix)
  check_for_cartograph = Callable(check_for_cartograph)


  def init(self, testdict):
    if not self.has_key(testdict):
      self[testdict] = {}
    self[testdict]['CartographCheck'] = CartographCheck.check_for_cartograph
  init = Callable(init)




##############################################################################
# check for OpenSceneGraph frameworks

class OSGCheck(object):

  def register_options(opts = None):
    from SCons.Script import PathOption
    if opts:
      opts.Add(PathOption('OSG_PREFIX', 'Base path of OpenSceneGraph '
            'installation; assumed to be a Framework.', None))
  register_options = Callable(register_options)


  def check_for_osg(context, *args, **kw):
    test_file = """
#include <iostream>
#include <osg/Version>

int main(int argc, char **argv)
{
  std::cout << osgGetVersion() << std::endl;
}
"""
    return context.sconf.env.check_for_framework_in(context, 'osg', test_file,
        '.cpp', 'OSG_PREFIX',
        '%s_OSG_VERSION' % context.sconf.env._custom_check_prefix)
  check_for_osg = Callable(check_for_osg)


  def init(self, testdict):
    if not self.has_key(testdict):
      self[testdict] = {}
    self[testdict]['OSGCheck'] = OSGCheck.check_for_osg
  init = Callable(init)


