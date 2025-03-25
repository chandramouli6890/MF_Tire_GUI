enable_testing()

add_executable(test_LabeledSlider test_LabeledSlider.cpp)
target_include_directories(test_LabeledSlider PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_LabeledSlider LabeledSlider Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)
add_test(NAME LabeledSliderTest COMMAND test_LabeledSlider)

add_executable(test_MainWindow test_MainWindow.cpp)
target_include_directories(test_MainWindow PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_MainWindow MainWindow Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)
add_test(NAME MainWindowTest COMMAND test_MainWindow)
