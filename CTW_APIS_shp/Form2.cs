using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CTW_APIS_shp_cmd
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        public void getshowimg(ref Bitmap inbmp)
        {
            if (inbmp != null )
            {
                if (pictureBox1.Image != null)
                    pictureBox1.Image.Dispose();//非空时，释放掉原有的图像内存，否则软件内存增涨
                this.pictureBox1.Image = inbmp.Clone() as Bitmap;
            }
        }

        public void picshow_wai(Image srcimg)
        {
            pictureBox1.Image = srcimg;
            this.Show();
        }

        public void LMap_MouseWheel(object sender, MouseEventArgs e)
        {
            int doupx = 10;
            int clen = e.Delta / 120;
            int sb = doupx * clen;
            Size nowsize = pictureBox1.Size;
            
            Point leftup_befor = pictureBox1.Location;

            Point center = new Point(leftup_befor.X + nowsize.Width / 2, leftup_befor.Y + nowsize.Height / 2);
            
            nowsize.Width += sb;
            nowsize.Height += (sb * nowsize.Height / nowsize.Width);
            pictureBox1.Size = nowsize;
            Point afterup_befor = pictureBox1.Location;
            nowsize = pictureBox1.Size;

            Point center_after = new Point(leftup_befor.X + nowsize.Width / 2, leftup_befor.Y + nowsize.Height / 2);

            afterup_befor.X += (center.X - center_after.X);
            afterup_befor.Y += (center.Y - center_after.Y);

            pictureBox1.Location = afterup_befor;



        }





        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void Form2_Load(object sender, EventArgs e)
        {

        }
    }
}
