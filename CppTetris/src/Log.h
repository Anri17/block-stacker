#pragma once

#include <iostream>

// TODO: every time a log is called, save that information into a file that is created on application execution

#ifdef _DEBUG

#define LOG_INFO(x) std::cout << "[INFO]: " << x << std::endl; 
#define LOG_WARNING(x) std::cout << "[WARNING]: " << x << std::endl; 

#else

#define LOG_INFO(x); 
#define LOG_WARNING(x); 

#endif // DEBUG


