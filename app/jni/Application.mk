# 用来配置整个整个应用程序的编译相关信息

# 对应于应用的 minSdkVersion
APP_PLATFORM := android-17
APP_ABI := armeabi-v7a arm64-v8a
# 使用共享C++运行时库, 编译后aar中将附带 libc++_shared.so
APP_STL := c++_shared