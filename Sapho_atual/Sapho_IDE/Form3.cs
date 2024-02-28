using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Sapho_IDE
{
    public partial class Form_NP : Form
    {

        static Form_NP newForm_NP;

        public Form_NP()
        {
            InitializeComponent();
        }

        public static void create_project()
        {
            newForm_NP = new Form_NP();

            newForm_NP.ShowDialog();
        }

        private void bt_createP_Click(object sender, EventArgs e)
        {

            String direct = newForm_NP.tb_directory.Text + "\\" + newForm_NP.tb_pname.Text;

            Sapho_IDE.Properties.Settings.Default.ProjectName = newForm_NP.tb_pname.Text;
            Sapho_IDE.Properties.Settings.Default.ProjectDirect = direct;


            bool exists = System.IO.Directory.Exists(direct);

            if (exists)
            {
                MessageBox.Show("This directory already exists!");
            }
            else
            {
                System.IO.Directory.CreateDirectory(direct);
                System.IO.Directory.CreateDirectory(direct + "\\Hardware");
                System.IO.Directory.CreateDirectory(direct + "\\Hardware\\Proc_IP");
                System.IO.Directory.CreateDirectory(direct + "\\Hardware\\Quartus_Files");
                System.IO.Directory.CreateDirectory(direct + "\\Software");

                copy_folder(".\\proc_ip", direct + "\\Hardware\\Proc_IP");

                newForm_NP.Dispose();

                Properties.Settings.Default.flag_create_project = true;
            }
        }

        private void bt_browsedirectory_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            tb_directory.Text = folderBrowserDialog1.SelectedPath;
        }

        private void copy_folder(string source, string destination)
        {
            string[] files = System.IO.Directory.GetFiles(source);

            // Copy the files and overwrite destination files if they already exist.
            foreach (string s in files)
            {
                // Use static Path methods to extract only the file name from the path.
                string fileName = System.IO.Path.GetFileName(s);
                string destFile = System.IO.Path.Combine(destination, fileName);
                System.IO.File.Copy(s, destFile, true);
            }
        }
    }
}
