#pragma once
namespace dae
{
	// The Singleton class template provides a thread-safe implementation of the singleton design pattern for any type T.
	// It ensures that only one instance of T is created and provides global access to that instance through the GetInstance() method.
	// The constructor, copy constructor, move constructor, copy assignment operator, and move assignment operator are all deleted to prevent multiple instances from being created or copied.
	// The destructor is virtual and defaulted to allow for proper cleanup of resources when the singleton instance is destroyed.
	template <typename T>
	class Singleton
	{
	public:
		[[nodiscard]] static T& GetInstance()
		{
			static T instance{};
			return instance;
		}

		virtual ~Singleton() = default;
		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

	protected:
		Singleton() = default;
	};
}