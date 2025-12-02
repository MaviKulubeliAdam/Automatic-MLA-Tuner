// Manyetik Loop Auto-Tuner v4.0 FINAL - AD9851 DDS ile
// Telsizden Bağımsız Sistem + Çin Klonu RF Amp (1MHz-3000MHz)
//
// BAĞLANTILAR:
// === MOTOR ===
// TB6600 PUL- -> PIN4
// TB6600 DIR- -> PIN3
// TB6600 ENA- -> PIN9
// TB6600 PUL+/DIR+/ENA+ -> 5V
//
// === DDS (AD9851) ===
// VCC -> 5V
// GND -> GND
// W_CLK  -> Pin 52
// FQ_UD  -> Pin 53
// DATA   -> Pin 51
// RESET  -> Pin 49
// OUT -> RF Amplifier IN (direkt 50Ω koax)
//
// === RF AMPLIFIER (1MHz-3000MHz Çin Klonu) ===
// VCC -> 12V (Harici güç kaynağı - 2A önerilen)
// GND -> Arduino GND ile ortak
// RFIN -> AD9833 OUT (50Ω)
// RFOUT -> 20dB Attenuator -> SWR Bridge IN
// Güç kontrolü: 5V Röle Modülü (aktif LOW) → Pin 10
//
// === SWR BRIDGE + AD8307 ===
// SWR Bridge FWD -> AD8307 #1 IN -> A0
// SWR Bridge REF -> AD8307 #2 IN -> A1
// AD8307 VCC -> 5V, GND -> GND

#define STEP_PIN 4       // PUL- pin4'e bağlı
#define DIR_PIN 3        // DIR- pin3'e bağlı
#define ENABLE_PIN 9     // ENA- pin9'a bağlı (Motor Enable)
#define ENCODER_CLK 2    // INTERRUPT pin
#define ENCODER_DT 5
#define ENCODER_SW 6
#define BUTON_ILERI 7    // Saat yönünde döndürme
#define BUTON_GERI 8     // Saat tersi döndürme

// DDS (AD9851) Pinleri
#define AD9851_DATA   51
#define AD9851_WCLK   52
#define AD9851_FQUD   53
#define AD9851_RESET  49

// SWR Ölçüm Pinleri
#define SWR_FORWARD_PIN   A0
#define SWR_REFLECTED_PIN A1

// RF Amp Kontrol
#define RF_AMP_ENABLE 10  // Röle IN (aktif LOW)
// RF Seçim Rölesi (Omron G5RV-2-12V bobin sürümü için kontrol pin)
#define RF_MODE_PIN 11    // NPN sürücüyü tetikleyen pin (HIGH=OPERATE/NO, LOW=TUNE/NC)

// Değişkenler
volatile long encoderDegeri = 0;
volatile bool encoderHareket = false;
volatile int sonCLK = HIGH;
int sonSW = HIGH;
unsigned long sonButonZamani = 0;
unsigned long sonMotorZamani = 0;
unsigned long sonIleriButonZamani = 0;
unsigned long sonGeriButonZamani = 0;
bool encoderButonKilitli = false; // Encoder butonuna basılıyken hareket kilidi

// Motor ayarları
int stepHiz = 2000;    // 2000 us - Yavaş ve hassas
int stepsPerTik = 2;   // Her düğme basışında kaç step
bool motorAktif = false;
const unsigned long BUTON_DEBOUNCE = 150; // 150ms debounce süresi

// Hassasiyet profilleri (encoder butonuna basınca değişir)
int profilIndex = 1; // Başlangıç: Hassas profil
const int profiller[3][2] = {
  {1, 3000},   // Profil 0: ÇOK HASSAS (1 step, yavaş)
  {2, 2000},   // Profil 1: HASSAS (2 step, orta)
  {5, 1500}    // Profil 2: HIZLI (5 step, hızlı)
};
const char* profilIsimleri[] = {"ÇOK HASSAS", "HASSAS", "HIZLI"};

