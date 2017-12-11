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

const int mWidth = 1280;
const int mHeight = 800;

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

float vertices[] = {
   0.5f,  0.5f,  0.5f,  // top right
   0.5f, -0.5f,  0.5f,  // bottom right
  -0.5f, -0.5f,  0.5f,  // bottom left
  -0.5f,  0.5f,  0.5f,  // top left
   0.5f,  0.5f, -0.5f,  // top right
   0.5f, -0.5f, -0.5f,  // bottom right
  -0.5f, -0.5f, -0.5f,  // bottom left
  -0.5f,  0.5f, -0.5f,  // top left
};
const int vertexAttribStride = 3 * sizeof(float);

unsigned int indices[] = {
  0, 1, 3,
  1, 2, 3,
  0, 1, 4,
  1, 4, 5,
  1, 2, 5,
  2, 5, 6,
  2, 3, 6,
  3, 6, 7,
  0, 3, 4,
  3, 4, 7,
  4, 5, 6,
  4, 6, 7,
};
const int numIndices = sizeof(indices) / sizeof(unsigned int);

const std::string vertexShaderSourceStr = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const std::string fragmentShaderSourceStr = R"(
#version 330 core

out vec4 FragColor;

void main()
{
  FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

int setupShaders() {
  // Vertex shader
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char* vertexShaderSource = vertexShaderSourceStr.data();
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Fragment shader
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fragmentShaderSource = fragmentShaderSourceStr.data();
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // link shaders
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void setupVertexData(unsigned int* VBO, unsigned int* VAO, unsigned int* EBO) {
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s),
  // and then configure vertex attributes(s).
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  {
    const int vertexAttribIndex = 0;
    const int componentsPerAttrib = 3;
    void* startOffsetPointer = (void*)(sizeof(float) * 0);
    glVertexAttribPointer(vertexAttribIndex, componentsPerAttrib, GL_FLOAT, GL_FALSE, vertexAttribStride, startOffsetPointer);
    glEnableVertexAttribArray(vertexAttribIndex);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound element
  // buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
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

  int shaderProgram = setupShaders();

  unsigned int VBO, VAO, EBO;
  setupVertexData(&VBO, &VAO, &EBO);

  // uncomment this call to draw in wireframe polygons.
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    processInput(mWindow);

    // Background Fill Color
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    glm::mat4 projection = glm::mat4(1.0);

    // (float)glfwGetTime()
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // (field of view, aspect ratio, zNear, zFar)
    projection = glm::perspective(glm::radians(45.0f), (float)mWidth / (float)mHeight, 0.1f, 100.0f);

    // retrieve the matrix uniform locations
    unsigned int modelLoc      = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc       = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // pass them to the shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();

  return EXIT_SUCCESS;
}
