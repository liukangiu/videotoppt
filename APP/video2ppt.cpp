#include "model.h"
#include <opencv.hpp>
#include <iostream>
#include <string>
#include <windows.h>　
#include<thread>
#include <regex>
#include <cstdlib>
using namespace std;

#define NUM_THREADS     3

//struct thread_data {
//    cv::VideoCapture cap;
//    int fpscount;
//    int interval;
//    int prob;
//};

void ce(int a)

{
    a = 1;
}
void getppt(vector<cv::Mat> fpsdanchou, vector<double> times,double prob,string imgpath)
//void getppt()
{

    cv::Mat lastimg;
    cv::Mat nowimg;
    lastimg = fpsdanchou[0];
    int fpscount = fpsdanchou.size();
    DWORD t1, t2;
    for (int i = 0; i < fpscount; i++)
    {
        t1 = GetTickCount();
        nowimg = fpsdanchou[i];
        double time = times[i];
        if (nowimg.empty())
        {
            std::cout << "nowimg抽帧错误，图像空白" << std::endl;
            continue;
        }

        double same = delsame(lastimg, nowimg);
        //double same = ssim(lastimg, nowimg);

        if (same < prob)
        {
            int min = time / 60;
            int sen = (int)time % 60;
            //string imgpath = "D:\\ApowerREC\\20220828_104110\\";
            //imgpath.append()
            cv::imwrite(imgpath + to_string(min) + "_" + to_string(sen) + ".jpg", nowimg);
            lastimg = nowimg.clone();
            std::cout << "保存新的帧，时间" + to_string(min) + ":" + to_string(sen) << std::endl;

            //cv::imshow("showpic", nowimg);

        }

        t2 = GetTickCount();
        double dantimems = (t2 - t1) * 1.0;
        t1 = t2;
        
        std::cout << this_thread::get_id() <<"相似"+to_string(same) + "循环时间" + to_string(dantimems) + "ms" << std::endl;
        
    }
    //return 0;

}

void putout(string st)
{
    cout << st << endl;
}

struct video_data {
    double invfps;
    double prob;
    string video_path;
    string imgpath;
    int numth;
    int startusesecond;
    int endusesecond;
};



