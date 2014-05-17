LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := png
FILE_LIST := $(wildcard $(LOCAL_PATH)/../modules/png/*.c*)
LOCAL_SRC_FILES :=$(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../modules/png
HEADERS += $(LOCAL_EXPORT_C_INCLUDES)
STATICLIBS += $(LOCAL_MODULE)
include $(BUILD_STATIC_LIBRARY)
#LOCAL_LDLIBS += -lz

include $(CLEAR_VARS)

LOCAL_MODULE    := ufo-survival
LOCAL_SRC_FILES := main.cpp log.cpp ../../src/Asteroid.cpp ../../src/Collider.cpp ../../src/Game.cpp ../../src/GameObject.cpp ../../src/Graphics.cpp ../../src/Utils.cpp
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM -lz
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../src/include
LOCAL_STATIC_LIBRARIES := android_native_app_glue png

include $(BUILD_SHARED_LIBRARY)

#$(call import-module,libpng)
$(call import-module,android/native_app_glue)