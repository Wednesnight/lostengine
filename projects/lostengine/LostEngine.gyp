{

  'variables': {
    'boost_path': '../../thirdparty/boost/boost_1_44_0',
    'box2d_path': '../../thirdparty/box2d/box2d_2_0_1',
    'fhtagn_path': '../../thirdparty/fhtagn/fhtagn-0.3',
    'freetype2_path': '../../thirdparty/freetype2/freetype_2_4_3',
    'glee_path': '../../thirdparty/glee/glee_5_21',
    'hashlib++_path': '../../thirdparty/hashlib++/hashlib++_0_3_1',
    'lua_path': '../../thirdparty/lua/lua_5_1_4',
    'luabind_path': '../../thirdparty/luabind/luabind_0_9_1',
    'openal_path': '../../thirdparty/OpenAL/OpenAL_1_1',
    'opengl_path': '../../thirdparty/OpenGL',
    'stb_image_path': '../../thirdparty/stb/stb_image_1_18',
    'stb_vorbis_path': '../../thirdparty/stb/stb_vorbis_0_99996',
    'unittest++_path': '../../thirdparty/UnitTest++',
    'zlib_path': '../../thirdparty/zlib/zlib_1_2_3',
    'zziplib_path': '../../thirdparty/zziplib/zziplib-0.13.58',
  },

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
            'ARCHS': '$(NATIVE_ARCH_ACTUAL)',
            'ONLY_ACTIVE_ARCH': 'YES',
            'VALID_ARCHS': 'i386 x86_64 armv6',
            'GCC_VERSION': '4.2',
            'GCC_C_LANGUAGE_STANDARD': 'c99',
            'GCC_WARN_ABOUT_RETURN_TYPE': 'YES',
            'GCC_WARN_UNUSED_VARIABLE': 'YES',
            'GCC_PRECOMPILE_PREFIX_HEADER': 'YES',
            'GCC_PFE_FILE_C_DIALECTS': 'c++ objective-c++',
            'GCC_INCREASE_PRECOMPILED_HEADER_SHARING': 'YES',
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
            'GCC_OBJC_CALL_CXX_CDTORS': 'YES',
            'ALWAYS_SEARCH_USER_PATHS': 'NO',
            'INFOPLIST_FILE': 'xcode/LostEngineLauncher-Info.plist',
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
            },
          },
          'msvs_precompiled_header': 'lost/prefix-debug.pch',
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
            },
          },
          'msvs_precompiled_header': 'lost/prefix-release.pch',
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
        '<@(boost_path)',
      ],

      'sources': [
        'main.cpp',
      ],

      'conditions': [

        ['OS == "mac"', {

          'libraries': [
            '/System/Library/Frameworks/Cocoa.framework',
            '/System/Library/Frameworks/OpenAL.framework',
            '/System/Library/Frameworks/OpenGL.framework',
          ],

          'mac_bundle': 1,

          'mac_bundle_resources': [
            'LostEngine.icns',
          ],

          'postbuilds': [{
            'variables': {
              # Define copy_resources_path in a variable ending in _path
              # so that gyp understands it's a path and performs proper
              # relativization during dict merging.
              'copy_resources_path':
              '<(DEPTH)/xcode/copy_resources.sh',
            },
            'postbuild_name': 'Copy Resources',
            'action': ['<(copy_resources_path)'],
          }],

        }],

        ['OS == "win"', {

          'defines': [
            'WIN32',
            'UNICODE',
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
            'VALID_ARCHS': 'i386 x86_64 armv6',
            'GCC_VERSION': '4.2',
            'GCC_C_LANGUAGE_STANDARD': 'c99',
            'GCC_WARN_ABOUT_RETURN_TYPE': 'YES',
            'GCC_WARN_UNUSED_VARIABLE': 'YES',
            'GCC_PRECOMPILE_PREFIX_HEADER': 'YES',
            'GCC_PFE_FILE_C_DIALECTS': 'c++ objective-c++',
            'GCC_INCREASE_PRECOMPILED_HEADER_SHARING': 'YES',
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
            'GCC_OBJC_CALL_CXX_CDTORS': 'YES',
            'ALWAYS_SEARCH_USER_PATHS': 'NO',
            'INFOPLIST_FILE': 'xcode/LostEngineUnittests-Info.plist',
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
            },
          },
          'msvs_precompiled_header': 'lost/prefix-debug.pch',
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
            },
          },
          'msvs_precompiled_header': 'lost/prefix-release.pch',
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
        '<@(boost_path)',
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
        'lost/test/TestLostGlRangeManager.cpp',
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

          'defines': [
            'WIN32',
            'UNICODE',
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
            'ARCHS': '$(NATIVE_ARCH_ACTUAL)',
            'ONLY_ACTIVE_ARCH': 'YES',
            'VALID_ARCHS': 'i386 x86_64 armv6',
            'GCC_VERSION': '4.2',
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
          'msvs_precompiled_header': 'lost/prefix-debug.pch',
          'msvs_settings': {
            'VCCLCompilerTool': {
              'Optimization': '0',
              'PreprocessorDefinitions': ['_DEBUG'],
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
          'msvs_precompiled_header': 'lost/prefix-release.pch',
          'msvs_settings': {
            'VCCLCompilerTool': {
              'Optimization': '2',
              'PreprocessorDefinitions': ['NDEBUG'],
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

      'include_dirs': [

        '.',
        '<@(boost_path)',
        '<@(box2d_path)/source/Include',
        '<@(fhtagn_path)',
        '<@(freetype2_path)/include',
        '<@(glee_path)/include',
        '<@(hashlib++_path)',
        '<@(lua_path)/include',
        '<@(luabind_path)',
        '<@(stb_image_path)',
        '<@(stb_vorbis_path)',
        '<@(zlib_path)/include',
        '<@(zziplib_path)',

      ],

      'sources': [

        # al
        'lost/al/Buffer.cpp',
        'lost/al/Context.cpp',
        'lost/al/Debug.cpp',
        'lost/al/Device.cpp',
        'lost/al/Listener.cpp',
        'lost/al/Source.cpp',

        # application
        'lost/application/Application.cpp',
        'lost/application/Queue.cpp',
        'lost/application/QueueEntityLua.cpp',
        'lost/application/Runner.cpp',
        'lost/application/Tasklet.cpp',
        'lost/application/TaskletConfig.cpp',
        'lost/application/TaskletThread.cpp',
        'lost/application/Window.cpp',

        # application/linux
        'lost/application/linux/Application.cpp',
        'lost/application/linux/Tasklet.cpp',
        'lost/application/linux/Window.cpp',
	      'lost/application/linux/WindowHandler.cpp',

        # application/mac
        'lost/application/mac/Application.mm',
        'lost/application/mac/ApplicationDelegate.mm',
        'lost/application/mac/ApplicationTaskletSpawn.cpp',
        'lost/application/mac/ApplicationWindow.mm',
        'lost/application/mac/GLView.mm',
        'lost/application/mac/Tasklet.mm',
        'lost/application/mac/Window.mm',

        # application/windows
        'lost/application/windows/Application.cpp',
        'lost/application/windows/Tasklet.cpp',
        'lost/application/windows/Window.cpp',

        # audio
        'lost/audio/Engine.cpp',
        'lost/audio/Source.cpp',
        'lost/audio/VorbisFile.cpp',

        # bitmap
        'lost/bitmap/Bitmap.cpp',
        'lost/bitmap/Packer.cpp',

        # camera
        'lost/camera/Camera.cpp',
        'lost/camera/Camera2D.cpp',
        'lost/camera/Camera3D.cpp',

        # common
        'lost/common/Color.cpp',
        'lost/common/ColorGradient.cpp',
        'lost/common/Data.cpp',
        'lost/common/io.cpp',
        'lost/common/Logger.cpp',
        'lost/common/Profiler.cpp',

        # event
        'lost/event/EventDispatcher.cpp',

        # font
        'lost/font/BitmapFont.cpp',
        'lost/font/Font.cpp',
        'lost/font/FontManager.cpp',
        'lost/font/Glyph.cpp',
        'lost/font/Render.cpp',
        'lost/font/RenderedText.cpp',
        'lost/font/TextBuffer.cpp',
        'lost/font/TrueTypeFont.cpp',
        'lost/font/freetype/Face.cpp',
        'lost/font/freetype/Library.cpp',

        # gl
        'lost/gl/Buffer.cpp',
        'lost/gl/BufferLayout.cpp',
        'lost/gl/Context.cpp',
        'lost/gl/FragmentShader.cpp',
        'lost/gl/FrameBuffer.cpp',
        'lost/gl/HostBuffer.cpp',
        'lost/gl/HybridBuffer.cpp',
        'lost/gl/HybridIndexBuffer.cpp',
        'lost/gl/HybridVertexBuffer.cpp',
        'lost/gl/RangeManager.cpp',
        'lost/gl/RenderBuffer.cpp',
        'lost/gl/Shader.cpp',
        'lost/gl/ShaderHelper.cpp',
        'lost/gl/ShaderProgram.cpp',
        'lost/gl/Texture.cpp',
        'lost/gl/Uniform.cpp',
        'lost/gl/UniformBlock.cpp',
        'lost/gl/Utils.cpp',
        'lost/gl/VertexAttribute.cpp',
        'lost/gl/VertexShader.cpp',

        # gl/linux
        'lost/gl/linux/Context.cpp',

        # gl/mac
        'lost/gl/mac/Context.mm',

        # gl/windows
        'lost/gl/windows/Context.cpp',

        # lua
        'lost/lua/BindAll.cpp',
        'lost/lua/BindingHelpers.cpp',
        'lost/lua/ModuleLoader.cpp',
        'lost/lua/State.cpp',

        # lua/bindings
        'lost/lua/bindings/LostApplication.cpp',
        'lost/lua/bindings/LostAudio.cpp',
        'lost/lua/bindings/LostBitmap.cpp',
        'lost/lua/bindings/LostCamera.cpp',
        'lost/lua/bindings/LostCommon.cpp',
        'lost/lua/bindings/LostEvent.cpp',
        'lost/lua/bindings/LostFont.cpp',
        'lost/lua/bindings/LostGL.cpp',
        'lost/lua/bindings/LostLGL.cpp',
        'lost/lua/bindings/LostLua.cpp',
        'lost/lua/bindings/LostMath.cpp',
        'lost/lua/bindings/LostMesh.cpp',
        'lost/lua/bindings/LostPlatform.cpp',
        'lost/lua/bindings/LostResource.cpp',
        'lost/lua/bindings/LostRg.cpp',
        'lost/lua/bindings/LostTime.cpp',
        'lost/lua/bindings/ThirdpartyBoost.cpp',
        'lost/lua/bindings/ThirdpartyBox2D.cpp',

        # math
        'lost/math/AABB.cpp',
        'lost/math/io.cpp',
        'lost/math/Matrix.cpp',
        'lost/math/Quat.cpp',
        'lost/math/Rect.cpp',
        'lost/math/RectPacker.cpp',
        'lost/math/Vec2.cpp',
        'lost/math/Vec3.cpp',
        'lost/math/Vec4.cpp',

        # mesh
        'lost/mesh/Disc.cpp',
        'lost/mesh/Line.cpp',
        'lost/mesh/Loader.cpp',
        'lost/mesh/Material.cpp',
        'lost/mesh/Mesh.cpp',
        'lost/mesh/Quad.cpp',
        'lost/mesh/Rect.cpp',
        'lost/mesh/RoundedRect.cpp',
        'lost/mesh/ScaleGrid.cpp',
        'lost/mesh/Sphere.cpp',
        'lost/mesh/TextureManager.cpp',

        # platform
        'lost/platform/Platform.cpp',
        'lost/platform/Platform_Mac.cpp',
        'lost/platform/Platform_Windows.cpp',
        'lost/platform/Platform_Linux.cpp',

        # resource
        'lost/resource/ApplicationResourceRepository.cpp',
        'lost/resource/DefaultLoader.cpp',
        'lost/resource/DefaultWriter.cpp',
        'lost/resource/FilesystemRepository.cpp',
        'lost/resource/Helper.cpp',
        'lost/resource/Loader.cpp',
        'lost/resource/Writer.cpp',

        # rg
        'lost/rg/Blend.cpp',
        'lost/rg/Camera.cpp',
        'lost/rg/Clear.cpp',
        'lost/rg/ClearColor.cpp',
        'lost/rg/DefaultFrameBuffer.cpp',
        'lost/rg/DepthTest.cpp',
        'lost/rg/Draw.cpp',
        'lost/rg/FrameBuffer.cpp',
        'lost/rg/Node.cpp',
        'lost/rg/Scissor.cpp',
        'lost/rg/ScissorRect.cpp',
        'lost/rg/ScissorStack.cpp',

        # time
        'lost/time/Clock.cpp',
        'lost/time/ThreadedTimerScheduler.cpp',
        'lost/time/Timer.cpp',

        # thirdparty/boost
        '<@(boost_path)/libs/filesystem/v3/src/path.cpp',
        '<@(boost_path)/libs/filesystem/v3/src/operations.cpp',
        '<@(boost_path)/libs/filesystem/v3/src/portability.cpp',
        '<@(boost_path)/libs/filesystem/v3/src/utf8_codecvt_facet.cpp',
        '<@(boost_path)/libs/signals/src/connection.cpp',
        '<@(boost_path)/libs/signals/src/named_slot_map.cpp',
        '<@(boost_path)/libs/signals/src/signal_base.cpp',
        '<@(boost_path)/libs/signals/src/slot.cpp',
        '<@(boost_path)/libs/signals/src/trackable.cpp',
        '<@(boost_path)/libs/system/src/error_code.cpp',
        '<@(boost_path)/libs/thread/src/tss_null.cpp',
        '<@(boost_path)/libs/thread/src/pthread/once.cpp',
        '<@(boost_path)/libs/thread/src/pthread/thread.cpp',
        '<@(boost_path)/libs/thread/src/win32/thread.cpp',
        '<@(boost_path)/libs/thread/src/win32/tss_dll.cpp',
        '<@(boost_path)/libs/thread/src/win32/tss_pe.cpp',

        # thirdparty/box2d
        '<@(box2d_path)/source/Source/Collision/b2BroadPhase.cpp',
        '<@(box2d_path)/source/Source/Collision/b2CollideCircle.cpp',
        '<@(box2d_path)/source/Source/Collision/b2CollidePoly.cpp',
        '<@(box2d_path)/source/Source/Collision/b2Collision.cpp',
        '<@(box2d_path)/source/Source/Collision/b2Distance.cpp',
        '<@(box2d_path)/source/Source/Collision/b2PairManager.cpp',
        '<@(box2d_path)/source/Source/Collision/b2TimeOfImpact.cpp',
        '<@(box2d_path)/source/Source/Collision/Shapes/b2CircleShape.cpp',
        '<@(box2d_path)/source/Source/Collision/Shapes/b2PolygonShape.cpp',
        '<@(box2d_path)/source/Source/Collision/Shapes/b2Shape.cpp',
        '<@(box2d_path)/source/Source/Common/b2BlockAllocator.cpp',
        '<@(box2d_path)/source/Source/Common/b2Math.cpp',
        '<@(box2d_path)/source/Source/Common/b2Settings.cpp',
        '<@(box2d_path)/source/Source/Common/b2StackAllocator.cpp',
        '<@(box2d_path)/source/Source/Dynamics/b2Body.cpp',
        '<@(box2d_path)/source/Source/Dynamics/b2ContactManager.cpp',
        '<@(box2d_path)/source/Source/Dynamics/b2Island.cpp',
        '<@(box2d_path)/source/Source/Dynamics/b2World.cpp',
        '<@(box2d_path)/source/Source/Dynamics/b2WorldCallbacks.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Contacts/b2CircleContact.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Contacts/b2Contact.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Contacts/b2ContactSolver.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Contacts/b2PolyAndCircleContact.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Contacts/b2PolyContact.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Joints/b2DistanceJoint.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Joints/b2GearJoint.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Joints/b2Joint.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Joints/b2MouseJoint.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Joints/b2PrismaticJoint.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Joints/b2PulleyJoint.cpp',
        '<@(box2d_path)/source/Source/Dynamics/Joints/b2RevoluteJoint.cpp',

        # thirdparty/fhtagn
        '<@(fhtagn_path)/fhtagn/variant.cpp',
        '<@(fhtagn_path)/fhtagn/version.cpp',
        '<@(fhtagn_path)/fhtagn/text/decoders.cpp',
        '<@(fhtagn_path)/fhtagn/text/encoders.cpp',
        '<@(fhtagn_path)/fhtagn/text/transcoding.cpp',
        '<@(fhtagn_path)/fhtagn/threads/tasklet.cpp',

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

        # thirdparty/glee
        '<@(glee_path)/source/GLee.c',

        # thirdparty/hashlib++
        '<@(hashlib++_path)/hl_md5.cpp',
        '<@(hashlib++_path)/hl_md5wrapper.cpp',
        '<@(hashlib++_path)/hl_sha1.cpp',
        '<@(hashlib++_path)/hl_sha1wrapper.cpp',
        '<@(hashlib++_path)/hl_sha2ext.cpp',
        '<@(hashlib++_path)/hl_sha256.cpp',
        '<@(hashlib++_path)/hl_sha256wrapper.cpp',
        '<@(hashlib++_path)/hl_sha384wrapper.cpp',
        '<@(hashlib++_path)/hl_sha512wrapper.cpp',

        # thirdparty/lua
        '<@(lua_path)/source/src/lzio.c',
        '<@(lua_path)/source/src/lvm.c',
        '<@(lua_path)/source/src/lundump.c',
        '<@(lua_path)/source/src/ltm.c',
        '<@(lua_path)/source/src/ltablib.c',
        '<@(lua_path)/source/src/ltable.c',
        '<@(lua_path)/source/src/lstrlib.c',
        '<@(lua_path)/source/src/lstring.c',
        '<@(lua_path)/source/src/lstate.c',
        '<@(lua_path)/source/src/lparser.c',
        '<@(lua_path)/source/src/loslib.c',
        '<@(lua_path)/source/src/lopcodes.c',
        '<@(lua_path)/source/src/lobject.c',
        '<@(lua_path)/source/src/loadlib.c',
        '<@(lua_path)/source/src/lmem.c',
        '<@(lua_path)/source/src/lmathlib.c',
        '<@(lua_path)/source/src/llex.c',
        '<@(lua_path)/source/src/liolib.c',
        '<@(lua_path)/source/src/linit.c',
        '<@(lua_path)/source/src/ldblib.c',
        '<@(lua_path)/source/src/ldebug.c',
        '<@(lua_path)/source/src/lauxlib.c',
        '<@(lua_path)/source/src/lbaselib.c',
        '<@(lua_path)/source/src/ldo.c',
        '<@(lua_path)/source/src/lfunc.c',
        '<@(lua_path)/source/src/ldump.c',
        '<@(lua_path)/source/src/lapi.c',
        '<@(lua_path)/source/src/lcode.c',
        '<@(lua_path)/source/src/lgc.c',

        # thirdparty/luabind
        '<@(luabind_path)/src/class_info.cpp',
        '<@(luabind_path)/src/class_registry.cpp',
        '<@(luabind_path)/src/class_rep.cpp',
        '<@(luabind_path)/src/class.cpp',
        '<@(luabind_path)/src/create_class.cpp',
        '<@(luabind_path)/src/error.cpp',
        '<@(luabind_path)/src/exception_handler.cpp',
        '<@(luabind_path)/src/function.cpp',
        '<@(luabind_path)/src/inheritance.cpp',
        '<@(luabind_path)/src/link_compatibility.cpp',
        '<@(luabind_path)/src/object_rep.cpp',
        '<@(luabind_path)/src/open.cpp',
        '<@(luabind_path)/src/pcall.cpp',
        '<@(luabind_path)/src/scope.cpp',
        '<@(luabind_path)/src/stack_content_by_name.cpp',
        '<@(luabind_path)/src/weak_ref.cpp',
        '<@(luabind_path)/src/wrapper_base.cpp',

        # thirdparty/stb
        '<@(stb_image_path)/stb_image.c',
        '<@(stb_vorbis_path)/stb_vorbis.c',

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
            'lost/application/mac/ApplicationTaskletSpawn.cpp',
            'lost/application/mac/ApplicationWindow.mm',
            'lost/application/mac/GLView.mm',
            'lost/application/mac/Tasklet.mm',
            'lost/application/mac/Window.mm',
            'lost/gl/mac/Context.mm',
            'lost/platform/Platform_Mac.cpp',
          ],
        }],

        ['OS != "win"', {
          'sources!': [
            'lost/application/windows/Application.cpp',
            'lost/application/windows/Tasklet.cpp',
            'lost/application/windows/Window.cpp',
            'lost/gl/windows/Context.cpp',
            'lost/platform/Platform_Windows.cpp',
            '<@(boost_path)/libs/thread/src/win32/thread.cpp',
            '<@(boost_path)/libs/thread/src/win32/tss_dll.cpp',
            '<@(boost_path)/libs/thread/src/win32/tss_pe.cpp',
          ],
        }],
        ['OS == "win"', {
          'sources!': [
            '<@(boost_path)/libs/thread/src/pthread/once.cpp',
            '<@(boost_path)/libs/thread/src/pthread/thread.cpp',
          ],
          'link_settings': {
            'libraries': [
              '-l<@(openal_path)/libs/Win32/EFX-Util.lib',
              '-l<@(openal_path)/libs/Win32/OpenAL32.lib',
            ],
          },
          'include_dirs': [
            '<@(openal_path)/include',
            '<@(opengl_path)/include',
          ],
          'defines': [
            'WIN32',
            'UNICODE',
            '_LIB',
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
              '-lopenal',
              '-lGL',
              '-lGLU',
            ],
          },
        }],

      ],
    },

  ],

}
