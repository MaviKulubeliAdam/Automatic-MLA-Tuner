# PCB Tasarımı - Manyetik Loop Auto-Tuner v4.0

Bu klasör, Manyetik Loop Auto-Tuner projesi için PCB tasarım dosyalarını içerir.

## 📁 Dosyalar

| Dosya | Açıklama |
|-------|----------|
| `MLA-Tuner.kicad_pro` | KiCad proje dosyası |
| `MLA-Tuner.kicad_sch` | Şematik tasarım |
| `MLA-Tuner.kicad_pcb` | PCB layout tasarımı |
| `BOM.md` | Malzeme listesi (Bill of Materials) |

## 🔧 PCB Özellikleri

- **Boyut**: 150mm x 100mm
- **Katman Sayısı**: 2 (Üst + Alt bakır)
- **Bakır Kalınlığı**: 1oz (35µm)
- **PCB Kalınlığı**: 1.6mm
- **Minimum Track Width**: 0.25mm (sinyal), 1.0mm (güç)
- **Minimum Via**: 0.8mm çap, 0.4mm delik
- **Montaj Delikleri**: M3 (4 köşe)

## 🗺️ PCB Bölge Haritası

```
┌────────────────────────────────────────────────────────────────┐
│  ARDUINO MEGA        │    RF SİNYAL ZİNCİRİ     │   GÜÇ &     │
│  ZONE                │                          │   I/O       │
│  (Modül Soketi)      │  AD9833 → RF AMP →       │             │
│                      │  ATTEN → SWR BRIDGE      │  5V Jack    │
│  Pin Headers:        │                          │  12V Jack   │
│  - DDS (51,52,53)    │  AD8307 #1 (FWD)        │             │
│  - Motor (3,4,9)     │  AD8307 #2 (REF)        │  SMA ANT    │
│  - Relay (10)        │                          │             │
│  - Analog (A0,A1)    │  5V Röle Modülü         │             │
│                      ├──────────────────────────┤             │
│  Encoder Conn (J5)   │    MOTOR DRIVER ZONE     │  Motor      │
│  Button Conn         │                          │  Power      │
│  (SW2, SW3)          │    TB6600 Connector      │             │
│                      │    Motor Connector       │             │
└────────────────────────────────────────────────────────────────┘
```

## 📍 Bağlantı Pin Haritası

### Arduino Mega → Modüller

| Arduino Pin | Bağlantı | Modül |
|------------|----------|-------|
| Pin 51 (MOSI) | SDATA | AD9833 DDS |
| Pin 52 (SCK) | SCLK | AD9833 DDS |
| Pin 53 (SS) | FSYNC | AD9833 DDS |
| Pin 4 | PUL- | TB6600 |
| Pin 3 | DIR- | TB6600 |
| Pin 9 | ENA- | TB6600 |
| Pin 10 | IN | 5V Röle |
| Pin 2 | CLK | Encoder |
| Pin 5 | DT | Encoder |
| Pin 6 | SW | Encoder |
| Pin 7 | BTN | İleri Buton |
| Pin 8 | BTN | Geri Buton |
| A0 | OUT | AD8307 #1 |
| A1 | OUT | AD8307 #2 |

### Güç Dağılımı

```
5V PSU ─────┬───► Arduino VIN
            ├───► AD9833 VCC
            ├───► AD8307 #1 VCC
            ├───► AD8307 #2 VCC
            ├───► TB6600 PUL+/DIR+/ENA+
            ├───► Encoder VCC
            └───► Röle VCC

12V PSU ────┬───► Röle COM
            └───► (Röle NO → RF Amp VCC)

24V PSU ────────► TB6600 Motor Power (opsiyonel)
```

## ⚡ RF Sinyal Akışı

```
AD9833 SMA ───┬──► RF Amplifier IN
              │       │
              │       ▼
              │   RF Amplifier OUT (~1W)
              │       │
              │       ▼
              │   20dB Attenuator (~10mW)
              │       │
              │       ▼
              │   SWR Bridge IN
              │       │
              │   ┌───┴───┐
              │   ▼       ▼
              │  FWD     REF
              │   │       │
              │   ▼       ▼
              │ AD8307#1 AD8307#2
              │   │       │
              │   ▼       ▼
              │  A0      A1
              │
              └──► SWR Bridge OUT ──► ANTEN (SMA)
```

