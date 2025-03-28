add_library(LabeledSlider LabeledSlider.cpp)
target_link_libraries(LabeledSlider Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)

add_library(MainWindow MainWindow.cpp LabeledSlider.cpp)
target_link_libraries(MainWindow LabeledSlider qwt-qt5 yaml-cpp::yaml-cpp)

add_executable(MF_Tire_Visualizer main.cpp)
target_link_libraries(MF_Tire_Visualizer PRIVATE MainWindow) 
