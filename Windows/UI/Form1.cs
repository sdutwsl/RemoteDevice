using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UI
{
    public partial class Form1 : Form
    {
        Process proc;
        private string appName = "Receiver.exe";
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ProcessStartInfo psi=new ProcessStartInfo();
            psi.FileName = appName;
            psi.WindowStyle = ProcessWindowStyle.Hidden;
            proc = Process.Start(startInfo: psi);
            this.Hide();

        }

        private void 甘ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void 文ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void 崔ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            proc.Kill();
            Application.Exit();
        }
    }
}
