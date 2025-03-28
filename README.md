# Magic Formula Visualizer

The **Magic Formula Tire Visualizer** is a Qt C++ graphical user interface
designed to illustrate the impact of each Magic Formula parameter on the tire
force vs. slip curve.

$$
F_x = F_z \cdot D \cdot \sin \left( C \cdot \arctan \left\{ B\kappa - E \left[ B\kappa - \arctan(B\kappa) \right] \right\} \right)
$$

![MF_Tire_Visualizer_Demo](pics/MF_Tire_Visualizer_Demo.gif)

## Building the Qt App

Before building the application, ensure you have all necessary dependencies.
You can refer to the GitHub Actions [file](.github/workflows/run_tests.yml) for
a list of required dependencies.

To build the app, run the following commands:

```
git clone git@github.com:chandramouli6890/MF_Tire_GUI.git
mkdir build && cd build
cmake ..
make
```

Once the build is complete, you can run the application using:

```
./build/src/MF_Tire_Visualizer
```

## Contributing

Thank you for your interest in contributing! Contributions of all kinds, from
bug reports to code, documentation, or suggestions for improvement are welcome.

If you're submitting code via a pull request, we recommend including relevant
tests. You can find example tests in the designated [test](./test) folder.

To run the test suite, execute:
```
cd <repo>/build
ctest
```

link to the original [matlab file_exchange project](https://de.mathworks.com/matlabcentral/fileexchange/48568-mf_tire_gui)
