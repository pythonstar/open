namespace PDF_Convert
{
    using System;
    using System.Collections.Generic;

    internal static class Version
    {
        private static string cells_ver = "7.7.1.0";
        public static Dictionary<string, string> dic_ver = new Dictionary<string, string>();
        private static string pdf_ver = "8.8.0.0";
        private static string slides_ver = "8.2.0.0";
        public static string version = "4.1";
        private static string words_ver = "13.12.0.0";

        static Version()
        {
            dic_ver.Add("exe_main", version);
            dic_ver.Add("dll_cells", cells_ver);
            dic_ver.Add("dll_pdf", pdf_ver);
            dic_ver.Add("dll_slides", slides_ver);
            dic_ver.Add("dll_words", words_ver);
        }
    }
}

