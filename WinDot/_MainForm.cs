using System;
using System.Drawing;
using System.Windows.Forms;

namespace WinDot
{
    public partial class _MainForm : Form
    {
        public _MainForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string txt = System.IO.File.ReadAllText("Network.gv");
            GraphResult g = GraphProvider.Generate(txt,"dot","bmp");
            
            if (g != null)
            {
                using(Bitmap nb = new Bitmap(g.Image.Width, g.Image.Height))
                {
                    using (Graphics gx = Graphics.FromImage(nb))
                    {
                        gx.DrawImage(g.Image, 0, 0);
                    }
                    nb.Save("\\WorkingCurrent\\gv\\images\\test.jpg");
                }
                g.Dispose();
            }

        }

        private void _MainForm_Load(object sender, EventArgs e)
        {
            button1_Click(sender, e);
        }
    }
}
