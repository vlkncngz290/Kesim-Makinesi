/*
 * İptal olan mekanizmalar pin tanımlamasından ve yazılım sürecinden silindi, pin numarasının kaybedilmemesi için pin numarası atamaları silinmedi.
 */

const int input_acil_stop=19;
const int input_surucu_hatasi=18;
const int input_motor_koruma_salteri=2;
const int input_voltaj_durumu=4; //İptal oldu
const int input_hava_durumu=3; //İptal oldu
const int input_testere_asagida=22; //İptal oldu
const int input_testere_yukarida=24; //İptal oldu
const int input_baski_yukarida=26;
const int input_testere_araba_ileride_yavaslatma=28;
const int input_testere_araba_ileride_durdurma=32;
const int input_testere_araba_geride_yavaslatma=30;
const int input_testere_araba_geride_durdurma=34;
const int input_pusher_arkada_yavaslatma=36;
const int input_pusher_arkada_durdurma=42;
const int input_pusher_onde_yavaslatma=38;
const int input_pusher_onde_durdurma=40;


volatile unsigned long temp, counter = 0;
const int input_encoderA=20;
const int input_encoderB=21;

void ai0();
void ai1();
void delayy(unsigned long);
void kalibrasyonYap();
void pusherOynat(unsigned long);
void kesimYap();


const int out_grupAcik=12;
const int out_testereArabaHizli=11;
const int out_testereArabaYavas=10;
const int out_testereArabaIleri=9;
const int out_testereArabaGeri=8;
const int out_testereAsagiValfi=7;
const int out_testereYukariValfi=6;
const int out_testereMotoru=5;
const int out_pusherOne=23;
const int out_pusherGeriye=25;
const int out_pusherHizli=27;
const int out_pusherYavas=29;
const int out_pusherCokYavas=37;
const int out_pusherFrenSerbest=31;
const int out_baskiAsagiValfi=33;
const int out_tirnakKapamaValfi=35;
volatile unsigned long anlik = 0;
float katsayi=19.96;
String inString = "";
int acilStop=0;
bool debug=false;


