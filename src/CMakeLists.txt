add_library(RotatedLabel RotatedLabel.cpp)
target_link_libraries(RotatedLabel Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)

add_library(LabeledSlider LabeledSlider.cpp)
target_link_libraries(LabeledSlider Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)

add_library(MainWindow MainWindow.cpp LabeledSlider.cpp)
target_link_libraries(MainWindow LabeledSlider RotatedLabel Qwt::Qwt yaml-cpp::yaml-cpp)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE MainWindow) 
