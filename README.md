# 🏗️ Apartman İnşaatı Simülasyonu

Bu proje, 2024-2025 Bahar Dönemi **İşletim Sistemleri** dersi kapsamında hazırlanmış, çok süreçli (process) ve çok iş parçacıklı (thread) programlama, semaforlar ve senkronizasyon kavramlarını uygulamalı olarak içeren bir **apartman inşaatı simülasyonudur**.

## 👥 Katkıda Bulunanlar

- **Zeynep Sude Güneş** –
- **Ahmet Can Yarba** –

---

## 🎯 Proje Amacı

Bu simülasyonun amacı, bir apartman inşaatındaki katları süreç (process) ve daireleri iş parçacığı (thread) olarak modelleyerek:

- Paralel programlamayı öğrenmek
- Ortak kaynak yönetimini simüle etmek
- Senkronizasyon (semaphore) kullanımını uygulamak

---

## ⚙️ Sistem Modeli

- **Katlar**: Her kat bir process olarak çalışır (`fork()`).
- **Daireler**: Her daire, kat içindeki bir thread olarak temsil edilir (`pthread_create()`).
- **Ortak Kaynaklar**: Semaforlarla senkronize edilen sınırlı kaynaklar:
  - Vinç
  - Beton mikseri
  - İşçi havuzu
  - Harç makinesi
  - Kalıp takımı
  - Su hattı ve ana vana
  - Elektrik ve doğalgaz sistemleri

---

## 🔄 Süreç Akışı

Her daire (thread) aşağıdaki işleri sırasıyla gerçekleştirir:

1. Vinç ile malzeme taşıma *(senkronize)*
2. İşçiye erişim *(senkronize)*
3. Kalıp kurma *(senkronize)*
4. Beton dökme *(senkronize)*
5. Harç makinesi ile sıva hazırlama *(senkronize)*
6. Sıva işlemi *(eşzamanlı olabilir)*
7. Kapı ve pencere montajı *(senkronizasyon yok)*
8. Zemin seramik döşeme *(senkronizasyon yok)*
9. Isı yalıtım montajı *(senkronizasyon yok)*
10. Su tesisatı ve vana bağlantısı *(senkronize)*
11. Elektrik ve doğalgaz hattı döşeme *(senkronize)*
12. İşçinin işi bitirmesi *(işçi havuzuna geri dönme)*

Her işlem sırasında, sınırlı kaynaklar `sem_wait()` / `sem_post()` ile kontrol altına alınır.

---

## 🧪 Teknik Detaylar

- **Dil**: C
- **Thread yönetimi**: `pthread`
- **Process yönetimi**: `fork()`, `wait()`
- **Senkronizasyon**: POSIX `sem_t` semaforları

> **Uyarı:** `fork()` ve `<sys/wait.h>` POSIX sistem çağrıları olduğundan proje **Linux** veya **WSL (Windows Subsystem for Linux)** ortamında derlenmeli ve çalıştırılmalıdır.

### 🔧 Derleme Komutu (Linux)
```bash
gcc -o apartman main.c -lpthread
./apartman