void setup() {
  pinMode(input_acil_stop,INPUT);
  pinMode(input_surucu_hatasi,INPUT);
  pinMode(input_motor_koruma_salteri,INPUT);
  pinMode(input_baski_yukarida,INPUT);
  pinMode(input_testere_araba_ileride_yavaslatma,INPUT);
  pinMode(input_testere_araba_ileride_durdurma,INPUT);
  pinMode(input_testere_araba_geride_yavaslatma,INPUT);
  pinMode(input_testere_araba_geride_durdurma,INPUT);
  pinMode(input_pusher_arkada_yavaslatma,INPUT);
  pinMode(input_pusher_arkada_durdurma,INPUT);
  pinMode(input_pusher_onde_yavaslatma,INPUT);
  pinMode(input_pusher_onde_durdurma,INPUT);
  pinMode(input_encoderA,INPUT_PULLUP);
  pinMode(input_encoderB,INPUT_PULLUP); 
  
  pinMode(out_grupAcik,OUTPUT);
  pinMode(out_testereArabaHizli,OUTPUT);
  pinMode(out_testereArabaYavas,OUTPUT);
  pinMode(out_testereArabaIleri,OUTPUT);
  pinMode(out_testereArabaGeri,OUTPUT);
  pinMode(out_testereAsagiValfi,OUTPUT);
  pinMode(out_testereYukariValfi,OUTPUT);
  pinMode(out_testereMotoru,OUTPUT);
  pinMode(out_pusherOne,OUTPUT);
  pinMode(out_pusherGeriye,OUTPUT);
  pinMode(out_pusherHizli,OUTPUT);
  pinMode(out_pusherYavas,OUTPUT);
  pinMode(out_pusherCokYavas,OUTPUT);
  pinMode(out_pusherFrenSerbest,OUTPUT);
  pinMode(out_baskiAsagiValfi,OUTPUT);
  pinMode(out_tirnakKapamaValfi,OUTPUT);


  // pin:input_acil_stop   interrupt:4
  //attachInterrupt(digitalPinToInterrupt(input_acil_stop),acilDurumKesmesi,LOW);
  // pin:input_surucu_hatasi   interrupt:5
  //attachInterrupt(digitalPinToInterrupt(input_surucu_hatasi),surucuHatasiKesmesi,FALLING);
  // pin:input_motor_koruma_salteri   interrupt:0
  //attachInterrupt(digitalPinToInterrupt(input_motor_koruma_salteri),motorKorumaKesmesi,FALLING);
  // pin:input_hava_durumu   interrupt:1
  //attachInterrupt(digitalPinToInterrupt(input_hava_durumu),havaKesmesi,FALLING);
  // pin:input_encoderA   interrupt:2
  attachInterrupt(digitalPinToInterrupt(input_encoderA), ai0, RISING);
  // pin:input_encoderB   interrupt:3
  attachInterrupt(digitalPinToInterrupt(input_encoderB), ai1, RISING);
  
  
  digitalWrite(out_grupAcik,LOW); 
  digitalWrite(out_testereArabaHizli,HIGH);
  digitalWrite(out_testereArabaYavas,HIGH);
  digitalWrite(out_testereArabaIleri,HIGH);
  digitalWrite(out_testereArabaGeri,HIGH);
  digitalWrite(out_testereAsagiValfi,LOW);
  digitalWrite(out_testereYukariValfi,HIGH);
  digitalWrite(out_testereMotoru,HIGH);
  digitalWrite(out_pusherOne,HIGH);
  digitalWrite(out_pusherGeriye,HIGH);
  digitalWrite(out_pusherHizli,HIGH);
  digitalWrite(out_pusherYavas,HIGH);
  digitalWrite(out_pusherFrenSerbest,HIGH);
  digitalWrite(out_baskiAsagiValfi,HIGH);
  digitalWrite(out_tirnakKapamaValfi,HIGH);
  digitalWrite(out_pusherCokYavas,HIGH);    
  Serial.begin(9600);
  
}

