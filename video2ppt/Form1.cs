using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using CTW_APIS_shp;

namespace video2ppt
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        const string core_labs_dir = "CORE.dll";

        [DllImport(core_labs_dir, EntryPoint = "core_startwork", CallingConvention = CallingConvention.StdCall)]//从非托管DLL中导出函数
        private static extern int core_startwork(double invfps, double prob, char[] vpd,int vpdlen, char[] imgpath,int ipdlen, int startusesecond, int endusesecond, IntPtr outinf_ptr);

        public static int readfile_tan(out string filepath)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Mp4|*.mp4";
            DialogResult dialogResult = openFileDialog.ShowDialog();
            if (dialogResult == DialogResult.OK)
            {
                filepath = openFileDialog.FileName;
                return 0;
            }
            else
            {
                filepath = "ERROR:读取错误";
                return 1;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.Description = "请选择⽂件夹";
            //folderBrowserDialog1.ShowNewFolderButton = true; 
            //folderBrowserDialog1.RootFolder = Environment.SpecialFolder.Personal;            
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {

                textBox3.Text = folderBrowserDialog1.SelectedPath;
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            string mp4path;
            readfile_tan(out mp4path);
            textBox1.Text = mp4path;
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            textBox2.Text = "";
            double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
            double prob = 0.9;//图像相似度

            string video_path = textBox1.Text;//视频路径
            string imgpath = textBox3.Text;//文件夹路径

            int starthour = Convert.ToInt32(textBox4.Text);//开始时间       
            int startmin = Convert.ToInt32(textBox5.Text);
            int startsecond = Convert.ToInt32(textBox6.Text);

            int endhour = Convert.ToInt32(textBox9.Text);
            int endmin = Convert.ToInt32(textBox8.Text);//结束时间 
            int endsecond = Convert.ToInt32(textBox7.Text);

            int startusesecond = (starthour * 60 + startmin) * 60 + startsecond;
            int endusesecond = (endhour * 60 + endmin) * 60 + endsecond;




            char[] outinf = new char[50];
            for (int i = 0; i < 20; i++)
            {
                outinf[i] = '0';
            }

            //int l=outinf.GetLength(0);
            IntPtr outinf_ptr = Marshal.AllocHGlobal(50);
            Marshal.Copy(outinf, 0, outinf_ptr, 50);

            //double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
            //double prob = 0.9;//图像相似度

            //string video_path = "D:\\桌面\\1\\f100040.mp4";//视频路径
            //string imgpath = "D:\\桌面\\2";//文件夹路径

            //int startusesecond = (0 * 60 + 25) * 60 + 35;
            //int endusesecond = (2 * 60 + 17) * 60 + 22;


            int jindu = 0;

            int recode = ctw_apis_model_shp.cs_startwork(invfps, prob, video_path, imgpath, startusesecond, endusesecond, outinf_ptr);

            byte[] infor = new byte[50];
            byte b;
            char c;
            //progressBar1.Value = 10;
            while (true)
            {
                Marshal.Copy(outinf_ptr, infor, 0, 2);
                b = infor[0];
                //Console.Write((int)b);
                progressBar1.Value = (int)b;
                if(b>=99)
                {
                    textBox2.Text = "处理完成，请打开文件夹查看！";
                    break;
                }
                Thread.Sleep(2000);
            }

        }

        private int startfun()
        {
            byte[] outinf = new byte[20];
            for (int i = 0; i < 20; i++)
            {
                outinf[i] = 0;
            }
            IntPtr outinf_ptr = Marshal.AllocHGlobal(20);
            Marshal.Copy(outinf, 0, outinf_ptr, 20);

            if (check() == 0) return 1;

            double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
            double prob = 0.9;//图像相似度

            string video_path = textBox1.Text;//视频路径
            string imgpath = textBox3.Text;//文件夹路径

            int starthour = Convert.ToInt32(textBox4.Text);//开始时间       
            int startmin = Convert.ToInt32(textBox5.Text);
            int startsecond = Convert.ToInt32(textBox6.Text);

            int endhour = Convert.ToInt32(textBox9.Text);
            int endmin = Convert.ToInt32(textBox8.Text);//结束时间 
            int endsecond = Convert.ToInt32(textBox7.Text);

            int startusesecond = (starthour * 60 + startmin) * 60 + startsecond;
            int endusesecond = (endhour * 60 + endmin) * 60 + endsecond;
            int jindu=0;


            int vpathlen = video_path.Length;
            int vpathlenadd = 0;
            for (int i = 0; i < vpathlen; i++)
            {
                if ((int)video_path[i] > 256)
                {
                    vpathlenadd += 1;
                }
            }

            int imgpathlen = imgpath.Length;
            int impathlenadd = 0;
            for (int i = 0; i < imgpathlen; i++)
            {
                if ((int)imgpath[i] > 256)
                {
                    impathlenadd += 1;
                }
            }

            vpathlen += vpathlenadd;
            imgpathlen += impathlenadd;
            char[] vpd = new char[vpathlen];
            char[] ipd = new char[imgpathlen];

            vpd = video_path.ToCharArray();
            ipd = imgpath.ToCharArray();


            startwork(invfps, prob, vpd, vpathlen, ipd, imgpathlen, startusesecond, endusesecond, outinf_ptr);

            while (true)
            {
                
                if (jindu!= outinf[0])
                {
                    jindu = outinf[0];
                    progressBar1.Value = jindu;
                } 
                if(outinf[1] == 1)
                {
                    break;
                }
            }
            return 1;
        }
        private int check()
        {
            string video_path = textBox1.Text;//视频路径
            string imgpath = textBox3.Text;//文件夹路径

            int starthour = Convert.ToInt32(textBox4.Text) ;//开始时间       
            int startmin = Convert.ToInt32(textBox5.Text) ;
            int startsecond = Convert.ToInt32(textBox6.Text);

            int endhour = Convert.ToInt32(textBox9.Text);
            int endmin = Convert.ToInt32(textBox8.Text);//结束时间 
            int endsecond = Convert.ToInt32(textBox7.Text); 

            int startusesecond = (starthour * 60 + startmin) * 60 + startsecond;
            int endusesecond = (endhour * 60 + endmin) * 60 + endsecond;

            if (video_path == null || imgpath == null)
            {
                MessageBox.Show("ERROR:视频或文件夹路径为空!");
                return 0;

            }
            if (startusesecond > endusesecond || endusesecond == 0)
            {
                MessageBox.Show("ERROR:分割时间段选择错误!");
                return 0;
            }

            //startwork( invfps, prob, video_path, imgpath, startusesecond, endusesecond,out outinform, out jindu);
            return 1;
        }

        private int startwork(double invfps, double prob, char[] vpd, int vpdlen, char[] ipd, int ipdlen, int startusesecond, int endusesecond,IntPtr outinf_ptr)
        {

            core_startwork(invfps, prob, vpd, vpdlen, ipd ,ipdlen, startusesecond, endusesecond, outinf_ptr);
            return 1;
        }
    }
}
