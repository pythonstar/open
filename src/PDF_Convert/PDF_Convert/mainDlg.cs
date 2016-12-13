namespace PDF_Convert
{
    using PDF_Convert.Properties;
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Diagnostics;
    using System.Drawing;
    using System.IO;
    using System.Runtime.CompilerServices;
    using System.Threading;
    using System.Windows.Forms;

    public class mainDlg : Form
    {
        private ToolStripButton btnAddFiles;
        private Button btnChooseDic;
        private ToolStripButton btnClear;
        private ToolStripButton btnDelete;
        private ToolStripButton btnReg;
        private ToolStripButton btnSetting;
        private ToolStripButton btnStart;
        private CheckBox cbIsMerger;
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private ColumnHeader columnHeader3;
        private ColumnHeader columnHeader4;
        private ColumnHeader columnHeader5;
        private ColumnHeader columnHeader6;
        private ComboBox comboBoxPage;
        private IContainer components = null;
        private Dictionary<string, bool> dic_lst = new Dictionary<string, bool>();
        private FolderBrowserDialog folderBrowserDialog;
        private PDF_Convert.Convert.config global_cfg = new PDF_Convert.Convert.config(1, 0, "", "", 700, 500, 0);
        private ImageList imgLstType;
        private bool isReg = false;
        private Label label2;
        private Label lbProgress;
        private LinkLabel linkLabelContact;
        private int lst_select = 0;
        public ListView lstFile;
        private ListView lstType;
        private ContextMenuStrip menuList;
        private OpenFileDialog openFileDialog;
        private PictureBox picBoxContact;
        private PictureBox picBoxHelp;
        private PictureBox picBoxQQ;
        private PictureBox pictureBoxAbout;
        private int process_list_item = 0;
        private ProgressBar progressBar;
        private RadioButton rdoNewPath;
        private RadioButton rdoPath;
        private List<string> task = new List<string>();
        private List<string> task_page = new List<string>();
        private ToolStripMenuItem toolAddFiles;
        private ToolStripMenuItem toolClear;
        private ToolStripMenuItem toolDelete;
        private ToolStrip toolStrip1;
        private ToolTip toolTipInfo;
        private TextBox txtOutPath;

        public mainDlg()
        {
            Control.CheckForIllegalCrossThreadCalls = false;
            this.InitializeComponent();
            this.InitializeListView();
        }

        private void btnAddFiles_Click(object sender, EventArgs e)
        {
            this.openFileDialog.FileName = "";
            if (this.lst_select == 0)
            {
                this.openFileDialog.Filter = "Any文件(*.pdf,*.xls,*.xlsx,*.ppt,*.pptx)|*.pdf;*.xls;*.xlsx;*.ppt;*.pptx;";
            }
            else if (this.lst_select == 1)
            {
                this.openFileDialog.Filter = "Any文件(*.pdf,*.ppt,*.pptx,*.doc,*.docx)|*.pdf;*.ppt;*.pptx;*.doc;*.docx";
            }
            else if (this.lst_select == 2)
            {
                this.openFileDialog.Filter = "Any文件(*.pdf,*.xls,*.xlsx,*.doc,*.docx)|*.pdf;*.xls;*.xlsx;*.doc;*.docx";
            }
            else if (this.lst_select == 4)
            {
                this.openFileDialog.Filter = "图片文件(*.jpg,*.gif,*.bmp,*.png,*.tiff)|*.jpg;*.gif;*.bmp;*.png;*.tiff";
            }
            else if (((this.lst_select == 3) || (this.lst_select == 5)) || (this.lst_select == 6))
            {
                this.openFileDialog.Filter = "Any文件(*.pdf,*.ppt,*.pptx,*.doc,*.docx,*.xls,*.xlsx)|*.pdf;*.ppt;*.pptx;*.doc;*.docx;*.xls;*.xlsx";
            }
            else if (this.lst_select == 7)
            {
                this.openFileDialog.Filter = "Word文件(*.doc,*.docx)|*.doc;*.docx";
            }
            else if (this.lst_select == 8)
            {
                this.openFileDialog.Filter = "Excel文件(*.xls,*.xlsx)|*.xls;*.xlsx";
            }
            else if (this.lst_select == 9)
            {
                this.openFileDialog.Filter = "PowerPoint文件(*.ppt,*.pptx)|*.ppt;*.pptx";
            }
            bool flag = true;
            if (this.openFileDialog.ShowDialog() == DialogResult.OK)
            {
                foreach (string str in this.openFileDialog.FileNames)
                {
                    if (this.dic_lst.ContainsKey(str))
                    {
                        if (flag && (this.openFileDialog.FileNames.Length == 1))
                        {
                            flag = false;
                            MessageBox.Show("您添加的文件 " + Path.GetFileName(str) + " 已存在,我们将会自动过滤这些文件!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                        }
                        else if (flag && (this.openFileDialog.FileNames.Length != 1))
                        {
                            flag = false;
                            MessageBox.Show("您添加的部分文件已存在,我们将会自动过滤这些文件!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                        }
                    }
                    else
                    {
                        ListViewItem item = new ListViewItem(str);
                        item.SubItems.Add(this.GetLastFileName(str));
                        item.SubItems.Add(this.isReg ? "所有" : "前3页");
                        item.SubItems.Add(Math.Round((double) ((((double) new FileInfo(str).Length) / 1024.0) / 1024.0), 2).ToString() + "MB");
                        item.SubItems.Add("");
                        string text = "1";
                        try
                        {
                            text = Common.GetFilePage(str).ToString();
                        }
                        catch (Exception)
                        {
                            text = "已加密";
                        }
                        item.SubItems.Add(text);
                        this.lstFile.Items.Add(item);
                        this.dic_lst.Add(str, false);
                    }
                }
            }
        }

        private void btnChooseDic_Click(object sender, EventArgs e)
        {
            if (this.folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                this.txtOutPath.Text = this.folderBrowserDialog.SelectedPath;
                this.global_cfg.target_dic = this.txtOutPath.Text;
            }
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            this.comboBoxPage.Visible = false;
            this.lstFile.Items.Clear();
            this.dic_lst.Clear();
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in this.lstFile.SelectedItems)
            {
                this.dic_lst.Remove(item.SubItems[0].Text);
                item.Remove();
            }
        }

        private void btnReg_Click(object sender, EventArgs e)
        {
            new regDlg().ShowDialog();
            if (new reg().is_reg())
            {
                this.Text = Encrypt.APPTITLE + " 正式版 v" + PDF_Convert.Version.version;
            }
            else
            {
                this.Text = Encrypt.APPTITLE + " 免费试用版 v" + PDF_Convert.Version.version;
            }
        }

        private void btnSetting_Click(object sender, EventArgs e)
        {
            new configDlg(ref this.global_cfg, new configDlg.edit_cfg(this.edit_this_cfg)).ShowDialog();
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (this.lstFile.Items.Count > 0)
            {
                if ((this.lst_select == 4) && (this.lstFile.Items.Count > 3))
                {
                    List<ListViewItem> list = new List<ListViewItem>();
                    if (!new reg().is_reg())
                    {
                        if (MessageBox.Show("未注册用户只能上传3张图片。请问是否立即购买注册，以转换全部图片？在您的支持下，我们会做得更好。", "提示", MessageBoxButtons.YesNo, MessageBoxIcon.Asterisk) == DialogResult.Yes)
                        {
                            new buyDlg().ShowDialog();
                            if (new reg().is_reg())
                            {
                                base.Invoke(new edit_window_text(this.edit_this_window_text), new object[] { Encrypt.APPTITLE + " 正式版 v" + PDF_Convert.Version.version });
                            }
                        }
                        for (int i = 0; i <= this.lstFile.Items.Count; i++)
                        {
                            if (i > 3)
                            {
                                ListViewItem item = this.lstFile.Items[i - 1];
                                list.Add(item);
                            }
                        }
                        foreach (ListViewItem item2 in list)
                        {
                            this.lstFile.Items.Remove(item2);
                        }
                    }
                }
                bool flag = false;
                this.process_list_item = 0;
                this.global_cfg.target_dic = this.rdoNewPath.Checked ? this.txtOutPath.Text : "";
                Thread thread = new Thread(new ParameterizedThreadStart(this.work_thread));
                this.task.Clear();
                this.task_page.Clear();
                foreach (ListViewItem item3 in this.lstFile.Items)
                {
                    if (!this.is_matched(item3.SubItems[0].Text, this.lst_select))
                    {
                        if (!flag)
                        {
                            MessageBox.Show(string.Format("您选择的是{0}，但您添加的文件中含有其他类型的文件,我们将会移除相应的文件", this.lstType.Items[this.lst_select].Text), "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                            flag = true;
                        }
                        item3.Remove();
                    }
                    else
                    {
                        this.task.Add(item3.SubItems[0].Text);
                        this.task_page.Add(item3.SubItems[2].Text);
                    }
                }
                this.btnStart.Enabled = false;
                thread.IsBackground = true;
                thread.Start();
            }
        }

        private void cbIsMerger_Click(object sender, EventArgs e)
        {
            string str = this.cbIsMerger.Checked ? "1" : "0";
            new ini_config("config.ini").write_ini("isMerger", str, "App");
        }

        private void comboBoxPage_Leave(object sender, EventArgs e)
        {
            string text = this.comboBoxPage.Text;
            try
            {
                if (text != "所有")
                {
                    string[] strArray = text.Split(new char[] { '-' });
                    if (strArray.Length != 2)
                    {
                        this.lstFile.SelectedItems[0].SubItems[2].Text = "所有";
                    }
                    else if (System.Convert.ToInt32(strArray[0]) > System.Convert.ToInt32(strArray[1]))
                    {
                        MessageBox.Show("起始页应小于等于最终页,请重新输入", "错误", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                        this.lstFile.SelectedItems[0].SubItems[2].Text = "所有";
                    }
                }
            }
            catch
            {
                this.lstFile.SelectedItems[0].SubItems[2].Text = "所有";
            }
        }

        private void comboBoxPage_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.lstFile.SelectedItems[0].SubItems[2].Text = this.comboBoxPage.Text;
        }

        private void comboBoxPage_TextUpdate(object sender, EventArgs e)
        {
            if (this.comboBoxPage.Text.Split(new char[] { '-' }).Length != 2)
            {
                this.lstFile.SelectedItems[0].SubItems[2].Text = "所有";
            }
            else
            {
                this.lstFile.SelectedItems[0].SubItems[2].Text = this.comboBoxPage.Text;
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

        private void edit_this_cfg(PDF_Convert.Convert.config cfg)
        {
            this.global_cfg = cfg;
        }

        private void edit_this_window_text(string text)
        {
            this.Text = text;
        }

        private void enable_btn_start()
        {
            this.btnStart.Enabled = true;
        }

        private string get_app_path()
        {
            string fileName = Process.GetCurrentProcess().MainModule.FileName;
            return fileName.Substring(0, fileName.LastIndexOf('\\') + 1);
        }

        private PDF_Convert.Convert.FORMAT get_format()
        {
            switch (this.lst_select)
            {
                case 0:
                    return PDF_Convert.Convert.FORMAT.PDF2WORD;

                case 1:
                    return PDF_Convert.Convert.FORMAT.PDF2EXCEL;

                case 2:
                    return PDF_Convert.Convert.FORMAT.PDF2PPT;

                case 3:
                    return PDF_Convert.Convert.FORMAT.PDF2HTML;

                case 4:
                    return PDF_Convert.Convert.FORMAT.IMG2PDF;

                case 5:
                    return PDF_Convert.Convert.FORMAT.PDF2TXT;

                case 6:
                    return PDF_Convert.Convert.FORMAT.PDF2IMG;

                case 7:
                    return PDF_Convert.Convert.FORMAT.DOC2PDF;

                case 8:
                    return PDF_Convert.Convert.FORMAT.XLS2PDF;

                case 9:
                    return PDF_Convert.Convert.FORMAT.PPT2PDF;
            }
            return PDF_Convert.Convert.FORMAT.PDF2WORD;
        }

        private string GetLastFileName(string file_name)
        {
            string str = file_name;
            if (file_name.Contains(@"\"))
            {
                string[] strArray = file_name.Split(new char[] { '\\' });
                if (strArray.Length <= 2)
                {
                    return file_name;
                }
                if (strArray.Length > 4)
                {
                    int num = file_name.LastIndexOf('\\');
                    string oldValue = file_name.Substring(file_name.IndexOf('\\'), num - 1);
                    str = file_name.Replace(oldValue, @"\" + strArray[1] + @"\...\");
                }
            }
            return str;
        }

        private void InitializeComponent()
        {
            this.components = new Container();
            ComponentResourceManager manager = new ComponentResourceManager(typeof(mainDlg));
            ListViewItem item = new ListViewItem("文件 转 Word", 0);
            ListViewItem item2 = new ListViewItem("文件 转 Excel", 0);
            ListViewItem item3 = new ListViewItem("文件 转 PowerPoint", 0);
            ListViewItem item4 = new ListViewItem("文件 转 HTML", 0);
            ListViewItem item5 = new ListViewItem("图片 转 PDF", 0);
            ListViewItem item6 = new ListViewItem("文件 转 TXT", 0);
            ListViewItem item7 = new ListViewItem("文件 转 图片", 0);
            ListViewItem item8 = new ListViewItem("DOC 转 PDF", 0);
            ListViewItem item9 = new ListViewItem("Excel 转 PDF", 0);
            ListViewItem item10 = new ListViewItem("PowerPoint 转 PDF", 0);
            this.toolStrip1 = new ToolStrip();
            this.btnAddFiles = new ToolStripButton();
            this.btnDelete = new ToolStripButton();
            this.btnClear = new ToolStripButton();
            this.btnSetting = new ToolStripButton();
            this.btnStart = new ToolStripButton();
            this.btnReg = new ToolStripButton();
            this.lstType = new ListView();
            this.imgLstType = new ImageList(this.components);
            this.lstFile = new ListView();
            this.columnHeader5 = new ColumnHeader();
            this.columnHeader1 = new ColumnHeader();
            this.columnHeader4 = new ColumnHeader();
            this.columnHeader2 = new ColumnHeader();
            this.columnHeader3 = new ColumnHeader();
            this.columnHeader6 = new ColumnHeader();
            this.menuList = new ContextMenuStrip(this.components);
            this.toolAddFiles = new ToolStripMenuItem();
            this.toolDelete = new ToolStripMenuItem();
            this.toolClear = new ToolStripMenuItem();
            this.txtOutPath = new TextBox();
            this.btnChooseDic = new Button();
            this.progressBar = new ProgressBar();
            this.lbProgress = new Label();
            this.openFileDialog = new OpenFileDialog();
            this.pictureBoxAbout = new PictureBox();
            this.folderBrowserDialog = new FolderBrowserDialog();
            this.picBoxQQ = new PictureBox();
            this.linkLabelContact = new LinkLabel();
            this.picBoxHelp = new PictureBox();
            this.picBoxContact = new PictureBox();
            this.toolTipInfo = new ToolTip(this.components);
            this.comboBoxPage = new ComboBox();
            this.cbIsMerger = new CheckBox();
            this.rdoPath = new RadioButton();
            this.label2 = new Label();
            this.rdoNewPath = new RadioButton();
            this.toolStrip1.SuspendLayout();
            this.menuList.SuspendLayout();
            ((ISupportInitialize) this.pictureBoxAbout).BeginInit();
            ((ISupportInitialize) this.picBoxQQ).BeginInit();
            ((ISupportInitialize) this.picBoxHelp).BeginInit();
            ((ISupportInitialize) this.picBoxContact).BeginInit();
            base.SuspendLayout();
            this.toolStrip1.BackColor = Color.Transparent;
            this.toolStrip1.GripStyle = ToolStripGripStyle.Hidden;
            this.toolStrip1.ImageScalingSize = new Size(0x40, 0x40);
            this.toolStrip1.Items.AddRange(new ToolStripItem[] { this.btnAddFiles, this.btnDelete, this.btnClear, this.btnSetting, this.btnStart, this.btnReg });
            this.toolStrip1.Location = new Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new Size(0x2ce, 0x61);
            this.toolStrip1.TabIndex = 3;
            this.toolStrip1.Text = "toolStrip1";
            this.btnAddFiles.Image = (Image) manager.GetObject("btnAddFiles.Image");
            this.btnAddFiles.ImageTransparentColor = Color.Magenta;
            this.btnAddFiles.Margin = new Padding(20, 10, 0, 2);
            this.btnAddFiles.Name = "btnAddFiles";
            this.btnAddFiles.Size = new Size(0x44, 0x55);
            this.btnAddFiles.Text = "添加文件";
            this.btnAddFiles.TextImageRelation = TextImageRelation.ImageAboveText;
            this.btnAddFiles.Click += new EventHandler(this.btnAddFiles_Click);
            this.btnDelete.Image = Resources.delete;
            this.btnDelete.ImageTransparentColor = Color.Magenta;
            this.btnDelete.Margin = new Padding(20, 10, 0, 2);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new Size(0x48, 0x55);
            this.btnDelete.Text = "删除选择项";
            this.btnDelete.TextImageRelation = TextImageRelation.ImageAboveText;
            this.btnDelete.Click += new EventHandler(this.btnDelete_Click);
            this.btnClear.Image = Resources.clear;
            this.btnClear.ImageTransparentColor = Color.Magenta;
            this.btnClear.Margin = new Padding(20, 10, 0, 2);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new Size(0x44, 0x55);
            this.btnClear.Text = "清空列表";
            this.btnClear.TextImageRelation = TextImageRelation.ImageAboveText;
            this.btnClear.Click += new EventHandler(this.btnClear_Click);
            this.btnSetting.Image = Resources.setting;
            this.btnSetting.ImageTransparentColor = Color.Magenta;
            this.btnSetting.Margin = new Padding(20, 10, 0, 2);
            this.btnSetting.Name = "btnSetting";
            this.btnSetting.Size = new Size(0x44, 0x55);
            this.btnSetting.Text = "设置";
            this.btnSetting.TextImageRelation = TextImageRelation.ImageAboveText;
            this.btnSetting.Click += new EventHandler(this.btnSetting_Click);
            this.btnStart.Image = Resources.play;
            this.btnStart.ImageTransparentColor = Color.Magenta;
            this.btnStart.Margin = new Padding(20, 10, 0, 2);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new Size(0x44, 0x55);
            this.btnStart.Text = "开始转换";
            this.btnStart.TextImageRelation = TextImageRelation.ImageAboveText;
            this.btnStart.Click += new EventHandler(this.btnStart_Click);
            this.btnReg.Image = Resources.reg;
            this.btnReg.ImageTransparentColor = Color.Magenta;
            this.btnReg.Margin = new Padding(20, 10, 0, 2);
            this.btnReg.Name = "btnReg";
            this.btnReg.Size = new Size(0x44, 0x55);
            this.btnReg.Text = "注册";
            this.btnReg.TextImageRelation = TextImageRelation.ImageAboveText;
            this.btnReg.Click += new EventHandler(this.btnReg_Click);
            this.lstType.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            item.StateImageIndex = 0;
            this.lstType.Items.AddRange(new ListViewItem[] { item, item2, item3, item4, item5, item6, item7, item8, item9, item10 });
            this.lstType.Location = new Point(12, 100);
            this.lstType.MultiSelect = false;
            this.lstType.Name = "lstType";
            this.lstType.Size = new Size(0xad, 0x157);
            this.lstType.SmallImageList = this.imgLstType;
            this.lstType.TabIndex = 4;
            this.lstType.UseCompatibleStateImageBehavior = false;
            this.lstType.View = View.List;
            this.lstType.ItemSelectionChanged += new ListViewItemSelectionChangedEventHandler(this.lstType_ItemSelectionChanged);
            this.lstType.SelectedIndexChanged += new EventHandler(this.lstType_SelectedIndexChanged);
            this.imgLstType.ImageStream = (ImageListStreamer) manager.GetObject("imgLstType.ImageStream");
            this.imgLstType.TransparentColor = Color.Transparent;
            this.imgLstType.Images.SetKeyName(0, "item.png");
            this.imgLstType.Images.SetKeyName(1, "item_choose.png");
            this.lstFile.Columns.AddRange(new ColumnHeader[] { this.columnHeader5, this.columnHeader1, this.columnHeader4, this.columnHeader2, this.columnHeader3, this.columnHeader6 });
            this.lstFile.ContextMenuStrip = this.menuList;
            this.lstFile.FullRowSelect = true;
            this.lstFile.GridLines = true;
            this.lstFile.HideSelection = false;
            this.lstFile.Location = new Point(0xbf, 100);
            this.lstFile.Name = "lstFile";
            this.lstFile.Size = new Size(0x203, 0x157);
            this.lstFile.TabIndex = 5;
            this.lstFile.UseCompatibleStateImageBehavior = false;
            this.lstFile.View = View.Details;
            this.lstFile.ItemDrag += new ItemDragEventHandler(this.lstFile_ItemDrag);
            this.lstFile.SelectedIndexChanged += new EventHandler(this.lstFile_SelectedIndexChanged);
            this.lstFile.DragDrop += new DragEventHandler(this.lstFile_DragDrop);
            this.lstFile.DragEnter += new DragEventHandler(this.lstFile_DragEnter);
            this.lstFile.DragOver += new DragEventHandler(this.lstFile_DragOver);
            this.lstFile.DragLeave += new EventHandler(this.lstFile_DragLeave);
            this.columnHeader5.Width = 0;
            this.columnHeader1.Text = "文件路径";
            this.columnHeader1.Width = 220;
            this.columnHeader4.Text = "页数选择";
            this.columnHeader4.Width = 90;
            this.columnHeader2.Text = "文件大小";
            this.columnHeader3.Text = "转换进度";
            this.columnHeader3.Width = 80;
            this.columnHeader6.Text = "总页数";
            this.columnHeader6.Width = 50;
            this.menuList.Items.AddRange(new ToolStripItem[] { this.toolAddFiles, this.toolDelete, this.toolClear });
            this.menuList.Name = "menuList";
            this.menuList.Size = new Size(0xa6, 70);
            this.toolAddFiles.Name = "toolAddFiles";
            this.toolAddFiles.Size = new Size(0xa5, 0x16);
            this.toolAddFiles.Text = "添加文件(&A)";
            this.toolAddFiles.Click += new EventHandler(this.toolAddFiles_Click);
            this.toolDelete.Name = "toolDelete";
            this.toolDelete.Size = new Size(0xa5, 0x16);
            this.toolDelete.Text = "删除选中文件(&D)";
            this.toolDelete.Click += new EventHandler(this.toolDelete_Click);
            this.toolClear.Name = "toolClear";
            this.toolClear.Size = new Size(0xa5, 0x16);
            this.toolClear.Text = "清空列表(&C)";
            this.toolClear.Click += new EventHandler(this.toolClear_Click);
            this.txtOutPath.Location = new Point(0x77, 0x1f9);
            this.txtOutPath.Name = "txtOutPath";
            this.txtOutPath.ReadOnly = true;
            this.txtOutPath.Size = new Size(0x1ef, 0x15);
            this.txtOutPath.TabIndex = 7;
            this.btnChooseDic.Location = new Point(620, 0x1f8);
            this.btnChooseDic.Name = "btnChooseDic";
            this.btnChooseDic.Size = new Size(0x4b, 0x17);
            this.btnChooseDic.TabIndex = 8;
            this.btnChooseDic.Text = "浏览";
            this.btnChooseDic.UseVisualStyleBackColor = true;
            this.btnChooseDic.Click += new EventHandler(this.btnChooseDic_Click);
            this.progressBar.Location = new Point(12, 0x213);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new Size(0x293, 0x17);
            this.progressBar.Style = ProgressBarStyle.Continuous;
            this.progressBar.TabIndex = 9;
            this.lbProgress.AutoSize = true;
            this.lbProgress.Location = new Point(0x2a6, 0x21e);
            this.lbProgress.Name = "lbProgress";
            this.lbProgress.Size = new Size(0x11, 12);
            this.lbProgress.TabIndex = 10;
            this.lbProgress.Text = "0%";
            this.openFileDialog.FileName = "openFileDialog1";
            this.openFileDialog.Multiselect = true;
            this.pictureBoxAbout.Image = Resources.Logo;
            this.pictureBoxAbout.Location = new Point(540, 8);
            this.pictureBoxAbout.Name = "pictureBoxAbout";
            this.pictureBoxAbout.Size = new Size(0xa6, 0x52);
            this.pictureBoxAbout.TabIndex = 11;
            this.pictureBoxAbout.TabStop = false;
            this.pictureBoxAbout.Click += new EventHandler(this.pictureBoxAbout_Click);
            this.picBoxQQ.Image = Resources.qq_chat;
            this.picBoxQQ.Location = new Point(12, 560);
            this.picBoxQQ.Name = "picBoxQQ";
            this.picBoxQQ.Size = new Size(0x44, 0x1f);
            this.picBoxQQ.TabIndex = 12;
            this.picBoxQQ.TabStop = false;
            this.picBoxQQ.Click += new EventHandler(this.picBoxQQ_Click);
            this.picBoxQQ.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxQQ.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.linkLabelContact.ActiveLinkColor = Color.Blue;
            this.linkLabelContact.AutoSize = true;
            this.linkLabelContact.Font = new Font("微软雅黑", 10.5f);
            this.linkLabelContact.LinkColor = Color.Red;
            this.linkLabelContact.Location = new Point(0x85, 0x235);
            this.linkLabelContact.Name = "linkLabelContact";
            this.linkLabelContact.Size = new Size(0x11f, 20);
            this.linkLabelContact.TabIndex = 13;
            this.linkLabelContact.TabStop = true;
            this.linkLabelContact.Text = "客服热线：4006685572 QQ：4006685572";
            this.linkLabelContact.LinkClicked += new LinkLabelLinkClickedEventHandler(this.linkLabelContact_LinkClicked);
            this.picBoxHelp.Image = Resources.help;
            this.picBoxHelp.Location = new Point(0x297, 0x22f);
            this.picBoxHelp.Name = "picBoxHelp";
            this.picBoxHelp.Size = new Size(0x20, 0x20);
            this.picBoxHelp.TabIndex = 14;
            this.picBoxHelp.TabStop = false;
            this.picBoxHelp.Click += new EventHandler(this.picBoxHelp_Click);
            this.picBoxHelp.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxHelp.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.picBoxContact.Image = Resources.comments;
            this.picBoxContact.Location = new Point(0x25c, 0x22f);
            this.picBoxContact.Name = "picBoxContact";
            this.picBoxContact.Size = new Size(0x20, 0x20);
            this.picBoxContact.TabIndex = 15;
            this.picBoxContact.TabStop = false;
            this.picBoxContact.Click += new EventHandler(this.picBoxContact_Click);
            this.picBoxContact.MouseLeave += new EventHandler(this.picBoxQQ_MouseLeave);
            this.picBoxContact.MouseMove += new MouseEventHandler(this.picBoxQQ_MouseMove);
            this.toolTipInfo.AutomaticDelay = 100;
            this.comboBoxPage.FormattingEnabled = true;
            this.comboBoxPage.Items.AddRange(new object[] { "所有", "例:1-30" });
            this.comboBoxPage.Location = new Point(0x249, 0x1c6);
            this.comboBoxPage.Name = "comboBoxPage";
            this.comboBoxPage.Size = new Size(0x79, 20);
            this.comboBoxPage.TabIndex = 0x10;
            this.comboBoxPage.Visible = false;
            this.comboBoxPage.SelectedIndexChanged += new EventHandler(this.comboBoxPage_SelectedIndexChanged);
            this.comboBoxPage.TextUpdate += new EventHandler(this.comboBoxPage_TextUpdate);
            this.comboBoxPage.Leave += new EventHandler(this.comboBoxPage_Leave);
            this.cbIsMerger.AutoSize = true;
            this.cbIsMerger.BackColor = Color.Transparent;
            this.cbIsMerger.ForeColor = Color.Red;
            this.cbIsMerger.Location = new Point(520, 0x1c7);
            this.cbIsMerger.Name = "cbIsMerger";
            this.cbIsMerger.Size = new Size(0xba, 0x10);
            this.cbIsMerger.TabIndex = 0x33;
            this.cbIsMerger.Text = "将所有图片合并成一个PDF文档";
            this.cbIsMerger.UseVisualStyleBackColor = false;
            this.cbIsMerger.Visible = false;
            this.cbIsMerger.Click += new EventHandler(this.cbIsMerger_Click);
            this.rdoPath.AutoSize = true;
            this.rdoPath.BackColor = Color.Transparent;
            this.rdoPath.Checked = true;
            this.rdoPath.Font = new Font("微软雅黑", 9f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.rdoPath.ForeColor = SystemColors.ControlText;
            this.rdoPath.Location = new Point(15, 0x1dd);
            this.rdoPath.Name = "rdoPath";
            this.rdoPath.Size = new Size(0x7a, 0x15);
            this.rdoPath.TabIndex = 0x34;
            this.rdoPath.TabStop = true;
            this.rdoPath.Text = "保存在原文件夹内";
            this.rdoPath.UseVisualStyleBackColor = false;
            this.rdoPath.Click += new EventHandler(this.rdoPath_Click);
            this.label2.AutoSize = true;
            this.label2.BackColor = Color.Transparent;
            this.label2.Font = new Font("微软雅黑", 10.5f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.label2.ForeColor = Color.DimGray;
            this.label2.Location = new Point(8, 0x1c7);
            this.label2.Name = "label2";
            this.label2.Size = new Size(0x41, 0x13);
            this.label2.TabIndex = 0x35;
            this.label2.Text = "输出选项";
            this.rdoNewPath.AutoSize = true;
            this.rdoNewPath.BackColor = Color.Transparent;
            this.rdoNewPath.Font = new Font("微软雅黑", 9f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.rdoNewPath.Location = new Point(15, 0x1f8);
            this.rdoNewPath.Name = "rdoNewPath";
            this.rdoNewPath.Size = new Size(0x62, 0x15);
            this.rdoNewPath.TabIndex = 0x36;
            this.rdoNewPath.Text = "自定义文件夹";
            this.rdoNewPath.UseVisualStyleBackColor = false;
            this.rdoNewPath.Click += new EventHandler(this.rdoNewPath_Click);
            this.AllowDrop = true;
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(0x2ce, 0x252);
            base.Controls.Add(this.rdoNewPath);
            base.Controls.Add(this.label2);
            base.Controls.Add(this.rdoPath);
            base.Controls.Add(this.cbIsMerger);
            base.Controls.Add(this.comboBoxPage);
            base.Controls.Add(this.picBoxContact);
            base.Controls.Add(this.picBoxHelp);
            base.Controls.Add(this.linkLabelContact);
            base.Controls.Add(this.picBoxQQ);
            base.Controls.Add(this.pictureBoxAbout);
            base.Controls.Add(this.lbProgress);
            base.Controls.Add(this.progressBar);
            base.Controls.Add(this.btnChooseDic);
            base.Controls.Add(this.txtOutPath);
            base.Controls.Add(this.lstFile);
            base.Controls.Add(this.lstType);
            base.Controls.Add(this.toolStrip1);
            base.FormBorderStyle = FormBorderStyle.FixedSingle;
            base.Icon = (Icon) manager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.Name = "mainDlg";
            base.StartPosition = FormStartPosition.CenterScreen;
            this.Text = "PDF Convert Mini";
            base.FormClosing += new FormClosingEventHandler(this.mainDlg_FormClosing);
            base.Load += new EventHandler(this.mainDlg_Load);
            base.DragDrop += new DragEventHandler(this.mainDlg_DragDrop);
            base.DragEnter += new DragEventHandler(this.mainDlg_DragEnter);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.menuList.ResumeLayout(false);
            ((ISupportInitialize) this.pictureBoxAbout).EndInit();
            ((ISupportInitialize) this.picBoxQQ).EndInit();
            ((ISupportInitialize) this.picBoxHelp).EndInit();
            ((ISupportInitialize) this.picBoxContact).EndInit();
            base.ResumeLayout(false);
            base.PerformLayout();
        }

        private void InitializeListView()
        {
            this.lstFile.ListViewItemSorter = new ListViewIndexComparer();
            this.lstFile.InsertionMark.Color = Color.Red;
            this.lstFile.AllowDrop = true;
        }

        private bool is_matched(string file_name, int lst_select)
        {
            bool flag = false;
            string str = Path.GetExtension(file_name).ToUpper();
            if (lst_select == 0)
            {
                switch (str)
                {
                    case ".PDF":
                    case ".XLS":
                    case ".XLSX":
                    case ".PPT":
                    case ".PPTX":
                        flag = true;
                        break;
                }
                return flag;
            }
            if (lst_select == 1)
            {
                switch (str)
                {
                    case ".PDF":
                    case ".DOC":
                    case ".DOCX":
                    case ".PPT":
                    case ".PPTX":
                        flag = true;
                        break;
                }
                return flag;
            }
            if (lst_select == 2)
            {
                switch (str)
                {
                    case ".PDF":
                    case ".DOC":
                    case ".DOCX":
                    case ".XLS":
                    case ".XLSX":
                        flag = true;
                        break;
                }
                return flag;
            }
            if (((lst_select == 3) || (lst_select == 5)) || (lst_select == 6))
            {
                switch (str)
                {
                    case ".PDF":
                    case ".DOC":
                    case ".DOCX":
                    case ".PPT":
                    case ".PPTX":
                    case ".XLS":
                    case ".XLSX":
                        flag = true;
                        break;
                }
                return flag;
            }
            if (lst_select == 4)
            {
                switch (str)
                {
                    case ".JPG":
                    case ".GIF":
                    case ".BMP":
                    case ".PNG":
                    case ".TIFF":
                        return (flag = true);
                }
                return flag;
            }
            if (lst_select == 7)
            {
                switch (str)
                {
                    case ".DOC":
                    case ".DOCX":
                        flag = true;
                        break;
                }
                return flag;
            }
            if (lst_select == 8)
            {
                switch (str)
                {
                    case ".XLS":
                    case ".XLSX":
                        return (flag = true);
                }
                return flag;
            }
            if ((lst_select == 9) && ((str == ".PPT") || (str == ".PPTX")))
            {
                return (flag = true);
            }
            return flag;
        }

        private void linkLabelContact_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/contact/qq/?version=" + PDF_Convert.Version.version);
        }

        private void lstFile_DragDrop(object sender, DragEventArgs e)
        {
            bool flag = true;
            if (((string[]) e.Data.GetData(DataFormats.FileDrop)) != null)
            {
                foreach (string str in (string[]) e.Data.GetData(DataFormats.FileDrop))
                {
                    if (this.dic_lst.ContainsKey(str))
                    {
                        if (flag && (this.openFileDialog.FileNames.Length == 1))
                        {
                            flag = false;
                            MessageBox.Show("您添加的文件 " + Path.GetFileName(str) + " 已存在,我们将会自动过滤这些文件!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                        }
                        else if (flag && (this.openFileDialog.FileNames.Length != 1))
                        {
                            flag = false;
                            MessageBox.Show("您添加的部分文件已存在,我们将会自动过滤这些文件!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                        }
                    }
                    else
                    {
                        ListViewItem item = new ListViewItem(str);
                        item.SubItems.Add(this.GetLastFileName(str));
                        item.SubItems.Add(this.isReg ? "所有" : "前3页");
                        item.SubItems.Add(Math.Round((double) ((((double) new FileInfo(str).Length) / 1024.0) / 1024.0), 2).ToString() + "MB");
                        item.SubItems.Add("");
                        string text = "1";
                        try
                        {
                            text = Common.GetFilePage(str).ToString();
                        }
                        catch (Exception)
                        {
                            text = "已加密";
                        }
                        item.SubItems.Add(text);
                        this.lstFile.Items.Add(item);
                        this.dic_lst.Add(str, false);
                    }
                }
            }
            else
            {
                int index = this.lstFile.InsertionMark.Index;
                if (index != -1)
                {
                    if (this.lstFile.InsertionMark.AppearsAfterItem)
                    {
                        index++;
                    }
                    ListViewItem data = (ListViewItem) e.Data.GetData(typeof(ListViewItem));
                    this.lstFile.Items.Insert(index, (ListViewItem) data.Clone());
                    this.lstFile.Items.Remove(data);
                }
            }
        }

        private void lstFile_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
            }
            else
            {
                e.Effect = e.AllowedEffect;
            }
        }

        private void lstFile_DragLeave(object sender, EventArgs e)
        {
            this.lstFile.InsertionMark.Index = -1;
        }

        private void lstFile_DragOver(object sender, DragEventArgs e)
        {
            Point pt = this.lstFile.PointToClient(new Point(e.X, e.Y));
            int index = this.lstFile.InsertionMark.NearestIndex(pt);
            if (index > -1)
            {
                Rectangle itemRect = this.lstFile.GetItemRect(index);
                if (pt.X > (itemRect.Left + (itemRect.Width / 2)))
                {
                    this.lstFile.InsertionMark.AppearsAfterItem = true;
                }
                else
                {
                    this.lstFile.InsertionMark.AppearsAfterItem = false;
                }
            }
            this.lstFile.InsertionMark.Index = index;
        }

        private void lstFile_ItemDrag(object sender, ItemDragEventArgs e)
        {
            this.lstFile.DoDragDrop(e.Item, DragDropEffects.Move);
        }

        private void lstFile_SelectedIndexChanged(object sender, EventArgs e)
        {
            if ((this.lstFile.SelectedIndices.Count == 1) && (this.lst_select != 4))
            {
                if (this.isReg)
                {
                    this.comboBoxPage.Location = new Point(this.lstFile.SelectedItems[0].SubItems[2].Bounds.Left, this.lstFile.SelectedItems[0].SubItems[2].Bounds.Top);
                    this.comboBoxPage.Text = this.lstFile.SelectedItems[0].SubItems[2].Text;
                    this.comboBoxPage.Visible = true;
                }
                else
                {
                    this.comboBoxPage.Visible = false;
                }
            }
            else
            {
                this.comboBoxPage.Visible = false;
            }
        }

        private void lstType_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            this.dic_lst.Clear();
            this.lstType.Items[this.lst_select].ImageIndex = 0;
            e.Item.ImageIndex = 1;
            this.lst_select = e.ItemIndex;
            if (this.lst_select == 4)
            {
                this.cbIsMerger.Visible = true;
                this.lstFile.Columns[2].Width = 0;
                this.comboBoxPage.Visible = false;
            }
            else
            {
                this.cbIsMerger.Visible = false;
                this.lstFile.Columns[2].Width = 90;
                this.comboBoxPage.Visible = true;
            }
        }

        private void lstType_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void mainDlg_DragDrop(object sender, DragEventArgs e)
        {
            bool flag = true;
            foreach (string str in (string[]) e.Data.GetData(DataFormats.FileDrop))
            {
                if (this.dic_lst.ContainsKey(str))
                {
                    if (flag && (this.openFileDialog.FileNames.Length == 1))
                    {
                        flag = false;
                        MessageBox.Show("您添加的文件 " + Path.GetFileName(str) + " 已存在,我们将会自动过滤这些文件!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                    }
                    else if (flag && (this.openFileDialog.FileNames.Length != 1))
                    {
                        flag = false;
                        MessageBox.Show("您添加的部分文件已存在,我们将会自动过滤这些文件!", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                    }
                }
                else
                {
                    ListViewItem item = new ListViewItem(str);
                    item.SubItems.Add(this.GetLastFileName(str));
                    item.SubItems.Add(this.isReg ? "所有" : "前3页");
                    item.SubItems.Add(Math.Round((double) ((((double) new FileInfo(str).Length) / 1024.0) / 1024.0), 2).ToString() + "MB");
                    item.SubItems.Add("");
                    string text = "1";
                    try
                    {
                        text = Common.GetFilePage(str).ToString();
                    }
                    catch (Exception)
                    {
                        text = "已加密";
                    }
                    item.SubItems.Add(text);
                    this.lstFile.Items.Add(item);
                    this.dic_lst.Add(str, false);
                }
            }
        }

        private void mainDlg_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Link;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        private void mainDlg_FormClosing(object sender, FormClosingEventArgs e)
        {
            ini_config _config = new ini_config("config.ini");
            _config.write_ini("StartPage", this.global_cfg.start_page.ToString(), "App");
            _config.write_ini("EndPage", this.global_cfg.end_page.ToString(), "App");
            _config.write_ini("TargetDic", this.global_cfg.target_dic, "App");
            _config.write_ini("PicX", this.global_cfg.pic_width.ToString(), "App");
            _config.write_ini("PicY", this.global_cfg.pic_height.ToString(), "App");
            _config.write_ini("Out", this.rdoPath.Checked ? "0" : "1", "App");
            _config.write_ini("Type", this.lst_select.ToString(), "App");
        }

        private void mainDlg_Load(object sender, EventArgs e)
        {
            ini_config _config = new ini_config("config.ini");
            string str = _config.read_ini("StartPage", "App");
            string str2 = _config.read_ini("EndPage", "App");
            string str3 = _config.read_ini("TargetDic", "App");
            string str4 = _config.read_ini("PicX", "App");
            string str5 = _config.read_ini("PicY", "App");
            string str6 = _config.read_ini("Type", "App");
            string str7 = _config.read_ini("isMerger", "App");
            this.cbIsMerger.Checked = str7 == "1";
            this.comboBoxPage.Width = this.lstFile.Columns[2].Width;
            this.lstFile.Controls.Add(this.comboBoxPage);
            this.lstFile.SmallImageList = new ImageList();
            this.lstFile.SmallImageList.ImageSize = new Size(1, 20);
            this.toolTipInfo.SetToolTip(this.picBoxContact, "查看用户使用评价");
            this.toolTipInfo.SetToolTip(this.picBoxHelp, "查看使用帮助");
            string str8 = _config.read_ini("Out", "App");
            int num = (str8 == string.Empty) ? 0 : System.Convert.ToInt32(str8);
            if (num == 0)
            {
                this.btnChooseDic.Enabled = false;
                this.txtOutPath.Enabled = false;
                this.rdoPath.Checked = true;
            }
            else
            {
                this.btnChooseDic.Visible = true;
                this.txtOutPath.Visible = true;
                this.rdoNewPath.Checked = true;
            }
            if (new reg().is_reg())
            {
                this.Text = Encrypt.APPTITLE + " 正式版 v" + PDF_Convert.Version.version;
                this.isReg = true;
            }
            else
            {
                this.Text = Encrypt.APPTITLE + " 免费试用版 v" + PDF_Convert.Version.version;
                this.isReg = false;
            }
            if (str != string.Empty)
            {
                this.global_cfg.start_page = System.Convert.ToInt32(str);
            }
            if (str2 != string.Empty)
            {
                this.global_cfg.end_page = System.Convert.ToInt32(str2);
            }
            if (str3 != string.Empty)
            {
                this.global_cfg.target_dic = str3;
                this.txtOutPath.Text = str3;
            }
            else
            {
                this.txtOutPath.Text = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
                this.global_cfg.target_dic = this.txtOutPath.Text;
            }
            if (str4 != string.Empty)
            {
                this.global_cfg.pic_width = System.Convert.ToInt32(str4);
            }
            if (str5 != string.Empty)
            {
                this.global_cfg.pic_height = System.Convert.ToInt32(str5);
            }
            if (str6 != string.Empty)
            {
                this.lst_select = System.Convert.ToInt32(str6);
            }
            this.select_node();
        }

        private void picBoxContact_Click(object sender, EventArgs e)
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

        private void pictureBoxAbout_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/about");
        }

        private void rdoNewPath_Click(object sender, EventArgs e)
        {
            this.btnChooseDic.Enabled = true;
            this.txtOutPath.Enabled = true;
        }

        private void rdoPath_Click(object sender, EventArgs e)
        {
            this.btnChooseDic.Enabled = false;
            this.txtOutPath.Enabled = false;
        }

        private void select_node()
        {
            if ((this.lst_select < 0) || (this.lst_select >= this.lstType.Items.Count))
            {
                this.lst_select = 0;
            }
            this.lstType.Items[this.lst_select].Selected = true;
        }

        private void show(int cur)
        {
            this.progressBar.Value = cur;
            this.lbProgress.Text = cur.ToString() + "%";
            this.lstFile.Items[this.process_list_item].SubItems[4].Text = cur.ToString() + "%";
        }

        private void toolAddFiles_Click(object sender, EventArgs e)
        {
            this.btnAddFiles_Click(sender, e);
        }

        private void toolClear_Click(object sender, EventArgs e)
        {
            this.btnClear_Click(sender, e);
        }

        private void toolDelete_Click(object sender, EventArgs e)
        {
            this.btnDelete_Click(sender, e);
        }

        private void work_thread(object obj)
        {
            PDF_Convert.Convert.save_progress progress = new PDF_Convert.Convert.save_progress(this.show);
            PDF_Convert.Convert.config cfg = new PDF_Convert.Convert.config();
            for (int i = 0; i < this.task.Count; i++)
            {
                cfg = this.global_cfg;
                if ((this.task_page[i] == "所有") || (this.task_page[i] == "前3页"))
                {
                    cfg.start_page = 1;
                    cfg.end_page = 0;
                }
                else
                {
                    string[] strArray = this.task_page[i].Split(new char[] { '-' });
                    cfg.start_page = System.Convert.ToInt32(strArray[0]);
                    cfg.end_page = System.Convert.ToInt32(strArray[1]);
                }
                PDF_Convert.Convert convert = new PDF_Convert.Convert(this.task[i], this.get_format(), cfg, this.lst_select);
                if (!convert.can_work())
                {
                    if (convert.get_err_msg() != string.Empty)
                    {
                        MessageBox.Show(convert.get_err_msg(), "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                    }
                }
                else
                {
                    convert.save(progress, this, i, Path.GetExtension(this.task[i]).ToLower(), this.lst_select);
                }
                convert.close();
                this.process_list_item++;
            }
            base.Invoke(new enable_button(this.enable_btn_start));
            MessageBox.Show("转换全部完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            if (((this.lst_select != 4) && !this.isReg) && (MessageBox.Show("未注册用户只能转换前3页文档。请问是否立即购买注册，以转换全部内容？在您的支持下，我们会做得更好。", "提示", MessageBoxButtons.YesNo, MessageBoxIcon.Asterisk) == DialogResult.Yes))
            {
                DialogResult result = new buyDlg().ShowDialog();
                if (this.isReg)
                {
                    base.Invoke(new edit_window_text(this.edit_this_window_text), new object[] { Encrypt.APPTITLE + " 正式版 v" + PDF_Convert.Version.version });
                }
            }
        }

        private delegate void edit_window_text(string text);

        private delegate void enable_button();
    }
}

