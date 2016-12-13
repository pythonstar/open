namespace PDF_Convert
{
    using Microsoft.Win32;
    using System;

    internal class registry
    {
        public readonly int READ_ERROR = -9999;
        private string reg_item;

        public registry(string item)
        {
            this.reg_item = item;
        }

        private bool create_reg_item(string item)
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("software", true);
                key.CreateSubKey(item);
                key.Close();
                return true;
            }
            catch
            {
                return false;
            }
        }

        public byte[] get_reg_byte(string item)
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey(@"software\" + this.reg_item, false);
                byte[] buffer = (byte[]) key.GetValue(item);
                key.Close();
                return buffer;
            }
            catch
            {
                return null;
            }
        }

        public int get_reg_int(string item)
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey(@"software\" + this.reg_item, false);
                int num = System.Convert.ToInt32(key.GetValue(item));
                key.Close();
                return num;
            }
            catch
            {
                return this.READ_ERROR;
            }
        }

        public string get_reg_string(string item)
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey(@"software\" + this.reg_item, false);
                string str = key.GetValue(item).ToString();
                key.Close();
                return str;
            }
            catch
            {
                return "";
            }
        }

        public bool is_reg_exist(string item)
        {
            try
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("software", false);
                string[] subKeyNames = key.GetSubKeyNames();
                foreach (string str in subKeyNames)
                {
                    if (item == str)
                    {
                        goto Label_005C;
                    }
                }
                key.Close();
                return false;
            Label_005C:
                key.Close();
                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool set_reg_byte(string item, ref byte[] data)
        {
            try
            {
                if (!this.is_reg_exist(this.reg_item))
                {
                    this.create_reg_item(this.reg_item);
                }
                RegistryKey key = Registry.CurrentUser.OpenSubKey(@"software\" + this.reg_item, true);
                key.SetValue(item, data);
                key.Close();
                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool set_reg_int(string item, int data)
        {
            try
            {
                if (!this.is_reg_exist(this.reg_item))
                {
                    this.create_reg_item(this.reg_item);
                }
                RegistryKey key = Registry.CurrentUser.OpenSubKey(@"software\" + this.reg_item, true);
                key.SetValue(item, data);
                key.Close();
                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool set_reg_string(string item, string data)
        {
            try
            {
                if (!this.is_reg_exist(this.reg_item))
                {
                    this.create_reg_item(this.reg_item);
                }
                RegistryKey key = Registry.CurrentUser.OpenSubKey("software" + this.reg_item, true);
                key.SetValue(item, data);
                key.Close();
                return true;
            }
            catch
            {
                return false;
            }
        }
    }
}

