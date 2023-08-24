#include <iostream>
#include "CORE.h"
int core_startwork(double invfps, double prob, char* vpd, int vpathlen, char* ipd, int imgpathlen,int startusesecond, int endusesecond, char* outinf_ptr)
{
	//int* a = new int[20];
	string video_path;
	string imgpath;
    
	for (int i = 0; i < vpathlen; i++)
	{
		video_path.push_back(vpd[i]);
	}
	for (int i = 0; i < imgpathlen; i++)
	{
		imgpath.push_back(ipd[i]);
	}

    //string infor = "123jhg|";
    //memcpy(outinf_ptr, infor.data(), 7);

	//invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
	//prob = 0.9;//图像相似度，数值越大筛选条件越高
	//video_path = "D:\\桌面\\1\\f100040.mp4";//视频路径
	//imgpath = "D:\\桌面\\2\\";//文件夹路径
	//outinf_ptr = new char[10];
	//startusesecond = (0 * 60 + 0) * 60 + 0;
	//endusesecond = (0 * 60 + 32) * 60 + 19;

	std::cout << invfps << std::endl;
	std::cout << prob << std::endl;
	std::cout << video_path << std::endl;
	std::cout << imgpath << std::endl;
	std::cout << startusesecond << std::endl;
	std::cout << endusesecond << std::endl;
	//char* outinf_ptr1 = new char[10];

	startwork( invfps,  prob,  video_path,  imgpath,  startusesecond,  endusesecond, outinf_ptr);

	return 1;

}

int core_nocan_work()
{
	double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
	double prob = 0.9;//图像相似度，数值越大筛选条件越高
	string video_path = "D:\\桌面\\1\\f100040.mp4";//视频路径
	string imgpath = "D:\\桌面\\2";//文件夹路径
	char* outinf_ptr = new char[10];
	int startusesecond = (0 * 60 + 0) * 60 + 0;
	int endusesecond = (0 * 60 + 32) * 60 + 19;
	startwork(invfps, prob, video_path, imgpath, startusesecond, endusesecond, outinf_ptr);
	int jindu = 0;
    return 1;
    //std::cout << "Hello World!\n";
}