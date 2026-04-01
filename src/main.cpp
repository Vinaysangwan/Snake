#include "pch.h"
#include "utils.h"

int main(void)
{
  if(!glfwInit())
  {
    SN_ASSERT(false, "Failed to Init GLFW!");
    return 0;
  }

  SN_INFO("GLFW Init Success!");
}
