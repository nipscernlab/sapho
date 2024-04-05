using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

namespace Sapho_IDE
{
    public partial class Form5 : Form
    {
        private Size originalSize; // Armazena o tamanho original da imagem

        public Form5()
        {
            InitializeComponent();
            email_icon.MouseEnter += Icon_MouseEnter;
            email_icon.MouseLeave += Icon_MouseLeave;

            github_icon.MouseEnter += Icon_MouseEnter;
            github_icon.MouseLeave += Icon_MouseLeave;

            processor_icon.MouseEnter += Icon_MouseEnter;
            processor_icon.MouseLeave += Icon_MouseLeave;

            originalSize = email_icon.Size;
        }

        private void Form5_Load(object sender, EventArgs e)
        {

        }

        private void Icon_MouseEnter(object sender, EventArgs e)
        {
            PictureBox pictureBox = (PictureBox)sender;
            int increaseAmount = 10; // Ajuste a quantidade de aumento desejada
            pictureBox.Size = new Size(originalSize.Width + increaseAmount, originalSize.Height + increaseAmount);
            pictureBox.Anchor = AnchorStyles.None; // Desativa o ancoramento para evitar reposicionamento
            pictureBox.Location = new Point(pictureBox.Location.X - increaseAmount / 2, pictureBox.Location.Y - increaseAmount / 2);
        }

        private void Icon_MouseLeave(object sender, EventArgs e)
        {
            PictureBox pictureBox = (PictureBox)sender;
            pictureBox.Size = originalSize;
            pictureBox.Anchor = AnchorStyles.Top | AnchorStyles.Left; // Restaura o ancoramento
            pictureBox.Location = new Point(pictureBox.Location.X + (originalSize.Width - pictureBox.Size.Width) / 2, pictureBox.Location.Y + (originalSize.Height - pictureBox.Size.Height) / 2);
        }

        private void github_icon_Click(object sender, EventArgs e)
        {
            Process.Start("https://github.com/nipscernlab/sapho");
        }

        private void email_icon_Click(object sender, EventArgs e)
        {
            Process.Start("mailto:contact@nipscern.com");
        }

        private void processor_icon_Click(object sender, EventArgs e)
        {
            Process.Start("https://nipscern.com");
        }

        private void website_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("https://nipscern.com");
        }

        private void email_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("mailto:contact@nipscern.com");
        }

        private void github_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("https://github.com/nipscernlab/sapho");
        }
    }
}
