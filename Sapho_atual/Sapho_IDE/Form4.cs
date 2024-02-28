using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Sapho_IDE
{
    public partial class Form4 : Form
    {
        public Form4()
        {
            InitializeComponent();
            this.BackColor = Color.LimeGreen;
            this.TransparencyKey = Color.LimeGreen;
        }

        private void Form4_Shown(object sender, EventArgs e)
        {
            timer1.Enabled = true;
            timer1.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Form_Main mainForm = new Form_Main();
            timer1.Enabled = false;
            timer1.Stop();
            this.Hide();
            mainForm.Show();
        }
    }
}