void loop() {
  if(acilStop>0){
    Serial.println("hata:"+String(acilStop)); 
    delayy(700);   
    while (Serial.available() > 0) {
      int inChar = Serial.parseInt();
      if (inChar==1) {
        // acil stop kapatılıyor ve düzen normale dönüyor
        acilStop=0;
        Serial.println("HATA KAPATILDI");               
      }      
    }      
  }
  else{
    if(debug){
      // Debug komutlarına geç
      while (Serial.available() > 0) {
        int gelen=Serial.parseInt();
        if(gelen==1){
          debug=false; 
          Serial.println("Debug Modu Kapatildi");         
        }
        if(gelen==2){
          while(!digitalRead(input_pusher_arkada_yavaslatma)){
          digitalWrite(out_tirnakKapamaValfi,HIGH);
          digitalWrite(out_pusherFrenSerbest,LOW);
          delay(500);
          digitalWrite(out_pusherOne,HIGH);
          digitalWrite(out_pusherGeriye,LOW);          
          digitalWrite(out_pusherHizli,HIGH);  
          digitalWrite(out_pusherYavas,LOW);
          digitalWrite(out_pusherCokYavas,HIGH);                  
          
          }
          digitalWrite(out_pusherFrenSerbest,HIGH);
          digitalWrite(out_pusherOne,HIGH);
          digitalWrite(out_pusherGeriye,HIGH);          
          digitalWrite(out_pusherHizli,HIGH);  
          digitalWrite(out_pusherYavas,HIGH);
          digitalWrite(out_pusherCokYavas,HIGH);
          digitalWrite(out_tirnakKapamaValfi,LOW);
          
          Serial.println("Geri Geldi");     
          Serial.println(String(anlik));
              
        }
        if(gelen==3){
          digitalWrite(out_tirnakKapamaValfi,HIGH);
          digitalWrite(out_pusherFrenSerbest,LOW);
          
          digitalWrite(out_pusherOne,HIGH);
          digitalWrite(out_pusherGeriye,LOW);          
          digitalWrite(out_pusherHizli,HIGH);  
          digitalWrite(out_pusherYavas,HIGH);
          digitalWrite(out_pusherCokYavas,LOW);                  
          delay(800);
          digitalWrite(out_pusherCokYavas,HIGH);
          digitalWrite(out_pusherFrenSerbest,HIGH);
          digitalWrite(out_pusherOne,HIGH);
          digitalWrite(out_pusherGeriye,HIGH);          
          digitalWrite(out_pusherHizli,HIGH);  
          digitalWrite(out_pusherYavas,HIGH);
          
          
          Serial.println(String(counter/katsayi));
        }
        if(gelen==4){
          digitalWrite(out_testereYukariValfi,HIGH);
          digitalWrite(out_testereAsagiValfi,LOW);
          delayy(2000);
          // yuvaya cek
          while(digitalRead(input_testere_araba_geride_durdurma)){
              while(!digitalRead(input_testere_araba_geride_yavaslatma)){
                digitalWrite(out_testereArabaYavas,HIGH);
                digitalWrite(out_testereArabaHizli,LOW);
                digitalWrite(out_testereArabaIleri,HIGH);
                digitalWrite(out_testereArabaGeri,LOW);
              }
              digitalWrite(out_testereArabaHizli,HIGH);
              digitalWrite(out_testereArabaYavas,LOW);
              digitalWrite(out_testereArabaIleri,HIGH);
              digitalWrite(out_testereArabaGeri,LOW);
              }
          digitalWrite(out_testereArabaHizli,HIGH);
          digitalWrite(out_testereArabaYavas,HIGH);
          digitalWrite(out_testereArabaGeri,HIGH);
          digitalWrite(out_testereArabaIleri,HIGH);
          }
          if(gelen==5){
            digitalWrite(out_testereYukariValfi,HIGH);
            digitalWrite(out_testereAsagiValfi,LOW);
            delayy(2000);
            // yuvaya cek
            while(digitalRead(input_testere_araba_ileride_durdurma)){
                while(!digitalRead(input_testere_araba_ileride_yavaslatma)){
                  digitalWrite(out_testereArabaYavas,HIGH);
                  digitalWrite(out_testereArabaHizli,LOW);
                  digitalWrite(out_testereArabaGeri,HIGH);
                  digitalWrite(out_testereArabaIleri,LOW);
                }
                digitalWrite(out_testereArabaHizli,HIGH);
                digitalWrite(out_testereArabaYavas,LOW);
                digitalWrite(out_testereArabaGeri,HIGH);
                digitalWrite(out_testereArabaIleri,LOW);
             }
            digitalWrite(out_testereArabaHizli,HIGH);
            digitalWrite(out_testereArabaYavas,HIGH);
            digitalWrite(out_testereArabaGeri,HIGH);
            digitalWrite(out_testereArabaIleri,HIGH);
          }

          if(gelen==6){
            digitalWrite(out_tirnakKapamaValfi,HIGH);
            //tırnakları aç
          }

          if(gelen==7){
            kesimYap();
          }
          if(gelen==8){
            digitalWrite(out_tirnakKapamaValfi,LOW);
            //tırnakları kapat
          }
          if(gelen==9){
            kalibrasyonYap();
          }
          if(gelen==10){
            Serial.println("hedef gir:");
            
            while(!Serial.available());
            delayy(2500);
            if(Serial.available()>0){
              float gelen=Serial.parseFloat();
              Serial.println(String(gelen));
              
              pusherOynat(gelen);
              
            }
            Serial.println("geldim.."+String(anlik));
          }
          if(gelen==11){
                digitalWrite(out_testereArabaHizli,HIGH);
                digitalWrite(out_testereArabaYavas,LOW);
                digitalWrite(out_testereArabaGeri,HIGH);
                digitalWrite(out_testereArabaIleri,LOW);
                delayy(3000);
                digitalWrite(out_testereArabaHizli,HIGH);
                digitalWrite(out_testereArabaYavas,HIGH);
                digitalWrite(out_testereArabaGeri,HIGH);
                digitalWrite(out_testereArabaIleri,HIGH);
                digitalWrite(out_testereYukariValfi,LOW);
                digitalWrite(out_testereAsagiValfi,HIGH);
          }
          if(gelen==12){
            digitalWrite(out_pusherFrenSerbest,LOW);
            digitalWrite(out_pusherGeriye,LOW);           
            digitalWrite(out_pusherCokYavas,LOW);
            delayy(4000);
            digitalWrite(out_pusherGeriye,LOW);           
            digitalWrite(out_pusherCokYavas,LOW);  
            digitalWrite(out_pusherFrenSerbest,HIGH);      
            Serial.println(anlik);
            Serial.println(counter);            
        
          }
    }
    }
    else{
      // Standart komutlarda kal
      while (Serial.available() > 0) {
        int gelen=Serial.parseInt();
        if(gelen==2){
          debug=true;
          Serial.println("Debug modu aktif");          
        }

        if(gelen==4){
          Serial.println("Kalibrasyon Basladi");
          kalibrasyonYap();
          Serial.println("Kalibrasyon Tamamlandi");
        }

        if(gelen==5){
           Serial.println("Pusher Konumlandiriliyor");            
            while(!Serial.available());
            delayy(1000);
            if(Serial.available()>0){
              float gelen=Serial.parseFloat();
              Serial.println(String(gelen));              
              pusherOynat(gelen);              
            }
            Serial.println("Pusher Konumlandirildi: "+String(anlik)+"MM");                    
        }
        
        if(gelen==7){
          //kesim bitti tırnakları aç
          Serial.println("Tirnaklar aciliyor");
          digitalWrite(out_tirnakKapamaValfi,HIGH);
          Serial.println("Tirnaklar acildi");
        }

        if(gelen==6){
          Serial.println("Tirnaklar kapaniyor");
          digitalWrite(out_tirnakKapamaValfi,LOW);
          Serial.println("Tirnaklar kapandi");
        }

        if(gelen==8){
          Serial.println("Kesime Baslandi");
          kesimYap();
          Serial.println("Kesim Bitti");
        }
      }
      
      }
    }
  }     





