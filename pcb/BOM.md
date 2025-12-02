# MANYETIK LOOP AUTO-TUNER - MALZEME LİSTESİ (BOM)
# Bill of Materials - v4.0 FINAL
# ============================================

## ANA BİLEŞENLER

| Ref | Bileşen | Değer/Model | Miktar | Açıklama | Temin |
|-----|---------|-------------|--------|----------|-------|
| U1 | Arduino Mega 2560 | ATmega2560 | 1 | Ana kontrolör | Arduino Store / AliExpress |
| U2 | DDS Modülü | AD9833 | 1 | Sinyal üretici (0-12.5MHz) | AliExpress / eBay |
| U3 | RF Amplifier | 1MHz-3GHz | 1 | +20dB wideband amplifier | AliExpress |
| K1 | Röle Modülü | 5V 1-CH | 1 | RF Amp güç kontrolü (aktif LOW) | AliExpress |
| U4 | Motor Sürücü | TB6600 | 1 | Step motor driver | AliExpress |
| M1 | Step Motor | NEMA 17 | 1 | Vakum kapasitör ayarı | AliExpress |
| U5 | RF Dedektör | AD8307 | 1 | Forward power ölçüm | AliExpress / Mouser |
| U6 | RF Dedektör | AD8307 | 1 | Reflected power ölçüm | AliExpress / Mouser |
| BR1 | SWR Bridge | T50-2 Toroid | 1 | Elle sarılacak (10T pri, 2T sec) | Amidon / AliExpress |
| ATT1 | Attenuator | 20dB | 1 | 50Ω, güç azaltma | AliExpress |
| SW1 | Rotary Encoder | EC11 | 1 | Profil seçimi | AliExpress |
| SW2 | Push Button | 6mm | 1 | İleri buton (Pin 7) | Yerel elektronikçi |
| SW3 | Push Button | 6mm | 1 | Geri buton (Pin 8) | Yerel elektronikçi |

## GÜÇ BİLEŞENLERİ

| Ref | Bileşen | Değer/Model | Miktar | Açıklama | Temin |
|-----|---------|-------------|--------|----------|-------|
| J1 | DC Jack | 2.1mm | 1 | 5V/2A güç girişi | Yerel elektronikçi |
| J2 | DC Jack | 2.1mm | 1 | 12V/2A güç girişi | Yerel elektronikçi |
| PS1 | Güç Kaynağı | 5V/2A | 1 | Arduino ve mantık devreleri | AliExpress |
| PS2 | Güç Kaynağı | 12V/2A | 1 | RF Amplifier | AliExpress |
| PS3 | Güç Kaynağı | 24V/2A | 1 | Step motor (opsiyonel) | AliExpress |

## KONNEKTÖRLER

| Ref | Bileşen | Değer/Model | Miktar | Açıklama | Temin |
|-----|---------|-------------|--------|----------|-------|
| J3 | SMA Konnektör | SMA-F | 1 | Anten çıkışı (panel mount) | AliExpress |
| J4 | Pin Header | 1x4 | 1 | Motor bağlantısı | Yerel elektronikçi |
| J5 | Pin Header | 1x5 | 1 | Encoder bağlantısı | Yerel elektronikçi |
| J6 | Pin Header | 1x6 | 1 | TB6600 kontrol sinyalleri | Yerel elektronikçi |
| J7 | Terminal Block | 2-pos | 2 | Güç bağlantıları | Yerel elektronikçi |
| - | SMA Kablo | Erkek-Erkek | 2 | RF bağlantıları (RG174, 15cm) | AliExpress |
| - | Koaksiyel Kablo | RG58/RG213 | 1m | Anten bağlantısı | Yerel ham radio mağazası |
| - | Dupont Kablo | Dişi-Dişi | 20pcs | Modül bağlantıları | AliExpress |

## BYPASS / DECOUPLING KAPASİTÖRLER

| Ref | Bileşen | Değer | Miktar | Açıklama | Temin |
|-----|---------|-------|--------|----------|-------|
| C1 | Seramik | 100nF | 1 | AD9833 bypass | Yerel elektronikçi |
| C2 | Elektrolitik | 100µF/25V | 1 | RF Amp bulk | Yerel elektronikçi |
| C3 | Seramik | 100nF | 1 | RF Amp bypass | Yerel elektronikçi |
| C4 | Seramik | 100nF | 1 | AD8307 #1 bypass | Yerel elektronikçi |
| C5 | Seramik | 100nF | 1 | AD8307 #2 bypass | Yerel elektronikçi |
| C6 | Seramik | 10nF | 2 | DDS filtre (opsiyonel) | Yerel elektronikçi |
| C7 | Elektrolitik | 10µF/10V | 2 | 5V rail bulk | Yerel elektronikçi |

