add_library(RotatedLabel RotatedLabel.cpp)
target_link_libraries(RotatedLabel Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)

add_executable(main main.cpp RotatedLabel.cpp LabeledSlider.cpp)
target_link_libraries(main Qt5::Core Qt5::Gui Qt5::Widgets Qwt::Qwt)