void setup() {
  // Pin modları
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);
  pinMode(BUTON_ILERI, INPUT_PULLUP);
  pinMode(BUTON_GERI, INPUT_PULLUP);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // Motor başlangıç
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(ENABLE_PIN, LOW); // Motor devre dışı (LOW = Disable for TB6600)
  
  // Interrupt
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderISR, CHANGE);
  
  pinMode(SWR_FORWARD_PIN, INPUT);
  pinMode(SWR_REFLECTED_PIN, INPUT);
  pinMode(RF_AMP_ENABLE, OUTPUT);
  pinMode(RF_MODE_PIN, OUTPUT);
  
  // DDS pinleri (AD9851)
  pinMode(AD9851_DATA, OUTPUT);
  pinMode(AD9851_WCLK, OUTPUT);
  pinMode(AD9851_FQUD, OUTPUT);
  pinMode(AD9851_RESET, OUTPUT);
  
  // Başlangıç durumları
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(ENABLE_PIN, LOW);  // Motor kapalı
  digitalWrite(RF_AMP_ENABLE, HIGH);  // Röle kapalı → RF Amp GÜÇ YOK
  digitalWrite(RF_MODE_PIN, LOW);     // Başlangıç: TUNE modu (NC devrede)
  digitalWrite(AD9851_DATA, LOW);
  digitalWrite(AD9851_WCLK, LOW);
  digitalWrite(AD9851_FQUD, LOW);
  digitalWrite(AD9851_RESET, LOW);
  
  // DDS başlat
  ddsInit();
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderISR, CHANGE);
  
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n");
  Serial.println("╔════════════════════════════════════════════╗");
  Serial.println("║  MANYETİK LOOP AUTO-TUNER v4.0 FINAL     ║");
  Serial.println("║  TELSİZDEN BAĞIMSIZ SİSTEM - AD9851      ║");
  Serial.println("╚════════════════════════════════════════════╝");
  Serial.println();
  Serial.println("📡 Sinyal Üreticisi: AD9851 DDS (180MHz)");
  Serial.println("📶 RF Amplifier: 1MHz-3GHz Çin Klonu");
  Serial.println("🔬 SWR Ölçüm: AD8307 x2 (Logaritmik)");
  Serial.println("⚙️  Motor Kontrol: TB6600 + Step Motor");
  Serial.println("════════════════════════════════════════════");
  Serial.println();
  Serial.println("📝 KOMUTLAR:");
  Serial.println("   T[frekans]     - Otomatik tune");
  Serial.println("                    Örnek: T7.100");
  Serial.println("   F[frekans]     - Sadece frekans ayarla");
  Serial.println("   M              - SWR ölç");
  Serial.println("   R              - RF Aç/Kapat");
  Serial.println("   MODE TUNE      - Röle: DDS (NC)");
  Serial.println("   MODE OPERATE   - Röle: TRX (NO)");
  Serial.println("   SWEEP [s] [e]  - Band tarama");
  Serial.println("                    Örnek: SWEEP 7.0 7.3");
  Serial.println("   S              - Durum göster");
  Serial.println("   ZERO           - Pozisyon sıfırla");
  Serial.println("   1/2/3          - Profil değiştir");
  Serial.println("   H              - Yardım");
  Serial.println();
  
  stepsPerTik = profiller[profilIndex][0];
  stepHiz = profiller[profilIndex][1];
  
  Serial.print("✓ Profil: ");
  Serial.println(profilIsimleri[profilIndex]);
  Serial.print("✓ Step/Tik: ");
  Serial.print(stepsPerTik);
  Serial.print(", Hız: ");
  Serial.print(stepHiz);
  Serial.println("µs");
  Serial.println("\n[Hazır. Komut bekliyor...]");
  
  // Başlangıç profili uygula
  stepsPerTik = profiller[profilIndex][0];
  stepHiz = profiller[profilIndex][1];
  
  Serial.print("✓ Profil: ");
  Serial.print(profilIsimleri[profilIndex]);
  Serial.print(" | Steps: ");
  Serial.print(stepsPerTik);
  Serial.print(" | Hız: ");
  Serial.print(stepHiz);
  Serial.println(" us");
  Serial.println("==========================================");
}
// ==================== AD9851 FONKSİYONLARI ====================
#define AD9851_CLOCK 180000000.0
#define AD9851_CONTROL 0x01  // Tipik: x6 PLL etkin

inline void ad9851Pulse(uint8_t pin) {
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void ad9851WriteByte(uint8_t b) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(AD9851_DATA, (b >> i) & 0x01); // LSB önce
    ad9851Pulse(AD9851_WCLK);
  }
}

void ddsInit() {
  // Reset sırası: RESET pulse, W_CLK pulse, FQ_UD pulse
  ad9851Pulse(AD9851_RESET);
  ad9851Pulse(AD9851_WCLK);
  ad9851Pulse(AD9851_FQUD);
}

