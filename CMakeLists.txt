# 指定 CMake 最低版本
cmake_minimum_required(VERSION 3.19)

# 【改】项目名改成 tic_tac_toe（不要用横杠 -）
project(tic_tac_toe VERSION 1.0 LANGUAGES CXX)

# 使用 C++17 标准，并强制要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 关键设置：自动运行三大预处理工具
set(CMAKE_AUTOMOC ON)   # 自动处理含 Q_OBJECT 的类
set(CMAKE_AUTORCC ON)   # 自动处理 .qrc 资源文件
set(CMAKE_AUTOUIC ON)   # 自动处理 .ui 设计师文件

# 查找 Qt，支持 Qt6 和 Qt5
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

# 【改】可执行文件名字改成 tic_tac_toe，源文件路径加上 tic-tac-toe/ 前缀
add_executable(tic_tac_toe
    tic-tac-toe/src/main.cpp
    tic-tac-toe/src/mainwindow.cpp
    tic-tac-toe/src/mainwindow.h
)

# 【改】链接库时直接用可执行文件名，而不是 ${PROJECT_NAME}
target_link_libraries(tic_tac_toe PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)