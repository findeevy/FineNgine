#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleApplication{
  public:
    void run(){
      initWindow();
      initVulkan();
      mainLoop();
      cleanup();
    }
  private:
    VkInstance instance;
    GLFWwindow* window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    void initWindow(){
      glfwInit();

      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZEABLE, GLFW_FALSE);

      window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }
    void createInstance(){
      //Info to set up the Vulkan application.
      VkApplicationInfo appInfo{};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "Finengine";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "No Engine";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;
      
      //Info to set up the Vulkan driver's extensions and validation layers.
      VkInstanceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      
      //Pipe our application info into the creation info via a pointer.
      createInfo.pApplicationInfo = &appInfo;

      //Window handling.
      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
      createInfo.enabledExtensionsCount = glfwExtensionCount;
      createInfo.ppEnabledExtensionNames = glfwExtensions;
      createInfo.enabledLayerCount 0;
      VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
        throw std::runtime_error("failed to create instance!");
      }

      //Check for extension support.

    }
    void initVulkan(){
      createInstance();
    }
    void mainLoop(){
      while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
      }
    }
    void cleanup(){
      glfwDestroyWindow(window);
      glfwTerminate();
    }
};

int main(){
  HelloTriangleApplication app;

  try {
    app.run();
  } catch (const std::exception& e){
    std::cerr << e.what() << std.endl;
    return EXIT_FAILURE;
  
  }

  return EXIT_SUCCESS;
}
