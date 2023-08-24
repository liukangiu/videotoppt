using CTW_APIS_shp;
using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using System.Windows.Media.Imaging;

using System.Windows.Controls;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

using System.Threading.Tasks;

namespace CTW_APIS_shp_cmd
{
    class CTW_APIS_shp_cmd
    {

		//public static Form1 stmaform;

		[STAThread]

		static void Main(string[] args)
        {


			Console.WriteLine("CTW_APIS_ CS test demo.=================================================");

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);


			


			char[] outinf = new char[50];
			for (int i = 0; i < 20; i++)
			{
				outinf[i] = '0';
			}

			//int l=outinf.GetLength(0);
			IntPtr outinf_ptr = Marshal.AllocHGlobal(50); 
			Marshal.Copy(outinf, 0, outinf_ptr, 50);

			double invfps = 1;//抽帧间隔,单位秒，数值越大处理时间越短
			double prob = 0.9;//图像相似度

			string video_path = "D:\\桌面\\1\\f100040.mp4";//视频路径
			string imgpath = "D:\\桌面\\2";//文件夹路径

			int startusesecond = (0 * 60 + 25) * 60 + 35;
			int endusesecond = (2 * 60 + 17) * 60 + 22;

			
			int jindu = 0;

			int recode = ctw_apis_model_shp.cs_startwork(invfps, prob, video_path, imgpath, startusesecond, endusesecond, outinf_ptr);

			byte[] infor = new byte[50];
			byte b;
			char c;
			//string inforstr = null;
			//Marshal.Copy(outinf_ptr, infor, 0, 7);

			//for (int i = 0; i < 50; i++)
			//{
			//	b = (char)infor[i];
			//	if (b == '|')
			//		break;
			//	inforstr += b;
			//}

			while (true)
            {
				
				var task_1 = Task.Run(async delegate
				{
					Marshal.Copy(outinf_ptr, infor, 0, 2);
					b = infor[0];
					Console.Write((int)b);
					await Task.Delay(30000);
					//Console.WriteLine("3秒后执行，方式一 输出语句...");
					//return "异步执行result"; //可以得到一个返回值(int,bool,string都试了)
				});

				//c = (char)infor[1];
				//if (jindu!=(int)b)
    //            {
				//	jindu = (int)b;
				//	Console.Write(jindu);

				//}
				//if (c == '1')
				//	break;

            }
		}
  
    }
}



