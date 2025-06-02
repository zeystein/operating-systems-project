#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

#define KAT_SAYISI 10
#define DAIRE_SAYISI 4
#define ISCISAYISI 3
#define HARC_MAKINE_SAYISI 2
#define KALIP_TAKIMI_SAYISI 3

sem_t su_sem;
sem_t elektrik_sem;
sem_t dogalgaz_sem;
sem_t mikser_sem;
sem_t vinc_sem;
sem_t isci_sem;
sem_t harc_sem;
sem_t kalip_sem;
sem_t vana_sem;

void* daire_isleri(void* arg) {
    int kat = ((int*)arg)[0];
    int daire = ((int*)arg)[1];
    free(arg);  /* Bellek sizintisini önlemek için malloc
     ile alinan bellegi direkt serbest biraktik */

    // Vinc ile malzeme tasima (ortak kaynak)
    sem_wait(&vinc_sem);
    printf("[Kat %d - Daire %d] Vinc ile malzeme tasiniyor...\n", kat, daire);
    sleep(1);
    sem_post(&vinc_sem);

    // Isci goreve basliyor(ortak havuz)
    sem_wait(&isci_sem);
    printf("[Kat %d - Daire %d] isci calismaya basladi...\n", kat, daire);
    sleep(1);

    // Kalip takimi
    sem_wait(&kalip_sem);
    printf("[Kat %d - Daire %d] Kalip kuruluyor...\n", kat, daire);
    sleep(1);
    sem_post(&kalip_sem);

    // Beton mikseri
    sem_wait(&mikser_sem);
    printf("[Kat %d - Daire %d] Beton dokuluyor...\n", kat, daire);
    sleep(1);
    sem_post(&mikser_sem);

    // Harc makinesi (siva icin)
    sem_wait(&harc_sem);
    printf("[Kat %d - Daire %d] Harc makinesi kullaniliyor...\n", kat, daire);
    sleep(1);
    sem_post(&harc_sem);

    // Siva islemi
    printf("[Kat %d - Daire %d] Siva basliyor...\n", kat, daire);
    sleep(1);
    printf("[Kat %d - Daire %d] Siva tamamlandi.\n", kat, daire);

    // Kapi/Pencere montaji
    printf("[Kat %d - Daire %d] Kapi ve pencere montaji basliyor...\n", kat, daire);
    sleep(1);
    printf("[Kat %d - Daire %d] Kapi ve pencere montaji tamamlandi.\n", kat, daire);

    // Zemin seramik döseme
    printf("[Kat %d - Daire %d] Zemin seramik döseme basliyor...\n", kat, daire);
    sleep(1);
    printf("[Kat %d - Daire %d] Zemin seramik döseme tamamlandi.\n", kat, daire);

    // Isi yalitim montaji (dis cephe)
    printf("[Kat %d - Daire %d] Isi yalitim montaji basliyor...\n", kat, daire);
    sleep(1);
    printf("[Kat %d - Daire %d] Isi yalitim montaji tamamlandi.\n", kat, daire);

    // Su tesisati
    sem_wait(&su_sem);
    printf("[Kat %d - Daire %d] Su tesisati döseniyor...\n", kat, daire);
    sleep(1);
    sem_post(&su_sem);

    // Su vanasina baglanti
    sem_wait(&vana_sem);
    printf("[Kat %d - Daire %d] Su vanasina baglanti yapiliyor...\n", kat, daire);
    sleep(1);
    sem_post(&vana_sem);

    // Elektrik tesisati
    sem_wait(&elektrik_sem);
    printf("[Kat %d - Daire %d] Elektrik kablolamasi yapiliyor...\n", kat, daire);
    sleep(1);
    sem_post(&elektrik_sem);

    // Dogalgaz hatti
    sem_wait(&dogalgaz_sem);
    printf("[Kat %d - Daire %d] Doðalgaz hattý döþeniyor...\n", kat, daire);
    sleep(1);
    sem_post(&dogalgaz_sem);

    // Is bitisi
    printf("[Kat %d - Daire %d] Isci isi tamamladi.\n", kat, daire);
    sem_post(&isci_sem);

    printf("[Kat %d - Daire %d] Daire tamamlandi.\n", kat, daire);
    pthread_exit(NULL);
}

void kat_insa_et(int kat) {
    pthread_t threadler[DAIRE_SAYISI];
    for (int i = 0; i < DAIRE_SAYISI; i++) {
        int* arg = malloc(2 * sizeof(int));
        arg[0] = kat;
        arg[1] = i + 1;
        pthread_create(&threadler[i], NULL, daire_isleri, arg);
    }
    for (int i = 0; i < DAIRE_SAYISI; i++) {
        pthread_join(threadler[i], NULL);
    }
    printf("[Kat %d] Tüm daireler tamamlandi.\n", kat);
}

int main() {
    // Ortak kaynaklar (semaforlar) baslatiliyor
    sem_init(&su_sem, 1, 1);
    sem_init(&elektrik_sem, 1, 1);
    sem_init(&dogalgaz_sem, 1, 1);
    sem_init(&mikser_sem, 1, 1);
    sem_init(&vinc_sem, 1, 1);
    sem_init(&isci_sem, 1, ISCISAYISI);
    sem_init(&harc_sem, 1, HARC_MAKINE_SAYISI);
    sem_init(&kalip_sem, 1, KALIP_TAKIMI_SAYISI);
    sem_init(&vana_sem, 1, 1);

    for (int i = 1; i <= KAT_SAYISI; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("[Kat %d] Insaat basliyor...\n", i);
            kat_insa_et(i);
            printf("[Kat %d] Insaat tamamlandi.\n", i);
            exit(0);
        } else {
            wait(NULL);
        }
    }

    // Kaynaklar serbest birakiliyor
    sem_destroy(&su_sem);
    sem_destroy(&elektrik_sem);
    sem_destroy(&dogalgaz_sem);
    sem_destroy(&mikser_sem);
    sem_destroy(&vinc_sem);
    sem_destroy(&isci_sem);
    sem_destroy(&harc_sem);
    sem_destroy(&kalip_sem);
    sem_destroy(&vana_sem);

    return 0;
}