void encoder0(){
  if(digitalRead(input_encoderB)==LOW) {
  counter--;
  }else{
  counter++;
  }
  Serial.println(String(counter));
}

void encoder1(){
  if(digitalRead(input_encoderA)==LOW) {
  counter--;
  }else{
  counter++;
  }
  Serial.println(String(counter));
}

void acilBekleme(){
  
}

void acilDurumKesmesi(){
  // acil duruma basildi herseyi kapat ve bilgisayardan sinyal bekle
  tumOutlariKapat();
  acilStop=1;
}


void voltajKesmesi(){
  // acil duruma basildi herseyi kapat ve bilgisayardan sinyal bekle
  tumOutlariKapat();
  acilStop=2;
}

void surucuHatasiKesmesi(){
  // acil duruma basildi herseyi kapat ve bilgisayardan sinyal bekle
  tumOutlariKapat();
  acilStop=3;
}

void motorKorumaKesmesi(){
  // acil duruma basildi herseyi kapat ve bilgisayardan sinyal bekle
  tumOutlariKapat();
  acilStop=4;
}

void havaKesmesi(){
  // acil duruma basildi herseyi kapat ve bilgisayardan sinyal bekle
  tumOutlariKapat();
  acilStop=5;
}

void kalibrasyonYap(){
  // Testere motorunu kapat
  digitalWrite(out_testereMotoru,HIGH);
  // Testereyi aşağı çek
  digitalWrite(out_testereYukariValfi,HIGH);
  digitalWrite(out_testereAsagiValfi,LOW);
  delayy(2000);
  // Testereyi yuvaya çek
  digitalWrite(out_testereYukariValfi,HIGH);
  digitalWrite(out_testereAsagiValfi,LOW);
  delayy(2000);
  // yuvaya cek
  if(digitalRead(input_testere_araba_geride_durdurma)){
    while(digitalRead(input_testere_araba_geride_durdurma)){
    while(!digitalRead(input_testere_araba_geride_yavaslatma)){
      digitalWrite(out_testereArabaYavas,HIGH);
      digitalWrite(out_testereArabaHizli,LOW);
      digitalWrite(out_testereArabaIleri,HIGH);
      digitalWrite(out_testereArabaGeri,LOW);
    }
    digitalWrite(out_testereArabaHizli,HIGH);
    digitalWrite(out_testereArabaYavas,LOW);
    digitalWrite(out_testereArabaIleri,HIGH);
    digitalWrite(out_testereArabaGeri,LOW);
  }
  digitalWrite(out_testereArabaHizli,HIGH);
  digitalWrite(out_testereArabaYavas,HIGH);
  digitalWrite(out_testereArabaGeri,HIGH);
  digitalWrite(out_testereArabaIleri,HIGH);
  } 
  // Pusher freni serbest bırak
  digitalWrite(out_pusherFrenSerbest,LOW);
  // Pusher'ı sıfır noktasına çek
  
  while(!digitalRead(input_pusher_onde_yavaslatma)){
    digitalWrite(out_pusherGeriye,HIGH);
      digitalWrite(out_pusherOne,LOW);      
      digitalWrite(out_pusherHizli,HIGH);
      digitalWrite(out_pusherCokYavas,HIGH);   
      digitalWrite(out_pusherYavas,LOW);
  }
  Serial.println(String(counter));
  while(!digitalRead(input_pusher_onde_durdurma)){    
    digitalWrite(out_pusherGeriye,HIGH);
    digitalWrite(out_pusherOne,LOW);
    digitalWrite(out_pusherHizli,HIGH);  
    digitalWrite(out_pusherYavas,HIGH);
    digitalWrite(out_pusherCokYavas,LOW);      
  }
  digitalWrite(out_pusherFrenSerbest,HIGH);
  digitalWrite(out_pusherGeriye,HIGH);
  digitalWrite(out_pusherOne,HIGH);
  digitalWrite(out_pusherHizli,HIGH);  
  digitalWrite(out_pusherCokYavas,HIGH);
  digitalWrite(out_pusherYavas,HIGH);
  Serial.println(String(3));
  delayy(500);
  // Counter'ı sıfırla
  counter=0;
  anlik=0;
  Serial.println(String(counter));
}

