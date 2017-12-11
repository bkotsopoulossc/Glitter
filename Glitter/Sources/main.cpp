// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>

// Define Some Constants
const int mWidth = 1280;
const int mHeight = 800;

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {

  // Load GLFW and Create a Window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

  // Check for Valid Context
  if (mWindow == nullptr) {
      fprintf(stderr, "Failed to Create OpenGL Context");
      return EXIT_FAILURE;
  }

  // Create Context and Load OpenGL Functions
  glfwMakeContextCurrent(mWindow);
  gladLoadGL();
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    processInput(mWindow);

    // Background Fill Color
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
