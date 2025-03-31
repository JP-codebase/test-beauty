#include <iostream>
#include <string>

std::string float_without_trailing_zeros(float f) {

  std::string float_str{std::to_string(f)};

  for (int i = float_str.size(); i >= 0; i--) {
    if (float_str[i] == '0') float_str.erase(i);
  }
  /*std::cout << float_str << std::endl;*/

  return float_str;
}

int main() {

  float f{3.3};

  std::cout << "Float : " << std::to_string(f) << std::endl;

  /*float_without_trailing_zeros(f);*/
  std::cout << "Float without 0s : " << float_without_trailing_zeros(f) << std::endl;

  return 0;
}