void pusherOynat(unsigned long hedef){
  if(hedef>70){
    digitalWrite(out_pusherFrenSerbest,LOW);
  
  if(hedef<anlik){
    // pusher ileri gitsin
    digitalWrite(out_pusherOne,LOW);
    while(hedef<anlik  && !digitalRead(input_pusher_onde_durdurma)){
      while(anlik-hedef>150 && !digitalRead(input_pusher_onde_yavaslatma)){        
        digitalWrite(out_pusherYavas,LOW);
      }      
      digitalWrite(out_pusherYavas,HIGH);
      digitalWrite(out_pusherCokYavas,LOW);
    }
    digitalWrite(out_pusherOne,HIGH);
    digitalWrite(out_pusherYavas,HIGH);
    digitalWrite(out_pusherCokYavas,HIGH);
  }
  else{
    // pusher geri gitsin
    digitalWrite(out_pusherGeriye,LOW);
    while(hedef>anlik && !digitalRead(input_pusher_arkada_durdurma)){
      while(hedef-anlik>150 && !digitalRead(input_pusher_arkada_yavaslatma)){        
        digitalWrite(out_pusherYavas,LOW);
      }
      digitalWrite(out_pusherYavas,HIGH);
      digitalWrite(out_pusherCokYavas,LOW);
    }
    digitalWrite(out_pusherGeriye,HIGH);
    digitalWrite(out_pusherYavas,HIGH);
    digitalWrite(out_pusherCokYavas,HIGH);
  }
  digitalWrite(out_pusherFrenSerbest,HIGH);
  Serial.println(anlik);
  Serial.println(counter);
  }
  
}

