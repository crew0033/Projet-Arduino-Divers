#include <LiquidCrystal.h> // Inclut la bibliothèque LiquidCrystal
#include <Keypad.h>

#define buzzer 8 // Buzzer connecté au pin 8

String motDePasse = "1234";
String motDePasseTemp;
boolean alarmeActivee = false;
boolean activerAlarme = false;
boolean motDePasseEntre; 
boolean modeChangementMotDePasse = false;
boolean motDePasseChange = false;
int messageEcranAffiche = 0;

const byte LIGNES = 4;
const byte COLONNES = 4;
char touchePressee;

// Carte des touches du clavier
char carteTouches[LIGNES][COLONNES] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinsLignes[LIGNES] = {14, 15, 16, 17}; 
byte pinsColonnes[COLONNES] = {18, 19, 20, 21}; 

Keypad monClavier = Keypad(makeKeymap(carteTouches), pinsLignes, pinsColonnes, LIGNES, COLONNES); 
LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // LCD connecté aux pins correspondants

void setup() {
  lcd.begin(16,2); 
  pinMode(buzzer, OUTPUT); 
}

void loop() {
  if (activerAlarme) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alarme dans...");
    lcd.setCursor(0, 1);
    lcd.print("Activation dans");

    int compteARebours = 9;
    while (compteARebours != 0) {
      lcd.setCursor(14, 1);
      lcd.print(compteARebours);
      compteARebours--;
      tone(buzzer, 700, 100);
      delay(1000);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alarme Activee !");
    activerAlarme = false;
    alarmeActivee = true;
  }

  if (alarmeActivee) {
    tone(buzzer, 1000); // Son de l'alarme
    lcd.clear();
    entrerMotDePasse();
  }

  if (!alarmeActivee) {
    if (messageEcranAffiche == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("A - Activer");
      lcd.setCursor(0, 1);
      lcd.print("B - Changer MDP");
      messageEcranAffiche = 1;
    }

    touchePressee = monClavier.getKey();
    if (touchePressee == 'A') {
      tone(buzzer, 1000, 200);
      activerAlarme = true;
    } else if (touchePressee == 'B') {
      lcd.clear();
      int i = 1;
      tone(buzzer, 2000, 100);
      motDePasseTemp = "";
      lcd.setCursor(0, 0);
      lcd.print("MDP Actuel:");
      lcd.setCursor(0, 1);
      lcd.print(">");

      modeChangementMotDePasse = true;
      motDePasseChange = true;

      while (motDePasseChange) {
        touchePressee = monClavier.getKey();
        if (touchePressee >= '0' && touchePressee <= '9') {
          motDePasseTemp += touchePressee;
          lcd.setCursor(i, 1);
          lcd.print("*");
          i++;
          tone(buzzer, 2000, 100);
        }

        if (i > 5 || touchePressee == '#') {
          motDePasseTemp = "";
          i = 1;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("MDP Actuel:");
          lcd.setCursor(0, 1);
          lcd.print(">");
        }

        if (touchePressee == '*') {
          i = 1;
          if (motDePasseTemp == motDePasse) {
            motDePasseTemp = "";
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Nouveau MDP:");
            lcd.setCursor(0, 1);
            lcd.print(">");

            while (modeChangementMotDePasse) {
              touchePressee = monClavier.getKey();
              if (touchePressee >= '0' && touchePressee <= '9') {
                motDePasseTemp += touchePressee;
                lcd.setCursor(i, 1);
                lcd.print("*");
                i++;
                tone(buzzer, 2000, 100);
              }

              if (i > 5 || touchePressee == '#') {
                motDePasseTemp = "";
                i = 1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Nouveau MDP:");
                lcd.setCursor(0, 1);
                lcd.print(">");
              }

              if (touchePressee == '*') {
                i = 1;
                tone(buzzer, 2000, 100);
                motDePasse = motDePasseTemp;
                modeChangementMotDePasse = false;
                motDePasseChange = false;
                messageEcranAffiche = 0;
              }
            }
          }
        }
      }
    }
  }
}

void entrerMotDePasse() {
  int k = 5;
  motDePasseTemp = "";
  boolean entreMotDePasse = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" *** ALARME *** ");
  lcd.setCursor(0, 1);
  lcd.print("MDP >");

  while (entreMotDePasse) {
    touchePressee = monClavier.getKey();
    if (touchePressee >= '0' && touchePressee <= '9') {
      motDePasseTemp += touchePressee;
      lcd.setCursor(k, 1);
      lcd.print("*");
      k++;
    }

    if (k > 9 || touchePressee == '#') {
      motDePasseTemp = "";
      k = 5;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" *** ALARME *** ");
      lcd.setCursor(0, 1);
      lcd.print("MDP >");
    }

    if (touchePressee == '*') {
      if (motDePasseTemp == motDePasse) {
        entreMotDePasse = false;
        alarmeActivee = false;
        noTone(buzzer);
        messageEcranAffiche = 0;
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Incorrect !");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" *** ALARME *** ");
        lcd.setCursor(0, 1);
        lcd.print("MDP >");
      }
    }
  }
}