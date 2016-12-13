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

    public class mainDlg_A : Form
    {
        private Button btnAbout;
        private Button btnAddFiles;
        private Button btnClean;
        private Button btnConfig;
        private Button btnDelete;
        private Button btnReg;
        private Button btnSelectPath;
        private CheckBox cbIsMerger;
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private ColumnHeader columnHeader3;
        private ColumnHeader columnHeader4;
        private ColumnHeader columnHeader5;
        private ColumnHeader columnHeader6;
        private ColumnHeader columnHeader7;
        private ComboBox comboBoxPage;
        private IContainer components = null;
        private Dictionary<string, bool> dic_lst = new Dictionary<string, bool>();
        private FolderBrowserDialog folderBrowserDialog;
        private PDF_Convert.Convert.FORMAT format = PDF_Convert.Convert.FORMAT.PDF2WORD;
        private PDF_Convert.Convert.config global_cfg = new PDF_Convert.Convert.config(1, 0, "", "", 700, 500, 0);
        private bool isReg = false;
        private Label label1;
        private LinkLabel linkLabelChat;
        private int lst_select = 0;
        public ListView lstFile;
        private ContextMenuStrip menuList;
        private OpenFileDialog openFileDialog;
        private PictureBox picBoxComments;
        private PictureBox picBoxEPUB;
        private PictureBox picBoxExcel;
        private PictureBox picBoxHelp;
        private PictureBox picBoxHTML;
        private PictureBox picBoxIMG;
        private PictureBox picBoxPDF;
        private PictureBox picBoxPPT;
        private PictureBox picBoxQQ;
        private PictureBox picBoxStart;
        private PictureBox picBoxTitle;
        private PictureBox picBoxTXT;
        private PictureBox picBoxWord;
        private int progres_list_item = 0;
        private RadioButton rdoNewPath;
        private RadioButton rdoPath;
        private List<string> task = new List<string>();
        private List<string> task_page = new List<string>();
        private ToolStripMenuItem toolAddFiles;
        private ToolStripMenuItem toolClear;
        private ToolStripMenuItem toolDelete;
        private ToolTip toolTipInfo;
        private TextBox txtPath;

        public mainDlg_A()
        {
            Control.CheckForIllegalCrossThreadCalls = false;
            this.InitializeComponent();
            this.InitializeListView();
        }

        private void btnAbout_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/about");
        }

        private void btnAddFiles_Click(object sender, EventArgs e)
        {
            this.openFileDialog.FileName = "";
            if (this.lst_select == 0)
            {
                this.openFileDialog.Filter = "支持文件(*.pdf,*.xls,*.xlsx,*.ppt,*.pptx)|*.pdf;*.xls;*.xlsx;*.ppt;*.pptx;";
            }
            else if (this.lst_select == 1)
            {
                this.openFileDialog.Filter = "支持文件(*.pdf,*.ppt,*.pptx,*.doc,*.docx)|*.pdf;*.ppt;*.pptx;*.doc;*.docx";
            }
            else if (this.lst_select == 3)
            {
                this.openFileDialog.Filter = "支持文件(*.pdf,*.xls,*.xlsx,*.doc,*.docx)|*.pdf;*.xls;*.xlsx;*.doc;*.docx";
            }
            else if (((this.lst_select == 2) || (this.lst_select == 4)) || (this.lst_select == 5))
            {
                this.openFileDialog.Filter = "支持文件(*.pdf,*.ppt,*.pptx,*.doc,*.docx,*.xls,*.xlsx)|*.pdf;*.ppt;*.pptx;*.doc;*.docx;*.xls;*.xlsx";
            }
            else if (this.lst_select == 6)
            {
                this.openFileDialog.Filter = "图片文件(*.jpg,*.gif,*.bmp,*.png,*.tiff)|*.jpg;*.gif;*.bmp;*.png;*.tiff";
            }
            else if (this.lst_select == 7)
            {
                this.openFileDialog.Filter = "Word文件(*.doc,*.docx)|*.doc;*.docx|PowerPoint文件(*.ppt,*.pptx)|*.ppt;*.pptx|Excel文件(*.xls,*.xlsx)|*.xls;*.xlsx";
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
                        int num2 = this.lstFile.Items.Count + 1;
                        ListViewItem item = new ListViewItem(num2.ToString());
                        item.SubItems.Add(str);
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

        private void btnClean_Click(object sender, EventArgs e)
        {
            this.comboBoxPage.Visible = false;
            this.lstFile.Items.Clear();
            this.dic_lst.Clear();
        }

        private void btnConfig_Click(object sender, EventArgs e)
        {
            new configDlg(ref this.global_cfg, new PDF_Convert.configDlg.edit_cfg(this.edit_cfg)).ShowDialog();
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in this.lstFile.SelectedItems)
            {
                this.dic_lst.Remove(item.SubItems[1].Text);
                item.Remove();
            }
            for (int i = 0; i < this.lstFile.Items.Count; i++)
            {
                this.lstFile.Items[i].Text = (i + 1).ToString();
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

        private void btnSelectPath_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK == this.folderBrowserDialog.ShowDialog())
            {
                this.txtPath.Text = this.folderBrowserDialog.SelectedPath;
            }
        }

        private void cancel_hot()
        {
            switch (this.format)
            {
                case PDF_Convert.Convert.FORMAT.PDF2WORD:
                    this.picBoxWord.Image = Resources.doc_gen;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2EXCEL:
                    this.picBoxExcel.Image = Resources.xls_gen;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2PPT:
                    this.picBoxPPT.Image = Resources.ppt_gen;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2HTML:
                    this.picBoxHTML.Image = Resources.html_gen;
                    break;

                case PDF_Convert.Convert.FORMAT.IMG2PDF:
                    this.picBoxEPUB.Image = Resources.epub_gen;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2IMG:
                    this.picBoxIMG.Image = Resources.img_gen;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2TXT:
                    this.picBoxTXT.Image = Resources.txt_gen;
                    break;

                default:
                    this.picBoxPDF.Image = Resources.pdf_gen;
                    break;
            }
        }

        private void cbIsMerger_Click(object sender, EventArgs e)
        {
            string str = this.cbIsMerger.Checked ? "1" : "0";
            new ini_config("config.ini").write_ini("isMerger", str, "App");
        }

        private void comboBoxPage_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                this.lstFile.Focus();
            }
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
                        this.lstFile.SelectedItems[0].SubItems[3].Text = "所有";
                    }
                    else if (System.Convert.ToInt32(strArray[0]) > System.Convert.ToInt32(strArray[1]))
                    {
                        MessageBox.Show("起始页应小于等于最终页,请重新输入", "错误", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                        this.lstFile.SelectedItems[0].SubItems[3].Text = "所有";
                    }
                }
            }
            catch
            {
                this.lstFile.SelectedItems[0].SubItems[3].Text = "所有";
            }
        }

        private void comboBoxPage_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.lstFile.SelectedItems[0].SubItems[3].Text = this.comboBoxPage.Text;
        }

        private void comboBoxPage_TextUpdate(object sender, EventArgs e)
        {
            if (this.comboBoxPage.Text.Split(new char[] { '-' }).Length != 2)
            {
                this.lstFile.SelectedItems[0].SubItems[3].Text = "所有";
            }
            else
            {
                this.lstFile.SelectedItems[0].SubItems[3].Text = this.comboBoxPage.Text;
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

        private void edit_cfg(PDF_Convert.Convert.config cfg)
        {
            this.global_cfg = cfg;
        }

        private void edit_this_window_text(string text)
        {
            this.Text = text;
        }

        private void enable_start_button()
        {
            this.picBoxStart.Image = Resources.start_gen;
            this.picBoxStart.Enabled = true;
        }

        private PDF_Convert.Convert.FORMAT get_format(string file_name)
        {
            string str = Path.GetExtension(file_name).ToUpper();
            switch (str)
            {
                case ".DOC":
                case ".DOCX":
                    return PDF_Convert.Convert.FORMAT.DOC2PDF;
            }
            if (((str != ".XLS") && (str != ".XLSX")) && ((str == ".PPT") || (str == ".PPTX")))
            {
                return PDF_Convert.Convert.FORMAT.PPT2PDF;
            }
            return PDF_Convert.Convert.FORMAT.XLS2PDF;
        }

        private string get_task_name()
        {
            switch (this.lst_select)
            {
                case 0:
                    return "PDF 转 Word";

                case 1:
                    return "PDF 转 Excel";

                case 2:
                    return "PDF 转 IMG";

                case 3:
                    return "PDF 转 PowerPoint";

                case 4:
                    return "PDF 转 HTML";

                case 5:
                    return "PDF 转 TXT";

                case 6:
                    return "IMG 转 PDF";

                case 7:
                    return "转到 PDF";
            }
            return "";
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
            ComponentResourceManager manager = new ComponentResourceManager(typeof(mainDlg_A));
            this.lstFile = new ListView();
            this.columnHeader1 = new ColumnHeader();
            this.columnHeader6 = new ColumnHeader();
            this.columnHeader2 = new ColumnHeader();
            this.columnHeader5 = new ColumnHeader();
            this.columnHeader3 = new ColumnHeader();
            this.columnHeader4 = new ColumnHeader();
            this.columnHeader7 = new ColumnHeader();
            this.menuList = new ContextMenuStrip(this.components);
            this.toolAddFiles = new ToolStripMenuItem();
            this.toolDelete = new ToolStripMenuItem();
            this.toolClear = new ToolStripMenuItem();
            this.btnAddFiles = new Button();
            this.label1 = new Label();
            this.rdoPath = new RadioButton();
            this.rdoNewPath = new RadioButton();
            this.txtPath = new TextBox();
            this.btnConfig = new Button();
            this.btnDelete = new Button();
            this.btnClean = new Button();
            this.btnAbout = new Button();
            this.btnSelectPath = new Button();
            this.openFileDialog = new OpenFileDialog();
            this.picBoxWord = new PictureBox();
            this.picBoxExcel = new PictureBox();
            this.picBoxIMG = new PictureBox();
            this.picBoxPPT = new PictureBox();
            this.picBoxHTML = new PictureBox();
            this.picBoxTXT = new PictureBox();
            this.picBoxEPUB = new PictureBox();
            this.picBoxPDF = new PictureBox();
            this.toolTipInfo = new ToolTip(this.components);
            this.picBoxTitle = new PictureBox();
            this.picBoxStart = new PictureBox();
            this.picBoxComments = new PictureBox();
            this.picBoxHelp = new PictureBox();
            this.linkLabelChat = new LinkLabel();
            this.picBoxQQ = new PictureBox();
            this.folderBrowserDialog = new FolderBrowserDialog();
            this.btnReg = new Button();
            this.comboBoxPage = new ComboBox();
            this.cbIsMerger = new CheckBox();
            this.menuList.SuspendLayout();
            ((ISupportInitialize) this.picBoxWord).BeginInit();
            ((ISupportInitialize) this.picBoxExcel).BeginInit();
            ((ISupportInitialize) this.picBoxIMG).BeginInit();
            ((ISupportInitialize) this.picBoxPPT).BeginInit();
            ((ISupportInitialize) this.picBoxHTML).BeginInit();
            ((ISupportInitialize) this.picBoxTXT).BeginInit();
            ((ISupportInitialize) this.picBoxEPUB).BeginInit();
            ((ISupportInitialize) this.picBoxPDF).BeginInit();
            ((ISupportInitialize) this.picBoxTitle).BeginInit();
            ((ISupportInitialize) this.picBoxStart).BeginInit();
            ((ISupportInitialize) this.picBoxComments).BeginInit();
            ((ISupportInitialize) this.picBoxHelp).BeginInit();
            ((ISupportInitialize) this.picBoxQQ).BeginInit();
            base.SuspendLayout();
            this.lstFile.Columns.AddRange(new ColumnHeader[] { this.columnHeader1, this.columnHeader6, this.columnHeader2, this.columnHeader5, this.columnHeader3, this.columnHeader4, this.columnHeader7 });
            this.lstFile.ContextMenuStrip = this.menuList;
            this.lstFile.FullRowSelect = true;
            this.lstFile.GridLines = true;
            this.lstFile.Location = new Point(12, 0xb6);
            this.lstFile.Name = "lstFile";
            this.lstFile.Size = new Size(0x2f3, 0xff);
            this.lstFile.TabIndex = 9;
            this.lstFile.UseCompatibleStateImageBehavior = false;
            this.lstFile.View = View.Details;
            this.lstFile.ItemDrag += new ItemDragEventHandler(this.lstFile_ItemDrag);
            this.lstFile.SelectedIndexChanged += new EventHandler(this.lstFile_SelectedIndexChanged);
            this.lstFile.DragDrop += new DragEventHandler(this.mainDlg_A_DragDrop);
            this.lstFile.DragEnter += new DragEventHandler(this.mainDlg_A_DragEnter);
            this.lstFile.DragOver += new DragEventHandler(this.lstFile_DragOver);
            this.lstFile.DragLeave += new EventHandler(this.lstFile_DragLeave);
            this.columnHeader1.Text = "编号";
            this.columnHeader6.Width = 0;
            this.columnHeader2.Text = "文件名";
            this.columnHeader2.Width = 300;
            this.columnHeader5.Text = "页数选择";
            this.columnHeader5.Width = 90;
            this.columnHeader3.Text = "文件大小";
            this.columnHeader3.Width = 100;
            this.columnHeader4.Text = "转换进度";
            this.columnHeader4.Width = 80;
            this.columnHeader7.Text = "总页数";
            this.columnHeader7.Width = 50;
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
            this.btnAddFiles.Location = new Point(12, 460);
            this.btnAddFiles.Name = "btnAddFiles";
            this.btnAddFiles.Size = new Size(0x4c, 30);
            this.btnAddFiles.TabIndex = 10;
            this.btnAddFiles.Text = "添加文件";
            this.btnAddFiles.UseVisualStyleBackColor = true;
            this.btnAddFiles.Click += new EventHandler(this.btnAddFiles_Click);
            this.label1.AutoSize = true;
            this.label1.BackColor = Color.Transparent;
            this.label1.Font = new Font("微软雅黑", 10.5f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.label1.ForeColor = Color.DimGray;
            this.label1.Location = new Point(12, 0x1f5);
            this.label1.Name = "label1";
            this.label1.Size = new Size(0x41, 0x13);
            this.label1.TabIndex = 0x15;
            this.label1.Text = "输出选项";
            this.rdoPath.AutoSize = true;
            this.rdoPath.BackColor = Color.Transparent;
            this.rdoPath.Checked = true;
            this.rdoPath.Font = new Font("微软雅黑", 9f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.rdoPath.ForeColor = SystemColors.ControlText;
            this.rdoPath.Location = new Point(15, 0x216);
            this.rdoPath.Name = "rdoPath";
            this.rdoPath.Size = new Size(0x7a, 0x15);
            this.rdoPath.TabIndex = 0x16;
            this.rdoPath.TabStop = true;
            this.rdoPath.Text = "保存在原文件夹内";
            this.rdoPath.UseVisualStyleBackColor = false;
            this.rdoPath.Click += new EventHandler(this.rdoPath_Click);
            this.rdoNewPath.AutoSize = true;
            this.rdoNewPath.BackColor = Color.Transparent;
            this.rdoNewPath.Font = new Font("微软雅黑", 9f, FontStyle.Bold, GraphicsUnit.Point, 0x86);
            this.rdoNewPath.Location = new Point(15, 0x233);
            this.rdoNewPath.Name = "rdoNewPath";
            this.rdoNewPath.Size = new Size(0x62, 0x15);
            this.rdoNewPath.TabIndex = 0x17;
            this.rdoNewPath.Text = "自定义文件夹";
            this.rdoNewPath.UseVisualStyleBackColor = false;
            this.rdoNewPath.Click += new EventHandler(this.rdoNewPath_Click);
            this.txtPath.Location = new Point(0x73, 0x233);
            this.txtPath.Name = "txtPath";
            this.txtPath.ReadOnly = true;
            this.txtPath.Size = new Size(0x109, 0x15);
            this.txtPath.TabIndex = 0x18;
            this.btnConfig.Location = new Point(0x5e, 460);
            this.btnConfig.Name = "btnConfig";
            this.btnConfig.Size = new Size(0x4c, 30);
            this.btnConfig.TabIndex = 0x19;
            this.btnConfig.Text = "选项";
            this.btnConfig.UseVisualStyleBackColor = true;
            this.btnConfig.Click += new EventHandler(this.btnConfig_Click);
            this.btnDelete.Location = new Point(0xb5, 460);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new Size(0x4c, 30);
            this.btnDelete.TabIndex = 0x1a;
            this.btnDelete.Text = "删除";
            this.btnDelete.UseVisualStyleBackColor = true;
            this.btnDelete.Click += new EventHandler(this.btnDelete_Click);
            this.btnClean.Location = new Point(0x107, 460);
            this.btnClean.Name = "btnClean";
            this.btnClean.Size = new Size(0x4c, 30);
            this.btnClean.TabIndex = 0x1b;
            this.btnClean.Text = "清空";
            this.btnClean.UseVisualStyleBackColor = true;
            this.btnClean.Click += new EventHandler(this.btnClean_Click);
            this.btnAbout.Location = new Point(0x159, 460);
            this.btnAbout.Name = "btnAbout";
            this.btnAbout.Size = new Size(0x4c, 30);
            this.btnAbout.TabIndex = 0x1c;
            this.btnAbout.Text = "关于";
            this.btnAbout.UseVisualStyleBackColor = true;
            this.btnAbout.Click += new EventHandler(this.btnAbout_Click);
            this.btnSelectPath.Location = new Point(0x187, 0x232);
            this.btnSelectPath.Name = "btnSelectPath";
            this.btnSelectPath.Size = new Size(0x30, 0x17);
            this.btnSelectPath.TabIndex = 30;
            this.btnSelectPath.Text = "浏览";
            this.btnSelectPath.UseVisualStyleBackColor = true;
            this.btnSelectPath.Click += new EventHandler(this.btnSelectPath_Click);
            this.openFileDialog.FileName = "*.PDF";
            this.openFileDialog.Multiselect = true;
            this.picBoxWord.BackColor = Color.Transparent;
            this.picBoxWord.Image = Resources.doc_gen;
            this.picBoxWord.Location = new Point(3, 0x2a);
            this.picBoxWord.Name = "picBoxWord";
            this.picBoxWord.Size = new Size(0x5d, 0x66);
            this.picBoxWord.TabIndex = 0x1f;
            this.picBoxWord.TabStop = false;
            this.picBoxWord.Click += new EventHandler(this.picBoxWord_Click);
            this.picBoxWord.MouseLeave += new EventHandler(this.picBoxWord_MouseLeave);
            this.picBoxWord.MouseMove += new MouseEventHandler(this.picBoxWord_MouseMove);
            this.picBoxExcel.BackColor = Color.Transparent;
            this.picBoxExcel.Image = Resources.xls_gen;
            this.picBoxExcel.Location = new Point(0x63, 0x2a);
            this.picBoxExcel.Name = "picBoxExcel";
            this.picBoxExcel.Size = new Size(0x5d, 0x66);
            this.picBoxExcel.TabIndex = 0x20;
            this.picBoxExcel.TabStop = false;
            this.picBoxExcel.Click += new EventHandler(this.picBoxExcel_Click);
            this.picBoxExcel.MouseLeave += new EventHandler(this.picBoxExcel_MouseLeave);
            this.picBoxExcel.MouseMove += new MouseEventHandler(this.picBoxExcel_MouseMove);
            this.picBoxIMG.BackColor = Color.Transparent;
            this.picBoxIMG.Image = Resources.img_gen;
            this.picBoxIMG.Location = new Point(0xc3, 0x2a);
            this.picBoxIMG.Name = "picBoxIMG";
            this.picBoxIMG.Size = new Size(0x5d, 0x66);
            this.picBoxIMG.TabIndex = 0x21;
            this.picBoxIMG.TabStop = false;
            this.picBoxIMG.Click += new EventHandler(this.picBoxIMG_Click);
            this.picBoxIMG.MouseLeave += new EventHandler(this.picBoxIMG_MouseLeave);
            this.picBoxIMG.MouseMove += new MouseEventHandler(this.picBoxIMG_MouseMove);
            this.picBoxPPT.BackColor = Color.Transparent;
            this.picBoxPPT.Image = Resources.ppt_gen;
            this.picBoxPPT.Location = new Point(0x123, 0x2a);
            this.picBoxPPT.Name = "picBoxPPT";
            this.picBoxPPT.Size = new Size(0x5d, 0x66);
            this.picBoxPPT.TabIndex = 0x22;
            this.picBoxPPT.TabStop = false;
            this.picBoxPPT.Click += new EventHandler(this.picBoxPPT_Click);
            this.picBoxPPT.MouseLeave += new EventHandler(this.picBoxPPT_MouseLeave);
            this.picBoxPPT.MouseMove += new MouseEventHandler(this.picBoxPPT_MouseMove);
            this.picBoxHTML.BackColor = Color.Transparent;
            this.picBoxHTML.Image = Resources.html_gen;
            this.picBoxHTML.Location = new Point(0x183, 0x2a);
            this.picBoxHTML.Name = "picBoxHTML";
            this.picBoxHTML.Size = new Size(0x5d, 0x66);
            this.picBoxHTML.TabIndex = 0x23;
            this.picBoxHTML.TabStop = false;
            this.picBoxHTML.Click += new EventHandler(this.picBoxHTML_Click);
            this.picBoxHTML.MouseLeave += new EventHandler(this.picBoxHTML_MouseLeave);
            this.picBoxHTML.MouseMove += new MouseEventHandler(this.picBoxHTML_MouseMove);
            this.picBoxTXT.BackColor = Color.Transparent;
            this.picBoxTXT.Image = Resources.txt_gen;
            this.picBoxTXT.Location = new Point(0x1e3, 0x2a);
            this.picBoxTXT.Name = "picBoxTXT";
            this.picBoxTXT.Size = new Size(0x5d, 0x66);
            this.picBoxTXT.TabIndex = 0x24;
            this.picBoxTXT.TabStop = false;
            this.picBoxTXT.Click += new EventHandler(this.picBoxTXT_Click);
            this.picBoxTXT.MouseLeave += new EventHandler(this.picBoxTXT_MouseLeave);
            this.picBoxTXT.MouseMove += new MouseEventHandler(this.picBoxTXT_MouseMove);
            this.picBoxEPUB.BackColor = Color.Transparent;
            this.picBoxEPUB.Image = Resources.epub_gen;
            this.picBoxEPUB.Location = new Point(0x243, 0x2a);
            this.picBoxEPUB.Name = "picBoxEPUB";
            this.picBoxEPUB.Size = new Size(0x5d, 0x66);
            this.picBoxEPUB.TabIndex = 0x25;
            this.picBoxEPUB.TabStop = false;
            this.picBoxEPUB.Click += new EventHandler(this.picBoxEPUB_Click);
            this.picBoxEPUB.MouseLeave += new EventHandler(this.picBoxEPUB_MouseLeave);
            this.picBoxEPUB.MouseMove += new MouseEventHandler(this.picBoxEPUB_MouseMove);
            this.picBoxPDF.BackColor = Color.Transparent;
            this.picBoxPDF.Image = (Image) manager.GetObject("picBoxPDF.Image");
            this.picBoxPDF.Location = new Point(0x2a3, 0x2a);
            this.picBoxPDF.Name = "picBoxPDF";
            this.picBoxPDF.Size = new Size(0x5d, 0x66);
            this.picBoxPDF.TabIndex = 0x26;
            this.picBoxPDF.TabStop = false;
            this.picBoxPDF.Click += new EventHandler(this.picBoxPDF_Click);
            this.picBoxPDF.MouseLeave += new EventHandler(this.picBoxPDF_MouseLeave);
            this.picBoxPDF.MouseMove += new MouseEventHandler(this.picBoxPDF_MouseMove);
            this.toolTipInfo.AutomaticDelay = 100;
            this.picBoxTitle.BackColor = Color.Transparent;
            this.picBoxTitle.Image = Resources.Title;
            this.picBoxTitle.Location = new Point(15, 12);
            this.picBoxTitle.Name = "picBoxTitle";
            this.picBoxTitle.Size = new Size(220, 0x1d);
            this.picBoxTitle.TabIndex = 0x27;
            this.picBoxTitle.TabStop = false;
            this.picBoxStart.BackColor = Color.Transparent;
            this.picBoxStart.Image = (Image) manager.GetObject("picBoxStart.Image");
            this.picBoxStart.Location = new Point(0x243, 0x1e7);
            this.picBoxStart.Name = "picBoxStart";
            this.picBoxStart.Size = new Size(0x92, 0x43);
            this.picBoxStart.TabIndex = 40;
            this.picBoxStart.TabStop = false;
            this.picBoxStart.Click += new EventHandler(this.picBoxStart_Click);
            this.picBoxStart.MouseLeave += new EventHandler(this.picBoxStart_MouseLeave);
            this.picBoxStart.MouseMove += new MouseEventHandler(this.picBoxStart_MouseMove);
            this.picBoxComments.BackColor = Color.Transparent;
            this.picBoxComments.Image = Resources.comments;
            this.picBoxComments.Location = new Point(0x2b5, 5);
            this.picBoxComments.Name = "picBoxComments";
            this.picBoxComments.Size = new Size(0x20, 0x20);
            this.picBoxComments.TabIndex = 0x2c;
            this.picBoxComments.TabStop = false;
            this.picBoxComments.Click += new EventHandler(this.picBoxComments_Click);
            this.picBoxComments.MouseLeave += new EventHandler(this.picBoxComments_MouseLeave);
            this.picBoxComments.MouseMove += new MouseEventHandler(this.picBoxComments_MouseMove);
            this.picBoxHelp.BackColor = Color.Transparent;
            this.picBoxHelp.Cursor = Cursors.Hand;
            this.picBoxHelp.Image = Resources.help;
            this.picBoxHelp.Location = new Point(0x2e0, 6);
            this.picBoxHelp.Name = "picBoxHelp";
            this.picBoxHelp.Size = new Size(0x20, 0x20);
            this.picBoxHelp.TabIndex = 0x2d;
            this.picBoxHelp.TabStop = false;
            this.picBoxHelp.Click += new EventHandler(this.picBoxHelp_Click);
            this.picBoxHelp.MouseLeave += new EventHandler(this.picBoxComments_MouseLeave);
            this.picBoxHelp.MouseMove += new MouseEventHandler(this.picBoxComments_MouseMove);
            this.linkLabelChat.ActiveLinkColor = Color.Blue;
            this.linkLabelChat.AutoSize = true;
            this.linkLabelChat.BackColor = Color.Transparent;
            this.linkLabelChat.Font = new Font("微软雅黑", 10.5f, FontStyle.Regular, GraphicsUnit.Point, 0x86);
            this.linkLabelChat.LinkColor = Color.Red;
            this.linkLabelChat.Location = new Point(0x1c5, 0x232);
            this.linkLabelChat.Name = "linkLabelChat";
            this.linkLabelChat.Size = new Size(0x117, 20);
            this.linkLabelChat.TabIndex = 0x2e;
            this.linkLabelChat.TabStop = true;
            this.linkLabelChat.Text = "客服热线：4006685572 QQ：4006685572";
            this.linkLabelChat.LinkClicked += new LinkLabelLinkClickedEventHandler(this.linkLabelChat_LinkClicked);
            this.picBoxQQ.BackColor = Color.Transparent;
            this.picBoxQQ.Image = Resources.qq_chat;
            this.picBoxQQ.Location = new Point(0x261, 6);
            this.picBoxQQ.Name = "picBoxQQ";
            this.picBoxQQ.Size = new Size(0x44, 0x1f);
            this.picBoxQQ.TabIndex = 0x2f;
            this.picBoxQQ.TabStop = false;
            this.picBoxQQ.Click += new EventHandler(this.picBoxQQ_Click);
            this.picBoxQQ.MouseLeave += new EventHandler(this.picBoxComments_MouseLeave);
            this.picBoxQQ.MouseMove += new MouseEventHandler(this.picBoxComments_MouseMove);
            this.btnReg.Location = new Point(0x1ac, 460);
            this.btnReg.Name = "btnReg";
            this.btnReg.Size = new Size(0x4c, 30);
            this.btnReg.TabIndex = 0x30;
            this.btnReg.Text = "注册";
            this.btnReg.UseVisualStyleBackColor = true;
            this.btnReg.Click += new EventHandler(this.btnReg_Click);
            this.comboBoxPage.FormattingEnabled = true;
            this.comboBoxPage.Items.AddRange(new object[] { "所有", "例:1-30" });
            this.comboBoxPage.Location = new Point(0x20a, 460);
            this.comboBoxPage.Name = "comboBoxPage";
            this.comboBoxPage.Size = new Size(0x79, 20);
            this.comboBoxPage.TabIndex = 0x31;
            this.comboBoxPage.SelectedIndexChanged += new EventHandler(this.comboBoxPage_SelectedIndexChanged);
            this.comboBoxPage.TextUpdate += new EventHandler(this.comboBoxPage_TextUpdate);
            this.comboBoxPage.KeyUp += new KeyEventHandler(this.comboBoxPage_KeyUp);
            this.comboBoxPage.Leave += new EventHandler(this.comboBoxPage_Leave);
            this.cbIsMerger.AutoSize = true;
            this.cbIsMerger.BackColor = Color.Transparent;
            this.cbIsMerger.ForeColor = Color.Red;
            this.cbIsMerger.Location = new Point(0x99, 0x21b);
            this.cbIsMerger.Name = "cbIsMerger";
            this.cbIsMerger.Size = new Size(0xba, 0x10);
            this.cbIsMerger.TabIndex = 50;
            this.cbIsMerger.Text = "将所有图片合并成一个PDF文档";
            this.cbIsMerger.UseVisualStyleBackColor = false;
            this.cbIsMerger.Visible = false;
            this.cbIsMerger.Click += new EventHandler(this.cbIsMerger_Click);
            this.AllowDrop = true;
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            this.BackgroundImage = (Image) manager.GetObject("$this.BackgroundImage");
            base.ClientSize = new Size(0x30b, 0x255);
            base.Controls.Add(this.cbIsMerger);
            base.Controls.Add(this.comboBoxPage);
            base.Controls.Add(this.btnReg);
            base.Controls.Add(this.picBoxQQ);
            base.Controls.Add(this.linkLabelChat);
            base.Controls.Add(this.picBoxHelp);
            base.Controls.Add(this.picBoxComments);
            base.Controls.Add(this.picBoxStart);
            base.Controls.Add(this.picBoxTitle);
            base.Controls.Add(this.picBoxPDF);
            base.Controls.Add(this.picBoxEPUB);
            base.Controls.Add(this.picBoxTXT);
            base.Controls.Add(this.picBoxHTML);
            base.Controls.Add(this.picBoxPPT);
            base.Controls.Add(this.picBoxIMG);
            base.Controls.Add(this.picBoxExcel);
            base.Controls.Add(this.picBoxWord);
            base.Controls.Add(this.btnSelectPath);
            base.Controls.Add(this.btnAbout);
            base.Controls.Add(this.btnClean);
            base.Controls.Add(this.btnDelete);
            base.Controls.Add(this.btnConfig);
            base.Controls.Add(this.txtPath);
            base.Controls.Add(this.rdoNewPath);
            base.Controls.Add(this.rdoPath);
            base.Controls.Add(this.label1);
            base.Controls.Add(this.btnAddFiles);
            base.Controls.Add(this.lstFile);
            this.DoubleBuffered = true;
            base.FormBorderStyle = FormBorderStyle.FixedSingle;
            base.Icon = (Icon) manager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.Name = "mainDlg_A";
            base.StartPosition = FormStartPosition.CenterScreen;
            this.Text = "PDF Convert";
            base.FormClosing += new FormClosingEventHandler(this.mainDlg_A_FormClosing);
            base.Load += new EventHandler(this.mainDlg_A_Load);
            base.DragDrop += new DragEventHandler(this.mainDlg_A_DragDrop);
            base.DragEnter += new DragEventHandler(this.mainDlg_A_DragEnter);
            base.DragOver += new DragEventHandler(this.lstFile_DragOver);
            base.DragLeave += new EventHandler(this.lstFile_DragLeave);
            this.menuList.ResumeLayout(false);
            ((ISupportInitialize) this.picBoxWord).EndInit();
            ((ISupportInitialize) this.picBoxExcel).EndInit();
            ((ISupportInitialize) this.picBoxIMG).EndInit();
            ((ISupportInitialize) this.picBoxPPT).EndInit();
            ((ISupportInitialize) this.picBoxHTML).EndInit();
            ((ISupportInitialize) this.picBoxTXT).EndInit();
            ((ISupportInitialize) this.picBoxEPUB).EndInit();
            ((ISupportInitialize) this.picBoxPDF).EndInit();
            ((ISupportInitialize) this.picBoxTitle).EndInit();
            ((ISupportInitialize) this.picBoxStart).EndInit();
            ((ISupportInitialize) this.picBoxComments).EndInit();
            ((ISupportInitialize) this.picBoxHelp).EndInit();
            ((ISupportInitialize) this.picBoxQQ).EndInit();
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
            if (lst_select == 3)
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
            if (((lst_select == 2) || (lst_select == 4)) || (lst_select == 5))
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
            if (lst_select == 6)
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
            if ((lst_select == 7) && (((((str == ".DOC") || (str == ".DOCX")) || ((str == ".PPT") || (str == ".PPTX"))) || (str == ".XLS")) || (str == ".XLSX")))
            {
                flag = true;
            }
            return flag;
        }

        private void linkLabelChat_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/contact/qq/?version=" + PDF_Convert.Version.version);
        }

        private void lstFile_DragDrop(object sender, DragEventArgs e)
        {
        }

        private void lstFile_DragEnter(object sender, DragEventArgs e)
        {
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
            if ((this.lstFile.SelectedIndices.Count == 1) && (this.lst_select != 6))
            {
                if (this.isReg)
                {
                    this.comboBoxPage.Location = new Point(this.lstFile.SelectedItems[0].SubItems[3].Bounds.Left, this.lstFile.SelectedItems[0].SubItems[3].Bounds.Top);
                    this.comboBoxPage.Visible = true;
                    this.comboBoxPage.Text = this.lstFile.SelectedItems[0].SubItems[3].Text;
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

        private void mainDlg_A_DragDrop(object sender, DragEventArgs e)
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
                        int num3 = this.lstFile.Items.Count + 1;
                        ListViewItem item = new ListViewItem(num3.ToString());
                        item.SubItems.Add(str);
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

        private void mainDlg_A_DragEnter(object sender, DragEventArgs e)
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

        private void mainDlg_A_FormClosing(object sender, FormClosingEventArgs e)
        {
            ini_config _config = new ini_config("config.ini");
            _config.write_ini("StartPage", this.global_cfg.start_page.ToString(), "App");
            _config.write_ini("EndPage", this.global_cfg.end_page.ToString(), "App");
            _config.write_ini("TargetDic", this.global_cfg.target_dic, "App");
            _config.write_ini("PicX", this.global_cfg.pic_width.ToString(), "App");
            _config.write_ini("PicY", this.global_cfg.pic_height.ToString(), "App");
            _config.write_ini("Type_", this.lst_select.ToString(), "App");
            _config.write_ini("Out", this.rdoPath.Checked ? "0" : "1", "App");
            _config.write_ini("isMerger", this.global_cfg.isMerger.ToString(), "App");
        }

        private void mainDlg_A_Load(object sender, EventArgs e)
        {
            this.toolTipInfo.SetToolTip(this.picBoxWord, "将PDF,PPT,Excel转换为Word文档");
            this.toolTipInfo.SetToolTip(this.picBoxExcel, "将PDF,Word,PPT转换为Excel文档");
            this.toolTipInfo.SetToolTip(this.picBoxIMG, "将PDF,Word,Excel,PPT转换为图片文件");
            this.toolTipInfo.SetToolTip(this.picBoxPPT, "将PDF,Word,Excel转换为PPT幻灯片");
            this.toolTipInfo.SetToolTip(this.picBoxHTML, "将PDF,Word,Excel,PPT转换为网页文件");
            this.toolTipInfo.SetToolTip(this.picBoxTXT, "将PDF,Word,Excel,PPT转换为txt文本文件");
            this.toolTipInfo.SetToolTip(this.picBoxEPUB, "将图片转换为PDF");
            this.toolTipInfo.SetToolTip(this.picBoxPDF, "将Word,Excel,PPT转换为PDF");
            this.toolTipInfo.SetToolTip(this.picBoxComments, "查看用户使用评价");
            this.toolTipInfo.SetToolTip(this.picBoxHelp, "查看使用帮助");
            this.comboBoxPage.Width = this.lstFile.Columns[3].Width;
            this.lstFile.SmallImageList = new ImageList();
            this.lstFile.SmallImageList.ImageSize = new Size(1, 20);
            this.lstFile.Controls.Add(this.comboBoxPage);
            ini_config _config = new ini_config("config.ini");
            string str = _config.read_ini("StartPage", "App");
            string str2 = _config.read_ini("EndPage", "App");
            string str3 = _config.read_ini("TargetDic", "App");
            string str4 = _config.read_ini("PicX", "App");
            string str5 = _config.read_ini("PicY", "App");
            string str6 = _config.read_ini("isMerger", "App");
            this.cbIsMerger.Checked = str6 == "1";
            string str7 = _config.read_ini("Type_", "App");
            this.lst_select = (str7 == string.Empty) ? 0 : System.Convert.ToInt32(str7);
            if (this.lst_select == 6)
            {
                this.cbIsMerger.Visible = true;
                this.lstFile.Columns[3].Width = 0;
                this.comboBoxPage.Visible = false;
            }
            string str8 = _config.read_ini("Out", "App");
            int num = (str8 == string.Empty) ? 0 : System.Convert.ToInt32(str8);
            if (num == 0)
            {
                this.rdoPath.Checked = true;
                this.btnSelectPath.Enabled = false;
                this.txtPath.Enabled = false;
            }
            else
            {
                this.rdoNewPath.Checked = true;
                this.btnSelectPath.Enabled = true;
                this.txtPath.Enabled = true;
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
                this.txtPath.Text = str3;
            }
            else
            {
                this.txtPath.Text = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
                this.global_cfg.target_dic = this.txtPath.Text;
            }
            if (str4 != string.Empty)
            {
                this.global_cfg.pic_width = System.Convert.ToInt32(str4);
            }
            if (str5 != string.Empty)
            {
                this.global_cfg.pic_height = System.Convert.ToInt32(str5);
            }
            this.select_node();
        }

        private void picBoxComments_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/praise/?version=" + PDF_Convert.Version.version);
        }

        private void picBoxComments_MouseLeave(object sender, EventArgs e)
        {
            this.Cursor = Cursors.Arrow;
        }

        private void picBoxComments_MouseMove(object sender, MouseEventArgs e)
        {
            this.Cursor = Cursors.Hand;
        }

        private void picBoxEPUB_Click(object sender, EventArgs e)
        {
            this.cbIsMerger.Visible = true;
            this.dic_lst.Clear();
            this.cancel_hot();
            this.format = PDF_Convert.Convert.FORMAT.IMG2PDF;
            this.lstFile.Columns[3].Width = 0;
            this.comboBoxPage.Visible = false;
            this.lst_select = 6;
            this.select_hot();
        }

        private void picBoxEPUB_MouseLeave(object sender, EventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.IMG2PDF)
            {
                this.picBoxEPUB.Image = Resources.epub_gen;
            }
        }

        private void picBoxEPUB_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.IMG2PDF)
            {
                this.picBoxEPUB.Image = Resources.epub_hot;
            }
        }

        private void picBoxExcel_Click(object sender, EventArgs e)
        {
            this.cbIsMerger.Visible = false;
            this.dic_lst.Clear();
            this.cancel_hot();
            this.format = PDF_Convert.Convert.FORMAT.PDF2EXCEL;
            this.lstFile.Columns[3].Width = 90;
            this.lst_select = 1;
            this.select_hot();
        }

        private void picBoxExcel_MouseLeave(object sender, EventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2EXCEL)
            {
                this.picBoxExcel.Image = Resources.xls_gen;
            }
        }

        private void picBoxExcel_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2EXCEL)
            {
                this.picBoxExcel.Image = Resources.xls_hot;
            }
        }

        private void picBoxHelp_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/help/?version=" + PDF_Convert.Version.version);
        }

        private void picBoxHTML_Click(object sender, EventArgs e)
        {
            this.cbIsMerger.Visible = false;
            this.dic_lst.Clear();
            this.cancel_hot();
            this.format = PDF_Convert.Convert.FORMAT.PDF2HTML;
            this.lstFile.Columns[3].Width = 90;
            this.lst_select = 4;
            this.select_hot();
        }

        private void picBoxHTML_MouseLeave(object sender, EventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2HTML)
            {
                this.picBoxHTML.Image = Resources.html_gen;
            }
        }

        private void picBoxHTML_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2HTML)
            {
                this.picBoxHTML.Image = Resources.html_hot;
            }
        }

        private void picBoxIMG_Click(object sender, EventArgs e)
        {
            this.cbIsMerger.Visible = false;
            this.dic_lst.Clear();
            this.cancel_hot();
            this.format = PDF_Convert.Convert.FORMAT.PDF2IMG;
            this.lstFile.Columns[3].Width = 90;
            this.lst_select = 2;
            this.select_hot();
        }

        private void picBoxIMG_MouseLeave(object sender, EventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2IMG)
            {
                this.picBoxIMG.Image = Resources.img_gen;
            }
        }

        private void picBoxIMG_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2IMG)
            {
                this.picBoxIMG.Image = Resources.img_hot;
            }
        }

        private void picBoxPDF_Click(object sender, EventArgs e)
        {
            this.cbIsMerger.Visible = false;
            this.dic_lst.Clear();
            this.cancel_hot();
            this.format = PDF_Convert.Convert.FORMAT.DOC2PDF;
            this.lstFile.Columns[3].Width = 90;
            this.lst_select = 7;
            this.select_hot();
        }

        private void picBoxPDF_MouseLeave(object sender, EventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.DOC2PDF)
            {
                this.picBoxPDF.Image = Resources.pdf_gen;
            }
        }

        private void picBoxPDF_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.DOC2PDF)
            {
                this.picBoxPDF.Image = Resources.pdf_hot;
            }
        }

        private void picBoxPPT_Click(object sender, EventArgs e)
        {
            this.cbIsMerger.Visible = false;
            this.dic_lst.Clear();
            this.cancel_hot();
            this.format = PDF_Convert.Convert.FORMAT.PDF2PPT;
            this.lstFile.Columns[3].Width = 90;
            this.lst_select = 3;
            this.select_hot();
        }

        private void picBoxPPT_MouseLeave(object sender, EventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2PPT)
            {
                this.picBoxPPT.Image = Resources.ppt_gen;
            }
        }

        private void picBoxPPT_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2PPT)
            {
                this.picBoxPPT.Image = Resources.ppt_hot;
            }
        }

        private void picBoxQQ_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.xunjiepdf.com/software/contact/?version=" + PDF_Convert.Version.version);
        }

        private void picBoxStart_Click(object sender, EventArgs e)
        {
            if (this.lstFile.Items.Count > 0)
            {
                if ((this.lst_select == 6) && (this.lstFile.Items.Count > 3))
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
                this.global_cfg.target_dic = this.rdoNewPath.Checked ? this.txtPath.Text : "";
                Thread thread = new Thread(new ParameterizedThreadStart(this.work_thread));
                this.task.Clear();
                this.task_page.Clear();
                this.progres_list_item = 0;
                foreach (ListViewItem item3 in this.lstFile.Items)
                {
                    if (!this.is_matched(item3.SubItems[1].Text, this.lst_select))
                    {
                        if (!flag)
                        {
                            MessageBox.Show(string.Format("您选择的是{0}，但您添加的文件中含有其他类型的文件,我们将会移除相应的文件", this.get_task_name()), "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                            flag = true;
                        }
                        item3.Remove();
                    }
                    else
                    {
                        this.task.Add(item3.SubItems[1].Text);
                        this.task_page.Add(item3.SubItems[3].Text);
                    }
                }
                int num2 = 0;
                foreach (ListViewItem item3 in this.lstFile.Items)
                {
                    ++num2;
                    item3.SubItems[0].Text = num2.ToString();
                }
                this.picBoxStart.Image = Resources.start_forbid;
                this.picBoxStart.Enabled = false;
                thread.IsBackground = true;
                thread.Start();
            }
        }

        private void picBoxStart_MouseLeave(object sender, EventArgs e)
        {
            if (this.picBoxStart.Enabled)
            {
                this.picBoxStart.Image = Resources.start_gen;
            }
        }

        private void picBoxStart_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.picBoxStart.Enabled)
            {
                this.picBoxStart.Image = Resources.start_hot;
            }
        }

        private void picBoxTXT_Click(object sender, EventArgs e)
        {
            this.cbIsMerger.Visible = false;
            this.dic_lst.Clear();
            this.cancel_hot();
            this.format = PDF_Convert.Convert.FORMAT.PDF2TXT;
            this.lstFile.Columns[3].Width = 90;
            this.lst_select = 5;
            this.select_hot();
        }

        private void picBoxTXT_MouseLeave(object sender, EventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2TXT)
            {
                this.picBoxTXT.Image = Resources.txt_gen;
            }
        }

        private void picBoxTXT_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2TXT)
            {
                this.picBoxTXT.Image = Resources.txt_hot;
            }
        }

        private void picBoxWord_Click(object sender, EventArgs e)
        {
            this.cbIsMerger.Visible = false;
            this.dic_lst.Clear();
            this.cancel_hot();
            this.format = PDF_Convert.Convert.FORMAT.PDF2WORD;
            this.lstFile.Columns[3].Width = 90;
            this.lst_select = 0;
            this.select_hot();
        }

        private void picBoxWord_MouseLeave(object sender, EventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2WORD)
            {
                this.picBoxWord.Image = Resources.doc_gen;
            }
        }

        private void picBoxWord_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.format != PDF_Convert.Convert.FORMAT.PDF2WORD)
            {
                this.picBoxWord.Image = Resources.doc_hot;
            }
        }

        private void rdoNewPath_Click(object sender, EventArgs e)
        {
            this.btnSelectPath.Enabled = true;
            this.txtPath.Enabled = true;
        }

        private void rdoPath_Click(object sender, EventArgs e)
        {
            this.btnSelectPath.Enabled = false;
            this.txtPath.Enabled = false;
        }

        private void select_hot()
        {
            switch (this.format)
            {
                case PDF_Convert.Convert.FORMAT.PDF2WORD:
                    this.picBoxWord.Image = Resources.doc_tick;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2EXCEL:
                    this.picBoxExcel.Image = Resources.xls_tick;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2PPT:
                    this.picBoxPPT.Image = Resources.ppt_tick;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2HTML:
                    this.picBoxHTML.Image = Resources.html_tick;
                    break;

                case PDF_Convert.Convert.FORMAT.IMG2PDF:
                    this.picBoxEPUB.Image = Resources.epub_tick;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2IMG:
                    this.picBoxIMG.Image = Resources.img_tick;
                    break;

                case PDF_Convert.Convert.FORMAT.PDF2TXT:
                    this.picBoxTXT.Image = Resources.txt_tick;
                    break;

                default:
                    this.picBoxPDF.Image = Resources.pdf_tick;
                    break;
            }
        }

        private void select_node()
        {
            this.cancel_hot();
            switch (this.lst_select)
            {
                case 0:
                    this.format = PDF_Convert.Convert.FORMAT.PDF2WORD;
                    break;

                case 1:
                    this.format = PDF_Convert.Convert.FORMAT.PDF2EXCEL;
                    break;

                case 2:
                    this.format = PDF_Convert.Convert.FORMAT.PDF2IMG;
                    break;

                case 3:
                    this.format = PDF_Convert.Convert.FORMAT.PDF2PPT;
                    break;

                case 4:
                    this.format = PDF_Convert.Convert.FORMAT.PDF2HTML;
                    break;

                case 5:
                    this.format = PDF_Convert.Convert.FORMAT.PDF2TXT;
                    break;

                case 6:
                    this.format = PDF_Convert.Convert.FORMAT.IMG2PDF;
                    break;

                case 7:
                    this.format = PDF_Convert.Convert.FORMAT.DOC2PDF;
                    break;
            }
            this.select_hot();
        }

        private void show_progress(int cur)
        {
            this.lstFile.Items[this.progres_list_item].SubItems[5].Text = cur.ToString() + "%";
        }

        private void toolAddFiles_Click(object sender, EventArgs e)
        {
            this.btnAddFiles_Click(sender, e);
        }

        private void toolClear_Click(object sender, EventArgs e)
        {
            this.btnClean_Click(sender, e);
        }

        private void toolDelete_Click(object sender, EventArgs e)
        {
            this.btnDelete_Click(sender, e);
        }

        private void work_thread(object obj)
        {
            PDF_Convert.Convert.save_progress progress = new PDF_Convert.Convert.save_progress(this.show_progress);
            PDF_Convert.Convert.config cfg = new PDF_Convert.Convert.config();
            for (int i = 0; i < this.task.Count; i++)
            {
                PDF_Convert.Convert convert;
                cfg = this.global_cfg;
                if ((this.task_page[i] == "所有") || (this.task_page[i] == "前3页"))
                {
                    cfg.start_page = 1;
                    cfg.end_page = 0;
                }
                else
                {
                    try
                    {
                        string[] strArray = this.task_page[i].Split(new char[] { '-' });
                        cfg.start_page = System.Convert.ToInt32(strArray[0]);
                        cfg.end_page = System.Convert.ToInt32(strArray[1]);
                    }
                    catch (Exception)
                    {
                        throw;
                    }
                }
                if (this.format != PDF_Convert.Convert.FORMAT.DOC2PDF)
                {
                    convert = new PDF_Convert.Convert(this.task[i], this.format, cfg, this.lst_select);
                }
                else
                {
                    convert = new PDF_Convert.Convert(this.task[i], this.get_format(this.task[i]), cfg, this.lst_select);
                }
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
                this.progres_list_item++;
            }
            base.Invoke(new enable_button(this.enable_start_button));
            MessageBox.Show("转换全部完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            if (((this.lst_select != 6) && !this.isReg) && (MessageBox.Show("未注册用户只能转换前3页文档。请问是否立即购买注册，以转换全部内容？在您的支持下，我们会做得更好。", "提示", MessageBoxButtons.YesNo, MessageBoxIcon.Asterisk) == DialogResult.Yes))
            {
                new buyDlg().ShowDialog();
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