void ddsSetFrequency(float freqMHz) {
  if (freqMHz < 0.0 || freqMHz > 60.0) {
    Serial.println("⚠️ Frekans aralığı: 0-60 MHz (AD9851)");
    return;
  }
  unsigned long freqHz = (unsigned long)(freqMHz * 1000000.0);
  // 32-bit tuning word: f_out = (word * f_clk) / 2^32
  uint32_t tuning = (uint32_t)((freqHz * 4294967295.0) / AD9851_CLOCK);
  // 5 bayt gönder: 4 bayt tuning word (LSB ilk), 1 bayt kontrol (AD9851_CONTROL)
  ad9851WriteByte((uint8_t)(tuning & 0xFF));
  ad9851WriteByte((uint8_t)((tuning >> 8) & 0xFF));
  ad9851WriteByte((uint8_t)((tuning >> 16) & 0xFF));
  ad9851WriteByte((uint8_t)((tuning >> 24) & 0xFF));
  ad9851WriteByte(AD9851_CONTROL);
  ad9851Pulse(AD9851_FQUD); // Güncelle
}

volatile unsigned long sonEncoderZamani = 0;
volatile int encoderPulseSayaci = 0;
const unsigned long ENCODER_DEBOUNCE = 5;

void encoderISR() {
  unsigned long simdikiZaman = millis();
  if (simdikiZaman - sonEncoderZamani < ENCODER_DEBOUNCE) return;
  
  int dtDurum = digitalRead(ENCODER_DT);
  int clkDurum = digitalRead(ENCODER_CLK);
  
  if (clkDurum != sonCLK) {
    if (dtDurum != clkDurum) {
      encoderPulseSayaci++;
    } else {
      encoderPulseSayaci--;
    }
    
    if (abs(encoderPulseSayaci) >= 4) {
      if (encoderPulseSayaci > 0) {
        encoderDegeri++;
      } else {
        encoderDegeri--;
      }
      encoderPulseSayaci = 0;
      encoderHareket = true;
    }
    
    sonCLK = clkDurum;
    sonEncoderZamani = simdikiZaman;
  }
}

void loop() {
  // Encoder kontrolü
  if (encoderHareket) {
    motorHareket(stepsPerTik * (encoderDegeri > 0 ? 1 : -1), stepHiz);
    encoderDegeri = 0;
    encoderHareket = false;
  }
  
  // Encoder butonu
  int swDurum = digitalRead(ENCODER_SW);
  if (swDurum == LOW && sonSW == HIGH && !encoderButonKilitli) {
    if (millis() - sonButonZamani > BUTON_DEBOUNCE) {
      profilIndex = (profilIndex + 1) % 3;
      stepsPerTik = profiller[profilIndex][0];
      stepHiz = profiller[profilIndex][1];
      Serial.print("✓ Profil: ");
      Serial.println(profilIsimleri[profilIndex]);
      sonButonZamani = millis();
      encoderButonKilitli = true;
    }
  }
  if (swDurum == HIGH) encoderButonKilitli = false;
  sonSW = swDurum;
  
  // İleri butonu
  if (digitalRead(BUTON_ILERI) == LOW) {
    if (millis() - sonIleriButonZamani > BUTON_DEBOUNCE) {
      motorHareket(stepsPerTik, stepHiz);
      sonIleriButonZamani = millis();
    }
  }
  
  // Geri butonu
  if (digitalRead(BUTON_GERI) == LOW) {
    if (millis() - sonGeriButonZamani > BUTON_DEBOUNCE) {
      motorHareket(-stepsPerTik, stepHiz);
      sonGeriButonZamani = millis();
    }
  }
  
  delay(10);
}

// ==================== SWR ÖLÇÜM ====================
float swrOku() {
  int fwdADC = analogRead(SWR_FORWARD_PIN);
  int refADC = analogRead(SWR_REFLECTED_PIN);
  
  float fwdVolt = (fwdADC / 1023.0) * 5.0;
  float refVolt = (refADC / 1023.0) * 5.0;
  
  float fwdDbm = (fwdVolt / 0.025) - 75.0;
  float refDbm = (refVolt / 0.025) - 75.0;
  
  float fwdPower = pow(10.0, (fwdDbm - 30.0) / 10.0);
  float refPower = pow(10.0, (refDbm - 30.0) / 10.0);
  
  float gamma = sqrt(refPower / (fwdPower + 0.000001));
  if (gamma > 1.0) gamma = 1.0;
  
  float swr = (1.0 + gamma) / (1.0 - gamma + 0.000001);
  if (swr < 1.0) swr = 1.0;
  if (swr > 99.0) swr = 99.0;
  return swr;
}

