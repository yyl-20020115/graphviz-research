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
            var txt = System.IO.File.ReadAllText("\\WorkingCurrent\\gv\\Network.gv");
            using (GraphResult g = GraphProvider.Generate(txt, "dot", "bmp"))
            {
                if (g != null)
                {
                    using (var bitmap = new Bitmap(g.Image.Width, g.Image.Height))
                    {
                        using (var gx = Graphics.FromImage(bitmap))
                        {
                            gx.DrawImage(g.Image, 0, 0);
                        }
                        bitmap.Save("\\WorkingCurrent\\gv\\test.jpg");
                    }
                }
            }
        }

        private void _MainForm_Load(object sender, EventArgs e)
        {
            button1_Click(sender, e);
        }
    }
}
