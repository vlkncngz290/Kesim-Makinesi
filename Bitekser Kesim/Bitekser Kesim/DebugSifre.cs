using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Bitekser_Kesim
{
    public partial class DebugSifre : MetroFramework.Forms.MetroForm
    {
        public DebugSifre()
        {
            InitializeComponent();
        }

        private void metroButton1_Click(object sender, EventArgs e)
        {
            if (metroTextBox1.Text == "bts0606*")
            {
                Form1.debugEkrani = true;
                this.Close();
            }
            else MetroFramework.MetroMessageBox.Show(this, "Şifre Yanlış");
        }
    }
}
