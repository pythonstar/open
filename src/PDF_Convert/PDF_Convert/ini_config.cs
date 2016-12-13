namespace PDF_Convert
{
    using System;
    using System.Diagnostics;
    using System.Runtime.InteropServices;
    using System.Text;

    internal class ini_config
    {
        private string ini_name;

        public ini_config(string file_name)
        {
            this.ini_name = file_name;
        }

        private string get_app_dic()
        {
            string fileName = Process.GetCurrentProcess().MainModule.FileName;
            return fileName.Substring(0, fileName.LastIndexOf(@"\") + 1);
        }

        [DllImport("kernel32")]
        private static extern bool GetPrivateProfileString(string section, string key, string defVal, StringBuilder retVal, int size, string filePath);
        public string read_ini(string node_name, string section_name)
        {
            StringBuilder retVal = new StringBuilder(100);
            GetPrivateProfileString(section_name, node_name, "", retVal, 100, this.get_app_dic() + this.ini_name);
            return retVal.ToString();
        }

        public bool write_ini(string node_name, string str, string section_name)
        {
            return WritePrivateProfileString(section_name, node_name, str, this.get_app_dic() + this.ini_name);
        }

        [DllImport("kernel32")]
        private static extern bool WritePrivateProfileString(string section, string key, string val, string filePath);
    }
}