void kesimYap(){
  digitalWrite(out_baskiAsagiValfi,LOW);
  
  if(!digitalRead(input_testere_araba_geride_durdurma)){
    // testere normal yerinde kesim yap
    // testereyi yukarı çıkar
    digitalWrite(out_testereAsagiValfi,HIGH);
    digitalWrite(out_testereYukariValfi,LOW);
    delayy(1000);
    // testere motorunu çalıştır
    digitalWrite(out_testereMotoru,LOW);
    // testereyi sona götür ve dur
    while(digitalRead(input_testere_araba_ileride_durdurma)){
                while(!digitalRead(input_testere_araba_ileride_yavaslatma)){
                  digitalWrite(out_testereArabaYavas,HIGH);
                  digitalWrite(out_testereArabaHizli,LOW);
                  digitalWrite(out_testereArabaGeri,HIGH);
                  digitalWrite(out_testereArabaIleri,LOW);
                }
                digitalWrite(out_testereArabaHizli,HIGH);
                digitalWrite(out_testereArabaYavas,LOW);
                digitalWrite(out_testereArabaGeri,HIGH);
                digitalWrite(out_testereArabaIleri,LOW);
             }
            digitalWrite(out_testereArabaHizli,HIGH);
            digitalWrite(out_testereArabaYavas,HIGH);
            digitalWrite(out_testereArabaGeri,HIGH);
            digitalWrite(out_testereArabaIleri,HIGH);
    // testere motoru durdur
    digitalWrite(out_testereMotoru,HIGH);
    // testereyi aşağı indir
    digitalWrite(out_testereYukariValfi,HIGH);
    digitalWrite(out_testereAsagiValfi,LOW);  
    digitalWrite(out_baskiAsagiValfi,HIGH);  
    delayy(500);
    // testereyi yuvaya getir
    while(digitalRead(input_testere_araba_geride_durdurma)){
              while(!digitalRead(input_testere_araba_geride_yavaslatma)){
                digitalWrite(out_testereArabaYavas,HIGH);
                digitalWrite(out_testereArabaHizli,LOW);
                digitalWrite(out_testereArabaIleri,HIGH);
                digitalWrite(out_testereArabaGeri,LOW);
              }
              digitalWrite(out_testereArabaHizli,HIGH);
              digitalWrite(out_testereArabaYavas,LOW);
              digitalWrite(out_testereArabaIleri,HIGH);
              digitalWrite(out_testereArabaGeri,LOW);
              }
          digitalWrite(out_testereArabaHizli,HIGH);
          digitalWrite(out_testereArabaYavas,HIGH);
          digitalWrite(out_testereArabaGeri,HIGH);
          digitalWrite(out_testereArabaIleri,HIGH);
    
  }
  else{
    //testereyi indir, yuvaya cek sonra kesim yap
    digitalWrite(out_testereYukariValfi,HIGH);
    digitalWrite(out_testereAsagiValfi,LOW);
    delayy(2000);
    // yuvaya cek
    while(digitalRead(input_testere_araba_geride_durdurma)){
      while(!digitalRead(input_testere_araba_geride_yavaslatma)){
        digitalWrite(out_testereArabaYavas,HIGH);
        digitalWrite(out_testereArabaHizli,LOW);
        digitalWrite(out_testereArabaIleri,HIGH);
        digitalWrite(out_testereArabaGeri,LOW);
      }
      digitalWrite(out_testereArabaHizli,HIGH);
      digitalWrite(out_testereArabaYavas,LOW);
      digitalWrite(out_testereArabaIleri,HIGH);
      digitalWrite(out_testereArabaGeri,LOW);
    }
    digitalWrite(out_testereArabaHizli,HIGH);
    digitalWrite(out_testereArabaYavas,HIGH);
    digitalWrite(out_testereArabaGeri,HIGH);
    digitalWrite(out_testereArabaIleri,HIGH);
    // testere yukarıda standart kesime geç
    // testereyi yukarı çıkar
    digitalWrite(out_testereAsagiValfi,HIGH);
    digitalWrite(out_testereYukariValfi,LOW);
    delayy(2000);
    // testere motorunu çalıştır
    digitalWrite(out_testereMotoru,LOW);
    // testereyi sona götür ve dur
    while(digitalRead(input_testere_araba_ileride_durdurma)){
      while(digitalRead(input_testere_araba_ileride_yavaslatma)){
        digitalWrite(out_testereArabaYavas,HIGH);
        digitalWrite(out_testereArabaHizli,LOW);
        digitalWrite(out_testereArabaGeri,HIGH);
        digitalWrite(out_testereArabaIleri,LOW);
      }
      digitalWrite(out_testereArabaHizli,HIGH);
      digitalWrite(out_testereArabaYavas,LOW);
      digitalWrite(out_testereArabaGeri,HIGH);
      digitalWrite(out_testereArabaIleri,LOW);
    }
    digitalWrite(out_testereArabaHizli,HIGH);
    digitalWrite(out_testereArabaYavas,HIGH);
    digitalWrite(out_testereArabaGeri,HIGH);
    digitalWrite(out_testereArabaIleri,HIGH);
    // testere motoru durdur
    digitalWrite(out_testereMotoru,HIGH);
    // testereyi aşağı indir
    digitalWrite(out_testereYukariValfi,HIGH);
    digitalWrite(out_testereAsagiValfi,LOW);    
    delayy(2000);
    // testereyi yuvaya getir
    while(digitalRead(input_testere_araba_geride_durdurma)){
      while(digitalRead(input_testere_araba_geride_yavaslatma)){
        digitalWrite(out_testereArabaYavas,HIGH);
        digitalWrite(out_testereArabaHizli,LOW);
        digitalWrite(out_testereArabaIleri,HIGH);
        digitalWrite(out_testereArabaGeri,LOW);
      }
      digitalWrite(out_testereArabaHizli,HIGH);
      digitalWrite(out_testereArabaYavas,LOW);
      digitalWrite(out_testereArabaIleri,HIGH);
      digitalWrite(out_testereArabaGeri,LOW);
    }
    digitalWrite(out_testereArabaHizli,HIGH);
    digitalWrite(out_testereArabaYavas,HIGH);
    digitalWrite(out_testereArabaGeri,HIGH);
    digitalWrite(out_testereArabaIleri,HIGH);
  }
  digitalWrite(out_baskiAsagiValfi,HIGH);
  
}

