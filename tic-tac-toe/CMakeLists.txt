# 计算器 Calculator
# 练习：QGridLayout 网格布局 + 信号槽 + 简单状态机（四则运算）

# 指定 CMake 最低版本
cmake_minimum_required(VERSION 3.19)

# 定义项目名、版本、使用的语言
project(Calculator VERSION 1.0 LANGUAGES CXX)

# 使用 C++17 标准，并强制要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 关键设置：自动运行三大预处理工具
set(CMAKE_AUTOMOC ON)   # 自动处理含 Q_OBJECT 的类（元对象编译器）
set(CMAKE_AUTORCC ON)   # 自动处理 .qrc 资源文件
set(CMAKE_AUTOUIC ON)   # 自动处理 .ui 设计师文件

# 查找 Qt，需要 Widgets 模块。Qt6 优先，找不到则用 Qt5
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

# 定义可执行文件，列出所有源文件
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
)

# 把 Qt::Widgets 库链接进来
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
