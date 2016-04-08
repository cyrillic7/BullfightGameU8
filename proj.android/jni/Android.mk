LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/GameLogic/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Tools/*.cpp)

FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Play/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Play/PlayerLayer/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Play/CardLayer/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Play/GameControl/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Play/OxHundred/*.cpp)

FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Shader/*.cpp)

FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/GameLobby/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Logo/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/LogonScene/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/MainScene/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/PopDialogBox/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/MTNotificationQueue/*.cpp)

FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Network/ListernerThread/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Network/MD5/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Network/TCPSocket/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Network/HttpDownload/*.cpp)

FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Platform/android/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Libs/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/jansson/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/NDKHelper/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/u8sdk/*.cpp)

LOCAL_SRC_FILES := hellocpp/main.cpp
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%) 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += libiconv_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
$(call import-module,libiconv)
