#pragma once
#include "model.h"
#include <opencv.hpp>
#include <iostream>
#include <string>
#include <windows.h>¡¡
#include<thread>
#include <regex>
#include <cstdlib>
using namespace std;


void startwork(double invfps, double prob, string video_path, string imgpath, int startusesecond, int endusesecond, char* outinf_ptr);

//extern "C" __declspec(dllexport) int starkwork_out(double invfps, double prob, string video_path, string imgpath, int startusesecond, int endusesecond, int* outinf_ptr)
//{
//	startwork(invfps, prob, video_path, imgpath, startusesecond, endusesecond, outinf_ptr);
//	return 0;
//}