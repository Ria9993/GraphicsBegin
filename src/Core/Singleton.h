#pragma once

template <typename T>
class Singleton {
private:
	Singleton() = default;
	Singleton(const Singleton& rhs) = delete;
	Singleton& operator=(const Singleton& rhs) = delete;
public:
	static T* GetInstance() 
	{
		static T inst{};
		return &inst;
	}
};