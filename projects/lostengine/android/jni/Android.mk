LOCAL_PATH:= $(call my-dir)

APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti

ndk_path:= /home/jomanto/coding/android/android-ndk-r5b
boost_path:= $(LOCAL_PATH)/../../../../thirdparty/boost/boost_1_44_0
box2d_path:= $(LOCAL_PATH)/../../../../thirdparty/box2d/box2d_2_0_1
fhtagn_path:= $(LOCAL_PATH)/../../../../thirdparty/fhtagn/fhtagn-0.3
freetype2_path:= $(LOCAL_PATH)/../../../../thirdparty/freetype2/freetype_2_4_3
glee_path:= $(LOCAL_PATH)/../../../../thirdparty/glee/glee_5_21
hashlib++_path:= $(LOCAL_PATH)/../../../../thirdparty/hashlib++/hashlib++_0_3_1
lua_path:= $(LOCAL_PATH)/../../../../thirdparty/lua/lua_5_1_4
luabind_path:= $(LOCAL_PATH)/../../../../thirdparty/luabind/luabind_0_9_1
openal_path:= $(LOCAL_PATH)/../../../../thirdparty/OpenAL/OpenAL_1_1
opengl_path:= $(LOCAL_PATH)/../../../../thirdparty/OpenGL
stb_image_path:= $(LOCAL_PATH)/../../../../thirdparty/stb/stb_image_1_18
stb_vorbis_path:= $(LOCAL_PATH)/../../../../thirdparty/stb/stb_vorbis_0_99996
unittest++_path:= $(LOCAL_PATH)/../../../../thirdparty/UnitTest++
zlib_path:= $(LOCAL_PATH)/../../../../thirdparty/zlib/zlib_1_2_3
zziplib_path:= $(LOCAL_PATH)/../../../../thirdparty/zziplib/zziplib-0.13.58

# LostEngine
#
include $(CLEAR_VARS)

LOCAL_MODULE  := LostEngine

LOCAL_CXXFLAGS:= \
-includelost/prefix-debug.pch \
-D_GLIBCXX_USE_WCHAR_T

LOCAL_C_INCLUDES:= \
$(LOCAL_PATH)/../../ \
$(boost_path) \
$(box2d_path)/source/Include \
$(fhtagn_path) \
$(freetype2_path)/include \
$(glee_path)/include \
$(hashlib++_path) \
$(lua_path)/include \
$(luabind_path) \
$(stb_image_path) \
$(stb_vorbis_path) \
$(zlib_path)/include \
$(zziplib_path)

LOCAL_SRC_FILES := \
../../lost/application/Application.cpp

include $(BUILD_STATIC_LIBRARY)

# Launcher
#
include $(CLEAR_VARS)

LOCAL_MODULE            := Launcher
LOCAL_CXXFLAGS          := -includelost/prefix-debug.pch
LOCAL_LDLIBS            := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES  := android_native_app_glue LostEngine

LOCAL_C_INCLUDES:= \
$(LOCAL_PATH)/../../ \
$(boost_path)

LOCAL_SRC_FILES:= \
../../main.cpp

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)