## DİRENÇLER

| Ref | Bileşen | Değer | Miktar | Açıklama | Temin |
|-----|---------|-------|--------|----------|-------|
| R1 | Direnç | 100Ω | 1 | DDS çıkış filtre (opsiyonel) | Yerel elektronikçi |
| R2 | Direnç | 50Ω | 1 | Termination (gerekirse) | Yerel elektronikçi |

## MONTAJ PARÇALARI

| Ref | Bileşen | Model | Miktar | Açıklama | Temin |
|-----|---------|-------|--------|----------|-------|
| H1-H4 | Montaj Vidası | M3x6 | 4 | PCB montajı | Yerel hırdavat |
| - | Standoff | M3x10 | 4 | PCB yükseltici | Yerel hırdavat |
| - | Somun | M3 | 8 | Montaj | Yerel hırdavat |
| - | Metal Kutu | 200x150x50mm | 1 | EMI koruması | AliExpress |
| - | Flexible Coupling | 5mm-8mm | 1 | Motor-kapasitör bağlantı | AliExpress |
| - | Heatsink | TO-220 | 1 | RF Amp soğutma | Yerel elektronikçi |

## SWR BRIDGE YAPIMI İÇİN

| Ref | Bileşen | Model | Miktar | Açıklama | Temin |
|-----|---------|-------|--------|----------|-------|
| T1 | Toroid | T50-2 | 1 | Kırmızı (1-30MHz) | Amidon / Ham radio |
| - | Emaye Tel | 0.5mm | 50cm | Sarım için | Yerel elektronikçi |
| - | SMA Konnektör | SMA-F | 2 | Giriş/Çıkış | AliExpress |

**NOT**: Schottky diyotlar (1N5711) opsiyoneldir ve AD8307 yerine basit
diode dedektörü kullanmak isteyenler için alternatif bir tasarımdır.
Bu projede AD8307 kullanıldığından gerekli değildir.

## OPSİYONEL BİLEŞENLER

| Ref | Bileşen | Model | Miktar | Açıklama | Temin |
|-----|---------|-------|--------|----------|-------|
| - | LCD Display | 16x2 / 20x4 | 1 | Durum gösterimi | AliExpress |
| - | I2C Adapter | PCF8574 | 1 | LCD için | AliExpress |
| - | LED | 3mm | 3 | Durum LED'leri | Yerel elektronikçi |
| - | Buzzer | 5V Pasif | 1 | Sesli uyarı | Yerel elektronikçi |

---

## FİYAT TAHMİNİ (Yaklaşık)

| Kategori | Tahmini Fiyat |
|----------|---------------|
| Ana bileşenler | ~$80-100 |
| Güç kaynakları | ~$15-20 |
| Konnektörler ve kablolar | ~$15-20 |
| Pasif bileşenler | ~$5-10 |
| Montaj parçaları | ~$10-15 |
| **TOPLAM** | **~$125-165** |

---

## SİPARİŞ NOTLARI

1. **AD9833 Modülü**: 25MHz kristal versiyonunu tercih edin 
   (25MHz kristal = 12.5MHz maksimum çıkış frekansı, Nyquist teoremi)
   Bu frekans aralığı HF amatör bantları için idealdir (1.8-30 MHz'in alt yarısı).
2. **RF Amplifier**: Heatsink ile birlikte alın, 12V/2A güç sağlayın
3. **AD8307**: Orijinal Analog Devices chip tercih edin (klon kalitesi düşük)
4. **TB6600**: 4A/40V versiyon yeterli, microstepping DIP switch ayarı var
5. **SMA Kablolar**: RG174 (ince, esnek) HF için yeterli
6. **Toroid**: T50-2 (kırmızı) 1-30MHz için ideal, alternatif FT50-43

## SATINALMA LİNKLERİ (Örnek)

- Arduino Mega: https://www.arduino.cc/
- AliExpress: https://www.aliexpress.com/
- Mouser: https://www.mouser.com/
- DigiKey: https://www.digikey.com/
- Amidon (Toroid): https://www.amidoncorp.com/

---
Son Güncelleme: 2024-01-15
Versiyon: 4.0 FINAL
