namespace PDF_Convert
{
    using System;
    using System.Collections;
    using System.Windows.Forms;

    public class ListViewIndexComparer : IComparer
    {
        public int Compare(object x, object y)
        {
            return (((ListViewItem) x).Index - ((ListViewItem) y).Index);
        }
    }
}

