# 用来配置具体模块的编译相关信息

# 源码为当前路径：my-dir是构建系统提供的宏函数，它将会返回当前目录的路径
LOCAL_PATH := $(call my-dir)

# 清除除LOCAL_PATH之外的所有LOCAL_XXX变量。这个清理动作是必须的，因为所有的编译控制文件都是由同一个GNU Make解析和执行，其变量是全局的，只有清理后才能避免相互影响
include $(CLEAR_VARS)