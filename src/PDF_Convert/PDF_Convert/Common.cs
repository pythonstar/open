namespace PDF_Convert
{
    using Aspose.Cells;
    using Aspose.Pdf;
    using Aspose.Pdf.Exceptions;
    using Aspose.Slides;
    using Aspose.Words;
    using System;
    using System.Diagnostics;
    using System.IO;

    public static class Common
    {
        public static string Execute(string dosCommand)
        {
            return Execute(dosCommand, 10);
        }

        public static string Execute(string command, int seconds)
        {
            string str = "";
            if ((command != null) && !command.Equals(""))
            {
                Process process = new Process();
                ProcessStartInfo info = new ProcessStartInfo {
                    FileName = "cmd.exe",
                    Arguments = "/C " + command,
                    UseShellExecute = false,
                    RedirectStandardInput = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                };
                process.StartInfo = info;
                try
                {
                    if (!process.Start())
                    {
                        return str;
                    }
                    if (seconds == 0)
                    {
                        process.WaitForExit();
                    }
                    else
                    {
                        process.WaitForExit(seconds);
                    }
                    return process.StandardOutput.ReadToEnd();
                }
                catch (Exception)
                {
                }
                finally
                {
                    if (process != null)
                    {
                        process.Close();
                    }
                }
            }
            return str;
        }

        public static int GetFilePage(string filePath)
        {
            int count = 0;
            string extension = Path.GetExtension(filePath);
            string fileName = Path.GetFileName(filePath);
            string password = new ini_config("config.ini").read_ini("pwd", "App");
            switch (extension.ToLower())
            {
                case ".pdf":
                    try
                    {
                        Aspose.Pdf.Document document = new Aspose.Pdf.Document(filePath, password);
                        count = document.Pages.Count;
                    }
                    catch (Aspose.Pdf.Exceptions.InvalidPasswordException exception)
                    {
                        throw new Exception(exception.Message);
                    }
                    return count;

                case ".doc":
                {
                    Aspose.Words.Document document2 = new Aspose.Words.Document(filePath);
                    return document2.PageCount;
                }
                case ".docx":
                {
                    Aspose.Words.Document document3 = new Aspose.Words.Document(filePath);
                    return document3.PageCount;
                }
                case ".xls":
                {
                    Workbook workbook = new Workbook(filePath);
                    return workbook.Worksheets.Count;
                }
                case ".xlsx":
                {
                    Workbook workbook2 = new Workbook(filePath);
                    return workbook2.Worksheets.Count;
                }
                case ".ppt":
                {
                    Presentation presentation = new Presentation(filePath);
                    return presentation.Slides.Count;
                }
                case ".pptx":
                {
                    Presentation presentation2 = new Presentation(filePath);
                    return presentation2.Slides.Count;
                }
            }
            return 1;
        }

        public static void License()
        {
            string licenseName = AppDomain.CurrentDomain.SetupInformation.ApplicationBase + @"\Aspose.Total.lic";
            new Aspose.Pdf.License().SetLicense(licenseName);
            new Aspose.Words.License().SetLicense(licenseName);
            new Aspose.Cells.License().SetLicense(licenseName);
            new Aspose.Slides.License().SetLicense(licenseName);
        }
    }
}

