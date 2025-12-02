# Parça Listesi (BOM)

Manyetik Loop Auto‑Tuner – v4.0 (AD9851)

Bu liste FULL-WIRING-DIAGRAM.txt temel alınarak hazırlanmıştır. Adetler tek bir sistem içindir; kablo/konnektörlerde uzunluklar şemadaki önerilere göredir.

## Ana Modüller
- Arduino Mega 2560
  - Adet: 1
  - Özellik: 5V ile besleme, USB programlama
- DDS Modülü: AD9851 (SMA çıkışlı tercih) — opsiyon: AD9850
  - Adet: 1
  - Özellik: AD9851: 0–60+ MHz sinüs (PLL x6 ile ~180 MHz sistem saat)
  - Opsiyon: AD9850 (0–40 MHz)
  - Bant notu: Tüm HF (1.8–30 MHz) + 6 m (50–54 MHz)
- RF Amplifier Modülü (1 MHz – 3 GHz Çin klonu)
  - Adet: 1
  - Özellik: ~+20 dB kazanç, ~0.5–1 W çıkış, 12 V besleme
  - Bant notu: 1–3000 MHz (uygulamada HF’de kullanılacak)
- Röle Modülü 5V (1 kanal)
  - Adet: 1
  - Özellik: Aktif LOW, Arduino Pin 10 ile sürülür, RF Amp 12V anahtarlama
- Step Motor Sürücü: TB6600
  - Adet: 1
  - Özellik: PUL/DIR/ENA, 12–24 V motor beslemesi
- Step Motor: NEMA 17
  - Adet: 1
  - Özellik: 4 telli, kapasitör şaftını sürer

## RF Zinciri
- 20 dB Attenuator, 50Ω, ≥2 W
  - Adet: 1
  - Özellik: RF Amp çıkışını ~10 mW seviyesine düşürür
- SWR Bridge (akım trafosu tipi)
  - Adet: 1
  - Özellik: T50‑2 toroid, 10 tur primer, 2 tur FWD, 2 tur REF
  - Bant notu: HF (yaklaşık 1–30 MHz)
- AD8307 Logaritmik RF Dedektör Modülü/Devresi
  - Adet: 2 (FWD ve REF)
  - Özellik: OUT 0–2.5 V DC (Arduino A0/A1)
  - Bant notu: Geniş bant; HF’de örnekleme için uygun

## Anten ve Ayar Mekaniği
- Manyetik Loop Anten (Loop elemanı)
  - Adet: 1
  - Özellik: Çap/iletken kesiti kullanıcıya bağlı
- Vakum Değişken Kapasitör (Loop tuning)
  - Adet: 1
  - Özellik: Değer aralığı anten boyutuna bağlı; HF için yüksek gerilim dayanımı
- Mekanik Kaplin / Kayış (Motor ↔ Kapasitör)
  - Adet: 1 set

## Güç ve Kontrol
- 12 V / 2 A Güç Kaynağı (RF Amplifier)
  - Adet: 1
- 5 V / 2 A Güç Kaynağı (Arduino ve modüller)
  - Adet: 1
- 24 V Motor Güç Kaynağı (opsiyonel, motor gereksinimine göre)
  - Adet: 1
- Rotary Encoder (CLK/DT/SW)
  - Adet: 1
- Push Buton (İleri)
  - Adet: 1
- Push Buton (Geri)
  - Adet: 1

## Kablolar
- RG174 SMA Koaksiyel Patch Kablo (Erkek‑Erkek), ~15 cm
  - Adet: 2 (AD9851→Amp, Amp→Attenuator)
- RG174 Koaksiyel, ~10–15 cm (SWR örnekleme ve kısa bağlantılar)
  - Adet: Çeşitli (projeye göre)
- RG58 veya RG213 Koaksiyel, 50–100 cm (SWR Bridge → Anten)
  - Adet: 1
- Dupont Jumper Kablolar (Erkek‑Dişi / Dişi‑Dişi)
  - Adet: Set (AD9851 DATA/W_CLK/FQ_UD/RESET, analog ölçüm, röle, encoder, TB6600)