int video2ppt(video_data vd, char* outinf_ptr)
{
    cout << "进入video2ppt" << endl;
    //参数选择区
    double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
    double prob = 0.9;//图像相似度，数值越大筛选条件越高
    string video_path = vd.video_path;//视频路径
    cout << video_path << endl;
    string imgpath = vd.imgpath + "/";//文件夹路径
    cout << imgpath << endl;
    int numth = NUM_THREADS;//线程数，谨慎小心CPU热死（^_^）
    int stop = 0;

    int eachxianread = 100;

    /*cv::Mat src = cv::imread("D:\\Projects\\c++\\video_ppt\\x64\\Debug\\lenagray.png");
    cv::imshow("1", src);

    cv::waitKey(0);*/
     


    cv::VideoCapture cap = cv::VideoCapture(video_path);
    bool suc = cap.isOpened();

    cout << suc << endl;

    //视频·参数
    double fps = cap.get(cv::CAP_PROP_FPS);
    int step = fps * invfps;
    int fpscount_allvideo = cap.get(cv::CAP_PROP_FRAME_COUNT);
    int startusesecond = vd.startusesecond;
    int endusesecond = vd.endusesecond;
    int startfps = startusesecond * fps;
    int endfps = endusesecond * fps;
    int fpscount = endfps - startfps;
    
    int imgcount = fpscount / step;
    int jindu;
    //jindu = (char)49;
     //invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
     //prob = 0.9;//图像相似度，数值越大筛选条件越高
     //video_path = "D:\\桌面\\1\\f100040.mp4";//视频路径
     //imgpath = "D:\\桌面\\2\\";//文件夹路径
     ////outinf_ptr = new char[10];
     //startusesecond = (0 * 60 + 0) * 60 + 0;
     //endusesecond = (0 * 60 + 32) * 60 + 19;
   


    //输入检测
    if (startfps > endfps) putout("ERROR:终止大于起始");
    else if (fpscount > fpscount_allvideo) putout("ERROR:终止时间超过视频总长");

    //定义线程
    thread th[NUM_THREADS];

    //处理参数
    int xianchengre = fpscount / (numth * eachxianread * step);
    int remainfpscount = fpscount % (numth * eachxianread * step);
    int remain_startfps = startfps + fpscount - remainfpscount;

    //容器定义
    vector<vector<cv::Mat>> fpsdanchou_duoxian;
    vector<vector<double>> times_duoxian;
    vector<cv::Mat> fpsdanchou;
    vector<double> times;
    cv::Mat linimg;
    double time;
    int nowfps = startfps;

    //0时刻封面
    cap.read(linimg);
    
    //开始第一次处理
    for (int i = 0; i < xianchengre; i++)
    {
        
        /*if (stop == 1)
        {
            vdret->outinform = "正在执行终止";
            return 1;
        }*/
        for (int j = 0; j < numth; j++)
        {
            for (int k = 0; k < eachxianread; k++)
            {
                cap.set(cv::CAP_PROP_POS_FRAMES, nowfps);
                cap.read(linimg);              
                fpsdanchou.push_back(linimg);               
                linimg.release();

                time = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
                times.push_back(time);  
                time = 0;

                cout <<"第" + to_string((int)nowfps/step) +"张图" << endl;
   
                nowfps += step;
                //*outinf_ptr = '1';
                jindu= (nowfps - startfps) * 100. / fpscount;
                *outinf_ptr = (char)jindu;
                
            }
            fpsdanchou_duoxian.push_back(fpsdanchou);
            times_duoxian.push_back(times);
            fpsdanchou.clear();
            times.clear();
            
        }

        for (int i = 0; i < numth; i++)
        {

            th[i] = thread(getppt, fpsdanchou_duoxian[i], times_duoxian[i], prob, imgpath);
        }

        for (int i = 0; i < numth; i++)
        {
            if (th[i].joinable())
                th[i].join();
        }
        
        fpsdanchou_duoxian.clear();
        times_duoxian.clear();
    }
   
    
    //开始第二次处理
    eachxianread = remainfpscount / step / numth;
    for (int j = 0; j < numth; j++)
    {
        for (int k = 0; k < eachxianread; k++)
        {
            cap.set(cv::CAP_PROP_POS_FRAMES, nowfps);
            cap.read(linimg);
            fpsdanchou.push_back(linimg);
            linimg.release();

            time = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
            times.push_back(time);
            time = 0;

            cout << "第" + to_string((int)nowfps / step) + "张图" << endl;

            nowfps += step;
            jindu = (nowfps - startfps) * 100. / fpscount;
            *outinf_ptr = (char)jindu;
            
        }
        fpsdanchou_duoxian.push_back(fpsdanchou);
        times_duoxian.push_back(times);
        fpsdanchou.clear();
        times.clear();
        

    }

    for (int i = 0; i < numth; i++)
    {

        th[i] = thread(getppt, fpsdanchou_duoxian[i], times_duoxian[i], prob, imgpath);
    }

    for (int i = 0; i < numth; i++)
    {
        if (th[i].joinable())
            th[i].join();
    }

    fpsdanchou_duoxian.clear();
    times_duoxian.clear();

    //结束
    //delete th;
    *(outinf_ptr + 1) = '1';
    return 0;
    
}

void startwork(double invfps, double prob, string video_path, string imgpath, int startusesecond, int endusesecond, char* outinf_ptr)
{
    std::cout << "进入app" << std::endl;
    struct video_data vd;
    //struct video_ret vdret;
    //struct video_ret *pvdret;
    //参数
    //double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
    //double prob = 0.9;//图像相似度，数值越大筛选条件越高
    //string video_path = video_path;//视频路径
    //string imgpath = imgpath;//文件夹路径
    int numth = NUM_THREADS;//线程数，谨慎小心CPU热死（^_^）

    replace(video_path.begin(), video_path.end(), '\\', '/');
    replace(imgpath.begin(), imgpath.end(), '\\', '/');

    //参数
    vd.invfps = invfps;
    vd.prob = prob;
    vd.video_path = video_path;
    vd.imgpath = imgpath;
    vd.numth = numth;
    vd.startusesecond = startusesecond;
    vd.endusesecond = endusesecond;
    
   
    ////放置
    //int stop = 0;
    //string outinform = "start";
    //cv::Mat ableppt;
    //int fpscounts_fan = 1;
    //int nowfps_fan = 0;

    ////放置
    //vdret.stop = stop;
    //vdret.outinform = outinform;
    //vdret.ableppt = ableppt;
    //vdret.fpscounts_fan = fpscounts_fan;
    //vdret.nowfps_fan = nowfps_fan;
    //pvdret = &vdret;

    thread videoppt;
    videoppt = thread(video2ppt, vd, outinf_ptr);
    //thread videoppt(video2ppt(vd));
    if(videoppt.joinable())
    videoppt.detach();
}