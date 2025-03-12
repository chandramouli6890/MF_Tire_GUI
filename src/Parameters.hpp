
#define DEFAULT_FILENAME "../tire_data_longitudinal.yaml"

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
  Value longitudinal_slip;
  double vertical_force{3000.0};
  bool is_lateral{false};
  int array_size{100};
};
