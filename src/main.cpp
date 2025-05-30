#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <optional>

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
    VkDebugUtilsMessengerEXT debugMessenger;
    GLFWwindow* window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;

    const std::vector<const char*> validationLayers = {
      "VK_LAYER_KHRONOS_validation"
    };

  #ifdef NDEBUG
    const bool enableValidationLayers = false;
  #else
    const bool enableValidationLayers = true;
  #endif
    
    struct QueueFamilyIndices{
      std::optional<uint32_t> graphicsFamily;
      bool isComplete() {
        return graphicsFamily.has_value();
      }
    }
    
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
      QueueFamilyIndices indices;
      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          indices.graphicsFamily = i;
        }
        if (indices.isComplete()) {
          break;
        }
        i++;
      }
      return indices;

    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator){
      auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
      if (func != nullptr){
        func(instance, debugMessenger, pAllocator);
      }
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
      createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      createInfo.pfnUserCallback = debugCallback;
    }

    void setupDebugMessenger() {
      if (!enableValidationLayers) {
        return;
      }
      VkDebugUtilsMessengerCreateInfoEXT createInfo;
      populateDebugMessengerCreateInfo(createInfo);
      if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
      }
    }


    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
      std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
      return VK_FALSE;
    }

    std::vector<const char*> getRequiredExtensions(){
      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
      
      //We submit the start and the end of the range when converting from an array to a vector.
      std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

      if (enableValidationLayers){
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      }

      return extensions;
    }

    bool checkValidationLayerSupport(){
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
      //Iterate through the available layers to check if we have all of the ones we need. 
      for (const char* layerName : validationLayers){
        bool layerFound = false;
        
	for (const auto& layerProperties : availableLayers){
	  if(strcmp(layerName, layerProperties.layerName) == 0) {
	    layerFound = true;
	    break;
	  }
	}
	if (!layerFound){
	  return false;
	}
      }
      return true;
    }
    
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
      auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
      if (func != nullptr){
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
      }
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void initWindow(){
      glfwInit();

      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

      window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void createInstance(){
      if (enableValidationLayers && !checkValidationLayerSupport()){
        throw std::runtime_error("validation layers requested, but not available!");
      }
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

      //Extension handling.
      auto glfwExtensions = getRequiredExtensions();
      createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
      createInfo.ppEnabledExtensionNames = glfwExtensions.data();

      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
      if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
      } 
      else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
      }

      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
      }

      //Extensions
      uint32_t extensionCount = 0;
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
      //Allocate an array told the details.
      std::vector<VkExtensionProperties> extensions(extensionCount);
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
      std::cout << "Available Support Extensions:\n";
      //Iterate through the extensions.
      for (const auto& extension : extensions){
        std::cout << '\t' << extension.extensionName << '\n';
      }

      //Initiate the validation layers.
      if (enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
      }
      else{
        createInfo.enabledLayerCount = 0;
      }
    }
    
    bool isDeviceSuitable(VkPhysicalDevice device) {
      QueueFamilyIndices indices = findQueueFamilies(device);
      return indices.isComplete();
    }
    
    //Maybe later allow the user to change GPU?
    void pickPhysicalDevice(){
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
      //If there is no GPU, return an error.
      if (deviceCount == 0){
        throw std::runtime_error("No GPUs detected.");
      }
      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
      for (const auto& device : devices){
        if (isDeviceSuitable(device)){
          physicalDevice = device;
          break;
        }
      }
      if (physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("No GPUs with Vulkan support detected.");
      }
    }

    void initVulkan(){
      createInstance();
      setupDebugMessenger();
      pickGraphicsCard();
    }
    
    void createLogicalDevice(){
      QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
      queueCreateInfo.queueCount = 1;

      float queuePriority = 1.0f;
      queueCreateInfo.pQueuePriorities = &queuePriority;

      VkPhysicalDeviceFeatures deviceFeatures{};

      VkDeviceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      createInfo.pQueueCreateInfos = &queueCreateInfo;
      createInfo.queueCreateInfoCount = 1;
      createInfo.pEnabledFeatures = &deviceFeatures;
       createInfo.enabledExtensionCount = 0;

      if (enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
      } 
      else{
        createInfo.enabledLayerCount = 0;
      }

      if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS){
        throw std::runtime_error("Failed to create logical device!");
      }

      vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }

    void mainLoop(){
      while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
      }
    }

    void cleanup(){
      vkDestroyDevice(device, nullptr);
      if (enableValidationLayers){
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
      }
      glfwDestroyWindow(window);
      glfwTerminate();
    }
};

int main(){
  HelloTriangleApplication app;
  try {
    app.run();
  } catch (const std::exception& e){
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  
  }

  return EXIT_SUCCESS;
}
