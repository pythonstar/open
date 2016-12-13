namespace PDF_Convert
{
    using System;
    using System.ComponentModel;
    using System.Drawing;
    using System.Windows.Forms;

    public class passwordDlg : Form
    {
        private Button btnCancel;
        private Button btnOK;
        private IContainer components = null;
        private string file_name;
        private Label label1;
        private Label label2;
        private Label lbFileName;
        public string new_password = "";
        private TextBox txtPassword;

        public passwordDlg(string file_name)
        {
            this.file_name = file_name;
            this.InitializeComponent();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            base.DialogResult = DialogResult.Cancel;
            base.Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            this.new_password = this.txtPassword.Text;
            base.DialogResult = DialogResult.OK;
            base.Close();
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
            ComponentResourceManager manager = new ComponentResourceManager(typeof(passwordDlg));
            this.btnOK = new Button();
            this.btnCancel = new Button();
            this.label1 = new Label();
            this.txtPassword = new TextBox();
            this.label2 = new Label();
            this.lbFileName = new Label();
            base.SuspendLayout();
            this.btnOK.Location = new Point(0x77, 0x47);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new Size(0x4b, 0x17);
            this.btnOK.TabIndex = 0;
            this.btnOK.Text = "确定";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new EventHandler(this.btnOK_Click);
            this.btnCancel.Location = new Point(0xc5, 0x47);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new Size(0x4b, 0x17);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "跳过";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new EventHandler(this.btnCancel_Click);
            this.label1.AutoSize = true;
            this.label1.Location = new Point(12, 0x2e);
            this.label1.Name = "label1";
            this.label1.Size = new Size(0x35, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "PDF密码:";
            this.txtPassword.ImeMode = ImeMode.Off;
            this.txtPassword.Location = new Point(0x47, 0x2a);
            this.txtPassword.Name = "txtPassword";
            this.txtPassword.Size = new Size(0xc9, 0x15);
            this.txtPassword.TabIndex = 0;
            this.txtPassword.UseSystemPasswordChar = true;
            this.txtPassword.KeyPress += new KeyPressEventHandler(this.txtPassword_KeyPress);
            this.label2.AutoSize = true;
            this.label2.Location = new Point(12, 0x13);
            this.label2.Name = "label2";
            this.label2.Size = new Size(0x35, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "PDF文件:";
            this.lbFileName.AutoSize = true;
            this.lbFileName.Location = new Point(0x47, 0x13);
            this.lbFileName.Name = "lbFileName";
            this.lbFileName.Size = new Size(0x41, 12);
            this.lbFileName.TabIndex = 4;
            this.lbFileName.Text = "lbFileName";
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(0x11c, 0x6f);
            base.Controls.Add(this.lbFileName);
            base.Controls.Add(this.label2);
            base.Controls.Add(this.txtPassword);
            base.Controls.Add(this.label1);
            base.Controls.Add(this.btnCancel);
            base.Controls.Add(this.btnOK);
            base.Icon = (Icon) manager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "passwordDlg";
            base.StartPosition = FormStartPosition.CenterScreen;
            this.Text = "passwordDlg";
            base.Load += new EventHandler(this.passwordDlg_Load);
            base.ResumeLayout(false);
            base.PerformLayout();
        }

        private void passwordDlg_Load(object sender, EventArgs e)
        {
            this.Text = "该PDF文档是经过加密的，请输入密码";
            this.lbFileName.Text = this.file_name;
        }

        private void txtPassword_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                this.btnOK_Click(sender, e);
            }
        }
    }
}

