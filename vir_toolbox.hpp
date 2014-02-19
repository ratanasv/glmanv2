#pragma once
#include <memory>
#include <vector>
#include <cstring>
#include <map>
#include <utility>
#include <mutex>
#include <cstdarg>

template <class T> std::shared_ptr<std::vector<T>> initSmartArray(const int size = 0) {
	std::shared_ptr<vector<T>> returned(new std::vector<T>(size));
	return returned;
}

template <class T> std::shared_ptr<T> initCStyleArray(T* dumb) {
	return std::shared_ptr<T>(dumb, [](T* ptr) {
		delete[] ptr;
	});
}

std::shared_ptr<FILE> InitFilePointer(const std::string& name, const char* mode);

std::string vvprintf(const char* format, ...);

template <class T, class V> static void InsertHelper(std::map<T,V>& theMap, 
	const T& key, const V& value) 
{
	theMap.insert(std::pair<T,V>(key, value));
}


template<class T> class LazyInitOnce {
private:
	std::once_flag _initFlag;
	T _data;
	std::function<T()> _initFunc;
private:
	void init() {
		_data = _initFunc();
	}
public:
	void Set(const std::function<T()>& lambda) {
		_initFunc = lambda ;
	}
	operator T&() {
		std::call_once(_initFlag, &LazyInitOnce::init, this);
		return _data;
	}

};