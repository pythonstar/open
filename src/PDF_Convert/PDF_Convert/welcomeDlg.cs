namespace PDF_Convert
{
    using PDF_Convert.Properties;
    using System;
    using System.ComponentModel;
    using System.Diagnostics;
    using System.Drawing;
    using System.Windows.Forms;

    public class welcomeDlg : Form
    {
        private Button btnBuy;
        private Button btnReg;
        private Button btnUse;
        private IContainer components = null;
        private Label label2;
        private Label label3;
        private Label label4;
        private Label label5;
        private Label label6;
        private Label labelShow;
        private LinkLabel linkLabelChat;
        private PictureBox picBoxComments;
        private PictureBox picBoxHelp;
        private PictureBox picBoxQQ;
        private ToolTip toolTipInfo;

        public welcomeDlg()
        {
            this.InitializeComponent();
        }

        private void btnBuy_Click(object sender, EventArgs e)
        {
            if (DialogResult.Abort == new buyDlg().ShowDialog())
            {
                base.Close();
            }
        }

        private void btnReg_Click(object sender, EventArgs e)
        {
            if (DialogResult.Abort == new regDlg().ShowDialog())
            {
                base.Close();
            }
        }

        private void btnUse_Click(object sender, EventArgs e)
        {
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
            this.components = new Container();
            ComponentResourceManager manager = new ComponentResourceManager(typeof(welcomeDlg));
            this.labelShow = new Label();
            this.label2 = new Label();
            this.label3 = new Label();
            this.label4 = new Label();
            this.label5 = new Label();
            this.label6 = new Label();
            this.btnReg = new Button();
            this.btnBuy = new Button();
            this.btnUse = new Button();
            this.picBoxQQ = new PictureBox();
            this.picBoxComments = new PictureBox();
            this.picBoxHelp = new PictureBox();
            this.linkLabelChat = new LinkLabel();
            this.toolTipInfo = new ToolTip(this.components);
            ((ISupportInitialize) this.picBoxQQ).BeginInit();
            ((ISupportInitialize) this.picBoxComments).BeginInit();
            ((ISupportInitialize) this.picBoxHelp).BeginInit();
            base.SuspendLayout();
            this.labelShow.AutoSize = true;
            this.labelShow.Font = new Font("微软雅黑", 14.25f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.labelShow.ForeColor = SystemColors.MenuHighlight;
            this.labelShow.Location = new Point(10, 0x15);
            this.labelShow.Name = "labelShow";
            this.labelShow.Size = new Size(0x188, 0x1a);
            this.labelShow.TabIndex = 0;
            this.labelShow.Text = "您现在使用的是迅捷PDF万能转换器的试用版";
            this.label2.AutoSize = true;
            this.label2.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label2.Location = new Point(12, 0x41);
            this.label2.Name = "label2";
            this.label2.Size = new Size(0x22b, 60);
            this.label2.TabIndex = 1;
            this.label2.Text = "您可以在试用期内无限制使用本程序所有功能，超过试用期将不能使用。\r\n\r\n正式版没有功能限制，可以终身免费使用、免费升级并拥有贴心的技术支持和售后服务。";
            this.label3.AutoSize = true;
            this.label3.Font = new Font("微软雅黑", 12f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.label3.ForeColor = SystemColors.Highlight;
            this.label3.Location = new Point(12, 0x8a);
            this.label3.Name = "label3";
            this.label3.Size = new Size(0x7a, 0x16);
            this.label3.TabIndex = 2;
            this.label3.Text = "我们的优势是：";
            this.label4.AutoSize = true;
            this.label4.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label4.Location = new Point(12, 0xae);
            this.label4.Name = "label4";
            this.label4.Size = new Size(0x14b, 20);
            this.label4.TabIndex = 3;
            this.label4.Text = "我们的软件安全、稳定、易用、成熟、完善、稳定。";
            this.label5.AutoSize = true;
            this.label5.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label5.Location = new Point(12, 210);
            this.label5.Name = "label5";
            this.label5.Size = new Size(0xf7, 20);
            this.label5.TabIndex = 4;
            this.label5.Text = "我们的客户服务完备，技术支持一流。";
            this.label6.AutoSize = true;
            this.label6.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label6.Location = new Point(12, 0xf5);
            this.label6.Name = "label6";
            this.label6.Size = new Size(0x16f, 20);
            this.label6.TabIndex = 5;
            this.label6.Text = "我们是拥有8年成长历史和上千万用户的专业软件开发商。";
            this.btnReg.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.btnReg.Location = new Point(0x10, 0x11e);
            this.btnReg.Name = "btnReg";
            this.btnReg.Size = new Size(0x58, 0x20);
            this.btnReg.TabIndex = 6;
            this.btnReg.Text = "输入注册码";
            this.btnReg.UseVisualStyleBackColor = true;
            this.btnReg.Click += new EventHandler(this.btnReg_Click);
            this.btnBuy.Font = new Font("微软雅黑", 9f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.btnBuy.Location = new Point(0x99, 0x11e);
            this.btnBuy.Name = "btnBuy";
            this.btnBuy.Size = new Size(0x115, 0x20);
            this.btnBuy.TabIndex = 7;
            this.btnBuy.Text = "购买正版";
            this.btnBuy.UseVisualStyleBackColor = true;
            this.btnBuy.Click += new EventHandler(this.btnBuy_Click);
            this.btnUse.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.btnUse.Location = new Point(0x1e3, 0x11e);
            this.btnUse.Name = "btnUse";
            this.btnUse.Size = new Size(0x4b, 0x20);
            this.btnUse.TabIndex = 0;
            this.btnUse.Text = "进入软件";
            this.btnUse.UseVisualStyleBackColor = true;
            this.btnUse.Click += new EventHandler(this.btnUse_Click);
            this.picBoxQQ.Image = Resources.qq_chat;
            this.picBoxQQ.Location = new Point(0x10, 0x14f);
            this.picBoxQQ.Name = "picBoxQQ";
            this.picBoxQQ.Size = new Size(70, 0x22);
            this.picBoxQQ.TabIndex = 9;
            this.picBoxQQ.TabStop = false;
            this.picBoxQQ.Click += new EventHandler(this.picBoxQQ_Click);
            this.picBoxQQ.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxQQ.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.picBoxComments.Image = Resources.comments;
            this.picBoxComments.Location = new Point(480, 0x14e);
            this.picBoxComments.Name = "picBoxComments";
            this.picBoxComments.Size = new Size(0x20, 0x20);
            this.picBoxComments.TabIndex = 10;
            this.picBoxComments.TabStop = false;
            this.picBoxComments.Click += new EventHandler(this.picBoxComments_Click);
            this.picBoxComments.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxComments.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.picBoxHelp.Image = Resources.help;
            this.picBoxHelp.Location = new Point(0x211, 0x14e);
            this.picBoxHelp.Name = "picBoxHelp";
            this.picBoxHelp.Size = new Size(0x20, 0x20);
            this.picBoxHelp.TabIndex = 11;
            this.picBoxHelp.TabStop = false;
            this.picBoxHelp.Click += new EventHandler(this.picBoxHelp_Click);
            this.picBoxHelp.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxHelp.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.linkLabelChat.ActiveLinkColor = Color.Blue;
            this.linkLabelChat.AutoSize = true;
            this.linkLabelChat.BackColor = Color.Transparent;
            this.linkLabelChat.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.linkLabelChat.LinkColor = Color.Red;
            this.linkLabelChat.Location = new Point(0x95, 0x151);
            this.linkLabelChat.Name = "linkLabelChat";
            this.linkLabelChat.Size = new Size(0x117, 20);
            this.linkLabelChat.TabIndex = 0x2f;
            this.linkLabelChat.TabStop = true;
            this.linkLabelChat.Text = "客服热线：4006685572 QQ：326231478";
            this.linkLabelChat.LinkClicked += new LinkLabelLinkClickedEventHandler(this.linkLabelChat_LinkClicked);
            this.toolTipInfo.AutomaticDelay = 100;
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(0x252, 0x171);
            base.Controls.Add(this.linkLabelChat);
            base.Controls.Add(this.picBoxHelp);
            base.Controls.Add(this.picBoxComments);
            base.Controls.Add(this.picBoxQQ);
            base.Controls.Add(this.btnUse);
            base.Controls.Add(this.btnBuy);
            base.Controls.Add(this.btnReg);
            base.Controls.Add(this.label6);
            base.Controls.Add(this.label5);
            base.Controls.Add(this.label4);
            base.Controls.Add(this.label3);
            base.Controls.Add(this.label2);
            base.Controls.Add(this.labelShow);
            base.FormBorderStyle = FormBorderStyle.FixedSingle;
            base.Icon = (Icon) manager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "welcomeDlg";
            base.StartPosition = FormStartPosition.CenterScreen;
            this.Text = "欢迎使用";
            base.Load += new EventHandler(this.welcomeDlg_Load);
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

        private void welcomeDlg_Load(object sender, EventArgs e)
        {
            if (Environment.OSVersion.Version.Major < 6)
            {
                this.label2.Font = new Font("宋体", 10.5f);
                this.label4.Font = new Font("宋体", 10.5f);
                this.label5.Font = new Font("宋体", 10.5f);
                this.label6.Font = new Font("宋体", 10.5f);
                this.labelShow.Font = new Font("宋体", 13.75f);
                this.label3.Font = new Font("宋体", 12f);
            }
            this.toolTipInfo.SetToolTip(this.picBoxComments, "查看用户使用评价");
            this.toolTipInfo.SetToolTip(this.picBoxHelp, "查看使用帮助");
            this.labelShow.Text = string.Format("您现在使用的是{0}的试用版", Encrypt.APPTITLE);
            this.btnUse.Focus();
        }
    }
}

