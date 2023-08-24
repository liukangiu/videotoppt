#include "model.h"
#include <opencv.hpp>
#include <iostream>
#include <string>
#include <windows.h>　
//#include <pthread.h>
#include <cstdlib>
#include<thread>
using namespace std;

#define NUM_THREADS     2

//struct thread_data {
//    cv::VideoCapture cap;
//    int fpscount;
//    int interval;
//    int prob;
//};
void getppt(cv::VideoCapture cap,int fpscount, int interval, int prob)
//void getppt()
{
    
    cv::Mat lastimg;
    cv::Mat nowimg;
    cap.read(lastimg);
    int fps = cap.get(cv::CAP_PROP_FPS);
    DWORD t1, t2;
    for (int i = interval; i < fpscount; i = i + interval)
    {
        t1 = GetTickCount();
        cap.set(cv::CAP_PROP_POS_FRAMES, i);
        cap.read(nowimg);
        if (nowimg.empty())
        {
            std::cout << "nowimg抽帧错误，图像空白" << std::endl;
            continue;
        }

        double same = delsame(lastimg, nowimg);
        //double same = ssim(lastimg, nowimg);

        if (same < prob)
        {

            double time = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
            int min = time / 60;
            int sen = (int)time % 60;
            string imgpath = "D:\\ApowerREC\\20220828_104110\\";
            //imgpath.append()
            cv::imwrite(imgpath + to_string(min) + "_" + to_string(sen) + ".jpg", nowimg);
            lastimg = nowimg.clone();
            std::cout << "保存新的帧，时间" + to_string(min) + ":" + to_string(sen) << std::endl;

            //cv::imshow("showpic", nowimg);

        }

        t2 = GetTickCount();
        double dantimems = (t2 - t1) * 1.0;
        t1 = t2;
        int shenmin = fpscount * fps / interval * dantimems / 60 / 1000;
        int shenhour = shenmin / 60;
        shenmin = shenmin % 60;
        std::cout << "此循环时间" + to_string(dantimems) + "ms" << std::endl;
        std::cout << "循环,剩余时间" + to_string(shenhour) + "时" + to_string(shenmin) + "分" << std::endl;

    }
    //return 0;

}


int mainv()
{
    string video_path = "D:\\ApowerREC\\20220828_104110.mp4";
    cv::VideoCapture cap = cv::VideoCapture(video_path);
    
    bool suc = cap.isOpened();
    cv::Mat lastimg;
    cv::Mat nowimg;
    int fps = cap.get(cv::CAP_PROP_FPS);
    int fpscount = cap.get(cv::CAP_PROP_FRAME_COUNT);
    cap.read(lastimg);
    if (lastimg.empty())
    {
        std::cout << "抽帧错误，图像空白" << std::endl;
    }

    int interval = fps*2;
    double prob = 0.9;
    //DWORD t1, t2;
    int eachfpscount = fpscount / NUM_THREADS;

    thread th[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        th[i] = thread(getppt, cap, eachfpscount, interval, prob);
    }
    
    for (auto& thread : th)
        thread.join();
    return 0;
}

