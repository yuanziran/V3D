aux_source_directory(. source_list)

add_executable(xmove ${source_list})

target_link_libraries(xmove PUBLIC qcustomplot Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)