void tumOutlariKapat(){  
  digitalWrite(out_grupAcik,HIGH);
  digitalWrite(out_testereArabaHizli,HIGH);
  digitalWrite(out_testereArabaYavas,HIGH);
  digitalWrite(out_testereArabaIleri,HIGH);
  digitalWrite(out_testereArabaGeri,HIGH);
  digitalWrite(out_testereAsagiValfi,LOW);
  digitalWrite(out_testereYukariValfi,HIGH);
  digitalWrite(out_testereMotoru,HIGH);
  digitalWrite(out_pusherOne,HIGH);
  digitalWrite(out_pusherGeriye,HIGH);
  digitalWrite(out_pusherHizli,HIGH);
  digitalWrite(out_pusherYavas,HIGH);
  digitalWrite(out_pusherFrenSerbest,HIGH);
  digitalWrite(out_baskiAsagiValfi,HIGH);
  digitalWrite(out_tirnakKapamaValfi,HIGH);
  
}

void ai0() {
 
  if(digitalRead(input_encoderB)==LOW) {
  counter++;
  }else{
  counter--;
  }
  anlik=(counter/katsayi);
  }
   
  void ai1() {
  
  if(digitalRead(input_encoderA)==LOW) {
  counter--;
  }else{
  counter++;
  }
  anlik=(counter/katsayi);
  }





void delayy(unsigned long ms)
{
  uint16_t start = (uint16_t)micros();

  while (ms > 0) {
    if (((uint16_t)micros() - start) >= 1000) {
      ms--;
      start += 1000;
    }
  }
}
