
#define DEFAULT_FILENAME "../data/tire_data_longitudinal.yaml"

struct Value {
  double min;
  double max;
  double val;
};

struct Params {
  Value stiffness;
  Value shape;
  Value peak;
  Value curvature;
  Value slip;
  double vertical_force{-1.0};
  bool is_lateral{false};
  int array_size{-1};
};
