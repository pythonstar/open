namespace PDF_Convert
{
    using System;
    using System.ComponentModel;
    using System.Drawing;
    using System.Runtime.CompilerServices;
    using System.Windows.Forms;

    public class configDlg : Form
    {
        private CheckBox cbIsMerger;
        private PDF_Convert.Convert.config cfg;
        private IContainer components = null;
        private edit_cfg edt;
        private string formtype = "";
        private string isMerger = "";
        private Label label1;
        private Label label2;
        private Label label6;
        private Label label7;
        private Label label8;
        private int new_type;
        private int old_type;
        private RadioButton rdobtnC;
        private RadioButton rdobtnR;
        private TabControl tabControl1;
        private TabPage tabPage1;
        private TabPage tabPage2;
        private TabPage tabPage3;
        private TextBox txtHeight;
        private TextBox txtPassword;
        private TextBox txtWidth;

        public configDlg(ref PDF_Convert.Convert.config cfg, edit_cfg edt)
        {
            this.cfg = cfg;
            this.edt = edt;
            this.InitializeComponent();
        }

        private void configDlg_FormClosing(object sender, FormClosingEventArgs e)
        {
            foreach (Control control in this.tabPage2.Controls)
            {
                if (control.GetType() == this.txtPassword.GetType())
                {
                    if ((control == this.txtWidth) && (control.Text == string.Empty))
                    {
                        control.Text = "700";
                    }
                    if ((control == this.txtHeight) && (control.Text == string.Empty))
                    {
                        control.Text = "500";
                    }
                }
            }
            this.cfg.password = this.txtPassword.Text;
            this.cfg.pic_width = System.Convert.ToInt32(this.txtWidth.Text);
            this.cfg.pic_height = System.Convert.ToInt32(this.txtHeight.Text);
            this.cfg.isMerger = this.cbIsMerger.Checked ? 1 : 0;
            this.edt(this.cfg);
            ini_config _config = new ini_config("config.ini");
            _config.write_ini("isMerger", this.cfg.isMerger.ToString(), "App");
            _config.write_ini("pwd", this.cfg.password, "App");
            if (this.old_type != this.new_type)
            {
                new ini_config("config.ini").write_ini("formtype", this.new_type.ToString(), "App");
                MessageBox.Show("界面修改将在下次软件重启后生效", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            }
        }

        private void configDlg_Load(object sender, EventArgs e)
        {
            this.txtPassword.Text = new ini_config("config.ini").read_ini("pwd", "App");
            this.txtWidth.Text = this.cfg.pic_width.ToString();
            this.txtHeight.Text = this.cfg.pic_height.ToString();
            this.isMerger = new ini_config("config.ini").read_ini("isMerger", "App");
            this.cbIsMerger.Checked = this.isMerger == "1";
            this.formtype = new ini_config("config.ini").read_ini("formtype", "App");
            if (((this.formtype == string.Empty) || (this.formtype == "0")) || (this.formtype != "1"))
            {
                this.rdobtnR.Checked = true;
                this.old_type = 0;
            }
            else
            {
                this.rdobtnC.Checked = true;
                this.old_type = 1;
            }
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing && (this.components != null))
            {
                this.components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            ComponentResourceManager manager = new ComponentResourceManager(typeof(configDlg));
            this.tabControl1 = new TabControl();
            this.tabPage1 = new TabPage();
            this.label2 = new Label();
            this.cbIsMerger = new CheckBox();
            this.txtPassword = new TextBox();
            this.label1 = new Label();
            this.tabPage2 = new TabPage();
            this.txtHeight = new TextBox();
            this.txtWidth = new TextBox();
            this.label8 = new Label();
            this.label7 = new Label();
            this.label6 = new Label();
            this.tabPage3 = new TabPage();
            this.rdobtnC = new RadioButton();
            this.rdobtnR = new RadioButton();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            base.SuspendLayout();
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new Size(0x11c, 0xd9);
            this.tabControl1.TabIndex = 0;
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.cbIsMerger);
            this.tabPage1.Controls.Add(this.txtPassword);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Location = new Point(4, 0x16);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new Padding(3);
            this.tabPage1.Size = new Size(0x114, 0xbf);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "通常";
            this.tabPage1.UseVisualStyleBackColor = true;
            this.label2.AutoSize = true;
            this.label2.Location = new Point(11, 0x62);
            this.label2.Name = "label2";
            this.label2.Size = new Size(0x41, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "图片设置：";
            this.cbIsMerger.AutoSize = true;
            this.cbIsMerger.Location = new Point(0x52, 0x61);
            this.cbIsMerger.Name = "cbIsMerger";
            this.cbIsMerger.Size = new Size(0xba, 0x10);
            this.cbIsMerger.TabIndex = 3;
            this.cbIsMerger.Text = "将所有图片合并成一个PDF文档";
            this.cbIsMerger.UseVisualStyleBackColor = true;
            this.txtPassword.ImeMode = ImeMode.Off;
            this.txtPassword.Location = new Point(0x52, 0x2a);
            this.txtPassword.Name = "txtPassword";
            this.txtPassword.Size = new Size(0xa6, 0x15);
            this.txtPassword.TabIndex = 1;
            this.txtPassword.UseSystemPasswordChar = true;
            this.label1.AutoSize = true;
            this.label1.Location = new Point(11, 0x2d);
            this.label1.Name = "label1";
            this.label1.Size = new Size(0x41, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "PDF 密码：";
            this.tabPage2.Controls.Add(this.txtHeight);
            this.tabPage2.Controls.Add(this.txtWidth);
            this.tabPage2.Controls.Add(this.label8);
            this.tabPage2.Controls.Add(this.label7);
            this.tabPage2.Controls.Add(this.label6);
            this.tabPage2.Location = new Point(4, 0x16);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new Padding(3);
            this.tabPage2.Size = new Size(0x114, 0xbf);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "幻灯片";
            this.tabPage2.UseVisualStyleBackColor = true;
            this.txtHeight.Location = new Point(0x40, 0x61);
            this.txtHeight.Name = "txtHeight";
            this.txtHeight.Size = new Size(0x48, 0x15);
            this.txtHeight.TabIndex = 4;
            this.txtHeight.Text = "500";
            this.txtHeight.KeyPress += new KeyPressEventHandler(this.txt_KeyPress);
            this.txtWidth.Location = new Point(0x40, 0x43);
            this.txtWidth.Name = "txtWidth";
            this.txtWidth.Size = new Size(0x48, 0x15);
            this.txtWidth.TabIndex = 3;
            this.txtWidth.Text = "700";
            this.txtWidth.KeyPress += new KeyPressEventHandler(this.txt_KeyPress);
            this.label8.AutoSize = true;
            this.label8.Location = new Point(0x20, 0x67);
            this.label8.Name = "label8";
            this.label8.Size = new Size(0x1d, 12);
            this.label8.TabIndex = 2;
            this.label8.Text = "长：";
            this.label7.AutoSize = true;
            this.label7.Location = new Point(0x20, 70);
            this.label7.Name = "label7";
            this.label7.Size = new Size(0x1d, 12);
            this.label7.TabIndex = 1;
            this.label7.Text = "宽：";
            this.label6.AutoSize = true;
            this.label6.Location = new Point(30, 0x1c);
            this.label6.Name = "label6";
            this.label6.Size = new Size(0x71, 12);
            this.label6.TabIndex = 0;
            this.label6.Text = "PDF转到PPT图片大小";
            this.tabPage3.Controls.Add(this.rdobtnC);
            this.tabPage3.Controls.Add(this.rdobtnR);
            this.tabPage3.Location = new Point(4, 0x16);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new Size(0x114, 0xbf);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "界面";
            this.tabPage3.UseVisualStyleBackColor = true;
            this.rdobtnC.AutoSize = true;
            this.rdobtnC.Location = new Point(0x45, 0x5f);
            this.rdobtnC.Name = "rdobtnC";
            this.rdobtnC.Size = new Size(0x35, 0x10);
            this.rdobtnC.TabIndex = 1;
            this.rdobtnC.TabStop = true;
            this.rdobtnC.Text = "界面2";
            this.rdobtnC.UseVisualStyleBackColor = true;
            this.rdobtnC.CheckedChanged += new EventHandler(this.rdobtnC_CheckedChanged);
            this.rdobtnR.AutoSize = true;
            this.rdobtnR.Location = new Point(0x45, 0x34);
            this.rdobtnR.Name = "rdobtnR";
            this.rdobtnR.Size = new Size(0x35, 0x10);
            this.rdobtnR.TabIndex = 0;
            this.rdobtnR.TabStop = true;
            this.rdobtnR.Text = "界面1";
            this.rdobtnR.UseVisualStyleBackColor = true;
            this.rdobtnR.CheckedChanged += new EventHandler(this.rdobtnR_CheckedChanged);
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(0x11c, 0xd7);
            base.Controls.Add(this.tabControl1);
            base.FormBorderStyle = FormBorderStyle.FixedSingle;
            base.Icon = (Icon) manager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "configDlg";
            base.StartPosition = FormStartPosition.CenterParent;
            this.Text = "设置";
            base.FormClosing += new FormClosingEventHandler(this.configDlg_FormClosing);
            base.Load += new EventHandler(this.configDlg_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            base.ResumeLayout(false);
        }

        private void rdobtnC_CheckedChanged(object sender, EventArgs e)
        {
            this.new_type = 1;
        }

        private void rdobtnR_CheckedChanged(object sender, EventArgs e)
        {
            this.new_type = 0;
        }

        private void txt_KeyPress(object sender, KeyPressEventArgs e)
        {
            char keyChar = e.KeyChar;
            if (((keyChar < '0') || (keyChar > '9')) && (keyChar != '\b'))
            {
                e.Handled = true;
            }
        }

        public delegate void edit_cfg(PDF_Convert.Convert.config cfg);
    }
}

