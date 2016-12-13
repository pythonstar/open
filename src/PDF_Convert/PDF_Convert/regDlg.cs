namespace PDF_Convert
{
    using PDF_Convert.Properties;
    using System;
    using System.ComponentModel;
    using System.Diagnostics;
    using System.Drawing;
    using System.Windows.Forms;

    public class regDlg : Form
    {
        private Button btnActive;
        private Button btnBuy;
        private IContainer components = null;
        private Label label1;
        private Label label2;
        private Label label3;
        private LinkLabel linkLabelChat;
        private PictureBox picBoxComments;
        private PictureBox picBoxHelp;
        private PictureBox picBoxQQ;
        private ToolTip toolTipInfo;
        private TextBox txtMachineCode;
        private TextBox txtRegCode;

        public regDlg()
        {
            this.InitializeComponent();
        }

        private void btnActive_Click(object sender, EventArgs e)
        {
            if (new reg().get_reg_code("") == this.txtRegCode.Text)
            {
                if (new reg().write_reg_code(this.txtRegCode.Text))
                {
                    MessageBox.Show("写入注册码成功!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                }
                else
                {
                    MessageBox.Show("写入注册码失败!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                }
            }
            else
            {
                MessageBox.Show("您输入的注册码不正确，如果您无注册码，请及时购买！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                return;
            }
            base.DialogResult = DialogResult.Abort;
            base.Close();
        }

        private void btnBuy_Click(object sender, EventArgs e)
        {
            if (new buyDlg().ShowDialog() == DialogResult.Abort)
            {
                base.DialogResult = DialogResult.Abort;
                base.Close();
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
            this.components = new Container();
            ComponentResourceManager manager = new ComponentResourceManager(typeof(regDlg));
            this.label1 = new Label();
            this.label2 = new Label();
            this.label3 = new Label();
            this.txtMachineCode = new TextBox();
            this.txtRegCode = new TextBox();
            this.btnBuy = new Button();
            this.btnActive = new Button();
            this.picBoxQQ = new PictureBox();
            this.linkLabelChat = new LinkLabel();
            this.picBoxComments = new PictureBox();
            this.picBoxHelp = new PictureBox();
            this.toolTipInfo = new ToolTip(this.components);
            ((ISupportInitialize) this.picBoxQQ).BeginInit();
            ((ISupportInitialize) this.picBoxComments).BeginInit();
            ((ISupportInitialize) this.picBoxHelp).BeginInit();
            base.SuspendLayout();
            this.label1.AutoSize = true;
            this.label1.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label1.ForeColor = SystemColors.Highlight;
            this.label1.Location = new Point(12, 0x18);
            this.label1.Name = "label1";
            this.label1.Size = new Size(0x121, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "请输入注册码后点击“激活”按钮开启正式版";
            this.label2.AutoSize = true;
            this.label2.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label2.Location = new Point(13, 0x45);
            this.label2.Name = "label2";
            this.label2.Size = new Size(0x38, 0x11);
            this.label2.TabIndex = 1;
            this.label2.Text = "机器码：";
            this.label3.AutoSize = true;
            this.label3.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label3.Location = new Point(13, 0x6a);
            this.label3.Name = "label3";
            this.label3.Size = new Size(0x38, 0x11);
            this.label3.TabIndex = 2;
            this.label3.Text = "注册码：";
            this.txtMachineCode.Location = new Point(0x4b, 0x44);
            this.txtMachineCode.Name = "txtMachineCode";
            this.txtMachineCode.ReadOnly = true;
            this.txtMachineCode.Size = new Size(0xe2, 0x15);
            this.txtMachineCode.TabIndex = 3;
            this.txtRegCode.Location = new Point(0x4b, 0x6a);
            this.txtRegCode.Multiline = true;
            this.txtRegCode.Name = "txtRegCode";
            this.txtRegCode.Size = new Size(0xe2, 0x7a);
            this.txtRegCode.TabIndex = 4;
            this.btnBuy.Location = new Point(120, 0xf5);
            this.btnBuy.Name = "btnBuy";
            this.btnBuy.Size = new Size(0x51, 0x1d);
            this.btnBuy.TabIndex = 5;
            this.btnBuy.Text = "购买正版";
            this.btnBuy.UseVisualStyleBackColor = true;
            this.btnBuy.Click += new EventHandler(this.btnBuy_Click);
            this.btnActive.Location = new Point(220, 0xf5);
            this.btnActive.Name = "btnActive";
            this.btnActive.Size = new Size(0x51, 0x1d);
            this.btnActive.TabIndex = 6;
            this.btnActive.Text = "激活";
            this.btnActive.UseVisualStyleBackColor = true;
            this.btnActive.Click += new EventHandler(this.btnActive_Click);
            this.picBoxQQ.Image = Resources.qq_chat;
            this.picBoxQQ.Location = new Point(12, 0xf5);
            this.picBoxQQ.Name = "picBoxQQ";
            this.picBoxQQ.Size = new Size(70, 0x22);
            this.picBoxQQ.TabIndex = 10;
            this.picBoxQQ.TabStop = false;
            this.picBoxQQ.Click += new EventHandler(this.picBoxQQ_Click);
            this.picBoxQQ.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxQQ.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.linkLabelChat.ActiveLinkColor = Color.Blue;
            this.linkLabelChat.AutoSize = true;
            this.linkLabelChat.BackColor = Color.Transparent;
            this.linkLabelChat.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.linkLabelChat.LinkColor = Color.Red;
            this.linkLabelChat.Location = new Point(12, 0x129);
            this.linkLabelChat.Name = "linkLabelChat";
            this.linkLabelChat.Size = new Size(0x11f, 20);
            this.linkLabelChat.TabIndex = 0x30;
            this.linkLabelChat.TabStop = true;
            this.linkLabelChat.Text = "客服热线：4006685572 QQ：4006685572";
            this.linkLabelChat.LinkClicked += new LinkLabelLinkClickedEventHandler(this.linkLabelChat_LinkClicked);
            this.picBoxComments.Image = Resources.comments;
            this.picBoxComments.Location = new Point(0x10, 0x95);
            this.picBoxComments.Name = "picBoxComments";
            this.picBoxComments.Size = new Size(0x20, 0x20);
            this.picBoxComments.TabIndex = 0x31;
            this.picBoxComments.TabStop = false;
            this.picBoxComments.Click += new EventHandler(this.picBoxComments_Click);
            this.picBoxComments.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxComments.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.picBoxHelp.Image = Resources.help;
            this.picBoxHelp.Location = new Point(0x10, 0xc4);
            this.picBoxHelp.Name = "picBoxHelp";
            this.picBoxHelp.Size = new Size(0x20, 0x20);
            this.picBoxHelp.TabIndex = 50;
            this.picBoxHelp.TabStop = false;
            this.picBoxHelp.Click += new EventHandler(this.picBoxHelp_Click);
            this.picBoxHelp.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxHelp.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.toolTipInfo.AutomaticDelay = 100;
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(0x145, 0x146);
            base.Controls.Add(this.picBoxHelp);
            base.Controls.Add(this.picBoxComments);
            base.Controls.Add(this.linkLabelChat);
            base.Controls.Add(this.picBoxQQ);
            base.Controls.Add(this.btnActive);
            base.Controls.Add(this.btnBuy);
            base.Controls.Add(this.txtRegCode);
            base.Controls.Add(this.txtMachineCode);
            base.Controls.Add(this.label3);
            base.Controls.Add(this.label2);
            base.Controls.Add(this.label1);
            base.FormBorderStyle = FormBorderStyle.FixedSingle;
            base.Icon = (Icon) manager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "regDlg";
            base.StartPosition = FormStartPosition.CenterParent;
            this.Text = "注册";
            base.Load += new EventHandler(this.regDlg_Load);
            ((ISupportInitialize) this.picBoxQQ).EndInit();
            ((ISupportInitialize) this.picBoxComments).EndInit();
            ((ISupportInitialize) this.picBoxHelp).EndInit();
            base.ResumeLayout(false);
            base.PerformLayout();
        }

        private void linkLabelChat_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/contact/qq/?version=" + PDF_Convert.Version.version);
        }

        private void picBoxComments_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/praise/?version=" + PDF_Convert.Version.version);
        }

        private void picBoxHelp_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/help/?version=" + PDF_Convert.Version.version);
        }

        private void picBoxQQ_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/contact/?version=" + PDF_Convert.Version.version);
        }

        private void picBoxQQ_MouseLeave(object sender, EventArgs e)
        {
            this.Cursor = Cursors.Arrow;
        }

        private void picBoxQQ_MouseMove(object sender, MouseEventArgs e)
        {
            this.Cursor = Cursors.Hand;
        }

        private void regDlg_Load(object sender, EventArgs e)
        {
            this.txtMachineCode.Text = new reg().get_machine_code();
            this.toolTipInfo.SetToolTip(this.picBoxComments, "查看用户使用评价");
            this.toolTipInfo.SetToolTip(this.picBoxHelp, "查看使用帮助");
        }
    }
}

