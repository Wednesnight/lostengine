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

  'includes': [
    'settings.gypi',
  ],

  'targets': [
    {

      'target_name': 'Launcher',

      'type': 'executable',

      'default_configuration': 'Debug',

      'msvs_guid': '472D67EC-ABEA-C7DD-DBE2-7FC039681EE8',

      'configurations': {

        'Common': {
          'abstract': 1,
          'xcode_settings': {
#            'ARCHS': '$(NATIVE_ARCH_ACTUAL)',
            'ARCHS': '$(ARCHS_STANDARD_32_BIT)',
            'CODE_SIGN_IDENTITY[sdk=iphoneos*]': 'iPhone Developer',
            'ONLY_ACTIVE_ARCH': 'YES',
            'IPHONEOS_DEPLOYMENT_TARGET': '5.0',
            'VALID_ARCHS': 'i386 x86_64 armv6 armv7',
            'SDKROOT': 'iphoneos',
            'TARGETED_DEVICE_FAMILY': '1,2',
            'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',
            'GCC_C_LANGUAGE_STANDARD': 'c99',
            'GCC_WARN_ABOUT_RETURN_TYPE': 'YES',
            'GCC_WARN_UNUSED_VARIABLE': 'YES',
            'GCC_PRECOMPILE_PREFIX_HEADER': 'YES',
            'GCC_PFE_FILE_C_DIALECTS': 'c++ objective-c++',
            'GCC_INCREASE_PRECOMPILED_HEADER_SHARING': 'YES',
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
            'GCC_OBJC_CALL_CXX_CDTORS': 'YES',
            'ALWAYS_SEARCH_USER_PATHS': 'NO',
#            'INFOPLIST_FILE': 'xcode/LostEngineLauncher-Info.plist',
            'INFOPLIST_FILE': 'xcode/Launcher-ios-Info.plist',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ObjectFile': '$(IntDir)\\%(RelativeDir)',
             },
           },
        },

        'Debug': {
          'inherit_from': ['Common'],
          'xcode_settings': {
            'GCC_OPTIMIZATION_LEVEL': '0',
            'GCC_PREFIX_HEADER': 'lost/prefix-debug.pch',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'Optimization': '0',
              'PreprocessorDefinitions': ['_DEBUG'],
              'ForcedIncludeFiles': 'lost/prefix-debug.pch',
            },
            'VCLinkerTool': {
              'AdditionalDependencies': ['msvcrtd.lib'],
              'AdditionalOptions': ['/DEBUG', '/NODEFAULTLIB:msvcrt.lib'],
            },
          },
          'conditions': [
            ['OS == "linux"', {
              'cflags_cc': [
                '-includelost/prefix-debug.pch',
              ],
            }],
          ],
        },

        'Release': {
          'inherit_from': ['Common'],
          'xcode_settings': {
            'GCC_OPTIMIZATION_LEVEL': '3',
            'GCC_PREFIX_HEADER': 'lost/prefix-release.pch',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'Optimization': '2',
              'PreprocessorDefinitions': ['NDEBUG'],
              'ForcedIncludeFiles': 'lost/prefix-release.pch',
            },
          },
          'conditions': [
            ['OS == "linux"', {
              'cflags_cc': [
                '-includelost/prefix-release.pch',
              ],
            }],
          ],
        },

      },

      'dependencies': [
        'LostEngine'
      ],

      'include_dirs': [
        '.',
        '<@(eastl_path)/include',
      ],

      'sources': [
        'main.cpp',
      ],

      'conditions': [

        ['OS == "mac"', {

          'libraries': [
#            '/System/Library/Frameworks/Cocoa.framework',
            '/System/Library/Frameworks/OpenAL.framework',
            '/System/Library/Frameworks/OpenGLES.framework',
            '/System/Library/Frameworks/Foundation.framework',
            '/System/Library/Frameworks/UIKit.framework',
            '/System/Library/Frameworks/QuartzCore.framework',
          ],

          'mac_bundle': 1,

          'mac_bundle_resources': [
            'LostEngine.icns',
            'resources/lost',
            '../samples/Guiroscope.tasklet',
          ],

          'postbuilds': [{
            'variables': {
              # Define copy_resources_path in a variable ending in _path
              # so that gyp understands it's a path and performs proper
              # relativization during dict merging.
              'copy_resources_path':
              '<(DEPTH)/xcode/copy_resources-ios.sh',
            },
            'postbuild_name': 'Copy Resources',
            'action': ['<(copy_resources_path)'],
          }],

        }],

        ['OS == "win"', {

          'defines': [
            'WIN32',
            'UNICODE',
            '_CHAR16T',
          ],

          'include_dirs': [
            '<@(msinttypes_path)',
          ],

          'postbuilds': [{
            'variables': {
              # Define copy_resources_path in a variable ending in _path
              # so that gyp understands it's a path and performs proper
              # relativization during dict merging.
              'copy_resources_path':
              '<(DEPTH)/copy_resources.bat',
            },
            'postbuild_name': 'Copy Resources',
            'action': ['<(copy_resources_path)'],
          }],

        }],

        ['OS == "linux"', {
          'postbuilds': [{
            'variables': {
              # Define copy_resources_path in a variable ending in _path
              # so that gyp understands it's a path and performs proper
              # relativization during dict merging.
              'copy_resources_path':
              '<(DEPTH)/scons/copy_resources.sh',
            },
            'postbuild_name': 'Copy Resources',
            'action': ['<(copy_resources_path)'],
          }],
        }],
      ],

    },

    {

      'target_name': 'UnitTests',

      'type': 'executable',

      'default_configuration': 'Debug',

      'msvs_guid': '758C194B-7D13-83E3-8A41-A6FC2E6B722D',

      'configurations': {

        'Common': {
          'abstract': 1,
          'xcode_settings': {
            'ARCHS': '$(NATIVE_ARCH_ACTUAL)',
            'ONLY_ACTIVE_ARCH': 'YES',
            'VALID_ARCHS': 'i386 x86_64 armv6 armv7',
            'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',
            'GCC_C_LANGUAGE_STANDARD': 'c99',
            'GCC_WARN_ABOUT_RETURN_TYPE': 'YES',
            'GCC_WARN_UNUSED_VARIABLE': 'YES',
            'GCC_PRECOMPILE_PREFIX_HEADER': 'YES',
            'GCC_PFE_FILE_C_DIALECTS': 'c++ objective-c++',
            'GCC_INCREASE_PRECOMPILED_HEADER_SHARING': 'YES',
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
            'GCC_OBJC_CALL_CXX_CDTORS': 'YES',
            'ALWAYS_SEARCH_USER_PATHS': 'NO',
#            'INFOPLIST_FILE': 'xcode/LostEngineUnittests-Info.plist',
            'INFOPLIST_FILE': 'xcode/Launcher-ios-Info.plist',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ObjectFile': '$(IntDir)\\%(RelativeDir)',
             },
           },
        },

        'Debug': {
          'inherit_from': ['Common'],
          'xcode_settings': {
            'GCC_OPTIMIZATION_LEVEL': '0',
            'GCC_PREFIX_HEADER': 'lost/prefix-debug.pch',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'Optimization': '0',
              'PreprocessorDefinitions': ['_DEBUG'],
              'ForcedIncludeFiles': 'lost/prefix-debug.pch',
            },
            'VCLinkerTool': {
              'AdditionalDependencies': ['msvcrtd.lib'],
              'AdditionalOptions': ['/DEBUG', '/NODEFAULTLIB:msvcrt.lib'],
            },
          },
          'conditions': [
            ['OS == "linux"', {
              'cflags_cc': [
                '-includelost/prefix-debug.pch',
              ],
            }],
          ],
        },

        'Release': {
          'inherit_from': ['Common'],
          'xcode_settings': {
            'GCC_OPTIMIZATION_LEVEL': '3',
            'GCC_PREFIX_HEADER': 'lost/prefix-release.pch',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'Optimization': '2',
              'PreprocessorDefinitions': ['NDEBUG'],
              'ForcedIncludeFiles': 'lost/prefix-release.pch',
            },
          },
          'conditions': [
            ['OS == "linux"', {
              'cflags_cc': [
                '-includelost/prefix-release.pch',
              ],
            }],
          ],
        },

      },

      'dependencies': [
        'LostEngine'
      ],

      'include_dirs': [
        '.',
        '<@(tinythread_path)',
        '<@(eastl_path)/include',
        '<@(unittest++_path)/include',
      ],

      'sources': [

        # test
        'lost/test/main.cpp',
        'lost/test/TestCompileLostMathVec2.cpp',
        'lost/test/TestEvent.cpp',
        'lost/test/TestEventDispatcher.cpp',
        'lost/test/TestGlUtils.cpp',
        'lost/test/TestLostGlHostBuffer.cpp',
        'lost/test/TestLostMathMatrix.cpp',
        'lost/test/TestLostMathQuat.cpp',
        'lost/test/TestLostMathVec4.cpp',
        'lost/test/TestPlatform.cpp',
        'lost/test/TestProfiler.cpp',
        'lost/test/TestTimer.cpp',

        # UnitTest++
        '<@(unittest++_path)/include/XmlTestReporter.cpp',
        '<@(unittest++_path)/include/TimeConstraint.cpp',
        '<@(unittest++_path)/include/TestRunner.cpp',
        '<@(unittest++_path)/include/TestResults.cpp',
        '<@(unittest++_path)/include/TestReporterStdout.cpp',
        '<@(unittest++_path)/include/TestReporter.cpp',
        '<@(unittest++_path)/include/TestList.cpp',
        '<@(unittest++_path)/include/TestDetails.cpp',
        '<@(unittest++_path)/include/Test.cpp',
        '<@(unittest++_path)/include/ReportAssert.cpp',
        '<@(unittest++_path)/include/MemoryOutStream.cpp',
        '<@(unittest++_path)/include/DeferredTestResult.cpp',
        '<@(unittest++_path)/include/DeferredTestReporter.cpp',
        '<@(unittest++_path)/include/Checks.cpp',
        '<@(unittest++_path)/include/AssertException.cpp',
        '<@(unittest++_path)/include/Posix/TimeHelpers.cpp',
        '<@(unittest++_path)/include/Posix/SignalTranslator.cpp',

      ],

      'conditions': [
        ['OS == "mac"', {

          'libraries': [
            '/System/Library/Frameworks/Cocoa.framework',
            '/System/Library/Frameworks/OpenAL.framework',
            '/System/Library/Frameworks/OpenGL.framework',
          ],

        }],

        ['OS == "win"', {

          'include_dirs': [
            '<@(msinttypes_path)',
          ],

          'defines': [
            'WIN32',
            'UNICODE',
            '_CHAR16T',
          ],

          'sources!': [
            '<@(unittest++_path)/include/Posix/TimeHelpers.cpp',
            '<@(unittest++_path)/include/Posix/SignalTranslator.cpp',
          ],

          'sources!': [
            '<@(unittest++_path)/include/Win32/TimeHelpers.cpp',
          ],

        }],
      ],

    },

    {

      'target_name': 'LostEngine',

      'type': 'static_library',

      'default_configuration': 'Debug',

      'msvs_guid': '8AAE7A8A-8DAC-204C-8BA8-1013866E2AD4',

      'configurations': {

        'Common': {
          'abstract': 1,
          'xcode_settings': {
#            'ARCHS': '$(NATIVE_ARCH_ACTUAL)',
            'ARCHS': '$(ARCHS_STANDARD_32_BIT)',
            'CODE_SIGN_IDENTITY[sdk=iphoneos*]': 'iPhone Developer',
            'ONLY_ACTIVE_ARCH': 'YES',
            'IPHONEOS_DEPLOYMENT_TARGET': '5.0',
            'VALID_ARCHS': 'i386 x86_64 armv6 armv7',
            'SDKROOT': 'iphoneos',
            'TARGETED_DEVICE_FAMILY': '1,2',
            'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',
            'GCC_C_LANGUAGE_STANDARD': 'c99',
            'GCC_WARN_ABOUT_RETURN_TYPE': 'YES',
            'GCC_WARN_UNUSED_VARIABLE': 'YES',
            'GCC_PRECOMPILE_PREFIX_HEADER': 'YES',
            'GCC_PFE_FILE_C_DIALECTS': 'c++ objective-c++',
            'GCC_INCREASE_PRECOMPILED_HEADER_SHARING': 'YES',
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
            'GCC_OBJC_CALL_CXX_CDTORS': 'YES',
            'ALWAYS_SEARCH_USER_PATHS': 'NO',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ObjectFile': '$(IntDir)\\%(RelativeDir)',
             },
          },
        },

        'Debug': {
          'inherit_from': ['Common'],
          'xcode_settings': {
            'GCC_OPTIMIZATION_LEVEL': '0',
            'GCC_PREFIX_HEADER': 'lost/prefix-debug.pch',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'Optimization': '0',
              'PreprocessorDefinitions': ['_DEBUG'],
              'ForcedIncludeFiles': 'lost/prefix-debug.pch',
            },
            'VCLinkerTool': {
              'AdditionalDependencies': ['msvcrtd.lib'],
              'AdditionalOptions': ['/DEBUG', '/NODEFAULTLIB:msvcrt.lib'],
            },
          },
          'defines': [
            '_DEBUG',
          ],
          'conditions': [
            ['OS == "linux"', {
              'cflags_cc': [
                '-includelost/prefix-debug.pch',
              ],
            }],
          ],
        },

        'Release': {
          'inherit_from': ['Common'],
          'xcode_settings': {
            'GCC_OPTIMIZATION_LEVEL': '3',
            'GCC_PREFIX_HEADER': 'lost/prefix-release.pch',
          },
          'msvs_settings': {
            'VCCLCompilerTool': {
              'Optimization': '2',
              'PreprocessorDefinitions': ['NDEBUG'],
              'ForcedIncludeFiles': 'lost/prefix-release.pch',
            },
          },
          'defines': [
            'NDEBUG',
          ],
          'conditions': [
            ['OS == "linux"', {
              'cflags_cc': [
                '-includelost/prefix-release.pch',
              ],
            }],
          ],
        },

      },

      'dependencies': [
        '<@(lua_path)/lua.gyp:lua',
        '<@(slub_path)/slub.gyp:slub',
      ],

      'include_dirs': [

        '.',
        '<@(box2d_path)/Box2D',
        '<@(eastl_path)/include',
        '<@(freetype2_path)/include',
        '<@(giflib_path)/lib',
        '<@(glee_path)/include',
        '<@(md5_path)',
        '<@(stb_image_path)',
        '<@(stb_vorbis_path)',
        '<@(utfcpp_path)/source',
        '<@(tinythread_path)',
        '<@(zlib_path)/include',
        '<@(zziplib_path)',

      ],

      'sources': [

        # al
        'lost/al/al.h',
        'lost/al/Buffer.h',
        'lost/al/Buffer.cpp',
        'lost/al/Context.h',
        'lost/al/Context.cpp',
        'lost/al/Debug.h',
        'lost/al/Debug.cpp',
        'lost/al/Device.h',
        'lost/al/Device.cpp',
        'lost/al/Listener.h',
        'lost/al/Listener.cpp',
        'lost/al/Source.h',
        'lost/al/Source.cpp',

        # application
        'lost/application/Application.h',
        'lost/application/Application.cpp',
        'lost/application/ApplicationEvent.h',
        'lost/application/DebugEvent.h',
        'lost/application/DragNDropEvent.h',
        'lost/application/forward.h',
        'lost/application/KeyCode.h',
        'lost/application/KeyEvent.h',
        'lost/application/MouseButton.h',
        'lost/application/MouseEvent.h',
        'lost/application/ProcessEvent.h',
        'lost/application/Queue.h',
        'lost/application/Queue.cpp',
        'lost/application/QueueEntity.h',
        'lost/application/QueueEntityLua.h',
        'lost/application/QueueEntityLua.cpp',
        'lost/application/ResizeEvent.h',
        'lost/application/Runner.h',
        'lost/application/Runner.cpp',
        'lost/application/SpawnTaskletEvent.h',
        'lost/application/Tasklet.h',
        'lost/application/Tasklet.cpp',
        'lost/application/TaskletConfig.h',
        'lost/application/TaskletConfig.cpp',
        'lost/application/TaskletEvent.h',
        'lost/application/TaskletThread.h',
        'lost/application/TaskletThread.cpp',
        'lost/application/Window.h',
        'lost/application/Window.cpp',
        'lost/application/WindowEvent.h',

        # application/linux
        'lost/application/linux/Application.cpp',
        'lost/application/linux/Tasklet.cpp',
        'lost/application/linux/Window.cpp',
        'lost/application/linux/WindowHandler.cpp',
        'lost/application/linux/WindowHandler.h',

        # application/mac
#        'lost/application/mac/Application.mm',
#        'lost/application/mac/ApplicationDelegate.h',
#        'lost/application/mac/ApplicationDelegate.mm',
#        'lost/application/mac/ApplicationWindow.h',
#        'lost/application/mac/ApplicationWindow.mm',
#        'lost/application/mac/GLView.h',
#        'lost/application/mac/GLView.mm',
#        'lost/application/mac/Tasklet.mm',
#        'lost/application/mac/Window.mm',
        'lost/application/iphone/Application.mm',
        'lost/application/iphone/LEApplication.mm',
        'lost/application/iphone/LEApplicationDelegate.mm',
        'lost/application/iphone/LEGLView.mm',
        'lost/application/iphone/LEWindow.mm',
        'lost/application/iphone/Tasklet.mm',
        'lost/application/iphone/Window.mm',

        # application/windows
        'lost/application/windows/Application.cpp',
        'lost/application/windows/Tasklet.cpp',
        'lost/application/windows/Window.cpp',

        # audio
        'lost/audio/Engine.h',
        'lost/audio/Engine.cpp',
        'lost/audio/Source.h',
        'lost/audio/Source.cpp',
        'lost/audio/VorbisFile.h',
        'lost/audio/VorbisFile.cpp',

        # bitmap
        'lost/bitmap/Bitmap.h',
        'lost/bitmap/Bitmap.cpp',
        'lost/bitmap/forward.h',
        'lost/bitmap/Components.h',
        'lost/bitmap/Packer.h',
        'lost/bitmap/Packer.cpp',
        'lost/bitmap/Gif.h',
        'lost/bitmap/Gif.cpp',
        'lost/bitmap/GifDecoder.h',
        'lost/bitmap/GifDecoder.cpp',

        # box2d
        'lost/box2d/DebugDraw.h',
        'lost/box2d/DebugDraw.cpp',

        # camera
        'lost/camera/Camera.h',
        'lost/camera/Camera.cpp',
        'lost/camera/Camera2D.h',
        'lost/camera/Camera2D.cpp',
        'lost/camera/Camera3D.h',
        'lost/camera/Camera3D.cpp',
        'lost/camera/forward.h',

        # common
        'lost/common/ArrayDeleter.h',
        'lost/common/Color.h',
        'lost/common/Color.cpp',
        'lost/common/ColorGradient.h',
        'lost/common/ColorGradient.cpp',
        'lost/common/ColorPoint.h',
        'lost/common/Data.h',
        'lost/common/Data.cpp',
        'lost/common/Disallow.h',
        'lost/common/eastlAllocators.cpp',
        'lost/common/eastlStreamSupport.h',
        'lost/common/eastlStreamSupport.cpp',
        'lost/common/forward.h',
        'lost/common/Function.h',
        'lost/common/Hash.h',
        'lost/common/Hash.cpp',
        'lost/common/io.h',
        'lost/common/io.cpp',
        'lost/common/Logger.h',
        'lost/common/Logger.cpp',
        'lost/common/NullDeleter.h',
        'lost/common/Profiler.h',
        'lost/common/Profiler.cpp',
        'lost/common/SpiritHelpers.h',
        'lost/common/StringStream.h',
        'lost/common/StringStream.cpp',

        # event
        'lost/event/Event.h',
        'lost/event/EventDispatcher.cpp',
        'lost/event/EventDispatcher.h',
        'lost/event/Connection.cpp',
        'lost/event/Connection.h',
        'lost/event/forward.h',
        'lost/event/Listener.h',
        'lost/event/Signal.cpp',
        'lost/event/Signal.h',
        'lost/event/Pool.cpp',
        'lost/event/Pool.h',

        # font
        'lost/font/BitmapFont.h',
        'lost/font/BitmapFont.cpp',
        'lost/font/BreakMode.h',
        'lost/font/Font.h',
        'lost/font/Font.cpp',
        'lost/font/FontManager.h',
        'lost/font/FontManager.cpp',
        'lost/font/forward.h',
        'lost/font/Glyph.h',
        'lost/font/Glyph.cpp',
        'lost/font/Range.h',
        'lost/font/Render.h',
        'lost/font/Render.cpp',
        'lost/font/RenderedText.h',
        'lost/font/RenderedText.cpp',
        'lost/font/TextBuffer.h',
        'lost/font/TextBuffer.cpp',
        'lost/font/TrueTypeFont.h',
        'lost/font/TrueTypeFont.cpp',
        'lost/font/freetype/Face.h',
        'lost/font/freetype/Face.cpp',
        'lost/font/freetype/forward.h',
        'lost/font/freetype/Library.h',
        'lost/font/freetype/Library.cpp',

        # fs
        'lost/fs/io.h',
        'lost/fs/io.cpp',
        'lost/fs/Path.h',
        'lost/fs/Path.cpp',

        # gl
        'lost/gl/Buffer.h',
        'lost/gl/Buffer.cpp',
        'lost/gl/BufferLayout.h',
        'lost/gl/BufferLayout.cpp',
        'lost/gl/Context.h',
        'lost/gl/Context.cpp',
        'lost/gl/forward.h',
        'lost/gl/FragmentShader.h',
        'lost/gl/FragmentShader.cpp',
        'lost/gl/FrameBuffer.h',
        'lost/gl/FrameBuffer.cpp',
        'lost/gl/gl.h',
        'lost/gl/gltypes.h',
        'lost/gl/HostBuffer.h',
        'lost/gl/HostBuffer.cpp',
        'lost/gl/HybridBuffer.h',
        'lost/gl/HybridBuffer.cpp',
        'lost/gl/HybridIndexBuffer.h',
        'lost/gl/HybridIndexBuffer.cpp',
        'lost/gl/HybridVertexBuffer.h',
        'lost/gl/HybridVertexBuffer.cpp',
        'lost/gl/RenderBuffer.h',
        'lost/gl/RenderBuffer.cpp',
        'lost/gl/Shader.h',
        'lost/gl/Shader.cpp',
        'lost/gl/ShaderHelper.h',
        'lost/gl/ShaderHelper.cpp',
        'lost/gl/ShaderProgram.h',
        'lost/gl/ShaderProgram.cpp',
        'lost/gl/Texture.h',
        'lost/gl/Texture.cpp',
        'lost/gl/TextureAtlas.h',
        'lost/gl/TextureTile.h',
        'lost/gl/Uniform.h',
        'lost/gl/Uniform.cpp',
        'lost/gl/UniformBlock.h',
        'lost/gl/UniformBlock.cpp',
        'lost/gl/Utils.h',
        'lost/gl/Utils.cpp',
        'lost/gl/Variant.h',
        'lost/gl/VariantType.h',
        'lost/gl/VertexAttribute.h',
        'lost/gl/VertexAttribute.cpp',
        'lost/gl/VertexShader.h',
        'lost/gl/VertexShader.cpp',

        # gl/linux
        'lost/gl/linux/Context.cpp',

        # gl/mac
#        'lost/gl/mac/Context.mm',
        'lost/gl/iphone/Context.mm',
        'lost/gl/iphone/GLContext.mm',

        # gl/windows
        'lost/gl/windows/Context.cpp',

        # lgl
        'lost/lgl/lgl_gl.h',
        'lost/lgl/lgl_gles.h',
        'lost/lgl/lgl.h',
        'lost/lgl/lglu.h',

        # lua
        'lost/lua/BindAll.h',
        'lost/lua/BindAll.cpp',
        'lost/lua/ModuleLoader.h',
        'lost/lua/ModuleLoader.cpp',
        'lost/lua/State.h',
        'lost/lua/State.cpp',
        'lost/lua/forward.h',
        'lost/lua/EventCast.h',

        # lua/bindings
        'lost/lua/bindings/LostApplication.h',
        'lost/lua/bindings/LostApplication.cpp',
        'lost/lua/bindings/LostAudio.h',
        'lost/lua/bindings/LostAudio.cpp',
        'lost/lua/bindings/LostBitmap.h',
        'lost/lua/bindings/LostBitmap.cpp',
        'lost/lua/bindings/LostBox2D.h',
        'lost/lua/bindings/LostBox2D.cpp',
        'lost/lua/bindings/LostCamera.h',
        'lost/lua/bindings/LostCamera.cpp',
        'lost/lua/bindings/LostCommon.h',
        'lost/lua/bindings/LostCommon.cpp',
        'lost/lua/bindings/LostEvent.h',
        'lost/lua/bindings/LostEvent.cpp',
        'lost/lua/bindings/LostFont.h',
        'lost/lua/bindings/LostFont.cpp',
        'lost/lua/bindings/LostFs.h',
        'lost/lua/bindings/LostFs.cpp',
        'lost/lua/bindings/LostGL.h',
        'lost/lua/bindings/LostGL.cpp',
        'lost/lua/bindings/LostLua.h',
        'lost/lua/bindings/LostLua.cpp',
        'lost/lua/bindings/LostMath.h',
        'lost/lua/bindings/LostMath.cpp',
        'lost/lua/bindings/LostMesh.h',
        'lost/lua/bindings/LostMesh.cpp',
        'lost/lua/bindings/LostPlatform.h',
        'lost/lua/bindings/LostPlatform.cpp',
        'lost/lua/bindings/LostProfiler.h',
        'lost/lua/bindings/LostProfiler.cpp',
        'lost/lua/bindings/LostResource.h',
        'lost/lua/bindings/LostResource.cpp',
        'lost/lua/bindings/LostRg.h',
        'lost/lua/bindings/LostRg.cpp',
        'lost/lua/bindings/LostTime.h',
        'lost/lua/bindings/LostTime.cpp',
        'lost/lua/bindings/ThirdpartyBox2D.h',
        'lost/lua/bindings/ThirdpartyBox2D.cpp',

        # math
        'lost/math/AABB.cpp',
        'lost/math/AABB.h',
        'lost/math/Bezier.h',
        'lost/math/forward.h',
        'lost/math/Generator.h',
        'lost/math/io.cpp',
        'lost/math/io.h',
        'lost/math/Matrix.cpp',
        'lost/math/Matrix.h',
        'lost/math/Quat.cpp',
        'lost/math/Quat.h',
        'lost/math/Rect.cpp',
        'lost/math/Rect.h',
        'lost/math/RectPacker.cpp',
        'lost/math/RectPacker.h',
        'lost/math/Vec2.cpp',
        'lost/math/Vec2.h',
        'lost/math/Vec3.cpp',
        'lost/math/Vec3.h',
        'lost/math/Vec4.cpp',
        'lost/math/Vec4.h',

        # mesh
        'lost/mesh/AABB.h',
        'lost/mesh/Axes.h',
        'lost/mesh/Circular.h',
        'lost/mesh/Disc.h',
        'lost/mesh/Disc.cpp',
        'lost/mesh/FlipBook.h',
        'lost/mesh/forward.h',
        'lost/mesh/Line.cpp',
        'lost/mesh/Line.h',
        'lost/mesh/Loader.cpp',
        'lost/mesh/Loader.h',
        'lost/mesh/Material.cpp',
        'lost/mesh/Material.h',
        'lost/mesh/Mesh.cpp',
        'lost/mesh/Mesh.h',
        'lost/mesh/Quad.cpp',
        'lost/mesh/Quad.h',
        'lost/mesh/Rect.cpp',
        'lost/mesh/Rect.h',
        'lost/mesh/RoundedRect.cpp',
        'lost/mesh/RoundedRect.h',
        'lost/mesh/ScaleGrid.cpp',
        'lost/mesh/ScaleGrid.h',
        'lost/mesh/Sphere.cpp',
        'lost/mesh/Sphere.h',
        'lost/mesh/TextureManager.cpp',
        'lost/mesh/TextureManager.h',

        # platform
        'lost/platform/Paths.h',
        'lost/platform/Platform.cpp',
        'lost/platform/Platform.h',
#        'lost/platform/Platform_Mac.mm',
        'lost/platform/Platform_ios.mm',
        'lost/platform/Platform_Windows.cpp',
        'lost/platform/Platform_Linux.cpp',
        'lost/platform/shared_ptr.h',
        'lost/platform/Thread.h',
        'lost/platform/Time.h',
        'lost/platform/Type.h',

        #profiler
        'lost/profiler/Blackbox.cpp',
        'lost/profiler/Blackbox.h',

        # resource
        'lost/resource/ApplicationResourceRepository.cpp',
        'lost/resource/ApplicationResourceRepository.h',
        'lost/resource/Bundle.cpp',
        'lost/resource/Bundle.h',
        'lost/resource/DefaultLoader.cpp',
        'lost/resource/DefaultLoader.h',
        'lost/resource/FilesystemRepository.cpp',
        'lost/resource/FilesystemRepository.h',
        'lost/resource/forward.h',
        'lost/resource/Helper.cpp',
        'lost/resource/Helper.h',
        'lost/resource/Loader.cpp',
        'lost/resource/Loader.h',
        'lost/resource/Repository.h',

        # rg
        'lost/rg/Blend.cpp',
        'lost/rg/Blend.h',
        'lost/rg/Camera.cpp',
        'lost/rg/Camera.h',
        'lost/rg/Clear.cpp',
        'lost/rg/Clear.h',
        'lost/rg/ClearColor.cpp',
        'lost/rg/ClearColor.h',
        'lost/rg/DefaultFrameBuffer.cpp',
        'lost/rg/DefaultFrameBuffer.h',
        'lost/rg/DepthTest.cpp',
        'lost/rg/DepthTest.h',
        'lost/rg/Draw.cpp',
        'lost/rg/Draw.h',
        'lost/rg/forward.h',
        'lost/rg/FrameBuffer.cpp',
        'lost/rg/FrameBuffer.h',
        'lost/rg/Node.cpp',
        'lost/rg/Node.h',
        'lost/rg/Scissor.cpp',
        'lost/rg/Scissor.h',
        'lost/rg/ScissorRect.cpp',
        'lost/rg/ScissorRect.h',
        'lost/rg/ScissorStack.cpp',
        'lost/rg/ScissorStack.h',

        # text
        'lost/text/types.h',
        
        # time
        'lost/time/Clock.cpp',
        'lost/time/Clock.h',
        'lost/time/forward.h',
        'lost/time/ThreadedTimerScheduler.cpp',
        'lost/time/ThreadedTimerScheduler.h',
        'lost/time/Timer.cpp',
        'lost/time/Timer.h',
        'lost/time/TimerScheduler.h',

        # thirdparty/box2d
        '<@(box2d_path)/Box2D/Box2D/Collision/b2BroadPhase.cpp',
        '<@(box2d_path)/Box2D/Box2D/Collision/b2CollideCircle.cpp',
        '<@(box2d_path)/Box2D/Box2D/Collision/b2CollidePolygon.cpp',
        '<@(box2d_path)/Box2D/Box2D/Collision/b2Collision.cpp',
        '<@(box2d_path)/Box2D/Box2D/Collision/b2Distance.cpp',
        '<@(box2d_path)/Box2D/Box2D/Collision/b2DynamicTree.cpp',
        '<@(box2d_path)/Box2D/Box2D/Collision/b2TimeOfImpact.cpp',
        '<@(box2d_path)/Box2D/Box2D/Collision/Shapes/b2CircleShape.cpp',
        '<@(box2d_path)/Box2D/Box2D/Collision/Shapes/b2PolygonShape.cpp',
        '<@(box2d_path)/Box2D/Box2D/Common/b2BlockAllocator.cpp',
        '<@(box2d_path)/Box2D/Box2D/Common/b2Math.cpp',
        '<@(box2d_path)/Box2D/Box2D/Common/b2Settings.cpp',
        '<@(box2d_path)/Box2D/Box2D/Common/b2StackAllocator.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/b2Body.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/b2ContactManager.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/b2Fixture.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/b2Island.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/b2World.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/b2WorldCallbacks.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Contacts/b2Contact.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Contacts/b2TOISolver.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2GearJoint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2Joint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2LineJoint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp',
        '<@(box2d_path)/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.cpp',

        # thirdparty/eastl
        '<@(eastl_path)/src/allocator.cpp',        
        '<@(eastl_path)/src/assert.cpp',        
        '<@(eastl_path)/src/fixed_pool.cpp',        
        '<@(eastl_path)/src/hashtable.cpp',        
        '<@(eastl_path)/src/red_black_tree.cpp',        
        '<@(eastl_path)/src/string.cpp',        

        # thirdparty/freetype2
        '<@(freetype2_path)/src/psnames/psnames.c',
        '<@(freetype2_path)/src/psaux/psaux.c',
        '<@(freetype2_path)/src/lzw/ftlzw.c',
        '<@(freetype2_path)/src/gzip/ftgzip.c',
        '<@(freetype2_path)/src/cache/ftcache.c',
        '<@(freetype2_path)/src/smooth/smooth.c',
        '<@(freetype2_path)/src/raster/raster.c',
        '<@(freetype2_path)/src/pshinter/pshinter.c',
        '<@(freetype2_path)/src/autofit/autofit.c',
        '<@(freetype2_path)/src/sfnt/sfnt.c',
        '<@(freetype2_path)/src/bdf/bdf.c',
        '<@(freetype2_path)/src/pcf/pcf.c',
        '<@(freetype2_path)/src/winfonts/winfnt.c',
        '<@(freetype2_path)/src/type42/type42.c',
        '<@(freetype2_path)/src/pfr/pfr.c',
        '<@(freetype2_path)/src/cid/type1cid.c',
        '<@(freetype2_path)/src/cff/cff.c',
        '<@(freetype2_path)/src/type1/type1.c',
        '<@(freetype2_path)/src/truetype/truetype.c',
        '<@(freetype2_path)/src/base/ftinit.c',
        '<@(freetype2_path)/src/base/ftsystem.c',
        '<@(freetype2_path)/src/base/ftdebug.c',
        '<@(freetype2_path)/src/base/ftbase.c',
        '<@(freetype2_path)/src/base/ftbdf.c',
        '<@(freetype2_path)/src/base/ftcid.c',
        '<@(freetype2_path)/src/base/ftgxval.c',
        '<@(freetype2_path)/src/base/ftglyph.c',
        '<@(freetype2_path)/src/base/ftsynth.c',
        '<@(freetype2_path)/src/base/ftstroke.c',
        '<@(freetype2_path)/src/base/ftlcdfil.c',
        '<@(freetype2_path)/src/base/ftgasp.c',
        '<@(freetype2_path)/src/base/ftpfr.c',
        '<@(freetype2_path)/src/base/ftpatent.c',
        '<@(freetype2_path)/src/base/ftotval.c',
        '<@(freetype2_path)/src/base/ftmm.c',
        '<@(freetype2_path)/src/base/ftbbox.c',
        '<@(freetype2_path)/src/base/ftwinfnt.c',
        '<@(freetype2_path)/src/base/fttype1.c',
        '<@(freetype2_path)/src/base/ftbitmap.c',
        '<@(freetype2_path)/src/base/ftxf86.c',

        # thirdparty/giflib
        '<@(giflib_path)/lib/dgif_lib.c',
        '<@(giflib_path)/lib/gif_hash.c',
        '<@(giflib_path)/lib/gifalloc.c',
        '<@(giflib_path)/lib/gif_err.c',

        # thirdparty/glee
#        '<@(glee_path)/source/GLee.c',

        # thirdparty/md5
        '<@(md5_path)/md5.cpp',

        # thirdparty/stb
        '<@(stb_image_path)/stb_image.c',
        '<@(stb_vorbis_path)/stb_vorbis.c',
        
        # thirdparty/tinythread
        '<@(tinythread_path)/fast_mutex.h',
        '<@(tinythread_path)/once.h',
        '<@(tinythread_path)/once.cpp',
        '<@(tinythread_path)/thread_specific_ptr.h',
        '<@(tinythread_path)/tinythread.h',
        '<@(tinythread_path)/tinythread.cpp',

        # thirdparty/utfcpp
        '<@(utfcpp_path)/source/utf8/checked.h',
        '<@(utfcpp_path)/source/utf8/core.h',
        '<@(utfcpp_path)/source/utf8/unchecked.h',
        '<@(utfcpp_path)/source/utf8.h',

        # thirdparty/zlib
        '<@(zlib_path)/source/zutil.c',
        '<@(zlib_path)/source/uncompr.c',
        '<@(zlib_path)/source/compress.c',
        '<@(zlib_path)/source/infback.c',
        '<@(zlib_path)/source/inflate.c',
        '<@(zlib_path)/source/adler32.c',
        '<@(zlib_path)/source/deflate.c',
        '<@(zlib_path)/source/inffast.c',
        '<@(zlib_path)/source/trees.c',
        '<@(zlib_path)/source/gzio.c',
        '<@(zlib_path)/source/crc32.c',
        '<@(zlib_path)/source/inftrees.c',

        # thirdparty/zziplib
        '<@(zziplib_path)/zzip/dir.c',
        '<@(zziplib_path)/zzip/err.c',
        '<@(zziplib_path)/zzip/fetch.c',
        '<@(zziplib_path)/zzip/file.c',
        '<@(zziplib_path)/zzip/fseeko.c',
        '<@(zziplib_path)/zzip/info.c',
        '<@(zziplib_path)/zzip/memdisk.c',
        '<@(zziplib_path)/zzip/mmapped.c',
        '<@(zziplib_path)/zzip/plugin.c',
        '<@(zziplib_path)/zzip/stat.c',
        '<@(zziplib_path)/zzip/write.c',
        '<@(zziplib_path)/zzip/zip.c',

      ],

      'conditions': [

        ['OS != "mac"', {
          'sources!': [
            'lost/application/mac/Application.mm',
            'lost/application/mac/ApplicationDelegate.mm',
            'lost/application/mac/ApplicationWindow.mm',
            'lost/application/mac/GLView.mm',
            'lost/application/mac/Tasklet.mm',
            'lost/application/mac/Window.mm',
            'lost/gl/mac/Context.mm',
            'lost/platform/Platform_Mac.mm',
          ],
        }],

        ['OS != "win"', {
          'sources!': [
            'lost/application/windows/Application.cpp',
            'lost/application/windows/Tasklet.cpp',
            'lost/application/windows/Window.cpp',
            'lost/gl/windows/Context.cpp',
            'lost/platform/Platform_Windows.cpp',
          ],
        }],
        ['OS == "win"', {
          'link_settings': {
            'libraries': [
              '-l<@(openal_path)/libs/Win32/EFX-Util.lib',
              '-l<@(openal_path)/libs/Win32/OpenAL32.lib',
              '-lopengl32.lib',
              '-lglu32.lib',
              '-lshell32.lib',
              '-lShLwApi.lib',
            ],
          },
          'include_dirs': [
            '<@(openal_path)/include',
            '<@(opengl_path)/include',
            '<@(msinttypes_path)',
          ],
          'defines': [
            'WIN32',
            'UNICODE',
            '_LIB',
            '_CHAR16T',
          ],
        }],

        ['OS != "linux"', {
          'sources!': [
            'lost/application/linux/Application.cpp',
            'lost/application/linux/Tasklet.cpp',
            'lost/application/linux/Window.cpp',
            'lost/application/linux/WindowHandler.cpp',
            'lost/gl/linux/Context.cpp',
            'lost/platform/Platform_Linux.cpp',
          ],
        }],
        ['OS == "linux"', {
          'link_settings': {
            'libraries': [
              # workaround: force correct link order
              'LostEngine',
              'GL',
              'GLU',
              'openal',
              'pthread',
              'X11',
              'rt',
            ],
          },
        }],

      ],
    },

  ],

}
