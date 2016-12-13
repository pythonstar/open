namespace PDF_Convert
{
    using System;
    using System.Management;
    using System.Runtime.InteropServices;
    using System.Security.Cryptography;
    using System.Text;

    internal class reg
    {
        private readonly int[] encode = new int[] { 8, 7, 8, 6, 5, 7, 9, 9, 8 };
        private ini_config ini = new ini_config("config.ini");
        private registry register = new registry("XJPDF Convert");

        private int get_cpu_code()
        {
            string str = "";
            string str2 = "";
            ManagementObjectCollection instances = new ManagementClass("Win32_Processor").GetInstances();
            foreach (ManagementObject obj2 in instances)
            {
                str = obj2.Properties["ProcessorId"].Value.ToString();
                break;
            }
            if (str == string.Empty)
            {
                return 0;
            }
            if (str.Length == 0x10)
            {
                str = string.Format("{0}-{1}-00000000-00000000", str.Substring(0, 8), str.Substring(8, 8));
            }
            else if (str.Length == 0x20)
            {
                str = string.Format("{0}-{1}-{2}-{3}", new object[] { str.Substring(0, 8), str.Substring(8, 8), str.Substring(0x10, 8), str.Substring(0x18, 8) });
            }
            str2 = this.get_md5(str, 0x20, true);
            string s = "";
            string str4 = "";
            for (int i = 1; i <= 0x1b; i += 3)
            {
                s = str2.Substring(i + 2, 1);
                int num = Encoding.ASCII.GetBytes(s)[0];
                int num2 = num % this.encode[((i + 2) / 3) - 1];
                if (0 == num2)
                {
                    num2 = 8;
                }
                str4 = str4 + num2.ToString();
            }
            return int.Parse(str4);
        }

        public string get_machine_code()
        {
            return this.get_cpu_code().ToString();
        }

        private string get_md5(string str, int bit, bool lower_case)
        {
            string str2 = "";
            byte[] buffer = new MD5CryptoServiceProvider().ComputeHash(Encoding.ASCII.GetBytes(str));
            for (int i = 0; i < buffer.Length; i++)
            {
                if (0x20 == bit)
                {
                    str2 = str2 + System.Convert.ToString(buffer[i], 0x10).PadLeft(2, '0');
                }
                else
                {
                    str2 = str2 + System.Convert.ToString(buffer[i], 0x10);
                }
            }
            return (lower_case ? str2 : str2.ToUpper());
        }

        public string get_reg_code(string machine_code)
        {
            if (machine_code == string.Empty)
            {
                machine_code = this.get_machine_code();
            }
            long num = System.Convert.ToInt64(machine_code);
            string str = "";
            for (int i = 0; i < 100; i++)
            {
                num *= 2L;
                str = num.ToString();
                if (str.Length <= 12)
                {
                    num = System.Convert.ToInt64(str);
                }
                else
                {
                    num = System.Convert.ToInt64(str.Substring(0, 12));
                }
            }
            //str = num.ToString();
            return num.ToString();
        }

        public bool is_reg()
        {
            return (this.ini.read_ini("RegCode", "App") == this.get_reg_code(""));
        }

        public bool write_reg_code(string reg_code)
        {
            return this.ini.write_ini("RegCode", reg_code, "App");
        }
    }
}

