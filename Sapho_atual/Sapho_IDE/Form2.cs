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
    public partial class Form_AddProc : Form
    {
        static Form_AddProc newForm_AddProc;

        public Form_AddProc()
        {
            InitializeComponent();
        }

        public static void configProc()
        {
            newForm_AddProc = new Form_AddProc();

            newForm_AddProc.ShowDialog();
        }

        private void bt_generate_Click(object sender, EventArgs e)
        {
            Sapho_IDE.Properties.Settings.Default.procName = tb_procname.Text;

            Sapho_IDE.Properties.Settings.Default.n_bitsfix = tb_nubits.Text;
            Sapho_IDE.Properties.Settings.Default.n_bitsmant = tb_nbitsmant.Text;
            Sapho_IDE.Properties.Settings.Default.n_bitsexp = tb_nbitsexp.Text;

            Sapho_IDE.Properties.Settings.Default.ndstack = tb_datastack.Text;
            Sapho_IDE.Properties.Settings.Default.sdepth = tb_instrstack.Text;
            Sapho_IDE.Properties.Settings.Default.n_io_in = tb_nioin.Text;
            Sapho_IDE.Properties.Settings.Default.n_io_out = tb_nioout.Text;

            Sapho_IDE.Properties.Settings.Default.Gain = tb_gain.Text;

            string direct = Sapho_IDE.Properties.Settings.Default.ProjectDirect;
            string pName = Sapho_IDE.Properties.Settings.Default.procName;


            bool exists = System.IO.Directory.Exists(direct + "\\Hardware\\" + pName + "_H");

            if (exists)
            {
                MessageBox.Show("There is already a processor with this name!");
            }
            else
            {
                System.IO.Directory.CreateDirectory(direct + "\\Hardware\\" + pName + "_H");
                System.IO.Directory.CreateDirectory(direct + "\\Software\\" + pName + "_S");

                if (Sapho_IDE.Properties.Settings.Default.dat_type == "1")
                {
                    string[] lines = {  "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/ula_fl.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/stack_pointer.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/stack.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/reg_file.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/proc_fl.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/prefetch.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/pc.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/positivo_fl.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/mem_instr.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/mem_data.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/int2float.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/instr_dec_fl.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/float2int.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/float2index.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/core_fl.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/addr_dec.v\"]"};

                    System.IO.File.WriteAllLines(direct + "\\Hardware\\" + pName + "_H" + "\\" + pName + ".qip", lines);
                }
                else
                {
                    string[] lines = {  "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/ula_fx.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/stack_pointer.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/stack.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/reg_file.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/proc_fx.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/prefetch.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/pc.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/positivo_fx.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/mem_instr.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/mem_data.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/instr_dec_fx.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/core_fx.v\"]",
                                    "set_global_assignment -name VERILOG_FILE [file join $::quartus(qip_path) \"../Proc_IP/addr_dec.v\"]"};

                    System.IO.File.WriteAllLines(direct + "\\Hardware\\" + pName + "_H" + "\\" + pName + ".qip", lines);

                }

                newForm_AddProc.Dispose();
            }

            
        }

        private void rb_fix_CheckedChanged(object sender, EventArgs e)
        {
            if (rb_fix.Checked == true)
            {
                tb_nbitsmant.ReadOnly = true;
                tb_nbitsexp.ReadOnly = true;
                tb_nubits.ReadOnly = false;
                Sapho_IDE.Properties.Settings.Default.dat_type = "0";
            }
            else
            {
                tb_nbitsmant.ReadOnly = false;
                tb_nbitsexp.ReadOnly = false;
                tb_nubits.ReadOnly = true;
                Sapho_IDE.Properties.Settings.Default.dat_type = "1";
            }
        }

        private void lb_nbitfix_Click(object sender, EventArgs e)
        {

        }

        private void rb_float_CheckedChanged(object sender, EventArgs e)
        {
            if(rb_float.Checked == true)
            {
                tb_gain.Enabled = false;
            }
            else
            {
                tb_gain.Enabled = true;
            }

        }

        private void Form_AddProc_Load(object sender, EventArgs e)
        {

        }
    }
}
