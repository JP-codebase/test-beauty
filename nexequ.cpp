#include <iostream>

int main(){

  unsigned int setSize{0};
  std::cout << "Insert set size: ";
  std::cin >> setSize;

  // Initial set composed of `setSize` distiguishable characters
  
  /*unsigned char* set {new unsigned char [setSize]};*/
  /**/
  /*for (int i = 0; i < setSize; i++){*/
  /*  set[i] = i + 'a';*/
  /*  std::cout << set[i] << ' ';*/
  /*}*/
  /*std::cout << std::endl;*/



  unsigned int* partition {new unsigned int [setSize]};

  for (int i = 0; i < setSize; i++){
    partition[i] = i;
  }


  // Print

  std::cout << "----------------------------------" << std::endl;
  for (int i = 0; i < setSize; i++){
    std::cout << partition[i] << ' ';
  }
  std::cout << std::endl;
  
  return 0;
}
