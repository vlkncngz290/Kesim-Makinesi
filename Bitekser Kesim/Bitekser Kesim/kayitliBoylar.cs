using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
namespace Bitekser_Kesim
{
    class kayitliBoylar
    {
        String isim;
        String veriler;
        String path;

        public kayitliBoylar(string isim,string veriler,string path)
        {
            this.isim = isim;
            this.veriler = veriler;
            this.path = path;
        }

        public void yaz1()
        {
            XmlTextWriter writer = new XmlTextWriter(path,UTF8Encoding.UTF8);
            writer.WriteStartDocument();
            writer.WriteComment("Oluşturuldu " + DateTime.Now.ToString());
            writer.WriteStartElement("kayitlar");
            writer.WriteElementString("isim", this.isim);
            writer.WriteElementString("veriler", this.veriler);
            writer.WriteEndElement();
            writer.WriteEndDocument();
            writer.Close();
        }

        public void yaz2()
        {
            XmlDocument doc = new XmlDocument();
            doc.Load(this.path);
            XmlNode root = doc.SelectSingleNode("/kayitlar");
            XmlDocumentFragment elem1 = doc.CreateDocumentFragment();
            elem1.InnerXml = "<kayitlar>" +
                "<isim>" + this.isim + "</isim>" +
                "<veriler>" + this.veriler + "</veriler>" +
                "</kayitlar>";
            root.InsertAfter(elem1,root.FirstChild);
            doc.Save(path);
        }

        
    }
}
