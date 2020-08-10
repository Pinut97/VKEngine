
#include "headers.h"
#include "VulkanApplication.h"

void printInstanceInformation(const VulkanApplication& app)
{
	std::cout << "Vulkan Instance Extensions: " << std::endl;

	for (const auto& extension : app.Extensions())
	{
		std::cout << "- " << extension.extensionName << std::endl;
	}
}

void printVulkanDevices(const VulkanApplication& app)
{
	std::cout << "Vulkan Devices: " << std::endl;

	for (const auto& device : app.PhysicalDevices())
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);

		std::cout << "- [" << properties.deviceID << "] ";
		std::cout << " '" << properties.deviceName << "' (";
		std::cout << properties.deviceType << ": ";
		std::cout << "vulkan " << properties.apiVersion << ", ";
		std::cout << "driver " << properties.driverVersion << ")" << std::endl;
	}
}

int main()
{
	VulkanApplication* app = new VulkanApplication();

	try {
		app->run();
		printInstanceInformation(*app);
		printVulkanDevices(*app);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}