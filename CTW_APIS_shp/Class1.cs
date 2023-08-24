using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;


namespace CTW_APIS_shp
{
    public class ctw_apis_model_shp
    {
        const string core_labs_dir = "./CTW_CORE.dll";
        public static Bitmap m_orgn_image;
        public static Bitmap m_comp_image;

        private static Byte[] m_orgn_bytes = null;
        private static Byte[] m_comp_bytes = null;

        private static BitmapData m_orgn_bd;
        private static BitmapData m_comp_bd;

        private static int m_orgn_bmpLen = 0;       //keep size
        private static int m_comp_bmpLen = 0;       //keep size

        private static IntPtr m_ptr;


        ////接口定义



    }
}