void profilDegistir() {
  // Profil ayarlarını uygula
  stepsPerTik = profiller[profilIndex][0];
  stepHiz = profiller[profilIndex][1];
  
  Serial.println();
  Serial.println("⚙️  ====== PROFİL DEĞİŞTİ ======");
  Serial.print("   ");
  Serial.println(profilIsimleri[profilIndex]);
  Serial.print("   Steps: ");
  Serial.print(stepsPerTik);
  Serial.print(" | Hız: ");
  Serial.print(stepHiz);
  Serial.println(" us");
  Serial.println("================================\n");
}

void encoderButonKontrol() {
  int swDeger = digitalRead(ENCODER_SW);
  
  // Buton basıldı
  if (swDeger == LOW && sonSW == HIGH) {
    if (millis() - sonButonZamani > 500) {
      encoderButonKilitli = true; // Encoder hareketini kilitle
      
      // Encoder değişikliklerini görmezden gel
      encoderDegeri = 0;
      encoderHareket = false;
      
      delay(100); // Butona basarken encoder titreşimini bekle
      
      // Profiller arasında geç
      profilIndex = (profilIndex + 1) % 3;
      profilDegistir();
      sonButonZamani = millis();
    }
  }
  
  // Buton bırakıldı
  if (swDeger == HIGH && sonSW == LOW) {
    delay(100); // Bırakırken titreşimi bekle
    encoderDegeri = 0; // Biriken değerleri temizle
    encoderHareket = false;
    encoderButonKilitli = false; // Kilidi kaldır
  }
  
  sonSW = swDeger;
}

void butonMotorKontrol() {
  static bool ileriBasili = false;
  static bool geriBasili = false;
  unsigned long suAn = millis();
  
  // Buton 7 - Saat yönünde (İLERİ)
  if (digitalRead(BUTON_ILERI) == LOW && !ileriBasili) {
    // Debounce kontrolü
    if (suAn - sonIleriButonZamani > BUTON_DEBOUNCE) {
      ileriBasili = true;
      sonIleriButonZamani = suAn;
      
      Serial.println(">>> BUTON 7 BASILDI - ILERI");
      
      // Motoru aktif et (ENA- HIGH = Enable)
      digitalWrite(ENABLE_PIN, HIGH);
      delayMicroseconds(100); // Enable için bekleme
      
      // İleri yön: DIR pin LOW
      digitalWrite(DIR_PIN, LOW);
      delayMicroseconds(50); // DIR değişimi için bekleme
      
      // Belirli sayıda step at
      for(int i = 0; i < stepsPerTik; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(stepHiz);
      }
      
      // Motoru devre dışı bırak (ENA- LOW = Disable)
      digitalWrite(ENABLE_PIN, LOW);
      
      Serial.print("✓ ");
      Serial.print(stepsPerTik);
      Serial.println(" step atıldı");
    }
  }
  
  // Buton bırakıldı
  if (digitalRead(BUTON_ILERI) == HIGH && ileriBasili) {
    ileriBasili = false;
  }
  
  // Buton 8 - Saat tersi (GERİ)
  if (digitalRead(BUTON_GERI) == LOW && !geriBasili) {
    // Debounce kontrolü
    if (suAn - sonGeriButonZamani > BUTON_DEBOUNCE) {
      geriBasili = true;
      sonGeriButonZamani = suAn;
      
      Serial.println(">>> BUTON 8 BASILDI - GERI");
      
      // Motoru aktif et (ENA- HIGH = Enable)
      digitalWrite(ENABLE_PIN, HIGH);
      delayMicroseconds(100); // Enable için bekleme
      
      // Geri yön: DIR pin HIGH
      digitalWrite(DIR_PIN, HIGH);
      delayMicroseconds(50); // DIR değişimi için bekleme
      
      // Belirli sayıda step at
      for(int i = 0; i < stepsPerTik; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(stepHiz);
      }
      
      // Motoru devre dışı bırak (ENA- LOW = Disable)
      digitalWrite(ENABLE_PIN, LOW);
      
      Serial.print("✓ ");
      Serial.print(stepsPerTik);
      Serial.println(" step atıldı");
    }
  }
  
  // Buton bırakıldı
  if (digitalRead(BUTON_GERI) == HIGH && geriBasili) {
    geriBasili = false;
  }
}



