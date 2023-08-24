using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Media.Imaging; 
namespace CTW_APIS_shp
{
    public class ctw_apis_model_shp
    {
       
        const string core_labs_dir = "CORE.dll";
       

        public static int cs_startwork(double invfps, double prob, string video_path, string imgpath, int startusesecond, int endusesecond, IntPtr outinf_ptr)
        {
            
            int vpathlen = video_path.Length;
            int vpathlenadd = 0;
            for(int i=0;i<vpathlen;i++)
            {
                if((int)video_path[i]>256)
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

            //Console.Write(vpd);
            //Console.Write(ipd);
            //int recode = core_nocan_work();


            //string infor= "123wed";



            //Marshal.Copy(outinf_ptr, infor, 0, 3);

            core_startwork( invfps, prob, vpd,vpathlen, ipd, imgpathlen, startusesecond, endusesecond, outinf_ptr);

            //core_nocan_work();



            


            return 0;
        }

        
        //--------------------------------------------下面是导入C++函数---------------------------------------------------------
        //[DllImport(core_labs_dir, EntryPoint = "ctw_core_removebg", CallingConvention = CallingConvention.StdCall)]//从非托管DLL中导出函数

        //private static extern int ctw_core_removebg(byte[] psrc, out byte[] ppdst, int vaule, int propo, int type);//图像为byte格式

        [DllImport(core_labs_dir, EntryPoint = "core_startwork", CallingConvention = CallingConvention.StdCall)]//从非托管DLL中导出函数
        private static extern int core_startwork(double invfps, double prob, char[] vpd, int vpathlen, char[] ipd, int imgpathlen, int startusesecond, int endusesecond, IntPtr outinf_ptr);

        [DllImport(core_labs_dir, EntryPoint = "core_nocan_work", CallingConvention = CallingConvention.StdCall)]//从非托管DLL中导出函数
        private static extern int core_nocan_work();

    }
}
