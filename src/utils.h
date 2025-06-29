#pragma once

#include <vulkan/vulkan.h>
#include <vector>

//Generic helper utilities for GPU management, mainly tied to memory buffers.
class VulkanUtils {
  public:
    static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice device, VkPhysicalDevice physicalDevice);
    
    static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);

    static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

    static VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);

    static void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
};
