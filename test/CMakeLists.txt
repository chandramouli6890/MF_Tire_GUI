enable_testing()

add_executable(test_RotatedLabel test_RotatedLabel.cpp)
target_include_directories(test_RotatedLabel PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_RotatedLabel RotatedLabel Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)
add_test(NAME RotatedLabelTest COMMAND test_RotatedLabel)

add_executable(test_MainWindow test_MainWindow.cpp)
target_include_directories(test_MainWindow PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_MainWindow MainWindow Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)
add_test(NAME MainWindowTest COMMAND test_MainWindow)
