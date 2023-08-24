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
    int starthour;
    int endhour;
    int startmin;
    int endmin;
    int startsecond;
    int endsecond;
};

struct video_ret {
    int stop;
    cv::Mat ableppt;
    string outinform;
    int fpscounts_fan;
    int nowfps_fan;
};

int video2ppt(video_data vd, video_ret* vdret, int *a)
{
    //参数选择区
    double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
    double prob = 0.9;//图像相似度，数值越大筛选条件越高
    string video_path = "D:\\ApowerREC\\20220828_104110.mp4";//视频路径
    string imgpath = "D:\\ApowerREC\\20220828_104110\\";//文件夹路径
    int numth = NUM_THREADS;//线程数，谨慎小心CPU热死（^_^）
    int starthour = 0;//开始时间
    int endhour = 0;
    int startmin = 0;
    int endmin = 1;//结束时间
    int startsecond = 0;
    int endsecond = 0;
    int stop = 0;

    int eachxianread = 100;

    cv::VideoCapture cap = cv::VideoCapture(video_path);
    bool suc = cap.isOpened();

    //视频·参数
    double fps = cap.get(cv::CAP_PROP_FPS);
    int step = fps * invfps;
    int fpscount_allvideo = cap.get(cv::CAP_PROP_FRAME_COUNT);
    int startusesecond = (starthour * 60 + startmin) * 60 + startsecond;
    int endusesecond = (endhour * 60 + endmin) * 60 + endsecond;
    int startfps = startusesecond * fps;
    int endfps = endusesecond * fps;
    int fpscount = endfps - startfps;
    vdret->fpscounts_fan = fpscount;
    int imgcount = fpscount / step;
    a[0] = 100;

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
    int nowfps = 0;

    //开始第一次处理
    for (int i = 0; i < xianchengre; i++)
    {
        stop = vdret->stop;
        if (stop == 1)
        {
            vdret->outinform = "正在执行终止";
            return 1;
        }
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
                vdret->nowfps_fan = nowfps;
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
            vdret->nowfps_fan = nowfps;
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
    a[0] = 20;
    return 0;
}

void main()
{
    struct video_data vd;
    struct video_ret vdret;
    struct video_ret *pvdret;
    //参数
    double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
    double prob = 0.9;//图像相似度，数值越大筛选条件越高
    string video_path = "D:\\ApowerREC\\20220828_104110.mp4";//视频路径
    string imgpath = "D:\\ApowerREC\\20220828_104110\\";//文件夹路径
    int numth = NUM_THREADS;//线程数，谨慎小心CPU热死（^_^）
    int starthour = 0;//开始时间
    int endhour = 0;
    int startmin = 0;
    int endmin = 1;//结束时间
    int startsecond = 0;
    int endsecond = 0;

    //参数
    vd.invfps = invfps;
    vd.prob = prob;
    vd.video_path = video_path;
    vd.imgpath = imgpath;
    vd.numth = numth;
    vd.starthour = starthour;
    vd.endhour = endhour;
    vd.startmin = startmin;
    vd.endmin = endmin;
    vd.startsecond = startsecond;
    vd.endsecond = endsecond;
   
    //放置
    int stop = 0;
    string outinform = "start";
    cv::Mat ableppt;
    int fpscounts_fan = 1;
    int nowfps_fan = 0;

    //放置
    vdret.stop = stop;
    vdret.outinform = outinform;
    vdret.ableppt = ableppt;
    vdret.fpscounts_fan = fpscounts_fan;
    vdret.nowfps_fan = nowfps_fan;
    pvdret = &vdret;

    thread videoppt;

    int* a = new int[2]{ 0,0 };
    videoppt = thread(video2ppt, vd, pvdret, a);
    //thread videoppt(video2ppt(vd));
    
    if(videoppt.joinable())
    videoppt.detach();
    thread::id video_id = videoppt.get_id();
    
    while (1)
    {
        
        fpscounts_fan = pvdret->fpscounts_fan;
        nowfps_fan = pvdret->nowfps_fan;
        int jindu = nowfps_fan * 100. / fpscounts_fan;
        cout << "from func():" << jindu << endl;
        /*if (nowfps_fan >= fpscounts_fan)
        {
            break;
        }*/
       
    }
}