- Güç kablosu 18AWG (kırmızı/siyah), 12 V hat için ~30 cm
  - Adet: 1

## Konnektörler ve Donanım
- SMA konnektörler / adaptörler (gerektiği kadar)
  - Adet: Çeşitli
  - Not: RF yolunda SMA tercih edilir
- BNC (opsiyonel) / SO239‑PL259 (anten paneli için opsiyonel)
  - Adet: İhtiyaca göre
- 2.54 mm pin header, JST‑XH (kalıcı montaj için opsiyonel)
  - Adet: Çeşitli
- DC Jack 2.1 mm (PSU bağlantıları için)
  - Adet: 2–3
- Metal kutu/şasi + standoff’lar (EMI koruma ve montaj)
  - Adet: 1 set

## Pasifler ve Küçük Parçalar
- T50‑2 Toroid (kırmızı, demir tozu)
  - Adet: 1
- Emaye tel (SWR bridge sarımı için)
  - Adet: Yeterli uzunluk
- By‑pass kapasitörleri: 100 nF seramik (her modül VCC yakınında)
  - Adet: 10–15
- Elektrolitik kapasitör: 100 µF (RF Amp beslemesi)
  - Adet: 1–2
- Opsiyonel RC filtre (AD9851 SINE OUT hattı için)
  - 100 Ω direnç (1/4 W): 1
  - 100 pF C0G/NP0 seramik: 1
- Isı dağıtımı: Heatsink/fan (RF Amp için önerilir)
  - Adet: 1

## Seri Bağlantılar (Pin Özet)
- DDS (Arduino Mega ↔ AD9851): DATA 51, W_CLK 52, FQ_UD 53, RESET 49, 5V, GND
- Analog (Arduino Mega): A0=FWD, A1=REF (AD8307 OUT)
- Motor Sürücü (TB6600): STEP 4, DIR 3, ENABLE 9; PUL+/DIR+/ENA+ → 5V
- Röle IN: Arduino Pin 10 (aktif LOW), Röle VCC=5V, GND ortak
- Encoder: CLK 2, DT 5, SW 6; VCC 5V, GND
- Butonlar: BTN_FWD 7, BTN_REV 8; karşı uçları GND

## Desteklenen Amatör Bantlar
- Kapsam (DDS=AD9851): Sistem ölçüm/sweep için yaklaşık 1.0–60 MHz.
  - Pratikte loop + vakum kapasitör ile HF: 1.8–30 MHz odak; 6 m de mümkündür.
  - Sınırlayıcı parça: `AD9851` (0–60+ MHz), RF Amp 1–3000 MHz, AD8307/T50‑2 HF için uygun.
- HF bantları (ITU Region 1 tipik atamalar):
  - 160 m: 1.8–2.0 MHz ✓
  - 80 m: 3.5–4.0 MHz ✓
  - 60 m: ~5 MHz (ülke/regülasyon kanal/segment) ✓
  - 40 m: 7.0–7.3 MHz ✓
  - 30 m: 10.1–10.15 MHz ✓
  - 20 m: 14.0–14.35 MHz ✓
  - 17 m: 18.068–18.168 MHz ✓
  - 15 m: 21.0–21.45 MHz ✓
  - 12 m: 24.89–24.99 MHz ✓
  - 10 m: 28.0–29.7 MHz ✓
  - 6 m: 50–54 MHz ✓
- Notlar:
  - 6 m (50–54 MHz) için AD9851 veya Si5351 + LPF tercih edilebilir.
  - Gerçek kapsama loop çapı ve kapasitör min‑maks pF değerine bağlıdır.

## Notlar
- Tüm GND’ler ortak ground bus’a bağlanmalı.
- RF yolunda 50Ω empedans korunmalı ve kablolar kısa tutulmalı.
- SWR > 3:1 durumunda sistemi durduracak yazılım eşiği ayarlayın.
- Bant kapsamı, loop çapı ve kapasitör değerine bağlıdır; AD9851 HF + 6 m için uygundur.