// Serial komutları
void autoTune(float freqMHz) {
  Serial.println("\n╔═══════════════════════════════════════╗");
  Serial.println("║     OTOMATİK TUNING BAŞLIYOR         ║");
  Serial.println("╚═══════════════════════════════════════╝");
  
  ddsSetFrequency(freqMHz);
  delay(100);
  
  if (!rfAktif) {
    digitalWrite(RF_AMP_ENABLE, LOW);   // Röle AÇIK (aktif LOW) → Amp GÜÇ VER
    rfAktif = true;
    Serial.println("✓ RF Amplifier açıldı");
    delay(200);
  }
  
  float basSWR = swrOku();
  Serial.print("📊 Başlangıç SWR: ");
  Serial.println(basSWR, 2);
  
  if (basSWR <= SWR_HEDEF) {
    Serial.println("✓ SWR zaten hedefte!");
    return;
  }
  
  // İleri yönde tara
  Serial.println("\n→ İleri yönde tarama...");
  float minSWR = basSWR;
  int bestPos = 0;
  
  for (int i = 0; i < MAX_TUNE_STEPS; i++) {
    motorHareket(2, stepHiz);
    delay(50);
    
    float swr = swrOku();
    
    if (i % 20 == 0) {
      Serial.print("  Step ");
      Serial.print(i);
      Serial.print(": SWR = ");
      Serial.println(swr, 2);
    }
    
    if (swr < minSWR) {
      minSWR = swr;
      bestPos = i;
      
      if (swr <= SWR_HEDEF) {
        Serial.println("✓ Hedef SWR bulundu!");
        break;
      }
    }
    
    if (swr > minSWR + 0.5) break;
  }
  
  Serial.println("\n← Geri dönüyor...");
  motorHareket(-(bestPos + 20) * 2, stepHiz);
  delay(200);
  
  Serial.println("\n← Geri yönde tarama...");
  for (int i = 0; i < MAX_TUNE_STEPS; i++) {
    motorHareket(-2, stepHiz);
    delay(50);
    
    float swr = swrOku();
    
    if (i % 20 == 0) {
      Serial.print("  Step ");
      Serial.print(i);
      Serial.print(": SWR = ");
      Serial.println(swr, 2);
    }
    
    if (swr < minSWR) {
      minSWR = swr;
      
      if (swr <= SWR_HEDEF) {
        Serial.println("✓ Hedef SWR bulundu!");
        break;
      }
    }
    
    if (swr > minSWR + 0.5) break;
  }
  
  Serial.println("\n╔═══════════════════════════════════════╗");
  Serial.println("║     TUNING TAMAMLANDI                 ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.print("📊 Final SWR: ");
  Serial.println(minSWR, 2);
}

void bandSweep(float startFreq, float endFreq) {
  Serial.println("\n╔═══════════════════════════════════════╗");
  Serial.println("║        BAND TARAMASI                  ║");
  Serial.println("╚═══════════════════════════════════════╝");
  
  if (!rfAktif) {
    digitalWrite(RF_AMP_ENABLE, LOW);   // Röle AÇIK (aktif LOW) → Amp GÜÇ VER
    rfAktif = true;
    delay(200);
  }
  
  float step = 0.1;  // 100 kHz adım
  Serial.println("Frekans(MHz) | SWR");
  Serial.println("─────────────┼─────");
  
  for (float f = startFreq; f <= endFreq; f += step) {
    ddsSetFrequency(f);
    delay(100);
    
    float swr = swrOku();
    
    Serial.print(f, 3);
    Serial.print("      | ");
    Serial.println(swr, 2);
  }
  
  Serial.println("─────────────┴─────");
  Serial.println("✓ Tarama tamamlandı");
}

void serialEvent() {
  if (Serial.available()) {
    String komut = Serial.readStringUntil('\n');
    komut.trim();
    komut.toUpperCase();
    
    if (komut.startsWith("T")) {
      float freq = komut.substring(1).toFloat();
      if (freq > 0) {
        autoTune(freq);
      }
    }
    else if (komut.startsWith("F")) {
      float freq = komut.substring(1).toFloat();
      if (freq > 0) {
        ddsSetFrequency(freq);
      }
    }
    else if (komut == "M") {
      float swr = swrOku();
      Serial.print("📊 SWR: ");
      Serial.println(swr, 2);
    }
    else if (komut == "R") {
      rfAktif = !rfAktif;
      digitalWrite(RF_AMP_ENABLE, rfAktif ? LOW : HIGH);
      Serial.print("📡 RF Amplifier: ");
      Serial.println(rfAktif ? "AÇIK" : "KAPALI");
    }
    else if (komut.startsWith("SWEEP")) {
      int idx1 = komut.indexOf(' ', 6);
      int idx2 = komut.indexOf(' ', idx1 + 1);
      if (idx1 > 0 && idx2 > 0) {
        float f1 = komut.substring(idx1 + 1, idx2).toFloat();
        float f2 = komut.substring(idx2 + 1).toFloat();
        if (f1 > 0 && f2 > f1) {
          bandSweep(f1, f2);
        }
      }
    }
    else if (komut == "S") {
      Serial.println("\n═══ SİSTEM DURUMU ═══");
      Serial.print("Frekans: ");
      Serial.print(aktifFrekans, 3);
      Serial.println(" MHz");
      Serial.print("RF: ");
      Serial.println(rfAktif ? "AÇIK" : "KAPALI");
      Serial.print("Pozisyon: ");
      Serial.println(mevcutPozisyon);
      Serial.print("Profil: ");
      Serial.println(profilIsimleri[profilIndex]);
    }
    else if (komut == "ZERO") {
      mevcutPozisyon = 0;
      Serial.println("✓ Pozisyon sıfırlandı");
    }
    else if (komut == "1") {
      profilIndex = 0;
      stepsPerTik = profiller[profilIndex][0];
      stepHiz = profiller[profilIndex][1];
      Serial.print("✓ Profil: ");
      Serial.println(profilIsimleri[profilIndex]);
    }
    else if (komut == "2") {
      profilIndex = 1;
      stepsPerTik = profiller[profilIndex][0];
      stepHiz = profiller[profilIndex][1];
      Serial.print("✓ Profil: ");
      Serial.println(profilIsimleri[profilIndex]);
    }
    else if (komut == "3") {
      profilIndex = 2;
      stepsPerTik = profiller[profilIndex][0];
      stepHiz = profiller[profilIndex][1];
      Serial.print("✓ Profil: ");
      Serial.println(profilIsimleri[profilIndex]);
    }
    else if (komut == "H") {
      Serial.println("\n📝 KOMUTLAR:");
      Serial.println("   T[freq]  - Otomatik tune (T7.100)");
      Serial.println("   F[freq]  - Frekans ayarla (F7.100)");
      Serial.println("   M        - SWR ölç");
      Serial.println("   R        - RF Aç/Kapat");
      Serial.println("   MODE TUNE/OPERATE - RF seçim rölesi modu");
      Serial.println("   SWEEP    - Band tarama (SWEEP 7.0 7.3)");
      Serial.println("   S        - Durum");
      Serial.println("   ZERO     - Pozisyon sıfırla");
      Serial.println("   1/2/3    - Profil değiştir");
    }
    else if (komut.startsWith("MODE")) {
      if (komut.indexOf("TUNE") > 0) {
        digitalWrite(RF_MODE_PIN, LOW); // NC devrede → DDS zinciri
        Serial.println("🔀 RF MODE: TUNE (DDS → anten)");
      } else if (komut.indexOf("OPERATE") > 0) {
        digitalWrite(RF_MODE_PIN, HIGH); // NO devrede → TRX
        Serial.println("🔀 RF MODE: OPERATE (TRX → anten)");
      } else {
        Serial.println("⚠️ MODE komutu: MODE TUNE veya MODE OPERATE");
      }
    }
  }
}

void testButonlar() {
  Serial.println("=== BUTON TESTI BASLIYOR ===");
  Serial.println("Butonlara basın, 10 saniye test edecek...");
  
  unsigned long baslangicZamani = millis();
  while(millis() - baslangicZamani < 10000) {
    Serial.print("Buton 7: ");
    Serial.print(digitalRead(BUTON_ILERI) == LOW ? "BASILI" : "SERBEST");
    Serial.print(" | Buton 8: ");
    Serial.println(digitalRead(BUTON_GERI) == LOW ? "BASILI" : "SERBEST");
    delay(200);
  }
  
  Serial.println("=== BUTON TESTI TAMAMLANDI ===");
}