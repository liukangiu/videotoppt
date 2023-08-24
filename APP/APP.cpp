#include <iostream>
#include "APP.h"
int core_startwork(double invfps, double prob, char* video_path_chars, int vpathlen, char* imgpath_chars, int imgpathlen, int startusesecond, int endusesecond, int* outinf_ptr)
{
	//int* a = new int[20];
	string video_path;
	string imgpath;
	for (int i = 0; i < vpathlen; i++)
	{
		video_path.push_back(video_path_chars[i]);
	}
	for (int i = 0; i < imgpathlen; i++)
	{
		imgpath.push_back(imgpath_chars[i]);
	}
	std::cout << invfps << std::endl;
	std::cout << prob << std::endl;
	std::cout << video_path << std::endl;
	std::cout << imgpath << std::endl;
	std::cout << startusesecond << std::endl;
	std::cout << endusesecond << std::endl;
	startwork(invfps, prob, video_path, imgpath, startusesecond, endusesecond, outinf_ptr);

	return 1;

}