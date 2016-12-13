namespace PDF_Convert
{
    using PDF_Convert.Properties;
    using System;
    using System.ComponentModel;
    using System.Diagnostics;
    using System.Drawing;
    using System.Text.RegularExpressions;
    using System.Windows.Forms;

    public class buyDlg : Form
    {
        private Button btnBuy;
        private Button btnUse;
        private IContainer components = null;
        private Label label1;
        private Label label2;
        private Label label3;
        private Label label5;
        private Label label6;
        private LinkLabel linkLabelChat;
        private Panel panel1;
        private PictureBox picBoxComments;
        private PictureBox picBoxHelp;
        private PictureBox picBoxQQ;
        private RadioButton rdbtn10086;
        private RadioButton rdbtnAlipay;
        private RadioButton rdbtnBank;
        private RadioButton rdbtnCard;
        private RadioButton rdbtnTenpay;
        private RadioButton rdbtnWebBank;
        private RadioButton rdobtnEmail;
        private RadioButton rdobtnMobile;
        private ToolTip toolTipInfo;
        private TextBox txtEmail;
        private TextBox txtMobile;

        public buyDlg()
        {
            this.InitializeComponent();
        }

        private void btnBuy_Click(object sender, EventArgs e)
        {
            string fileName = "";
            string text = "";
            if (this.rdobtnEmail.Checked)
            {
                if (!Regex.IsMatch(this.txtEmail.Text, @"\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*"))
                {
                    MessageBox.Show("请输入正确的Email地址", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                    this.txtEmail.Focus();
                    return;
                }
                text = this.txtEmail.Text;
            }
            else if (this.rdobtnMobile.Checked)
            {
                if (!Regex.IsMatch(this.txtMobile.Text, @"^[1][3-8]+\d{9}"))
                {
                    MessageBox.Show("请输入有效的手机号", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                    this.txtMobile.Focus();
                    return;
                }
                text = this.txtMobile.Text;
            }
            if (this.rdbtnAlipay.Checked)
            {
                fileName = string.Format("http://www.xunjiepdf.com/software/buy/buy.php?paymode={0}&mail={1}&machine={2}", "alipay", text, new reg().get_machine_code());
            }
            else if (this.rdbtnWebBank.Checked)
            {
                fileName = string.Format("http://www.xunjiepdf.com/software/buy/buy.php?paymode={0}&mail={1}&machine={2}", "cbank", text, new reg().get_machine_code());
            }
            else if (this.rdbtnTenpay.Checked)
            {
                fileName = string.Format("http://www.xunjiepdf.com/software/buy/buy.php?paymode={0}&mail={1}&machine={2}", "tencentpay", text, new reg().get_machine_code());
            }
            else if (this.rdbtn10086.Checked)
            {
                fileName = string.Format("http://www.xunjiepdf.com/software/buy/buy.php?paymode={0}&mail={1}&machine={2}", "mobile", text, new reg().get_machine_code());
            }
            else if (this.rdbtnCard.Checked)
            {
                fileName = string.Format("http://www.xunjiepdf.com/software/buy/buy.php?paymode={0}&mail={1}&machine={2}", "card", text, new reg().get_machine_code());
            }
            else
            {
                fileName = string.Format("http://www.xunjiepdf.com/software/buy/buy.php?paymode={0}&mail={1}&machine={2}", "bank", text, new reg().get_machine_code());
            }
            Process.Start(fileName);
        }

        private void btnUse_Click(object sender, EventArgs e)
        {
            base.DialogResult = DialogResult.Abort;
            base.Close();
        }

        private void buyDlg_Load(object sender, EventArgs e)
        {
            if (Environment.OSVersion.Version.Major < 6)
            {
                this.label1.Font = new Font("宋体", 12f);
                this.label2.Font = new Font("宋体", 12f);
            }
            this.toolTipInfo.SetToolTip(this.picBoxComments, "查看用户使用评价");
            this.toolTipInfo.SetToolTip(this.picBoxHelp, "查看使用帮助");
            this.rdobtnEmail.Checked = true;
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
            ComponentResourceManager manager = new ComponentResourceManager(typeof(buyDlg));
            this.label1 = new Label();
            this.label2 = new Label();
            this.label3 = new Label();
            this.label5 = new Label();
            this.txtEmail = new TextBox();
            this.rdbtnWebBank = new RadioButton();
            this.rdbtnAlipay = new RadioButton();
            this.rdbtnTenpay = new RadioButton();
            this.rdbtn10086 = new RadioButton();
            this.rdbtnCard = new RadioButton();
            this.rdbtnBank = new RadioButton();
            this.btnBuy = new Button();
            this.btnUse = new Button();
            this.label6 = new Label();
            this.linkLabelChat = new LinkLabel();
            this.picBoxQQ = new PictureBox();
            this.picBoxHelp = new PictureBox();
            this.picBoxComments = new PictureBox();
            this.toolTipInfo = new ToolTip(this.components);
            this.txtMobile = new TextBox();
            this.panel1 = new Panel();
            this.rdobtnMobile = new RadioButton();
            this.rdobtnEmail = new RadioButton();
            ((ISupportInitialize) this.picBoxQQ).BeginInit();
            ((ISupportInitialize) this.picBoxHelp).BeginInit();
            ((ISupportInitialize) this.picBoxComments).BeginInit();
            this.panel1.SuspendLayout();
            base.SuspendLayout();
            this.label1.AutoSize = true;
            this.label1.Font = new Font("微软雅黑", 12f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.label1.ForeColor = SystemColors.Highlight;
            this.label1.Location = new Point(12, 0x15);
            this.label1.Name = "label1";
            this.label1.Size = new Size(0x10b, 0x16);
            this.label1.TabIndex = 0;
            this.label1.Text = "迅捷PDF万能转换器，请支持正版。";
            this.label2.AutoSize = true;
            this.label2.Font = new Font("微软雅黑", 12f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.label2.ForeColor = SystemColors.Highlight;
            this.label2.Location = new Point(12, 60);
            this.label2.Name = "label2";
            this.label2.Size = new Size(0x174, 0x16);
            this.label2.TabIndex = 1;
            this.label2.Text = "一次购买可在3台电脑上永久使用，终身免费升级。";
            this.label3.AutoSize = true;
            this.label3.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label3.Location = new Point(11, 100);
            this.label3.Name = "label3";
            this.label3.Size = new Size(0x158, 0x11);
            this.label3.TabIndex = 2;
            this.label3.Text = "请填写电子邮件或手机号，选择支付方式后点击开始购买按钮。";
            this.label5.AutoSize = true;
            this.label5.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.label5.Location = new Point(12, 0xef);
            this.label5.Name = "label5";
            this.label5.Size = new Size(0x164, 0x11);
            this.label5.TabIndex = 4;
            this.label5.Text = "提示：注册码由销售系统自动发送，支付成功后马上就可以收到。";
            this.txtEmail.Location = new Point(0x60, 0x80);
            this.txtEmail.Name = "txtEmail";
            this.txtEmail.Size = new Size(250, 0x15);
            this.txtEmail.TabIndex = 5;
            this.txtEmail.KeyPress += new KeyPressEventHandler(this.txtEmail_KeyPress);
            this.rdbtnWebBank.AutoSize = true;
            this.rdbtnWebBank.Checked = true;
            this.rdbtnWebBank.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.rdbtnWebBank.Location = new Point(0x4d, 0xb6);
            this.rdbtnWebBank.Name = "rdbtnWebBank";
            this.rdbtnWebBank.Size = new Size(0x4a, 0x15);
            this.rdbtnWebBank.TabIndex = 6;
            this.rdbtnWebBank.TabStop = true;
            this.rdbtnWebBank.Text = "网上银行";
            this.rdbtnWebBank.UseVisualStyleBackColor = true;
            this.rdbtnAlipay.AutoSize = true;
            this.rdbtnAlipay.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.rdbtnAlipay.Location = new Point(0xa7, 0xb6);
            this.rdbtnAlipay.Name = "rdbtnAlipay";
            this.rdbtnAlipay.Size = new Size(0x3e, 0x15);
            this.rdbtnAlipay.TabIndex = 7;
            this.rdbtnAlipay.Text = "支付宝";
            this.rdbtnAlipay.UseVisualStyleBackColor = true;
            this.rdbtnTenpay.AutoSize = true;
            this.rdbtnTenpay.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.rdbtnTenpay.Location = new Point(0x109, 0xb6);
            this.rdbtnTenpay.Name = "rdbtnTenpay";
            this.rdbtnTenpay.Size = new Size(0x3e, 0x15);
            this.rdbtnTenpay.TabIndex = 8;
            this.rdbtnTenpay.Text = "财付通";
            this.rdbtnTenpay.UseVisualStyleBackColor = true;
            this.rdbtn10086.AutoSize = true;
            this.rdbtn10086.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.rdbtn10086.Location = new Point(0x4d, 0xd1);
            this.rdbtn10086.Name = "rdbtn10086";
            this.rdbtn10086.Size = new Size(0x4a, 0x15);
            this.rdbtn10086.TabIndex = 9;
            this.rdbtn10086.Text = "移动话费";
            this.rdbtn10086.UseVisualStyleBackColor = true;
            this.rdbtnCard.AutoSize = true;
            this.rdbtnCard.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.rdbtnCard.Location = new Point(0xa7, 0xd1);
            this.rdbtnCard.Name = "rdbtnCard";
            this.rdbtnCard.Size = new Size(0x62, 0x15);
            this.rdbtnCard.TabIndex = 10;
            this.rdbtnCard.Text = "神州行充值卡";
            this.rdbtnCard.UseVisualStyleBackColor = true;
            this.rdbtnBank.AutoSize = true;
            this.rdbtnBank.Font = new Font("微软雅黑", 9f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.rdbtnBank.Location = new Point(0x109, 0xd1);
            this.rdbtnBank.Name = "rdbtnBank";
            this.rdbtnBank.Size = new Size(0x4a, 0x15);
            this.rdbtnBank.TabIndex = 11;
            this.rdbtnBank.Text = "银行电汇";
            this.rdbtnBank.UseVisualStyleBackColor = true;
            this.btnBuy.Font = new Font("宋体", 9f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.btnBuy.Location = new Point(0xa7, 0x112);
            this.btnBuy.Name = "btnBuy";
            this.btnBuy.Size = new Size(0x53, 0x1d);
            this.btnBuy.TabIndex = 12;
            this.btnBuy.Text = "开始购买";
            this.btnBuy.UseVisualStyleBackColor = true;
            this.btnBuy.Click += new EventHandler(this.btnBuy_Click);
            this.btnUse.Location = new Point(0x113, 0x112);
            this.btnUse.Name = "btnUse";
            this.btnUse.Size = new Size(0x53, 0x1d);
            this.btnUse.TabIndex = 13;
            this.btnUse.Text = "继续试用";
            this.btnUse.UseVisualStyleBackColor = true;
            this.btnUse.Click += new EventHandler(this.btnUse_Click);
            this.label6.AutoSize = true;
            this.label6.Location = new Point(12, 0xb9);
            this.label6.Name = "label6";
            this.label6.Size = new Size(0x41, 12);
            this.label6.TabIndex = 14;
            this.label6.Text = "支付方式：";
            this.linkLabelChat.ActiveLinkColor = Color.Blue;
            this.linkLabelChat.AutoSize = true;
            this.linkLabelChat.BackColor = Color.Transparent;
            this.linkLabelChat.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.linkLabelChat.LinkColor = Color.Red;
            this.linkLabelChat.Location = new Point(0x57, 0x150);
            this.linkLabelChat.Name = "linkLabelChat";
            this.linkLabelChat.Size = new Size(0x117, 20);
            this.linkLabelChat.TabIndex = 0x31;
            this.linkLabelChat.TabStop = true;
            this.linkLabelChat.Text = "客服热线：4006685572 QQ：4006685572";
            this.linkLabelChat.LinkClicked += new LinkLabelLinkClickedEventHandler(this.linkLabelChat_LinkClicked);
            this.picBoxQQ.Image = Resources.qq_chat;
            this.picBoxQQ.Location = new Point(8, 0x14e);
            this.picBoxQQ.Name = "picBoxQQ";
            this.picBoxQQ.Size = new Size(70, 0x22);
            this.picBoxQQ.TabIndex = 50;
            this.picBoxQQ.TabStop = false;
            this.picBoxQQ.Click += new EventHandler(this.picBoxQQ_Click);
            this.picBoxQQ.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxQQ.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.picBoxHelp.Image = Resources.help;
            this.picBoxHelp.Location = new Point(0x36, 0x128);
            this.picBoxHelp.Name = "picBoxHelp";
            this.picBoxHelp.Size = new Size(0x20, 0x20);
            this.picBoxHelp.TabIndex = 0x33;
            this.picBoxHelp.TabStop = false;
            this.picBoxHelp.Click += new EventHandler(this.picBoxHelp_Click);
            this.picBoxHelp.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxHelp.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.picBoxComments.Image = Resources.comments;
            this.picBoxComments.Location = new Point(0x10, 0x128);
            this.picBoxComments.Name = "picBoxComments";
            this.picBoxComments.Size = new Size(0x20, 0x20);
            this.picBoxComments.TabIndex = 0x34;
            this.picBoxComments.TabStop = false;
            this.picBoxComments.Click += new EventHandler(this.picBoxComments_Click);
            this.picBoxComments.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxComments.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.toolTipInfo.AutomaticDelay = 100;
            this.txtMobile.Enabled = false;
            this.txtMobile.Location = new Point(0x60, 0x9d);
            this.txtMobile.Name = "txtMobile";
            this.txtMobile.Size = new Size(250, 0x15);
            this.txtMobile.TabIndex = 0x37;
            this.txtMobile.KeyPress += new KeyPressEventHandler(this.txtMobile_KeyPress);
            this.panel1.Controls.Add(this.rdobtnMobile);
            this.panel1.Controls.Add(this.rdobtnEmail);
            this.panel1.Location = new Point(14, 0x80);
            this.panel1.Name = "panel1";
            this.panel1.Size = new Size(0x4a, 0x36);
            this.panel1.TabIndex = 0x38;
            this.rdobtnMobile.AutoSize = true;
            this.rdobtnMobile.Location = new Point(2, 0x20);
            this.rdobtnMobile.Name = "rdobtnMobile";
            this.rdobtnMobile.Size = new Size(0x4d, 0x10);
            this.rdobtnMobile.TabIndex = 1;
            this.rdobtnMobile.Text = "手机号码:";
            this.rdobtnMobile.UseVisualStyleBackColor = true;
            this.rdobtnMobile.CheckedChanged += new EventHandler(this.rdobtnMobile_CheckedChanged);
            this.rdobtnEmail.AutoSize = true;
            this.rdobtnEmail.Checked = true;
            this.rdobtnEmail.Location = new Point(2, 3);
            this.rdobtnEmail.Name = "rdobtnEmail";
            this.rdobtnEmail.Size = new Size(0x4d, 0x10);
            this.rdobtnEmail.TabIndex = 0;
            this.rdobtnEmail.TabStop = true;
            this.rdobtnEmail.Text = "电子邮件:";
            this.rdobtnEmail.UseVisualStyleBackColor = true;
            this.rdobtnEmail.CheckedChanged += new EventHandler(this.rdobtnEmail_CheckedChanged);
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(0x17a, 0x16d);
            base.Controls.Add(this.txtMobile);
            base.Controls.Add(this.picBoxComments);
            base.Controls.Add(this.picBoxHelp);
            base.Controls.Add(this.picBoxQQ);
            base.Controls.Add(this.linkLabelChat);
            base.Controls.Add(this.label6);
            base.Controls.Add(this.btnUse);
            base.Controls.Add(this.btnBuy);
            base.Controls.Add(this.rdbtnBank);
            base.Controls.Add(this.rdbtnCard);
            base.Controls.Add(this.rdbtn10086);
            base.Controls.Add(this.rdbtnTenpay);
            base.Controls.Add(this.rdbtnAlipay);
            base.Controls.Add(this.rdbtnWebBank);
            base.Controls.Add(this.txtEmail);
            base.Controls.Add(this.label5);
            base.Controls.Add(this.label3);
            base.Controls.Add(this.label2);
            base.Controls.Add(this.label1);
            base.Controls.Add(this.panel1);
            base.FormBorderStyle = FormBorderStyle.FixedSingle;
            base.Icon = (Icon) manager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "buyDlg";
            base.StartPosition = FormStartPosition.CenterParent;
            this.Text = "购买";
            base.Load += new EventHandler(this.buyDlg_Load);
            ((ISupportInitialize) this.picBoxQQ).EndInit();
            ((ISupportInitialize) this.picBoxHelp).EndInit();
            ((ISupportInitialize) this.picBoxComments).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
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

        private void rdobtnEmail_CheckedChanged(object sender, EventArgs e)
        {
            this.txtMobile.Enabled = false;
            this.txtEmail.Enabled = true;
            this.txtEmail.Focus();
        }

        private void rdobtnMobile_CheckedChanged(object sender, EventArgs e)
        {
            this.txtMobile.Enabled = true;
            this.txtEmail.Enabled = false;
            this.txtMobile.Focus();
        }

        private void txtEmail_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                this.btnBuy_Click(sender, e);
            }
        }

        private void txtMobile_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                this.btnBuy_Click(sender, e);
            }
        }
    }
}

