file(GLOB source *.h *.cpp)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_library(qcustomplot OBJECT ${source})# cmake version >3.12
target_link_libraries(qcustomplot PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)
add_subdirectory(ximage)
add_subdirectory(xmove)