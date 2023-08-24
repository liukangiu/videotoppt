#include "model.h"
#include <opencv.hpp>
#include <iostream>
#include <string>
#include <windows.h>��
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
            std::cout << "nowimg��֡����ͼ��հ�" << std::endl;
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
            std::cout << "�����µ�֡��ʱ��" + to_string(min) + ":" + to_string(sen) << std::endl;

            //cv::imshow("showpic", nowimg);

        }

        t2 = GetTickCount();
        double dantimems = (t2 - t1) * 1.0;
        t1 = t2;
        
        std::cout << this_thread::get_id() <<"����"+to_string(same) + "ѭ��ʱ��" + to_string(dantimems) + "ms" << std::endl;
        
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
    //����ѡ����
    double invfps = 1;//��֡���,��λ�룬��ֵԽ����ʱ��Խ��
    double prob = 0.9;//ͼ�����ƶȣ���ֵԽ��ɸѡ����Խ��
    string video_path = "D:\\ApowerREC\\20220828_104110.mp4";//��Ƶ·��
    string imgpath = "D:\\ApowerREC\\20220828_104110\\";//�ļ���·��
    int numth = NUM_THREADS;//�߳���������С��CPU������^_^��
    int starthour = 0;//��ʼʱ��
    int endhour = 0;
    int startmin = 0;
    int endmin = 1;//����ʱ��
    int startsecond = 0;
    int endsecond = 0;
    int stop = 0;

    int eachxianread = 100;

    cv::VideoCapture cap = cv::VideoCapture(video_path);
    bool suc = cap.isOpened();

    //��Ƶ������
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

    //������
    if (startfps > endfps) putout("ERROR:��ֹ������ʼ");
    else if (fpscount > fpscount_allvideo) putout("ERROR:��ֹʱ�䳬����Ƶ�ܳ�");

    //�����߳�
    thread th[NUM_THREADS];

    //�������
    int xianchengre = fpscount / (numth * eachxianread * step);
    int remainfpscount = fpscount % (numth * eachxianread * step);
    int remain_startfps = startfps + fpscount - remainfpscount;

    //��������
    vector<vector<cv::Mat>> fpsdanchou_duoxian;
    vector<vector<double>> times_duoxian;
    vector<cv::Mat> fpsdanchou;
    vector<double> times;
    cv::Mat linimg;
    double time;
    int nowfps = 0;

    //��ʼ��һ�δ���
    for (int i = 0; i < xianchengre; i++)
    {
        stop = vdret->stop;
        if (stop == 1)
        {
            vdret->outinform = "����ִ����ֹ";
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

                cout <<"��" + to_string((int)nowfps/step) +"��ͼ" << endl;
   
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
   
    //��ʼ�ڶ��δ���
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

            cout << "��" + to_string((int)nowfps / step) + "��ͼ" << endl;

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

    //����
    //delete th;
    a[0] = 20;
    return 0;
}

void main()
{
    struct video_data vd;
    struct video_ret vdret;
    struct video_ret *pvdret;
    //����
    double invfps = 1;//��֡���,��λ�룬��ֵԽ����ʱ��Խ��
    double prob = 0.9;//ͼ�����ƶȣ���ֵԽ��ɸѡ����Խ��
    string video_path = "D:\\ApowerREC\\20220828_104110.mp4";//��Ƶ·��
    string imgpath = "D:\\ApowerREC\\20220828_104110\\";//�ļ���·��
    int numth = NUM_THREADS;//�߳���������С��CPU������^_^��
    int starthour = 0;//��ʼʱ��
    int endhour = 0;
    int startmin = 0;
    int endmin = 1;//����ʱ��
    int startsecond = 0;
    int endsecond = 0;

    //����
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
   
    //����
    int stop = 0;
    string outinform = "start";
    cv::Mat ableppt;
    int fpscounts_fan = 1;
    int nowfps_fan = 0;

    //����
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