namespace PDF_Convert
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Diagnostics;
    using System.Drawing;
    using System.IO;
    using System.Net;
    using System.Runtime.CompilerServices;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Threading;
    using System.Windows.Forms;

    public class Update : Form
    {
        private ColumnHeader columnHeader1;
        private ColumnHeader columnHeader2;
        private IContainer components = null;
        private List<detail_list> dl_lst = new List<detail_list>();
        private GroupBox groupBox1;
        private GroupBox groupBox2;
        private ListView lstDl;
        private string text = string.Empty;
        private TextBox txtInfo;

        public Update()
        {
            this.InitializeComponent();
        }

        private int add_list_item()
        {
            ListViewItem item = this.lstDl.Items.Add("");
            item.SubItems.Add("");
            return item.Index;
        }

        private void add_text(string text)
        {
            this.txtInfo.Text = this.txtInfo.Text + text + "\r\n";
        }

        private void create_dic()
        {
            if (!Directory.Exists(Application.StartupPath + @"\Update"))
            {
                Directory.CreateDirectory(Application.StartupPath + @"\Update");
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

        private void dl_config()
        {
            AddText method = new AddText(this.add_text);
            AddListItem item = new AddListItem(this.add_list_item);
            base.Invoke(method, new object[] { "检测是否有新版本..." });
            base.Invoke(method, new object[] { "读取远程配置文件..." });
            ini_config _config = new ini_config("config.ini");
            try
            {
                bool flag = false;
                Thread thread = new Thread(new ThreadStart(this.GetAddress)) {
                    IsBackground = true
                };
                thread.Start();
                thread.Join();
                if (!string.IsNullOrEmpty(this.text))
                {
                    if (this.text.IndexOf("#END#") == -1)
                    {
                        throw new Exception();
                    }
                    string[] strArray = this.text.Split(Environment.NewLine.ToCharArray());
                    string str = string.Empty;
                    string str2 = _config.read_ini("VeryPdf", "App");
                    string str3 = string.Empty;
                    string str4 = "";
                    for (int i = 0; i < (strArray.Length - 1); i++)
                    {
                        if (strArray[i] != string.Empty)
                        {
                            if (strArray[i].Contains("#END#"))
                            {
                                break;
                            }
                            if (strArray[i].Contains("version"))
                            {
                                str = strArray[i].Split(new char[] { ',' })[1];
                            }
                            else if (strArray[i].Contains("veryPdf"))
                            {
                                str3 = strArray[i].Split(new char[] { ',' })[1];
                            }
                            else
                            {
                                string[] strArray2 = strArray[i].Split(new char[] { ',' });
                                if (!(PDF_Convert.Version.dic_ver.TryGetValue(strArray2[0], out str4) && (System.Convert.ToInt32(PDF_Convert.Version.dic_ver[strArray2[0]].Replace(".", "")) >= System.Convert.ToInt32(strArray2[1].Replace(".", "")))))
                                {
                                    this.dl_lst.Add(new detail_list(strArray2[2], System.Convert.ToInt32(strArray2[3])));
                                    base.Invoke(item);
                                    flag = true;
                                }
                            }
                        }
                    }
                    if (!string.IsNullOrEmpty(str3))
                    {
                        try
                        {
                            if (string.IsNullOrEmpty(str2) || (System.Convert.ToDouble(str2) < System.Convert.ToDouble(str3)))
                            {
                                this.dl_lst.Add(new detail_list("VeryPdf.rar", 0x4f832a));
                                base.Invoke(item);
                                flag = true;
                            }
                        }
                        catch
                        {
                        }
                    }
                    if (flag)
                    {
                        base.Invoke(new _Show(this.show_this_window));
                        base.Invoke(method, new object[] { "开始下载新文件..." });
                        Thread thread2 = new Thread(new ThreadStart(this.dl_file)) {
                            IsBackground = true
                        };
                        thread2.Start();
                        thread2.Join();
                        ProcessStartInfo startInfo = new ProcessStartInfo(Application.StartupPath + @"\PDF Update.exe") {
                            Arguments = "Madoka " + Process.GetCurrentProcess().Id.ToString()
                        };
                        foreach (detail_list _list in this.dl_lst)
                        {
                            startInfo.Arguments = startInfo.Arguments + string.Format(" \"{0}\"", _list.file_name);
                        }
                        Process.Start(startInfo);
                        base.Invoke(method, new object[] { "下载新文件成功,程序即将退出以便进行更新" });
                        Thread.Sleep(0x3e8);
                        _config.write_ini("Version", str, "App");
                        _config.write_ini("VeryPdf", str3, "App");
                    }
                    base.Invoke(new _Exit(this.Close));
                }
            }
            catch
            {
                base.Invoke(new _Exit(this.Close));
            }
        }

        private void dl_file()
        {
            string str = "http://www.xunjiepdf.com/software/update1.1/";
            SetListInfo method = new SetListInfo(this.set_list_info);
            this.create_dic();
            byte[] buffer = new byte[0x2800];
            int num2 = 0;
            for (int i = 0; i < this.dl_lst.Count; i++)
            {
                base.Invoke(method, new object[] { i, 0, this.dl_lst[i].file_name });
                Stream stream = new WebClient().OpenRead(str + this.dl_lst[i].file_name);
                FileStream stream2 = new FileStream(Application.StartupPath + @"\Update\" + this.dl_lst[i].file_name, FileMode.Create);
                for (int j = stream.Read(buffer, 0, 0x2800); j != 0; j = stream.Read(buffer, 0, 0x2800))
                {
                    stream2.Write(buffer, 0, j);
                    num2 += j;
                    base.Invoke(method, new object[] { i, 1, System.Convert.ToInt32((float) ((((float) num2) / ((float) this.dl_lst[i].file_size)) * 100f)).ToString() + "%" });
                }
                base.Invoke(method, new object[] { i, 1, "100%" });
                stream2.Close();
                stream.Close();
                num2 = 0;
            }
        }

        private void GetAddress()
        {
            try
            {
                WebResponse response = null;
                WebRequest request = WebRequest.Create("http://www.xunjiepdf.com/software/update1.1/config.txt");
                request.Timeout = 0x1770;
                response = request.GetResponse();
                this.text = new StreamReader(response.GetResponseStream(), Encoding.GetEncoding("GB2312")).ReadToEnd();
            }
            catch (Exception)
            {
            }
        }

        private void hide()
        {
            base.WindowState = FormWindowState.Minimized;
            base.ShowInTaskbar = false;
        }

        private void InitializeComponent()
        {
            ComponentResourceManager manager = new ComponentResourceManager(typeof(Update));
            this.groupBox1 = new GroupBox();
            this.txtInfo = new TextBox();
            this.groupBox2 = new GroupBox();
            this.lstDl = new ListView();
            this.columnHeader1 = new ColumnHeader();
            this.columnHeader2 = new ColumnHeader();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            base.SuspendLayout();
            this.groupBox1.Controls.Add(this.txtInfo);
            this.groupBox1.Location = new Point(2, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new Size(240, 100);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "日志窗口";
            this.txtInfo.Location = new Point(9, 0x11);
            this.txtInfo.Multiline = true;
            this.txtInfo.Name = "txtInfo";
            this.txtInfo.Size = new Size(0xdf, 0x4d);
            this.txtInfo.TabIndex = 0;
            this.groupBox2.Controls.Add(this.lstDl);
            this.groupBox2.Location = new Point(2, 0x67);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new Size(240, 0x97);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "下载文件";
            this.lstDl.Columns.AddRange(new ColumnHeader[] { this.columnHeader1, this.columnHeader2 });
            this.lstDl.FullRowSelect = true;
            this.lstDl.GridLines = true;
            this.lstDl.Location = new Point(10, 20);
            this.lstDl.Name = "lstDl";
            this.lstDl.Size = new Size(0xde, 0x7d);
            this.lstDl.TabIndex = 0;
            this.lstDl.UseCompatibleStateImageBehavior = false;
            this.lstDl.View = View.Details;
            this.columnHeader1.Text = "文件";
            this.columnHeader1.Width = 140;
            this.columnHeader2.Text = "进度";
            this.columnHeader2.Width = 70;
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(0xf6, 0x105);
            base.Controls.Add(this.groupBox2);
            base.Controls.Add(this.groupBox1);
            this.DoubleBuffered = true;
            base.FormBorderStyle = FormBorderStyle.FixedSingle;
            base.Icon = (Icon) manager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "Update";
            base.ShowInTaskbar = false;
            base.StartPosition = FormStartPosition.CenterScreen;
            this.Text = "Update";
            base.WindowState = FormWindowState.Minimized;
            base.Load += new EventHandler(this.Update_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            base.ResumeLayout(false);
        }

        private void set_list_info(int row_index, int hor_index, string t)
        {
            try
            {
                this.lstDl.Items[row_index].SubItems[hor_index].Text = t;
            }
            catch
            {
            }
        }

        private void show_this_window()
        {
            base.WindowState = FormWindowState.Normal;
            base.ShowInTaskbar = true;
        }

        private void Update_Load(object sender, EventArgs e)
        {
            Thread thread = new Thread(new ThreadStart(this.dl_config));
            this.Text = "自动升级";
            thread.IsBackground = true;
            thread.Start();
            this.hide();
        }

        private delegate void _Exit();

        private delegate void _Show();

        private delegate int AddListItem();

        private delegate void AddText(string text);

        [StructLayout(LayoutKind.Sequential)]
        private struct detail_list
        {
            public string file_name;
            public int file_size;
            public detail_list(string file, int size)
            {
                this.file_name = file;
                this.file_size = size;
            }
        }

        private delegate void SetListInfo(int row_index, int hor_index, string t);
    }
}

