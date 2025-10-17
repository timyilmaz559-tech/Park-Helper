#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#define OLED_WIDTH 128
#define OLED_HEIGTH 64
#define OLED_RESET -1
Adafruit_SSD1306 display(OLER_WIDTH, OLED_HEIGTH, &Wire, OLED_RESET);

const int trig = 2;
const int echo = 3;
const int buzzer = 4;
const int button = 5;

struct datas {
    long duration;
    int distance;
    long dk;
    bool state = LOW;// buzzer için
    String durum = "";// sistem aktivasyon durum
};
datas room;

unsigned long baslangic_zamani = 0;
const long sure = room.dk;

void setup() {
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(button, INPUT);

    if(!display.begin(SSD1306_SWITCHAPPVCC)) {
        for(;;);
    }

}

void loop() {
    // Ultrasonik mesafe sensörü okuması
    digitalWrite(trig, HIGH);
    delayMicroseconds(2);
    digitalWrite(trig, LOW);
    delayMicroseconds(10);
    digitalWrite(trig, HIGH);
    delayMicroseconds(2);
    room.duration = pulseIn(echo, HIGH);
    room.distance = room.duration * 0.034 / 2;

    unsigned long gecen_zaman = millis();

    // Sıralı işlemler
    if(digitalRead(button) == HIGH) {
        room.durum = "Aktif";
        if(room.distance >= 20) {
            digitalWrite(buzzer, LOW);
        }else if(room.distance >= 15) {
            room.dk = 750;
            if(gecen_zaman - baslangic_zamani >= sure) {
                room.state = !room.state; // durumu true false yap
                digitalWrite(buzzer, room.state);
                baslangic_zamani = gecen_zaman;
            }
        }else if(room.distance >= 10) {
            room.dk = 500;
            if(gecen_zaman - baslangic_zamani >= sure) {
                room.state = !room.state; // durumu true false yap
                digitalWrite(buzzer, room.state);
                baslangic_zamani = gecen_zaman;
            }
        }else if(room.distance >= 5) {
            room.dk = 250;
            if(gecen_zaman - baslangic_zamani >= sure) {
                room.state = !room.state; // durumu true false yap
                digitalWrite(buzzer, room.state);
                baslangic_zamani = gecen_zaman;
            }
        }
    } else {
        room.durum = "Pasif";
        digitalWrite(buzzer, LOW);
    }

    // Ekran işlemleri
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);

    display.println("Uyari: " + room.durum);
    display.println("Mesafe: " + String(room.distance));

    display.clearDisplay();
    display.display();

    delay(250);// Genel döngü için süre
}