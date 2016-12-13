namespace PDF_Convert
{
    using System;
    using System.Text;

    internal class Encrypt
    {
        public static string APPTITLE;

        static Encrypt()
        {
            string decryptString = new ini_config("config.ini").read_ini("Data", "App");
            if (decryptString == string.Empty)
            {
                APPTITLE = "迅捷PDF万能转换器";
            }
            else
            {
                APPTITLE = DecryptDES(decryptString);
            }
        }

        public static string DecryptDES(string decryptString)
        {
            try
            {
                string str = Encoding.Default.GetString(System.Convert.FromBase64String(decryptString));
                if (str == string.Empty)
                {
                    return "迅捷PDF万能转换器";
                }
                return str;
            }
            catch
            {
                return "迅捷PDF万能转换器";
            }
        }

        public static string EncryptDES(string encryptString)
        {
            try
            {
                return System.Convert.ToBase64String(Encoding.Default.GetBytes(encryptString));
            }
            catch
            {
                return encryptString;
            }
        }
    }
}

