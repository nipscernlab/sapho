namespace Sapho_IDE
{
    partial class Form_NP
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form_NP));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.bt_browsedirectory = new System.Windows.Forms.Button();
            this.tb_directory = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tb_pname = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.bt_createP = new System.Windows.Forms.Button();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.bt_browsedirectory);
            this.groupBox1.Controls.Add(this.tb_directory);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.tb_pname);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(435, 100);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Project Settings:";
            // 
            // bt_browsedirectory
            // 
            this.bt_browsedirectory.Font = new System.Drawing.Font("Times New Roman", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bt_browsedirectory.Location = new System.Drawing.Point(396, 64);
            this.bt_browsedirectory.Name = "bt_browsedirectory";
            this.bt_browsedirectory.Size = new System.Drawing.Size(33, 23);
            this.bt_browsedirectory.TabIndex = 3;
            this.bt_browsedirectory.Text = "...";
            this.bt_browsedirectory.UseVisualStyleBackColor = true;
            this.bt_browsedirectory.Click += new System.EventHandler(this.bt_browsedirectory_Click);
            // 
            // tb_directory
            // 
            this.tb_directory.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_directory.Location = new System.Drawing.Point(127, 64);
            this.tb_directory.Name = "tb_directory";
            this.tb_directory.Size = new System.Drawing.Size(262, 20);
            this.tb_directory.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(7, 67);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(110, 16);
            this.label2.TabIndex = 2;
            this.label2.Text = "Project Directory:";
            // 
            // tb_pname
            // 
            this.tb_pname.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_pname.Location = new System.Drawing.Point(127, 26);
            this.tb_pname.Name = "tb_pname";
            this.tb_pname.Size = new System.Drawing.Size(302, 20);
            this.tb_pname.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(7, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(93, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "Project Name:";
            // 
            // bt_createP
            // 
            this.bt_createP.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bt_createP.Location = new System.Drawing.Point(170, 129);
            this.bt_createP.Name = "bt_createP";
            this.bt_createP.Size = new System.Drawing.Size(118, 23);
            this.bt_createP.TabIndex = 4;
            this.bt_createP.Text = "Create Project";
            this.bt_createP.UseVisualStyleBackColor = true;
            this.bt_createP.Click += new System.EventHandler(this.bt_createP_Click);
            // 
            // Form_NP
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(460, 162);
            this.Controls.Add(this.bt_createP);
            this.Controls.Add(this.groupBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form_NP";
            this.Text = "New Project";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button bt_browsedirectory;
        private System.Windows.Forms.TextBox tb_directory;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tb_pname;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button bt_createP;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
    }
}