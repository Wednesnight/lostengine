##############################################################################
# Custom Builders

def substitute_build_function(target, source, env):
  map(lambda s, t: env._substitute_file(s.rstr(), t.rstr()), source, target)
  return None

def substitute_build_str(target, source, env):
  import sys
  map(lambda t: sys.stdout.write("Generating `%s'...\n" % t.rstr()), target)


##############################################################################
# class ExtendedEnvironment

from SCons.Environment import Environment
class ExtendedEnvironment(Environment):
  """The ExtendedEnvironment class extends SCons.Environment.Environment by two
functions for checking for libraries/frameworks in default and user-defined
installation paths.
"""

  def Options(self, file_name, defaults = {}):
    from SCons.Script import Options, EnumOption, PathOption

    opts = Options(file_name)

    opts.Add('CXXFLAGS', 'Flags for the C++ compiler',
        defaults.get('CXXFLAGS',
          '-ansi -std=c++98 -Wall -Wsign-promo -fstrict-aliasing -Wstrict-aliasing'))

    self.BUILD_CONFIGS = {
      'debug': {
        'CPPDEFINES': { 'DEBUG': '1' },
        'CXXFLAGS': [ '-ggdb', '-O0' ],
      },
      'release': {
        'CPPDEFINES': { 'NDEBUG': '1' },
        'CXXFLAGS': [ '-ggdb', '-O2' ],
      },
    }
    opts.Add(EnumOption('BUILD_CONFIG', 'Target for the build',
          defaults.get('BUILD_CONFIG', 'release'),
          allowed_values = self.BUILD_CONFIGS.keys(),
        ))

    opts.Add(EnumOption('BUILD_LIB_TYPE', 'Type of library to build',
          defaults.get('BUILD_LIB_TYPE', 'both'),
          allowed_values = ['static', 'shared', 'both']))

    self.BUILD_PREFIX = 'BUILD_PREFIX'
    opts.Add(PathOption(self.BUILD_PREFIX, 'Build directory',
          defaults.get('BUILD_PREFIX', 'build'),
          PathOption.PathIsDirCreate))

    import os.path
    opts.INSTALL_PREFIX = 'INSTALL_PREFIX'
    opts.Add(PathOption(opts.INSTALL_PREFIX, 'Base path for the installation.',
            defaults.get('INSTALL_PREFIX',
              os.path.join(os.path.sep, 'opt', 'local')),
            PathOption.PathIsDirCreate))

    return opts


  def __init__(self, *args, **kw):
    ### Target resources lists
    self._sources = {}
    self._headers = {}
    self._libs = {}
    self._named_libs = {}

    #### Create regexes for later use
    import re
    self.__find_regex = re.compile(r'(@(.*?)@)')
    self.__match_regex = re.compile(r'^[a-zA-Z_]+$')

    ### Superclass
    Environment.__init__(self, *args, **kw)

    ### Custom builders
    from SCons.Builder import Builder
    from SCons.Action import Action
    substitute_builder = Builder(action = Action(
          substitute_build_function,
          substitute_build_str),
        src_suffix = '.in',
        name = 'SubstituteBuilder',
        explain = 'foo')
    self.Append(BUILDERS = {
        'Substitute' : substitute_builder,
    })

    ### Set a few variables, for autotools-compatibility
    import os.path
    self['prefix'] = self['INSTALL_PREFIX']
    self['exec_prefix'] = os.path.join(self['INSTALL_PREFIX'], 'bin')
    self['libdir'] = os.path.join(self['INSTALL_PREFIX'], 'lib')
    self['includedir'] = os.path.join(self['INSTALL_PREFIX'], 'include')

    ### Fixup Options
    self['CXXFLAGS'] = self['CXXFLAGS'].split()

    for k, v in self.BUILD_CONFIGS[self['BUILD_CONFIG']].items():
      if not self.has_key(k):
        self[k] = v
      else:
        if type(v) == type(dict()):
          self[k].update(v)
        elif type(v) == type(list()):
          self[k].extend(v)
        else:
          self[k] += v

    ### Store version
    if kw.has_key('version'):
      version_name = 'VERSION'
      if kw.has_key('name'):
        version_name = '%s_%s' % (kw['name'].upper(), version_name)
      self[version_name] = '%d.%d' % kw['version']


  def is_unix(self):
    import sys
    if sys.platform in ('darwin', 'linux2', 'sunos5'):
      return True
    elif sys.platform[:7] == 'freebsd':
      return True
    return False


  def register_check(self, check, opts = None):
    check.register_options(opts)
    if not self.__dict__.has_key('_custom_checks'):
      self._custom_checks = ()
    self._custom_checks += (check, )


  def init_checks(self, prefix, testdict):
    self._custom_check_prefix = prefix
    for c in self._custom_checks:
      c.init(self, testdict)


  def check_for_framework_in(self, context, text, test_file, suffix, prefixvar,
      outputvar):
    if self['PLATFORM'] != 'darwin':
      # No frameworks on this platform, falling back to libraries...
      return self.check_for_lib(context, text,
          test_file = test_file,
          suffix = suffix,
          prefix_var = prefixvar,
          output_var = outputvar)

    context.Message('Checking for framework %s... ' % text)

    import os.path
    install_dirs = [
      '',
      '/Library/Frameworks',
      os.path.expanduser('~/Library/Frameworks'),
    ]

    if self.has_key(prefixvar) and self[prefixvar]:
      install_dirs = [self[prefixvar]] + install_dirs

    for install_dir in install_dirs:
      old_flags = {
        'FRAMEWORKPATH': self['FRAMEWORKPATH'][:],
        'FRAMEWORKS': self['FRAMEWORKS'][:],
      }

      if install_dir:
        import os.path

        self['FRAMEWORKPATH'].append(install_dir)
        self['FRAMEWORKS'].append(text)


      result, output = context.TryRun(test_file, suffix)
      output = output.strip()

      if result:
        # keep modification to CXXFLAGS, also remember the install_dir in
        # prefixvar
        self[prefixvar] = install_dir
        self[outputvar] = output
        context.sconf.Define(outputvar, output,
            'Version of the installed %s framework' % text)
        context.Result(output)
        return (result, output)
      else:
        self['FRAMEWORKPATH'] = old_flags['FRAMEWORKPATH'][:]
        self['FRAMEWORKS'] = old_flags['FRAMEWORKS'][:]


    result = 0
    context.Result(result)

    # Fall back to libraries.
    return self.check_for_lib_in(context, text, test_file, suffix, prefixvar,
        outputvar, text)


  def _find_libs(self, dirs, patterns):
    # XXX currently only works on unices
    if not self.is_unix():
      raise AssertionError("Library patterns are only implemented on UNIX.")

    results = {}
    import os
    for install_dir in dirs:
      if not install_dir:
        install_dir = '/usr'

      lib_dir = os.path.join(install_dir, 'lib') # FIXME
      entries = os.listdir(lib_dir)
      for lib, libpatterns in patterns.items():
        for pattern in libpatterns:
          import re
          c = re.compile(pattern)
          for entry in entries:
            if c.match(entry):
              matches = c.search(entry)
              groups = matches.groupdict()
              # version
              version = []
              if groups.has_key('VERSION_MAJOR'):
                version.append(groups['VERSION_MAJOR'])
              if groups.has_key('VERSION_MINOR'):
                version.append(groups['VERSION_MINOR'])
              if groups.has_key('VERSION_MICRO'):
                version.append(groups['VERSION_MICRO'])
              if groups.has_key('VERSION_PATCH'):
                version.append(groups['VERSION_PATCH'])
              if groups.has_key('VERSION_BUILD'):
                version.append(groups['VERSION_BUILD'])

              if not results.has_key(lib):
                results[lib] = {}

              if results[lib].has_key(entry):
                # Skip identical results in different paths; we assume that the
                # earlier the path in the list, the more 'default' the location,
                # i.e. the more preferred the result
                continue

              results[lib][entry] = {
                'path' : install_dir,
                'version' : tuple(self.int_version(version)),
                'name' : groups.get('LINK_NAME', None),
              }

    # Filter results. We prefer:
    # a) A higher version number over a lower
    # b) A version number over none
    filtered_results = {}
    for name, info in results.items():
      highest_version = (None, None)
      for fname, data in info.items():
        if data['version'] > highest_version[0]:
          highest_version = (data['version'], fname)

      filtered_results[name] = info[highest_version[1]]

    # Post-process for sanity. If the path & version in the results don't match,
    # we'd better abort the build.
    path = None
    version = None
    for name, info in filtered_results.items():
      if not path:
        path = info['path']
      if not version:
        version = info['version']
      if version != info['version'] or path != info['path']:
        raise AssertionError('Found different libraries of the same package in '
            'different locations.')

    return filtered_results


  def int_version(self, version):
    result = []
    for v in version:
      try:
        result.append(int(v))
      except TypeError:
        result.append(v)
    return result


  def compare_version(self, version1, version2):
    mapped_version = map(lambda x, y: (x, y), self.int_version(version1),
        self.int_version(version2))
    result = 0
    for v in mapped_version:
      if v[0] == None or v[1] == None:
        continue
      if v[0] > v[1]:
        result = 1
        break
      elif v[0] < v[1]:
        result = -1
        break
    return result


  def check_for_lib(self, context, name, **kw):
    # Testfile suffix, default to C
    suffix = kw.get('suffix', '.c')
    # Testfile contents - in the default, it doesn't contain anything really.
    test_file = kw.get('test_file', "int main(int argc, char **argv) {}")
    # There might be a config variable with an installation directory to look
    # for the library in. The default assumes the name variable is very simple.
    prefix_var = kw.get('prefix_var', '%s_PREFIX' % name.upper())
    # When we detect the library's version, we save it in an output variable.
    output_var = kw.get('output_var', '%s_VERSION' % name.upper())
    # Define for Substitute that we've detected a library.
    have_var = kw.get('have_var', 'HAVE_%s' % name.upper())
    # In order to successfully link the test file, we might need a few extra
    # flags.
    extra_flags = kw.get('extra_flags', {})
    # The check might just require headers, no linking against a library.
    header_only = kw.get('header_only', False)
    # If library patterns are defined, the function needs to attempt and detect
    # file names based on these patterns...
    patterns = kw.get('lib_patterns', {})
    # If libnames are given, match those against patterns
    libnames = kw.get('lib_names', [])
    # The user might specify a version range
    min_version = kw.get('min_version', ())
    if type(min_version) == type(str()):
      min_version = min_version.split('.')
    max_version = kw.get('max_version', ())
    if type(max_version) == type(str()):
      max_version = max_version.split('.')

    # Validity checking.
    if libnames and header_only:
      raise AssertionError("You cannot specify lib_names and header_only at the"
          " same time.")
    if libnames and patterns and libnames != patterns.keys():
      raise AssertionError("If you specify lib_names and lib_patterns, the "
          "patterns must match lib_names exactly.")
    libnames += patterns.keys()

    context.Message('Checking for %s... ' % name)

    # Default dirs to check in.
    install_dirs = [
      '',
      '/usr/local',
      '/opt/local',
    ]

    # User-provided install dir?
    if self.has_key(prefix_var) and self[prefix_var]:
      install_dirs = [self[prefix_var]] + install_dirs

    # If library patterns are defined, we'll have to search all install dirs
    # for files matching them.
    pattern_results = {}
    libs = []
    lib_matches = {}
    version = None
    if patterns:
      # Find library files, then build the install_dirs and libs around the
      # findings.
      pattern_results = self._find_libs(install_dirs, patterns)
      install_dirs = []
      for lname, info in pattern_results.items():
        lib_matches[lname] = info['name']
        libs.append(info['name'])
        if not install_dirs.count(info['path']):
          install_dirs.append(info['path'])
        if not version:
          version = info['version']
    else:
      # Use the names from the command line, and the default install dirs
      libs = libnames
      if header_only:
        libs = []

    # Store libs
    pre_test_libs = context.AppendLIBS(libs)

    # Crawl through all installation directories
    for install_dir in install_dirs:
      # Save and extend current flags
      old_flags = {
        'CXXFLAGS': self['CXXFLAGS'][:],
        'LIBPATH': self['LIBPATH'][:],
      }

      old_libs = context.AppendLIBS(extra_flags.get('LIBS', []))
      self['CXXFLAGS'] += extra_flags.get('CXXFLAGS', [])
      self['LIBPATH'] += extra_flags.get('LIBPATH', [])

      # If there was an install dir (i.e. we're not just using the default
      # environment), we need to add to include and library search paths.
      if install_dir:
        import os.path

        include_dir = os.path.join(install_dir, 'include')
        self['CXXFLAGS'].append(self['INCPREFIX'] + include_dir
            + self['INCSUFFIX'])

        lib_dir = os.path.join(install_dir, 'lib')
        self['LIBPATH'].append(lib_dir)

      # Run test, save output from the test.
      result, output = context.TryRun(test_file, suffix)
      output = output.strip()

      if result:
        # Restore libs completely.
        context.SetLIBS(pre_test_libs)

        # keep modification to CXXFLAGS, also remember the install_dir in
        # prefix_var
        self[prefix_var] = install_dir

        # If there's no output (which would be the case with the default
        # test_file, we'll default to saying "found")
        if not output:
          output = 'found'

        # If there was a min_version or max_version specified, or searching for
        # patterns resulted in a specific version to link against, we might need
        # to do some checking on the output now.
        # We assume that version components are separated by dots.
        output_version = tuple(output.split('.'))

        # Check for exact version. Now the number of components may vary
        # between what's specified and what is detected.
        if version:
          if 0 != self.compare_version(version, output_version):
            context.Result(0)
            return 0

        if min_version:
          if self.compare_version(min_version, output_version) not in (-1, 0):
            context.Result('required at least %s but only found %s' % ('.'.join(min_version),
                  '.'.join(output_version)))
            return 0
        if max_version:
          if self.compare_version(max_version, output_version) not in (1, 0):
            context.Result('required at most %s but found %s' % ('.'.join(max_version),
                  '.'.join(output_version)))
            return 0

        self[output_var] = output
        context.sconf.Define(output_var, output,
            'Version of the installed %s library' % name)

        self[have_var] = 1
        context.sconf.Define(have_var, 1, 'Found %s library?' % name)

        if lib_matches:
          context.sconf.env.storeNamedLibs(name, lib_matches)

        context.Result(output)
        return (result, output)

      # Restore libs to pre-testrun values
      context.SetLIBS(old_libs)

      # Restore flags
      self['CXXFLAGS'] = old_flags['CXXFLAGS'][:]
      self['LIBPATH'] = old_flags['LIBPATH'][:]

      # continue on to try next path...

    context.SetLIBS(pre_test_libs)

    # no result.
    result = 0
    context.Result(result)
    return result



  def _substitute(self, line):
    """
    substitutes occurrences of @FOO@ patterns with the contents of the environment
    variable 'FOO'.
    """
    mod_line = line

    matches = self.__find_regex.findall(line)
    for pattern, variable in matches:
      if not self.__match_regex.match(variable):
        raise AssertionError('Illegal variable name "%s"' % variable)

      if not self.has_key(variable):
        continue

      import re
      r = re.compile(pattern)
      mod_line = r.sub(self[variable], mod_line)

    return mod_line


  def _substitute_file(self, in_filename, out_filename):
    """
    substitute() on all lines of in_filename, written to out_filename.
    """
    if not in_filename or not out_filename:
      raise AssertionError('Invalid input or output filename: %s, %s' % (
            in_filename, out_filename))

    context = []
    depth = 0
    for line in file(in_filename).readlines():
      line = self._substitute(line)

      if line.startswith('if'):
        cond = line[3:].strip()
        # For now, the only condition we check for is the existence of a variable.
        # By implication, the whole cond string must match a variable name.
        if not self.__match_regex.match(cond):
          raise AssertionError('Illegal condition "%s" in if-statement' % cond)
        depth += 1
        if len(context) < (depth + 1):
          context.append({'content': []})
        if not self.has_key(cond):
          context[depth]['skip'] = True

      elif line.startswith('else'):
        if context[depth].has_key('skip'):
          del context[depth]['skip']
          context[depth]['content'] = []
        else:
          context[depth - 1]['content'] += context[depth]['content']
          context[depth]['content'] = []
          context[depth]['skip'] = True

      elif line.startswith('endif'):
        # Paste the contents of the current context onto the parent context and
        # then delete it.
        if not context[depth].has_key('skip'):
          context[depth - 1]['content'] += context[depth]['content']
        context.pop()
        depth -= 1

      else:
        if len(context) < (depth + 1):
          context.append({'content': []})
        context[depth]['content'].append(line)

    file(out_filename, 'w').writelines(context[0]['content'])



  def addSources(self, target, sources):
    if not self._sources.has_key(target):
      self._sources[target] = []
    self._sources[target].extend(self.arg2nodes(sources))


  def addLibs(self, target, libs):
    if not self._libs.has_key(target):
      self._libs[target] = []
    for lib in libs:
      if type(lib) == type(str()):
        self._libs[target].append(lib)
      elif type(lib) in (type(list()), type(tuple())):
        if not self._named_libs.has_key(lib[0]):
          raise AssertionError, 'No named library "%s" known.' % lib[0]
        if not self._named_libs[lib[0]].has_key(lib[1]):
          raise AssertionError, 'No library "%s" known for named lib "%s".' % (lib[1], lib[0])
        self._libs[target].append(self._named_libs[lib[0]][lib[1]])


  def addHeaders(self, target, headers):
    if not self._headers.has_key(target):
      self._headers[target] = []
    self._headers[target].extend(self.arg2nodes(headers))


  def storeNamedLibs(self, name, matches):
    if not self._named_libs.has_key(name):
      self._named_libs[name] = {}
    self._named_libs[name].update(matches)


  def getNamedLibs(self, name, libs):
    if not self._named_libs.has_key(name):
      return []
    retval = []
    for lib in libs:
      if self._named_libs[name].has_key(lib):
        retval.append(self._named_libs[name][lib])
    return retval

  def getSources(self, target):
    return self._sources.get(target, [])


  def getLibs(self, target):
    return self._libs.get(target, [])


  def getHeaders(self, target):
    return self._headers.get(target, [])


  def stripBuildPrefix(self, file_list):
    retval = []
    import os.path

    build_prefix = self[self.BUILD_PREFIX]

    for filename in file_list:
      build_path = filename.get_path()
      if build_path.startswith(build_prefix):
        build_path = build_path[len(build_prefix):]
        while build_path[0] == os.path.sep:
          build_path = build_path[1:]

      retval.append(build_path)

    return retval


  def getHeadersRelative(self, target):
    retval = {}
    import os.path

    build_prefix = self[self.BUILD_PREFIX]

    for header in self.getHeaders(target):
      build_path = header.get_path()
      if not build_path.startswith(build_prefix):
        import sys
        sys.stderr.write('Header file %s in unknown location.\n' % build_path)
        continue

      build_path = build_path[len(build_prefix):]
      while build_path[0] == os.path.sep:
        build_path = build_path[1:]

      path, filename = os.path.split(build_path)

      if not retval.has_key(path):
        retval[path] = []
      retval[path].append(header)

    return retval.items()


  def getManifestFiles(self):
    import os

    filenames = [l.strip() for l in file('MANIFEST', 'rb').readlines()]

    def visitor(arg, dirname, entries):
      import fnmatch
      filtered_entries = []
      for pattern in ('*.pyc', '.*.sw*', '.svn'):
        filtered_entries += fnmatch.filter(entries, pattern)
      for fe in filtered_entries:
        i = entries.index(fe)
        del entries[i]

      arg += [os.path.join(dirname, e) for e in entries]

    result = []
    for p in filenames:
      if os.path.isdir(p):
        os.path.walk(p, visitor, result)
      else:
        result.append(p)
    return result

