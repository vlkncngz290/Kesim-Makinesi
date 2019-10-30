using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace Bitekser_Kesim
{
    public partial class Form1 : MetroFramework.Forms.MetroForm
    {
        public Form1()
        {
            InitializeComponent();
        }
        String globalYol;
        String gelenVeri = "";
        bool pusherTamam = false;
        bool tirnakKapali = false;
        bool kesimeHazir = false;
        bool kesimBitti = false;
        bool debugTimerHatirlatici = true;
        public static bool debugEkrani = false;
        
        private void Form1_Load(object sender, EventArgs e)
        {
            string path = Directory.GetCurrentDirectory();
            globalYol = path + "\\kayitlar.xml";
            if (!File.Exists(path+"\\kayitlar.xml"))
            {
                kayitliBoylar kb = new kayitliBoylar("kayitlar.xml", "deneme", path+"\\kayitlar.xml");
                kb.yaz1();
            }
            string[] portlar = SerialPort.GetPortNames();
            foreach (String port in portlar) metroComboBox1.Items.Add(port);
        }

        private void metroButton1_Click(object sender, EventArgs e)
        {
            if (metroComboBox1.SelectedIndex > -1)
            {
                try
                {
                    serialPort1.PortName = metroComboBox1.SelectedItem.ToString();
                    serialPort1.BaudRate = 9600;

                    serialPort1.Open();
                    timer1.Interval = 100;
                    timer1.Enabled = true;
                }
                catch
                {
                    MetroFramework.MetroMessageBox.Show(this,"Bağlantı yapılamadı, başka port seçiniz","HATA");
                }
                if (serialPort1.IsOpen)
                {
                    metroLabel2.Text = "Bağlandı";
                    metroLabel4.Visible = true;
                    metroLabel3.Visible = true;
                    metroButton2.Visible = true;
                    metroTile3.Visible = true;
                    
                }

            }
            else
            {
                MetroFramework.MetroMessageBox.Show(this,"Lütfen Bağlantı Portunu Seçiniz","HATA");
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (debugEkrani)
            {
                if (debugTimerHatirlatici)
                {
                    debugTimerHatirlatici = false;
                    groupBox2.Visible = false;
                    groupBox6.Visible = false;
                    metroTile3.Text = "Debug Modunu Kapat";
                    serialPort1.WriteLine("2");
                    metroGrid3.Rows.Add("", "Debug Modunu Aç");
                }
            }
            if (serialPort1.IsOpen) if (!backgroundWorker1.IsBusy) backgroundWorker1.RunWorkerAsync();
            
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                e.Result = serialPort1.ReadLine();
            }
            catch
            {
                ;
            }
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Result.ToString().Length > 0)
            {
                if (e.Result.ToString().Contains("Debug modu aktif"))
                {
                    MetroFramework.MetroMessageBox.Show(this, "Debug modu açıldı", "BİLGİ");
                    
                }
                if (e.Result.ToString().Contains("hata:"))
                {
                    timer1.Enabled = false;
                    metroTile2.Visible = true;
                    groupBox2.Visible = false;
                    groupBox6.Visible = false;
                    String hata_adi = "";
                    string[] veri = e.Result.ToString().Split(':');
                    if (veri[1].Contains("1")) hata_adi = "ACİL STOP BUTONUNA BASILDI";
                    else if (veri[1].Contains("2")) hata_adi = "VOLTAJ HATASI";
                    else if (veri[1].Contains("3")) hata_adi = "MOTOR SÜRÜCÜ HATASI";
                    else if (veri[1].Contains("4")) hata_adi = "MOTOR KORUMA HATASI";
                    else if (veri[1].Contains("5")) hata_adi = "HAVA YOK";
                    MetroFramework.MetroMessageBox.Show(this, "\n\nHATA ALGILANDI,\n" + hata_adi, "ACİL DURUM");

                }
                if (e.Result.ToString().Contains("Kalibrasyon Tamamlandi") )
                {
                    groupBox2.Visible = true;
                    groupBox6.Visible = true;
                    configYap();
                    kayitliKesimleriGetir();
                    metroLabel3.Text = "Kalibrasyon Yapıldı";
                }
                if (e.Result.ToString().Contains("Pusher Konumlandirildi"))
                {
                    pusherTamam = true;
                    metroGrid3.Rows.Add(e.Result.ToString(), "");

                }
                if (e.Result.ToString().Contains("Pusher Konumlandirildi"))
                {
                    pusherTamam = true;
                    metroGrid3.Rows.Add(e.Result.ToString(), "");

                }
                if (e.Result.ToString().Contains("Tirnaklar kapandi"))
                {
                    tirnakKapali = true;
                    metroGrid3.Rows.Add(e.Result.ToString(), "");

                }
                if (e.Result.ToString().Contains("Kesim Bitti"))
                {
                    kesimBitti = true;
                    metroGrid3.Rows.Add(e.Result.ToString(), "");

                }
                if (e.Result.ToString().Contains("Tirnaklar acildi"))
                {
                    tirnakKapali = false;
                    metroGrid3.Rows.Add(e.Result.ToString(), "");

                }
                metroGrid3.Rows.Add(e.Result.ToString(), "");
            }
                //dataGridView1.Rows.Add(e.Result.ToString());
            Thread.Sleep(100);
        }

        public void configYap()
        {
            try
            {
                String yol = Directory.GetCurrentDirectory();
                String tamYol = yol + "\\veriler\\baslangic.cnf";
                String veriler = File.ReadAllText(tamYol);
                String[] birimler = veriler.Split('-');
                metroTextBox2.Text = birimler[0];
                metroTextBox3.Text = birimler[1];
            }
            catch(Exception Ex)
            {
                MessageBox.Show(Ex.ToString());
            }
        }

        public void kayitliKesimleriGetir()
        {
            metroGrid2.Rows.Clear();
            String yol = Directory.GetCurrentDirectory();
            String tamYol = yol + "\\veriler";
            DirectoryInfo d = new DirectoryInfo(tamYol);
            FileInfo[] Files = d.GetFiles("*.bts");
            foreach (FileInfo file in Files)
            {
                metroGrid2.Rows.Add(file.Name);
            }
        }

        private void metroTile1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void metroTile2_Click(object sender, EventArgs e)
        {
            timer1.Enabled = true;
            metroTile2.Visible = false;
        }

        private void metroTile3_Click(object sender, EventArgs e)
        {
            if (debugEkrani)
            {
                debugEkrani = false;
                debugTimerHatirlatici = true;
                groupBox2.Visible = true;
                groupBox6.Visible = true;
                metroTile3.Text = "Debug Modunu Aktifleştir";
                serialPort1.WriteLine("1");
                metroGrid3.Rows.Add("", "Debug Modunu Kapat");
            }
            else
            {
                DebugSifre ds = new DebugSifre();
                ds.Show();
            }
            
        }

        private void metroButton2_Click(object sender, EventArgs e)
        {
            kayitliKesimleriGetir();
            veriYaz("4");
            
            
        }

        public void veriYaz(string gelenveri)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.WriteLine(gelenveri);
                if(gelenveri=="4")
                    metroGrid3.Rows.Add("","Kalibrasyon Başlat");
                if (gelenveri == "5")
                    metroGrid3.Rows.Add("", "Pusher Konumlandırılıyor");
                if (gelenveri == "6")
                    metroGrid3.Rows.Add("", "Tırnaklar kapatılıyor");
                if (gelenveri == "7")
                    metroGrid3.Rows.Add("", "Tırnaklar açılıyor");

            }
        }

        private void metroGrid2_CellContentDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            String dosyaIsmi = metroGrid2.Rows[e.RowIndex].Cells[0].Value.ToString();
            String yol = Directory.GetCurrentDirectory();
            String tamYol = yol + "\\veriler\\"+dosyaIsmi;
            String veriler = File.ReadAllText(tamYol);
            String[] birimler = veriler.Split('-');
            metroGrid1.Rows.Clear();
            metroTextBox1.Text = birimler[0];
            for (int i = 1; i <= birimler.Length - 1; i++)
                metroGrid1.Rows.Add(birimler[i]);
        }

        private void metroTile4_Click(object sender, EventArgs e)
        {
            // Formüle göre boy kontrolü yap
            // PlakaBoyu <= traşÖlçüsü+ testereEni*(kesimBoylariToplami)
            Boolean formulDogrulama = true;
            try
            {
                int testereEni = Convert.ToInt32(metroTextBox3.Text);
                int plakaBoyu = Convert.ToInt32(metroTextBox1.Text);
                List<Int32> kesimBoylari = new List<int>();
                int toplam = 0;
                foreach (DataGridViewRow satir in metroGrid1.Rows)
                {
                    try
                    {
                        Int32 gelenBoy = Convert.ToInt32(satir.Cells[0].Value.ToString());
                        kesimBoylari.Add(gelenBoy);
                        toplam += gelenBoy;
                    }
                    catch { }
                }
                int testToplami = (testereEni * kesimBoylari.Count)+toplam;

                if (plakaBoyu < testToplami)
                {
                    formulDogrulama = false;
                }
                    

                if (formulDogrulama)
                {
                    //formül doğrulandı, kesime hazırlanılıyor;
                    //tırnaklari kapat
                    
                    veriYaz("6");
                    int kalanKesim = kesimBoylari.Count;
                    while (!tirnakKapali) ;                    
                    Thread.Sleep(2000);
                    //pusherı test toplamına konumlandır kes
                    veriYaz("5");
                    Thread.Sleep(300);
                    veriYaz(testToplami.ToString());
                    while (!pusherTamam) ;
                    pusherTamam = false;
                    veriYaz("8");
                    while (!kesimBitti) ;
                    kesimBitti = false;
                    kalanKesim--;
                    testToplami = testToplami - testereEni;
                    //fireyi çıkardın, şimdi standart boy kesimlerine geç
                    foreach (int boy in kesimBoylari)
                    {
                        if (kalanKesim > 0)
                        {
                            testToplami = testToplami - boy;
                            veriYaz("5");
                            Thread.Sleep(300);
                            veriYaz(testToplami.ToString());
                            while (!pusherTamam) ;
                            pusherTamam = false;
                            veriYaz("8");
                            while (!kesimBitti) ;
                            kesimBitti = false;
                            testToplami = testToplami - testereEni;
                            kalanKesim--;
                            
                        }
                        else
                        {
                            testToplami = testToplami - boy;
                            veriYaz("5");
                            Thread.Sleep(300);
                            veriYaz(testToplami.ToString());
                            while (!pusherTamam) ;
                            pusherTamam = false;
                            if (testToplami < 100)
                            {
                                veriYaz("7");
                                while (tirnakKapali) ;
                                veriYaz("5");
                                Thread.Sleep(300);
                                veriYaz("300");
                                while (!pusherTamam) ;
                                pusherTamam = false;

                            }
                            veriYaz("8");
                            while (!kesimBitti) ;
                            kesimBitti = false;
                            veriYaz("7");
                            while (tirnakKapali) ;
                            testToplami = testToplami - testereEni;
                            kalanKesim--;
                        }

                    }
                    MetroFramework.MetroMessageBox.Show(this, "", "Kesim Tamamlandı");
                }
                else
                {
                    MetroFramework.MetroMessageBox.Show(this, "Verilen ölçüler plaka boyunu geçiyor.", "HATA");
                }
            }
            catch(Exception Ex)
            {
                MetroFramework.MetroMessageBox.Show(this, Ex.ToString(), "Hatalı veri girildi, lütfen kontrol ediniz..");
            }
        }

        private void metroButton3_Click(object sender, EventArgs e)
        {            
            if (metroTextBox4.Text.Length > 0)
            {
                try
                {
                    String icerik = metroTextBox1.Text + "-";
                    foreach (DataGridViewRow satir in metroGrid1.Rows)
                    {
                        try
                        {
                            icerik = icerik + satir.Cells[0].Value.ToString()+"-";
                        }
                        catch
                        {
                            ;
                        }
                    }                        
                    String yol = Directory.GetCurrentDirectory();
                    String tamYol = yol + "\\veriler\\" + metroTextBox4.Text + ".bts";
                    using (StreamWriter writetext = new StreamWriter(tamYol))
                    {
                        writetext.WriteLine(icerik);
                    }
                    kayitliKesimleriGetir();
                }
                catch(Exception Ex)
                {
                    MessageBox.Show(Ex.ToString());
                }
            }
        }

        private void metroButton4_Click(object sender, EventArgs e)
        {
            int yer = Convert.ToInt32(metroTextBox5.Text);
            veriYaz("5");
            Thread.Sleep(300);
            veriYaz(yer.ToString());
            while (!pusherTamam) ;
            pusherTamam = false;
        }

        private void metroButton5_Click(object sender, EventArgs e)
        {
            veriYaz("8");
            while (!kesimBitti) ;
        }
    }
}
