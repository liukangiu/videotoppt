#pragma once
#ifndef APP_H_
#define APP_H_
/*
主要实现对选定图像的基本编辑操作的接口的重新汇总整合：
（1）CTW_LABS
（2）CTW_MAPS
 (3)
*/


#include <string>
#include <opencv.hpp>
#include "D:\\Projects\\c++\\video_ppt\\APP\\video2ppt.h"
using namespace std;
#ifndef _CTW_CORE_EXPORT
#define CLASS_DECLSPEC extern "C" __declspec(dllexport)
#else
#define CLASS_DECLSPEC extern "C" __declspec(dllimport)
#endif


CLASS_DECLSPEC int core_startwork(double invfps, double prob, char* video_path_chars, int vpathlen, char* imgpath_chars, int imgpathlen, int startusesecond, int endusesecond, int* outinf_ptr);

//CLASS_DECLSPEC int ctw_core_rend3d(int rdmode);

//CLASS_DECLSPEC int ctw_core_window3d(int w, int h, std::string title);



#endif 