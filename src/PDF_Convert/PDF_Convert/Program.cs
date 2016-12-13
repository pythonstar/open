namespace PDF_Convert
{
    using System;
    using System.Net;
    using System.Threading;
    using System.Windows.Forms;

    internal static class Program
    {
        private static void Application_ThreadException(object sender, ThreadExceptionEventArgs e)
        {
        }

        private static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
        }

        [STAThread]
        private static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);
            Application.ThreadException += new ThreadExceptionEventHandler(Program.Application_ThreadException);
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(Program.CurrentDomain_UnhandledException);
            Common.License();
            RunForm();
        }

        private static void RunForm()
        {
            bool flag;
            Mutex mutex = new Mutex(false, "XJ_PDF_CONVERT", out flag);
            if (!flag)
            {
                MessageBox.Show("本程序已经在运行了", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            }
            else
            {
                string str = new ini_config("config.ini").read_ini("formtype", "App");
                if (((str == string.Empty) || (str == "0")) || (str != "1"))
                {
                    Application.Run(new mainDlg());
                }
                else
                {
                    Application.Run(new mainDlg_A());
                }
                try
                {
                    mutex.ReleaseMutex();
                }
                catch
                {
                }
            }
        }

        private static void visit_website()
        {
            string str = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)";
            HttpWebRequest request = WebRequest.Create(string.Format("http://www.xunjiepdf.com/software/count/software.asp?version={0}&machine={1}&method=open", PDF_Convert.Version.version, new reg().get_machine_code())) as HttpWebRequest;
            request.Method = "GET";
            request.UserAgent = str;
            request.Timeout = 0x2710;
            try
            {
                request.GetResponse();
            }
            catch
            {
            }
        }
    }
}

