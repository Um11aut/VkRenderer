#include <vulkan/vulkan.h>

#include <vector>
#include "logger.hpp"

namespace VkRenderer {
	class PhysicalDevise {
	private:
		VkPhysicalDevice* m_device = VK_NULL_HANDLE;
		VkInstance* m_instance;

		uint32_t m_device_count;

		std::vector<VkPhysicalDevice> m_devices;

		bool isDeviceSuitable(VkPhysicalDevice device);
	public:
		PhysicalDevise(VkPhysicalDevice device, VkInstance* instance);
		~PhysicalDevise();
	};
}