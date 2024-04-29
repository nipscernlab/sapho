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
using System.Runtime.Serialization.Formatters.Binary;
using FastColoredTextBoxNS;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace Sapho_IDE
{
    public partial class Form_Main : Form
    {

        string projdirect;
        string projectname;

        // Define static variables shared by class methods.
        private static StringBuilder sortOutput, sortError = null;
        private static int numOutputLines, numErrorLines = 0;


        //styles
        TextStyle BlueStyle = new TextStyle(Brushes.Blue, null, FontStyle.Regular);
        TextStyle MagentaStyle = new TextStyle(Brushes.Magenta, null, FontStyle.Regular);
        TextStyle BrownStyle = new TextStyle(Brushes.Brown, null, FontStyle.Italic);
        TextStyle GreenStyle = new TextStyle(Brushes.Green, null, FontStyle.Italic);
        TextStyle GreenStyleD = new TextStyle(Brushes.Green, null, FontStyle.Regular);
        TextStyle AssmblyStyleD = new TextStyle(Brushes.Black, null, FontStyle.Bold);
        MarkerStyle SameWordsStyle = new MarkerStyle(new SolidBrush(Color.FromArgb(40, Color.Gray)));

        Color currentLineColor = Color.FromArgb(100, 210, 210, 255);

        public Form_Main()
        {
            InitializeComponent();

            this.KeyPreview = true;
            this.KeyDown += new KeyEventHandler(Form_KeyDown);

            bt_AddProc.Enabled = false;
            bt_build.Enabled = false;
            bt_copy.Enabled = false;
            bt_cut.Enabled = false;
            bt_paste.Enabled = false;
            bt_save.Enabled = false;
            bt_saveall.Enabled = false;
            bt_undo.Enabled = false;
            bt_redo.Enabled = false;
            bt_assembler.Enabled = false;

            saveToolStripMenuItem.Enabled = false;
            saToolStripMenuItem.Enabled = false;
            saveAsToolStripMenuItem.Enabled = false;

            Properties.Settings.Default.flag_create_project = false;

        }

        private void helpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form5 form5 = new Form5();
            form5.ShowDialog();
        }




        private void CodeSyntaxHighlight(TextChangedEventArgs e)
        {
            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));

            fctb.LeftBracket = '(';
            fctb.RightBracket = ')';
            //clear style of changed range
            e.ChangedRange.ClearStyle(BlueStyle, MagentaStyle, GreenStyle);

            //string highlighting
            e.ChangedRange.SetStyle(BrownStyle, @"""""|@""""|''|@"".*?""|(?<!@)(?<range>"".*?[^\\]"")|'.*?[^\\]'");

            //define highlighting
            e.ChangedRange.SetStyle(GreenStyleD, @"#define|#PRNAME|#DIRNAM|#DATYPE|#NUBITS|#NBMANT|#NBEXPO|#NDSTAC|#SDEPTH|#NUIOIN|#NUIOOU|#NUGAIN", RegexOptions.Multiline);

            // Assembly Highlighting
            e.ChangedRange.SetStyle(AssmblyStyleD, @"LOAD|PLD|SET|SETP|PUSH|JZ|JMP|CALL|RETURN|SRF|IN|OUT|NEG|ADD|SADD|MLT|SMLT|DIV|SDIV|MOD|SMOD|AND|SAND|LAND|SLAND|OR|SOR|LOR|SLOR|XOR|SXOR|XORB|SXORB|INV|LINV|EQU|SEQU|NOP|GRE|SGRE|LES|SLES|SHR|SSHR|SHL|SSHL|SRS|SSRS|", RegexOptions.Multiline);
            e.ChangedRange.SetStyle(AssmblyStyleD, @"PSETP|PSET|", RegexOptions.Multiline);
            //comment highlighting
            e.ChangedRange.SetStyle(GreenStyle, @"//.*$", RegexOptions.Multiline);
            e.ChangedRange.SetStyle(GreenStyle, @"(/\*.*?\*/)|(/\*.*)", RegexOptions.Singleline);
            e.ChangedRange.SetStyle(GreenStyle, @"(/\*.*?\*/)|(.*\*/)", RegexOptions.Singleline | RegexOptions.RightToLeft);
            //number highlighting
            e.ChangedRange.SetStyle(MagentaStyle, @"\b\d+[\.]?\d*([eE]\-?\d+)?[lLdDfF]?\b|\b0x[a-fA-F\d]+\b");
            //keyword highlighting
            e.ChangedRange.SetStyle(BlueStyle, @"\b(if|@|else|while|int|float|float|void|out|return|)\b|#region\b|#endregion\b");

            //clear folding markers
            e.ChangedRange.ClearFoldingMarkers();

            //set folding markers
            e.ChangedRange.SetFoldingMarkers("{", "}");//allow to collapse brackets block
            e.ChangedRange.SetFoldingMarkers(@"#region\b", @"#endregion\b");//allow to collapse #region blocks
            e.ChangedRange.SetFoldingMarkers(@"/\*", @"\*/");//allow to collapse comment block
        }

        private void fastColoredTextBox1_TextChanged(object sender, TextChangedEventArgs e)
        {
            string fName = tabControl1.SelectedTab.Text;
            if((fName.Contains('*') == false)) { tabControl1.SelectedTab.Text = tabControl1.SelectedTab.Text + '*'; }
            CodeSyntaxHighlight(e);
        }

        private void newProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNodeCollection hier = hierarchy.Nodes;
            if (hier.Count != 0)
            {
                DialogResult dialogResult = MessageBox.Show("There is already a project. Do you want to close it and create a new one?", "", MessageBoxButtons.YesNo);

                if (dialogResult == DialogResult.Yes)
                {
                    DialogResult dialogResult2 = MessageBox.Show("Do you want to save files?", "", MessageBoxButtons.YesNo);
                    if (dialogResult == DialogResult.Yes)
                    {
                        save_all();
                    }

                    hierarchy.Nodes.Clear();
                    tabControl1.TabPages.Clear();

                    Form_NP.create_project();
                    if (Properties.Settings.Default.flag_create_project == true)
                    {
                        Properties.Settings.Default.flag_create_project = false;

                        projectname = Properties.Settings.Default.ProjectName;
                        projdirect = Properties.Settings.Default.ProjectDirect;

                        hierarchy.Nodes.Add(projectname, projectname);

                        SaveTree(hierarchy, projdirect + "\\" + projectname + ".spf");

                        bt_AddProc.Enabled = true;
                        Properties.Settings.Default.flag_create_project = false;
                    }
                }
            }
            else
            {
                Form_NP.create_project();

                if (Properties.Settings.Default.flag_create_project == true)
                {
                    Properties.Settings.Default.flag_create_project = false;

                    projectname = Properties.Settings.Default.ProjectName;
                    projdirect = Properties.Settings.Default.ProjectDirect;

                    hierarchy.Nodes.Add(projectname, projectname);

                    SaveTree(hierarchy, projdirect + "\\" + projectname + ".spf");

                    bt_AddProc.Enabled = true;
                    Properties.Settings.Default.flag_create_project = false;
                }
            }    
        }

        private void bt_AddProc_Click(object sender, EventArgs e)
        {
            Form_AddProc.configProc();

            string pName = Properties.Settings.Default.procName;

            hierarchy.Nodes[projectname].Nodes.Add(pName, pName);
            hierarchy.Nodes[projectname].Nodes[pName].Nodes.Add(pName + ".c", pName + ".c");

            SaveTree(hierarchy, projdirect + "\\" + projectname + ".spf");

            tabControl1.TabPages.Add(pName + ".c", pName + ".c");
            tabControl1.SelectedIndex = tabControl1.TabPages.Count - 1;

            FastColoredTextBox fctb = new FastColoredTextBox()
            {
                Dock = DockStyle.Fill
            };
            fctb.TextChanged += fastColoredTextBox1_TextChanged;

            fctb.AutoCompleteBrackets = true;

            fctb.LeftPadding = 17;
            fctb.CurrentLineColor = currentLineColor;
            fctb.BookmarkColor = Color.Red;

            tabControl1.SelectedTab.Controls.Add(fctb);

            Write_proc_parameters(fctb);

            File.WriteAllText(projdirect + "\\Software\\" + pName + "_S\\" + pName + ".c", fctb.Text);

            bt_assembler.Enabled = true;
            bt_AddProc.Enabled = true;
            bt_build.Enabled = true;
            bt_copy.Enabled = true;
            bt_cut.Enabled = true;
            bt_paste.Enabled = true;
            bt_save.Enabled = true;
            bt_saveall.Enabled = true;
            bt_undo.Enabled = true;
            bt_redo.Enabled = true;

            saveToolStripMenuItem.Enabled = true;
            saToolStripMenuItem.Enabled = true;
            saveAsToolStripMenuItem.Enabled = true;
        }

        private void Write_proc_parameters(FastColoredTextBox fctb)
        {
            fctb.Clear();
            fctb.GoHome();

            fctb.InsertText("#PRNAME " + Properties.Settings.Default.procName + "\n");

            string path = Directory.GetCurrentDirectory();
            string direct = Properties.Settings.Default.ProjectDirect;
            string rel_path = MakeRelativePath(path, direct);

            fctb.InsertText("#DIRNAM \"" + direct + "\\Hardware\\" + Properties.Settings.Default.procName + "_H" + "\"\n");

            fctb.InsertText("#DATYPE " + Properties.Settings.Default.dat_type + "\n");
            if (Sapho_IDE.Properties.Settings.Default.dat_type == "0")
            {
                fctb.InsertText("#NUBITS " + Properties.Settings.Default.n_bitsfix + "\n");
                fctb.InsertText("#NUGAIN " + Convert.ToString(Math.Pow(2, Convert.ToInt32(Properties.Settings.Default.Gain))) + "\n");
            }
            if (Sapho_IDE.Properties.Settings.Default.dat_type == "1")
            {
                fctb.InsertText("#NBMANT " + Properties.Settings.Default.n_bitsmant + "\n");
                fctb.InsertText("#NBEXPO " + Properties.Settings.Default.n_bitsexp + "\n");
            }

            fctb.InsertText("#NDSTAC " + Properties.Settings.Default.ndstack + "\n");
            fctb.InsertText("#SDEPTH " + Properties.Settings.Default.sdepth + "\n");

            fctb.InsertText("#NUIOIN " + Properties.Settings.Default.n_io_in + "\n");
            fctb.InsertText("#NUIOOU " + Properties.Settings.Default.n_io_out + "\n");

            

            fctb.InsertText("\nvoid main() \n{\n\n}");
        }

        public static string MakeRelativePath(string workingDirectory, string fullPath)
        {
            string result = string.Empty;
            int offset;

            // this is the easy case.  The file is inside of the working directory.
            if (fullPath.StartsWith(workingDirectory))
            {
                return fullPath.Substring(workingDirectory.Length + 1);
            }

            // the hard case has to back out of the working directory
            string[] baseDirs = workingDirectory.Split(new char[] { ':', '\\', '/' });
            string[] fileDirs = fullPath.Split(new char[] { ':', '\\', '/' });

            // if we failed to split (empty strings?) or the drive letter does not match
            if (baseDirs.Length <= 0 || fileDirs.Length <= 0 || baseDirs[0] != fileDirs[0])
            {
                // can't create a relative path between separate harddrives/partitions.
                return fullPath;
            }

            // skip all leading directories that match
            for (offset = 1; offset < baseDirs.Length; offset++)
            {
                if (baseDirs[offset] != fileDirs[offset])
                    break;
            }

            // back out of the working directory
            for (int i = 0; i < (baseDirs.Length - offset); i++)
            {
                result += "..\\";
            }

            // step into the file path
            for (int i = offset; i < fileDirs.Length - 1; i++)
            {
                result += fileDirs[offset] + "\\";
            }

            // append the file
            result += fileDirs[fileDirs.Length - 1];

            return result;
        }

        public static void SaveTree(TreeView tree, string filename)
        {
            using (Stream file = File.Open(filename, FileMode.Create))
            {
                BinaryFormatter bf = new BinaryFormatter();
                bf.Serialize(file, tree.Nodes.Cast<TreeNode>().ToList());
            }
        }

        public static void LoadTree(TreeView tree, string filename)
        {
            using (Stream file = File.Open(filename, FileMode.Open))
            {
                BinaryFormatter bf = new BinaryFormatter();
                object obj = bf.Deserialize(file);

                TreeNode[] nodeList = (obj as IEnumerable<TreeNode>).ToArray();
                tree.Nodes.AddRange(nodeList);
            }
        }

        private void openProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {

            openFileDialog1.InitialDirectory = ".";
            openFileDialog1.Filter = "Sapho Project Files (*.spf)|*.spf";
            openFileDialog1.FilterIndex = 1;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                projdirect = Path.GetDirectoryName(openFileDialog1.FileName);
                projectname = Path.GetFileNameWithoutExtension(openFileDialog1.FileName);


                Properties.Settings.Default.ProjectName = projectname;
                Properties.Settings.Default.ProjectDirect = projdirect;

                LoadTree(hierarchy, projdirect + "\\" + projectname + ".spf");
                bt_AddProc.Enabled = true;
                bt_AddProc.Enabled = true;
                bt_build.Enabled = true;
                bt_copy.Enabled = true;
                bt_cut.Enabled = true;
                bt_paste.Enabled = true;
                bt_save.Enabled = true;
                bt_saveall.Enabled = true;
                bt_undo.Enabled = true;
                bt_redo.Enabled = true;
                bt_assembler.Enabled = true;

                saveToolStripMenuItem.Enabled = true;
                saToolStripMenuItem.Enabled = true;
                saveAsToolStripMenuItem.Enabled = true;
            }
        }

        private void bt_save_Click(object sender, EventArgs e)
        {
            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));

            string fName = tabControl1.SelectedTab.Text;

            string[] pname = fName.Split('.');
            string[] ffname = fName.Split('*');

            File.WriteAllText(projdirect + "\\Software\\" + pname[0] + "_S\\" + ffname[0], fctb.Text);

            tabControl1.SelectedTab.Text = ffname[0];
        }

        private void hierarchy_DoubleClick(object sender, EventArgs e)
        {
            TreeNode Node = hierarchy.SelectedNode;

            string selectedNode = Node.Name;

            string[] pname = selectedNode.Split('.');

            int node_level = hierarchy.SelectedNode.Level;
            if(hierarchy.SelectedNode.Level == 2)
            {
                if ((tab_exists(tabControl1, selectedNode) == false) && (tab_exists(tabControl1, selectedNode + '*') == false))
                {
                    tabControl1.TabPages.Add(selectedNode);
                    tabControl1.SelectedIndex = tabControl1.TabPages.Count - 1;

                    FastColoredTextBox fctb = new FastColoredTextBox()
                    {
                        Dock = DockStyle.Fill
                    };
                    fctb.TextChanged += fastColoredTextBox1_TextChanged;

                    fctb.AutoCompleteBrackets = true;

                    fctb.LeftPadding = 17;
                    fctb.CurrentLineColor = currentLineColor;
                    fctb.BookmarkColor = Color.Red;

                    tabControl1.SelectedTab.Controls.Add(fctb);

                    fctb.InsertText(File.ReadAllText(projdirect + "\\Software\\" + pname[0] + "_S\\" + selectedNode));

                    //if (selectedNode.Contains("asm"))
                    //{
                    //    fctb.ReadOnly = true;
                    //}
                }
            }
                      
        }

        private bool tab_exists(TabControl tabCtrl, string text)
        {
            foreach (TabPage tp in tabCtrl.TabPages)
            {
                if (tp.Text == text)
                {
                    return true;
                }
            }

            return false;
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string fName = tabControl1.SelectedTab.Text;

            string[] pname = fName.Split('.');

            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));

            File.WriteAllText(projdirect + "\\Software\\" + pname[0] + "_S\\" + fName, fctb.Text);

            tabControl1.TabPages.Remove(tabControl1.SelectedTab);
        }


        private void Form_KeyDown(object sender, KeyEventArgs e)
        {
            // Verifique se as teclas Ctrl, Shift e C foram pressionadas
            if (e.Control && e.Shift && e.KeyCode == Keys.C)
            {
                // Execute o método bt_build_Click
                bt_build_Click(sender, e);
            }
        }

        private void bt_build_Click(object sender, EventArgs e)
        {
            bt_save_Click(sender, e);

            console_tb.Clear();

            Process proc1 = new Process();
            

            string fName = tabControl1.SelectedTab.Text;

            string[] pname = fName.Split('.');

            AppendText(console_tb, Color.Green, "Compiling Processor " + pname[0] + "\n\n");

            //============================================================
            // C Compiler
            //============================================================

            proc1.StartInfo.FileName = "myccomp.exe";

            proc1.StartInfo.Arguments = fName + " " + pname[0] + ".asm";

            proc1.StartInfo.WorkingDirectory = projdirect + "\\Software\\" + pname[0] + "_S\\";

            proc1.StartInfo.UseShellExecute = false;
            proc1.StartInfo.CreateNoWindow = true;

            proc1.StartInfo.RedirectStandardOutput = true;
            proc1.StartInfo.RedirectStandardError = true;

            sortOutput = new StringBuilder("");
            sortError = new StringBuilder("");

            // Set our event handler to asynchronously read the sort output.
            proc1.OutputDataReceived += new DataReceivedEventHandler(SortOutputHandler);
            proc1.ErrorDataReceived += new DataReceivedEventHandler(SortErrorHandler);

            proc1.Start();

            AppendText(console_tb, Color.Green, "Running C compiler ...\n\n");

            proc1.BeginOutputReadLine();

            proc1.BeginErrorReadLine();

            proc1.WaitForExit();

            if (numOutputLines > 0)
            {
                // Warnings of C compiler
                AppendText(console_tb, Color.Blue, "Warnings:\n----------------------------------\n");
                AppendText(console_tb, Color.Blue, sortOutput.ToString() + "\n\n");

            }

            if (numErrorLines > 0)
            {
                // Errors of C compiler
                AppendText(console_tb, Color.Red, "Errors:\n----------------------------------\n");
                AppendText(console_tb, Color.Red, sortError.ToString() + "\n\n");
            }

            proc1.Close();

            numOutputLines = 0;
            numErrorLines = 0;

            if (hierarchy.Nodes[projectname].Nodes[pname[0]].Nodes.ContainsKey(pname[0] + ".asm") == false)
            {
                hierarchy.Nodes[projectname].Nodes[pname[0]].Nodes.Add(pname[0] + ".asm", pname[0] + ".asm");
            }

            SaveTree(hierarchy, projdirect + "\\" + projectname + ".spf");
        }

        private static void SortOutputHandler(object sendingProcess, DataReceivedEventArgs outLine)
        {
            // Collect the sort command output.
            if (!String.IsNullOrEmpty(outLine.Data))
            {
                numOutputLines++;

                // Add the text to the collected output.
                sortOutput.Append(Environment.NewLine + "[" + numOutputLines.ToString() + "] - " + outLine.Data);
            }
        }

        private void bt_cut_Click(object sender, EventArgs e)
        {
            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));
            fctb.Cut();
        }

        private void bt_copy_Click(object sender, EventArgs e)
        {
            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));
            fctb.Copy();
        }

        private void bt_paste_Click(object sender, EventArgs e)
        {
            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));
            fctb.Paste();
        }

        private void bt_undo_Click(object sender, EventArgs e)
        {
            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));
            fctb.Undo();
        }

        private void bt_redo_Click(object sender, EventArgs e)
        {
            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));
            fctb.Redo();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bt_undo_Click(sender, e);
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bt_redo_Click(sender, e);
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bt_copy_Click(sender, e);
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bt_cut_Click(sender, e);
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bt_paste_Click(sender, e);
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bt_save_Click(sender, e);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bool unsaved = false;
            foreach (TabPage t in tabControl1.TabPages)
            {
                string fName = t.Text;

                if(fName.Contains('*') == true)
                {
                    unsaved = true;
                }
            }

            if (unsaved == true)
            {
                DialogResult dialogResult = MessageBox.Show("There are unsaved files. Do you want to save them?", "", MessageBoxButtons.YesNo);

                if (dialogResult == DialogResult.Yes)
                {
                    save_all();
                }
            }

            Application.Exit();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string fName = tabControl1.SelectedTab.Text;

            string[] pname = fName.Split('.');
            saveFileDialog1.Filter = "c files (*.c)|*.c";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.InitialDirectory = projdirect + "\\Software\\" + pname[0] + "_S\\";
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string filename = saveFileDialog1.FileName;
                FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));
                File.WriteAllText(filename, fctb.Text);
            }
        }

        private void bt_saveall_Click(object sender, EventArgs e)
        {
            save_all();
        }

        private void save_all()
        {
            foreach (TabPage t in tabControl1.TabPages)
            {
                FastColoredTextBox fctb = ((FastColoredTextBox)(t.Controls[0]));

                string fName = t.Text;

                string[] pname = fName.Split('.');
                string[] ffname = fName.Split('*');

                File.WriteAllText(projdirect + "\\Software\\" + pname[0] + "_S\\" + ffname[0], fctb.Text);

                t.Text = pname[0] + ".c";
            }
        }

        private void commentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FastColoredTextBox fctb = ((FastColoredTextBox)(tabControl1.SelectedTab.Controls[0]));
            fctb.CommentSelected();
        }

        private void saToolStripMenuItem_Click(object sender, EventArgs e)
        {
            save_all();
        }

        private void Form_Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            bool unsaved = false;
            foreach (TabPage t in tabControl1.TabPages)
            {
                string fName = t.Text;

                if (fName.Contains('*') == true)
                {
                    unsaved = true;
                }
            }

            if (unsaved == true)
            {
                DialogResult dialogResult = MessageBox.Show("There are unsaved files. Do you want to save them?", "", MessageBoxButtons.YesNo);

                if (dialogResult == DialogResult.Yes)
                {
                    save_all();
                }
            }
            Application.Exit();
        }

        private void Bt_assembler_Click(object sender, EventArgs e)
        {

            string fName = tabControl1.SelectedTab.Text;

            string[] pname = fName.Split('.');

            Process proc2 = new Process();

            //============================================================
            // Assembler
            //============================================================

            proc2.StartInfo.FileName = "asm2mif.exe";

            proc2.StartInfo.Arguments = pname[0] + ".asm";

            proc2.StartInfo.WorkingDirectory = projdirect + "\\Software\\" + pname[0] + "_S\\";

            proc2.StartInfo.UseShellExecute = false;
            proc2.StartInfo.CreateNoWindow = true;

            proc2.StartInfo.RedirectStandardOutput = true;
            proc2.StartInfo.RedirectStandardError = true;

            sortOutput = new StringBuilder("");
            sortError = new StringBuilder("");

            // Set our event handler to asynchronously read the sort output.
            proc2.OutputDataReceived += new DataReceivedEventHandler(SortOutputHandler);
            proc2.ErrorDataReceived += new DataReceivedEventHandler(SortErrorHandler);

            proc2.Start();

            AppendText(console_tb, Color.Green, "Running Assembler ...\n\n");

            proc2.BeginOutputReadLine();

            proc2.BeginErrorReadLine();

            proc2.WaitForExit();

            if (numOutputLines > 0)
            {
                // Warnings of the Assembler
                AppendText(console_tb, Color.Blue, "Warnings:\n----------------------------------\n");
                AppendText(console_tb, Color.Blue, sortOutput.ToString() + "\n\n");
            }

            if (numErrorLines > 0)
            {
                // Errors of the Assembler
                AppendText(console_tb, Color.Red, "Errors:\n----------------------------------\n");
                AppendText(console_tb, Color.Red, sortError.ToString() + "\n\n");
            }
            else

           proc2.Close();

            if (hierarchy.Nodes[projectname].Nodes[pname[0]].Nodes.ContainsKey(pname[0] + ".asm") == false)
            {
                hierarchy.Nodes[projectname].Nodes[pname[0]].Nodes.Add(pname[0] + ".asm", pname[0] + ".asm");
            }

            SaveTree(hierarchy, projdirect + "\\" + projectname + ".spf");
        }

        private void TabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Hierarchy_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }

        private static void SortErrorHandler(object sendingProcess, DataReceivedEventArgs outLine)
        {
            // Collect the sort command output.
            if (!String.IsNullOrEmpty(outLine.Data))
            {
                numErrorLines++;

                // Add the text to the collected output.
                sortError.Append(Environment.NewLine + "[" + numOutputLines.ToString() + "] - " + outLine.Data);
            }
        }

        void AppendText(RichTextBox box, Color color, string text)
        {
            int start = box.TextLength;
            box.AppendText(text);
            int end = box.TextLength;

            // Textbox may transform chars, so (end-start) != text.Length
            box.Select(start, end - start);
            {
                box.SelectionColor = color;
                // could set box.SelectionBackColor, box.SelectionFont too.
            }
            box.SelectionLength = 0; // clear
        }
    }
}