## 🔩 Montaj Talimatları

### Adım 1: PCB Üretimi
1. Gerber dosyalarını PCB üreticisine gönderin (JLCPCB, PCBWay, vb.)
2. Önerilen ayarlar:
   - 2 katman
   - 1.6mm kalınlık
   - HASL finish
   - Yeşil soldermask

### Adım 2: SMD Bileşenler
1. Bypass kapasitörleri lehimleyin (C1-C5)
2. Pad'leri kontrol edin

### Adım 3: THT Bileşenler
1. Pin header'ları yerleştirin
2. DC jack'leri lehimleyin
3. SMA konnektörü monte edin
4. Montaj deliklerine standoff'ları takın

### Adım 4: Modül Montajı
1. Arduino Mega'yı pin header'lara takın
2. AD9833 modülünü bağlayın
3. AD8307 modüllerini bağlayın
4. RF Amplifier'ı bağlayın
5. 5V röle modülünü bağlayın
6. TB6600'ü harici olarak bağlayın

### Adım 5: Kablo Bağlantıları
1. RF kablolarını bağlayın (SMA)
2. Güç kablolarını bağlayın
3. Motor kablolarını bağlayın
4. Encoder/buton kablolarını bağlayın

## ⚠️ Dikkat Edilmesi Gerekenler

### Güç
- [ ] 5V ve 12V polaritesini kontrol edin
- [ ] Ortak GND bağlantısını sağlayın
- [ ] İlk açılışta RF Amp'i kapalı tutun

### RF
- [ ] SMA konnektörleri sıkıca bağlayın
- [ ] RF kabloları kısa tutun (<15cm)
- [ ] Koaksiyel kabloların shield'lerini GND'ye bağlayın

### EMI
- [ ] Dijital ve RF bölgelerini ayırın
- [ ] Ground plane bütünlüğünü koruyun
- [ ] Bypass kapasitörlerini modüllere yakın yerleştirin

## 🧪 Test Prosedürü

### 1. Görsel Kontrol
- [ ] Kısa devre kontrolü (multimetre)
- [ ] Lehim kalitesi kontrolü
- [ ] Bileşen yönleri kontrolü

### 2. Güç Testi
- [ ] 5V rail voltajı (beklenen: 4.8-5.2V)
- [ ] 12V rail voltajı (beklenen: 11.5-12.5V)
- [ ] Akım tüketimi (beklenen: <500mA idle)

### 3. İşlevsel Test
- [ ] Arduino'ya kod yükle
- [ ] Serial monitor'ü aç (115200 baud)
- [ ] DDS frekans testi (F7.100 komutu)
- [ ] Röle testi (R komutu)
- [ ] Motor testi (butonlar)
- [ ] SWR ölçüm testi (M komutu)

## 📐 Gerber Dosyaları

KiCad'de Gerber dosyalarını oluşturmak için:

1. **File → Plot**
2. Aşağıdaki katmanları seçin:
   - F.Cu (Üst bakır)
   - B.Cu (Alt bakır)
   - F.SilkS (Üst silkscreen)
   - B.SilkS (Alt silkscreen)
   - F.Mask (Üst soldermask)
   - B.Mask (Alt soldermask)
   - Edge.Cuts (Kart sınırları)
3. **Plot Format: Gerber**
4. **Generate Drill Files** → Excellon format

## 🔗 İlgili Dökümanlar

- [FULL-WIRING-DIAGRAM.txt](../FULL-WIRING-DIAGRAM.txt) - Detaylı bağlantı şeması
- [auto-mla-tuner.ino](../code/auto-mla-tuner.ino) - Arduino kaynak kodu
- [BOM.md](./BOM.md) - Malzeme listesi

## 📝 Versiyon Geçmişi

| Versiyon | Tarih | Değişiklikler |
|----------|-------|---------------|
| 4.0 | 2024-01-15 | İlk PCB tasarımı, AD9833 DDS tabanlı |

---

**Not**: Bu PCB tasarımı amatör radyo kullanımı içindir. RF yayın yapmadan önce lisans gereksinimlerini kontrol edin.
