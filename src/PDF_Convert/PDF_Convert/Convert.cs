namespace PDF_Convert
{
    using Aspose.Cells;
    using Aspose.Pdf;
    using Aspose.Pdf.Devices;
    using Aspose.Pdf.Exceptions;
    using Aspose.Pdf.Facades;
    using Aspose.Pdf.Generator;
    using Aspose.Slides;
    using Aspose.Slides.Export;
    using Aspose.Words;
    using System;
    using System.Collections.Generic;
    using System.Drawing;
    using System.IO;
    using System.Runtime.CompilerServices;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Threading;
    using System.Windows.Forms;
    using System.Xml;

    public class Convert
    {
        private string err_msg = "";
        private Workbook excel_doc;
        private bool file_can_work = true;
        private string file_path;
        private FileStream file_stream;
        private Dictionary<FORMAT, string> format_text;
        private config global_config;
        private int pages;
        private Aspose.Pdf.Document pdf_doc;
        private Presentation ppt_doc;
        private const int START_PAGE = 1;
        private FORMAT target_format;
        private PdfExtractor txt_doc;
        private Aspose.Words.Document word_doc;

        public Convert(string file_path, FORMAT format, config cfg, int lst_select)
        {
            this.file_path = file_path;
            this.global_config = cfg;
            this.target_format = format;
            try
            {
                this.file_stream = new FileStream(file_path, FileMode.Open, FileAccess.Read);
            }
            catch (Exception exception)
            {
                if (exception.Message.Contains("正由另一进程使用"))
                {
                    MessageBox.Show("您的 " + file_path + " 文件已打开，请先关闭文件再进行转换！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                }
                return;
            }
            try
            {
                switch (Path.GetExtension(file_path).ToLower())
                {
                    case ".pdf":
                        this.pdf_doc = new Aspose.Pdf.Document(this.file_stream, this.global_config.password);
                        this.pages = this.pdf_doc.Pages.Count;
                        return;

                    case ".doc":
                    case ".docx":
                        this.word_doc = new Aspose.Words.Document(this.file_stream);
                        this.pages = this.word_doc.PageCount;
                        break;

                    case ".ppt":
                    case ".pptx":
                        this.ppt_doc = new Presentation(this.file_stream);
                        this.pages = this.ppt_doc.Slides.Count;
                        break;

                    case ".xls":
                    case ".xlsx":
                        this.excel_doc = new Workbook(this.file_stream);
                        this.pages = this.excel_doc.Worksheets.Count;
                        break;
                }
            }
            catch (Aspose.Pdf.Exceptions.InvalidPasswordException)
            {
                passwordDlg dlg = new passwordDlg(Path.GetFileName(this.file_path));
                DialogResult result = dlg.ShowDialog();
                bool flag = true;
                while ((result == DialogResult.OK) && flag)
                {
                    try
                    {
                        this.file_stream = new FileStream(file_path, FileMode.Open, FileAccess.Read);
                        this.pdf_doc = new Aspose.Pdf.Document(this.file_stream, dlg.new_password);
                        this.pages = this.pdf_doc.Pages.Count;
                        flag = false;
                    }
                    catch (Aspose.Pdf.Exceptions.InvalidPasswordException)
                    {
                        result = dlg.ShowDialog();
                    }
                }
                if (result == DialogResult.Cancel)
                {
                    this.err_msg = "";
                    this.file_can_work = false;
                }
            }
            catch (Exception)
            {
                this.file_can_work = false;
                this.err_msg = "发生未知错误";
            }
        }

        public bool can_work()
        {
            return this.file_can_work;
        }

        public void close()
        {
            try
            {
                if (this.file_stream != null)
                {
                    this.file_stream.Close();
                }
            }
            catch
            {
            }
        }

        private void create_dic()
        {
            if (!Directory.Exists(this.global_config.target_dic))
            {
                Directory.CreateDirectory(this.global_config.target_dic);
            }
        }

        private void create_temp_dic()
        {
            if (!Directory.Exists(this.global_config.target_dic + "TEMP"))
            {
                Directory.CreateDirectory(this.global_config.target_dic + "TEMP");
            }
        }

        private void delete_temp_dic()
        {
            Directory.Delete(this.get_temp_dic());
        }

        private Aspose.Pdf.Document doc_to_pdf(save_progress progress, System.Windows.Forms.Form dlg, int lst_select)
        {
            int num = 50;
            if (lst_select == 7)
            {
                num = 100;
            }
            Aspose.Pdf.Document document2 = new Aspose.Pdf.Document();
            try
            {
                document2.Pages.Delete();
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { 0 });
                }
                if (this.word_doc == null)
                {
                    return null;
                }
                for (int i = 0; i < this.word_doc.ChildNodes.Count; i++)
                {
                    MemoryStream stream = new MemoryStream();
                    Aspose.Words.Document document = new Aspose.Words.Document();
                    document.ChildNodes.RemoveAt(0);
                    document.AppendChild(document.ImportNode(this.word_doc.ChildNodes[i], true));
                    document.Save(stream, Aspose.Words.SaveFormat.Pdf);
                    document2.Pages.Add(new Aspose.Pdf.Document(stream).Pages);
                    if (progress != null)
                    {
                        dlg.Invoke(progress, new object[] { (i * num) / this.word_doc.ChildNodes.Count });
                    }
                }
                if (lst_select == 7)
                {
                    document2.Save(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + this.get_suffix());
                }
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { num });
                }
            }
            catch (Exception)
            {
                return null;
            }
            return document2;
        }

        public string get_err_msg()
        {
            return this.err_msg;
        }

        private ORIGIN_FORMAT get_orgin_format()
        {
            if (this.target_format == FORMAT.IMG2PDF)
            {
                return ORIGIN_FORMAT.IMG;
            }
            if (this.target_format <= FORMAT.PDF2TXT)
            {
                return ORIGIN_FORMAT.PDF;
            }
            if (this.target_format == FORMAT.DOC2PDF)
            {
                return ORIGIN_FORMAT.WORD;
            }
            if (this.target_format == FORMAT.XLS2PDF)
            {
                return ORIGIN_FORMAT.EXCEL;
            }
            if (this.target_format == FORMAT.IMG2PDF)
            {
                return ORIGIN_FORMAT.PDF;
            }
            return ORIGIN_FORMAT.PPT;
        }

        public int get_pages_count()
        {
            return this.pages;
        }

        private string get_path(string path, bool need_file_name)
        {
            if (need_file_name)
            {
                if (path.Substring(path.Length - 1, 1) == @"\")
                {
                    path = path + Path.GetFileNameWithoutExtension(path);
                    return path;
                }
                path = path.Substring(0, path.LastIndexOf(@"\") + 1) + Path.GetFileNameWithoutExtension(path);
                return path;
            }
            if (path.Substring(path.Length - 1, 1) == @"\")
            {
                return path;
            }
            return path.Substring(0, path.LastIndexOf(@"\") + 1);
        }

        private string get_suffix()
        {
            if (this.format_text == null)
            {
                this.format_text = new Dictionary<FORMAT, string>();
                this.format_text.Add(FORMAT.PDF2WORD, ".doc");
                this.format_text.Add(FORMAT.PDF2EXCEL, ".xls");
                this.format_text.Add(FORMAT.PDF2PPT, ".ppt");
                this.format_text.Add(FORMAT.PDF2HTML, ".html");
                this.format_text.Add(FORMAT.IMG2PDF, ".pdf");
                this.format_text.Add(FORMAT.PDF2IMG, ".jpg");
                this.format_text.Add(FORMAT.PDF2TXT, ".txt");
                this.format_text.Add(FORMAT.DOC2PDF, ".pdf");
                this.format_text.Add(FORMAT.XLS2PDF, ".pdf");
                this.format_text.Add(FORMAT.PPT2PDF, ".pdf");
            }
            return this.format_text[this.target_format];
        }

        private string get_temp_dic()
        {
            return (this.global_config.target_dic + @"TEMP\");
        }

        private void pdf_to_epub(save_progress progress, System.Windows.Forms.Form dlg, int c)
        {
            try
            {
                Pdf pdf;
                ListViewItem item;
                string text;
                Aspose.Pdf.Generator.Section section;
                Aspose.Pdf.Generator.Image image;
                string str4;
                string extension;
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { 0 });
                }
                string str = new ini_config("config.ini").read_ini("isMerger", "App");
                ListView.ListViewItemCollection items = null;
                int num = 0;
                if (dlg.Name == "mainDlg")
                {
                    items = ((mainDlg) dlg).lstFile.Items;
                    num = 0;
                }
                else
                {
                    items = ((mainDlg_A) dlg).lstFile.Items;
                    num = 1;
                }
                if (str == "1")
                {
                    if (c == (items.Count - 1))
                    {
                        pdf = new Pdf();
                        for (int i = 0; i < items.Count; i++)
                        {
                            item = items[i];
                            text = item.SubItems[num].Text;
                            section = pdf.Sections.Add();
                            image = new Aspose.Pdf.Generator.Image(section);
                            section.Paragraphs.Add(image);
                            image.ImageInfo.File = text;
                            extension = Path.GetExtension(text);
                            if (extension == null)
                            {
                                goto Label_01C8;
                            }
                            if (!(extension == ".jpg"))
                            {
                                if (extension == ".gif")
                                {
                                    goto Label_0188;
                                }
                                if (extension == ".bmp")
                                {
                                    goto Label_0198;
                                }
                                if (extension == ".png")
                                {
                                    goto Label_01A8;
                                }
                                if (extension == ".tiff")
                                {
                                    goto Label_01B8;
                                }
                                goto Label_01C8;
                            }
                            image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Jpeg;
                            continue;
                        Label_0188:
                            image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Gif;
                            continue;
                        Label_0198:
                            image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Bmp;
                            continue;
                        Label_01A8:
                            image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Png;
                            continue;
                        Label_01B8:
                            image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Tiff;
                            continue;
                        Label_01C8:
                            image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Jpeg;
                        }
                        str4 = this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + this.get_suffix();
                        pdf.Save(str4);
                    }
                    goto Label_0366;
                }
                pdf = new Pdf();
                item = items[c];
                text = item.SubItems[num].Text;
                section = pdf.Sections.Add();
                image = new Aspose.Pdf.Generator.Image(section);
                section.Paragraphs.Add(image);
                image.ImageInfo.File = text;
                extension = Path.GetExtension(text);
                if (extension == null)
                {
                    goto Label_0329;
                }
                if (!(extension == ".jpg"))
                {
                    if (extension == ".gif")
                    {
                        goto Label_02E9;
                    }
                    if (extension == ".bmp")
                    {
                        goto Label_02F9;
                    }
                    if (extension == ".png")
                    {
                        goto Label_0309;
                    }
                    if (extension == ".tiff")
                    {
                        goto Label_0319;
                    }
                    goto Label_0329;
                }
                image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Jpeg;
                goto Label_0339;
            Label_02E9:
                image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Gif;
                goto Label_0339;
            Label_02F9:
                image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Bmp;
                goto Label_0339;
            Label_0309:
                image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Png;
                goto Label_0339;
            Label_0319:
                image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Tiff;
                goto Label_0339;
            Label_0329:
                image.ImageInfo.ImageFileType = Aspose.Pdf.Generator.ImageFileType.Jpeg;
            Label_0339:
                str4 = this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + this.get_suffix();
                pdf.Save(str4);
            Label_0366:
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { 100 });
                }
            }
            catch (Exception)
            {
            }
        }

        private void pdf_to_excel(save_progress progress, System.Windows.Forms.Form dlg, string fileType)
        {
            Aspose.Pdf.Document document = null;
            int num = 0;
            try
            {
                if (fileType == ".pdf")
                {
                    document = this.pdf_doc;
                    num = 0;
                }
                else if ((fileType == ".ppt") || (fileType == ".pptx"))
                {
                    document = this.ppt_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                else if ((fileType == ".doc") || (fileType == ".docx"))
                {
                    document = this.doc_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                Workbook workbook = new Workbook();
                Aspose.Pdf.Document document2 = new Aspose.Pdf.Document();
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { num });
                }
                workbook.Worksheets.Clear();
                for (int i = 1; i <= document.Pages.Count; i++)
                {
                    MemoryStream outputStream = new MemoryStream();
                    document2.Pages.Add(document.Pages[i]);
                    document2.Save(outputStream, Aspose.Pdf.SaveFormat.Excel);
                    Workbook workbook2 = new Workbook(outputStream);
                    workbook.Worksheets.Add(i.ToString());
                    workbook.Worksheets[i - 1].Copy(workbook2.Worksheets[0]);
                    document2.Pages.Delete();
                    if (progress != null)
                    {
                        if (num == 50)
                        {
                            dlg.Invoke(progress, new object[] { ((i * 50) / document.Pages.Count) + 50 });
                        }
                        else
                        {
                            dlg.Invoke(progress, new object[] { (i * 100) / this.pdf_doc.Pages.Count });
                        }
                    }
                }
                workbook.Save(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + ".xls");
            }
            catch (Exception)
            {
            }
        }

        private void pdf_to_html(save_progress progress, System.Windows.Forms.Form dlg, string fileType)
        {
            int num = 0;
            if (fileType == ".pdf")
            {
                num = 0;
            }
            else if ((fileType == ".doc") || (fileType == ".docx"))
            {
                this.pdf_doc = this.doc_to_pdf(progress, dlg, 0);
                num = 50;
            }
            else if ((fileType == ".xls") || (fileType == ".xlsx"))
            {
                this.pdf_doc = this.xls_to_pdf(progress, dlg, 0);
                num = 50;
            }
            else if ((fileType == ".ppt") || (fileType == ".pptx"))
            {
                this.pdf_doc = this.ppt_to_pdf(progress, dlg, 0);
                num = 50;
            }
            new Thread(new ThreadStart(this.pdf_to_html_callback)).Start();
            int num2 = 1;
            int num3 = num2;
            int count = this.pdf_doc.Pages.Count;
            if (progress != null)
            {
                dlg.Invoke(progress, new object[] { num });
            }
            while (true)
            {
                bool flag2 = true;
                num3 = num2;
                try
                {
                    if ((Directory.GetFiles(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + "_files").Length != 0) || (Directory.GetFiles(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + "_files", "img_" + num2.ToString().PadLeft(2, '0') + ".*").Length != 0))
                    {
                        num2++;
                    }
                }
                catch
                {
                }
                if (num2 == count)
                {
                    if (progress != null)
                    {
                        try
                        {
                            bool flag = false;
                            goto Label_028D;
                        Label_0250:
                            if (Directory.GetFiles(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + "_files", "style.css").Length != 0)
                            {
                                flag = true;
                                goto Label_0292;
                            }
                        Label_028D:
                            flag2 = true;
                            goto Label_0250;
                        Label_0292:
                            if (flag)
                            {
                                dlg.Invoke(progress, new object[] { 100 });
                            }
                        }
                        catch (Exception)
                        {
                        }
                    }
                    return;
                }
                if (progress != null)
                {
                    if (num == 50)
                    {
                        dlg.Invoke(progress, new object[] { ((num2 * 50) / count) + 50 });
                    }
                    else
                    {
                        dlg.Invoke(progress, new object[] { (num2 * 100) / count });
                    }
                }
                Thread.Sleep(30);
            }
        }

        private void pdf_to_html_callback()
        {
            try
            {
                this.pdf_doc.Save(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + this.get_suffix(), Aspose.Pdf.SaveFormat.Html);
            }
            catch (Exception)
            {
            }
        }

        private void pdf_to_img(save_progress progress, System.Windows.Forms.Form dlg, string fileType)
        {
            try
            {
                Aspose.Pdf.Document document = null;
                int num = 0;
                if (fileType == ".pdf")
                {
                    document = this.pdf_doc;
                    num = 0;
                }
                else if ((fileType == ".doc") || (fileType == ".docx"))
                {
                    document = this.doc_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                else if ((fileType == ".xls") || (fileType == ".xlsx"))
                {
                    document = this.xls_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                else if ((fileType == ".ppt") || (fileType == ".pptx"))
                {
                    document = this.ppt_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                if (document != null)
                {
                    try
                    {
                        JpegDevice device = new JpegDevice(new Resolution(300), 100);
                        if (progress != null)
                        {
                            dlg.Invoke(progress, new object[] { num });
                        }
                        for (int i = 1; i <= document.Pages.Count; i++)
                        {
                            device.Process(document.Pages[i], this.global_config.target_dic + i.ToString() + this.get_suffix());
                            if (progress != null)
                            {
                                if (num == 50)
                                {
                                    dlg.Invoke(progress, new object[] { ((i * 50) / document.Pages.Count) + 50 });
                                }
                                else
                                {
                                    dlg.Invoke(progress, new object[] { (i * 100) / document.Pages.Count });
                                }
                            }
                        }
                    }
                    catch (Exception)
                    {
                        return;
                    }
                    if (progress != null)
                    {
                        dlg.Invoke(progress, new object[] { 100 });
                    }
                }
            }
            catch (Exception)
            {
                throw;
            }
        }

        private void pdf_to_ppt(save_progress progress, System.Windows.Forms.Form dlg, string fileType)
        {
            try
            {
                Aspose.Pdf.Document document = null;
                int num = 0;
                if (fileType == ".pdf")
                {
                    document = this.pdf_doc;
                    num = 0;
                }
                else if ((fileType == ".doc") || (fileType == ".docx"))
                {
                    document = this.doc_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                else if ((fileType == ".xls") || (fileType == ".xlsx"))
                {
                    document = this.xls_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                Presentation presentation = new Presentation();
                JpegDevice device = new JpegDevice(new Resolution(300), 100);
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { num });
                }
                int num3 = 0;
                for (int i = 1; num3 < document.Pages.Count; i++)
                {
                    presentation.Slides.AddEmptySlide(presentation.LayoutSlides[0]);
                    presentation.Slides[num3].Shapes.AddAutoShape(Aspose.Slides.ShapeType.Rectangle, 10f, 20f, (float) this.global_config.pic_width, (float) this.global_config.pic_height);
                    int num2 = presentation.Slides[num3].Shapes.Count - 1;
                    presentation.Slides[num3].Shapes[num2].FillFormat.FillType = FillType.Picture;
                    presentation.Slides[num3].Shapes[num2].FillFormat.PictureFillFormat.PictureFillMode = PictureFillMode.Stretch;
                    MemoryStream output = new MemoryStream();
                    device.Process(document.Pages[i], output);
                    IPPImage image = presentation.Images.AddImage(new Bitmap(output));
                    presentation.Slides[num3].Shapes[num2].FillFormat.PictureFillFormat.Picture.Image = image;
                    if (progress != null)
                    {
                        if (num == 50)
                        {
                            dlg.Invoke(progress, new object[] { ((num3 * 50) / document.Pages.Count) + 50 });
                        }
                        else
                        {
                            dlg.Invoke(progress, new object[] { (num3 * 100) / document.Pages.Count });
                        }
                    }
                    num3++;
                }
                presentation.Save(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + this.get_suffix(), Aspose.Slides.Export.SaveFormat.Ppt);
            }
            catch (Exception)
            {
                return;
            }
            if (progress != null)
            {
                dlg.Invoke(progress, new object[] { 100 });
            }
        }

        private void pdf_to_txt(save_progress progress, System.Windows.Forms.Form dlg, string fileType)
        {
            try
            {
                Aspose.Pdf.Document document = null;
                int num = 0;
                if (fileType == ".pdf")
                {
                    document = this.pdf_doc;
                    num = 0;
                }
                else if ((fileType == ".doc") || (fileType == ".docx"))
                {
                    document = this.doc_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                else if ((fileType == ".xls") || (fileType == ".xlsx"))
                {
                    document = this.xls_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                else if ((fileType == ".ppt") || (fileType == ".pptx"))
                {
                    document = this.ppt_to_pdf(progress, dlg, 0);
                    num = 50;
                }
                PdfExtractor extractor = new PdfExtractor(document);
                FileStream outputStream = new FileStream(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + this.get_suffix(), FileMode.Create);
                extractor.ExtractTextMode = 0;
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { num });
                }
                for (int i = 1; i <= document.Pages.Count; i++)
                {
                    extractor.StartPage = i;
                    extractor.EndPage = i;
                    extractor.ExtractText(Encoding.UTF8);
                    extractor.GetText(outputStream);
                    if (progress != null)
                    {
                        if (num == 50)
                        {
                            dlg.Invoke(progress, new object[] { ((i * 50) / document.Pages.Count) + 50 });
                        }
                        else
                        {
                            dlg.Invoke(progress, new object[] { (i * 100) / document.Pages.Count });
                        }
                    }
                }
                outputStream.Close();
            }
            catch (Exception)
            {
                return;
            }
            if (progress != null)
            {
                dlg.Invoke(progress, new object[] { 100 });
            }
        }

        private void pdf_to_word(save_progress progress, System.Windows.Forms.Form dlg, string fileType)
        {
            Aspose.Pdf.Document document = null;
            int num = 0;
            if (fileType == ".pdf")
            {
                document = this.pdf_doc;
                num = 0;
            }
            else if ((fileType == ".ppt") || (fileType == ".pptx"))
            {
                document = this.ppt_to_pdf(progress, dlg, 0);
                num = 50;
            }
            else if ((fileType == ".xls") || (fileType == ".xlsx"))
            {
                document = this.xls_to_pdf(progress, dlg, 0);
                num = 50;
            }
            Aspose.Words.Document document2 = new Aspose.Words.Document();
            Aspose.Pdf.Document document3 = new Aspose.Pdf.Document();
            if (progress != null)
            {
                dlg.Invoke(progress, new object[] { num });
            }
            document2.ChildNodes.Clear();
            for (int i = 1; i <= document.Pages.Count; i++)
            {
                try
                {
                    MemoryStream outputStream = new MemoryStream();
                    document3.Pages.Add(document.Pages[i]);
                    document3.Save(outputStream, Aspose.Pdf.SaveFormat.Doc);
                    document2.AppendDocument(new Aspose.Words.Document(outputStream), ImportFormatMode.KeepSourceFormatting);
                    document3.Pages.Delete();
                }
                catch (Exception)
                {
                    break;
                }
                if (progress != null)
                {
                    if (num == 50)
                    {
                        dlg.Invoke(progress, new object[] { ((i * 50) / document.Pages.Count) + 50 });
                    }
                    else
                    {
                        dlg.Invoke(progress, new object[] { (i * 100) / this.pdf_doc.Pages.Count });
                    }
                }
            }
            document2.Save(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + this.get_suffix());
            if (progress != null)
            {
                dlg.Invoke(progress, new object[] { 100 });
            }
        }

        private Aspose.Pdf.Document ppt_to_pdf(save_progress progress, System.Windows.Forms.Form dlg, int lst_select)
        {
            int num = 50;
            if (lst_select >= 7)
            {
                num = 100;
            }
            Presentation presentation = new Presentation();
            Aspose.Pdf.Document document = new Aspose.Pdf.Document();
            try
            {
                presentation.Slides.RemoveAt(0);
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { 0 });
                }
                if (this.ppt_doc == null)
                {
                    return null;
                }
                for (int i = 0; i < this.ppt_doc.Slides.Count; i++)
                {
                    MemoryStream stream = new MemoryStream();
                    presentation.Slides.AddClone(this.ppt_doc.Slides[i]);
                    presentation.Save(stream, Aspose.Slides.Export.SaveFormat.Pdf);
                    presentation.Slides.RemoveAt(0);
                    document.Pages.Add(new Aspose.Pdf.Document(stream).Pages);
                    if (progress != null)
                    {
                        dlg.Invoke(progress, new object[] { (i * num) / this.ppt_doc.Slides.Count });
                    }
                }
                if (lst_select >= 7)
                {
                    document.Save(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + ".pdf");
                }
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { num });
                }
            }
            catch (Exception)
            {
                return null;
            }
            return document;
        }

        public void save(save_progress progress, System.Windows.Forms.Form dlg, int c, string fileType, int lst_select)
        {
            if (this.file_can_work)
            {
                if (new reg().is_reg())
                {
                    if ((this.global_config.start_page > this.get_pages_count()) || (this.global_config.start_page <= 0))
                    {
                        this.global_config.start_page = 1;
                    }
                    if ((this.global_config.end_page > this.get_pages_count()) || (this.global_config.end_page <= 0))
                    {
                        this.global_config.end_page = this.get_pages_count();
                    }
                }
                else
                {
                    if ((this.global_config.start_page > this.get_pages_count()) || (this.global_config.start_page <= 0))
                    {
                        this.global_config.start_page = 1;
                    }
                    if ((this.global_config.end_page > this.get_pages_count()) || (this.global_config.end_page <= 0))
                    {
                        this.global_config.end_page = this.get_pages_count();
                    }
                    this.global_config.start_page = 1;
                    if (this.global_config.end_page > 3)
                    {
                        this.global_config.end_page = 3;
                    }
                }
                if ((this.global_config.start_page != 1) || (this.global_config.end_page != this.get_pages_count()))
                {
                    int num2;
                    int num = 1;
                    int[] pages = new int[((this.global_config.start_page - 1) + this.get_pages_count()) - this.global_config.end_page];
                    for (num2 = 1; num2 <= (this.global_config.start_page - 1); num2++)
                    {
                        pages[num2 - 1] = num2;
                    }
                    num2 = this.global_config.start_page;
                    while (num2 <= pages.Length)
                    {
                        pages[num2 - 1] = this.global_config.end_page + num;
                        num2++;
                        num++;
                    }
                    if (fileType == ".pdf")
                    {
                        this.pdf_doc.Pages.Delete(pages);
                    }
                    else if ((((fileType != ".ppt") && !(fileType == ".pptx")) && (ORIGIN_FORMAT.EXCEL != this.get_orgin_format())) && (ORIGIN_FORMAT.PPT == this.get_orgin_format()))
                    {
                    }
                }
                if (this.global_config.target_dic == "")
                {
                    this.global_config.target_dic = this.get_path(this.file_path, false);
                }
                else if (this.global_config.target_dic.Substring(this.global_config.target_dic.Length - 1, 1) != @"\")
                {
                    this.global_config.target_dic = this.global_config.target_dic + @"\";
                }
                this.create_dic();
                switch (this.target_format)
                {
                    case FORMAT.PDF2WORD:
                        this.pdf_to_word(progress, dlg, fileType);
                        break;

                    case FORMAT.PDF2EXCEL:
                        this.pdf_to_excel(progress, dlg, fileType);
                        break;

                    case FORMAT.PDF2PPT:
                        this.pdf_to_ppt(progress, dlg, fileType);
                        break;

                    case FORMAT.PDF2HTML:
                        this.pdf_to_html(progress, dlg, fileType);
                        break;

                    case FORMAT.IMG2PDF:
                        this.pdf_to_epub(progress, dlg, c);
                        break;

                    case FORMAT.PDF2IMG:
                        this.pdf_to_img(progress, dlg, fileType);
                        break;

                    case FORMAT.PDF2TXT:
                        this.pdf_to_txt(progress, dlg, fileType);
                        break;

                    case FORMAT.DOC2PDF:
                        this.doc_to_pdf(progress, dlg, lst_select);
                        break;

                    case FORMAT.XLS2PDF:
                        this.xls_to_pdf(progress, dlg, lst_select);
                        break;

                    case FORMAT.PPT2PDF:
                        this.ppt_to_pdf(progress, dlg, lst_select);
                        break;
                }
            }
        }

        private Aspose.Pdf.Document xls_to_pdf(save_progress progress, System.Windows.Forms.Form dlg, int lst_select)
        {
            int num = 50;
            if (lst_select >= 7)
            {
                num = 100;
            }
            Aspose.Pdf.Document document = null;
            try
            {
                Workbook workbook = new Workbook();
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { 0 });
                }
                workbook.Worksheets.Clear();
                if (this.excel_doc == null)
                {
                    return document;
                }
                document = new Aspose.Pdf.Document();
                for (int i = 0; i < this.excel_doc.Worksheets.Count; i++)
                {
                    MemoryStream stream = new MemoryStream();
                    workbook.Worksheets.Add(i.ToString());
                    workbook.Worksheets[0].Copy(this.excel_doc.Worksheets[i]);
                    workbook.Save(stream, Aspose.Cells.SaveFormat.Pdf);
                    document.Pages.Add(new Aspose.Pdf.Document(stream).Pages);
                    workbook.Worksheets.RemoveAt(0);
                    if (progress != null)
                    {
                        dlg.Invoke(progress, new object[] { (i * num) / this.excel_doc.Worksheets.Count });
                    }
                }
                if (lst_select >= 7)
                {
                    document.Save(this.global_config.target_dic + Path.GetFileNameWithoutExtension(this.file_path) + ".pdf");
                }
                if (progress != null)
                {
                    dlg.Invoke(progress, new object[] { num });
                }
            }
            catch (Exception)
            {
                return null;
            }
            return document;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct config
        {
            public int start_page;
            public int end_page;
            public string password;
            public string target_dic;
            public int pic_width;
            public int pic_height;
            public int isMerger;
            public config(int start_page, int end_page, string password, string target_dic, int pic_width, int pic_height, int isMerger)
            {
                this.start_page = start_page;
                this.end_page = end_page;
                this.password = password;
                this.target_dic = target_dic;
                this.pic_width = pic_width;
                this.pic_height = pic_height;
                this.isMerger = isMerger;
            }
        }

        public enum FORMAT
        {
            PDF2WORD,
            PDF2EXCEL,
            PDF2PPT,
            PDF2HTML,
            IMG2PDF,
            PDF2IMG,
            PDF2TXT,
            DOC2PDF,
            XLS2PDF,
            PPT2PDF
        }

        private enum ORIGIN_FORMAT
        {
            PDF,
            WORD,
            EXCEL,
            PPT,
            IMG
        }

        public delegate void save_progress(int cur);
    }
}

