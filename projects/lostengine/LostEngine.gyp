{

  'xcode_settings': {
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

    # FIXME: split for debug/release
    'GCC_PREFIX_HEADER': 'lost/prefix-shared.h',
  },

  'variables': {
    'boost_path': '../../thirdparty/boost/boost_1_44_0',
    'box2d_path': '../../thirdparty/box2d/box2d_2_0_1',
    'fhtagn_path': '../../thirdparty/fhtagn/fhtagn-0.3',
    'freetype2_path': '../../thirdparty/freetype2/freetype_2_4_3',
    'glee_path': '../../thirdparty/glee/glee_5_21',
    'hashlib++_path': '../../thirdparty/hashlib++/hashlib++_0_3_1',
    'lua_path': '../../thirdparty/lua/lua_5_1_4',
    'luabind_path': '../../thirdparty/luabind/luabind_0_9_1',
    'stb_image_path': '../../thirdparty/stb/stb_image_1_18',
    'stb_vorbis_path': '../../thirdparty/stb/stb_vorbis_0_99996',
    'zlib_path': '../../thirdparty/zlib/zlib_1_2_3',
    'zziplib_path': '../../thirdparty/zziplib/zziplib-0.13.58',
  },

  'targets': [
    {

      'target_name': 'Launcher',

      'type': 'executable+bundle',

      'dependencies': [
        'LostEngine'
      ],

      'libraries': [
        '/System/Library/Frameworks/Cocoa.framework',
        '/System/Library/Frameworks/OpenAL.framework',
        '/System/Library/Frameworks/OpenGL.framework',
      ],

      'include_dirs': [
        '.',
        '<@(boost_path)',
      ],

      'xcode_settings': {
        'GCC_VERSION': '4.2',
        'GCC_C_LANGUAGE_STANDARD': 'c99',
        'GCC_WARN_ABOUT_RETURN_TYPE': 'YES',
        'GCC_WARN_UNUSED_VARIABLE': 'YES',
        'GCC_PRECOMPILE_PREFIX_HEADER': 'YES',
        'GCC_PFE_FILE_C_DIALECTS': 'c++ objective-c++',
        'GCC_INCREASE_PRECOMPILED_HEADER_SHARING': 'YES',
        'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
        'GCC_OBJC_CALL_CXX_CDTORS': 'YES',

        # FIXME: split for debug/release
        'GCC_PREFIX_HEADER': 'lost/prefix-shared.h',
      },

      'sources': [
        'main.cpp',
      ],

      'postbuilds': [{
        'variables': {
          # Define copy_resources_path in a variable ending in _path
          # so that gyp understands it's a path and performs proper
          # relativization during dict merging.
          'copy_resources_path':
          '<(DEPTH)/copy_resources.sh',
        },
        'postbuild_name': 'Copy Resources',
        'action': ['<(copy_resources_path)'],
      }],

    },

    {

      'target_name': 'LostEngine',

      'type': 'static_library',

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

        # application/mac
        'lost/application/mac/Application.mm',
        'lost/application/mac/ApplicationDelegate.mm',
        'lost/application/mac/ApplicationTaskletSpawn.cpp',
        'lost/application/mac/ApplicationWindow.mm',
        'lost/application/mac/GLView.mm',
        'lost/application/mac/Tasklet.mm',
        'lost/application/mac/Window.mm',

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

        # gl/mac
        'lost/gl/mac/Context.mm',

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
        'lost/platform/Platform_Mac.cpp',
        'lost/platform/Platform.cpp',

        # resource
        'lost/resource/ApplicationResourceRepository.cpp',
        'lost/resource/DefaultLoader.cpp',
        'lost/resource/FilesystemRepository.cpp',
        'lost/resource/Helper.cpp',
        'lost/resource/Loader.cpp',

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
          ]
        }],
      ],
    },

  ],

}
