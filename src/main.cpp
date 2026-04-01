#include "pch.h"

int main(void)
{
  if(!glfwInit())
  {
    std::cout<<"Failed to Init GLFW"<<std::endl;
    return 0;
  }

  std::cout<<"GLFW Init Success!"<<std::endl;
}
