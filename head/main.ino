#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

// Instancia del controlador PCA9685
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x43);

// Configuración de pulsos
#define SERVOMIN 150
#define SERVOMAX 600

typedef void (*CommandFunction)();

struct Command {
  int id;
  const char *name;
  CommandFunction fn;
};

void listarComandosJSON();
void PosicionNeutra();
void modoManualContinuo();
void GestoSorprendido();
void GestoFurioso();
void GestoFeliz();
void GestoParpadeo();
void GuinoIzquierdo();
void GuinoDerecho();
void GestoDormir();
void Sospechoso();
void Risa();
void Triste();
void Despertar();
void OjoIzq();

Command commands[] = {{-1, "Listar comandos", listarComandosJSON},
                      {0, "Posición Neutra", PosicionNeutra},
                      {1, "Modo Manual", modoManualContinuo},
                      {2, "Sorprendido", GestoSorprendido},
                      {3, "Furioso", GestoFurioso},
                      {4, "Feliz", GestoFeliz},
                      {5, "Parpadeo", GestoParpadeo},
                      {6, "Guino Izquierdo", GuinoIzquierdo},
                      {7, "Guino Derecho", GuinoDerecho},
                      {8, "Dormir", GestoDormir},
                      {9, "Sospechoso", Sospechoso},
                      {10, "Risa", Risa},
                      {11, "Triste", Triste},
                      {12, "Despertar", Despertar},
                      {13, "Ojo Izquierdo", OjoIzq}};

const int commandCount = sizeof(commands) / sizeof(commands[0]);

// --- 0: POSICIÓN NEUTRA ---
void PosicionNeutra() {
  Serial.println(F("[ESTADO] Neutro (Valores Base Actualizados)"));

  // Establecemos un valor base general de 400 para todos los canales
  for (int i = 0; i < 16; i++) {
    pca.setPWM(i, 0, calcularPulso(400));
  }

  // Ajustes específicos solicitados:
  pca.setPWM(4, 0, calcularPulso(350));  // Pupila Derecha D/I
  pca.setPWM(13, 0, calcularPulso(400)); // Ceja derecha
  pca.setPWM(7, 0, calcularPulso(450));  // Pupila izquierda A/B
  pca.setPWM(5, 0, calcularPulso(350));  // Pupila derecha A/B
  pca.setPWM(11, 0, calcularPulso(450)); // Pupila izquierda D/I
  pca.setPWM(3, 0, calcularPulso(350));  // Parpado superior derecho
  pca.setPWM(2, 0, calcularPulso(300));  // Parpado inferior derecho
  pca.setPWM(10, 0, calcularPulso(350)); // Parpado superior izquierdo
  pca.setPWM(1, 0, calcularPulso(500));  // Parpado inferior izquierdo
}

// --- 1: MODO MANUAL ---
void modoManualContinuo() {
  limpiarBuffer();
  Serial.println(F("\n >>> MODO MANUAL ACTIVADO <<<"));
  Serial.println(F(" Formato: [Canal] [Pasos] (Ej: 15 800) | Salir: -1"));

  bool salir = false;
  while (!salir) {
    if (Serial.available() > 0) {
      char primerChar = Serial.peek();
      if (primerChar == '\n' || primerChar == '\r' || primerChar == ' ') {
        Serial.read();
        continue;
      }

      int canal = Serial.parseInt();
      if (canal == -1) {
        salir = true;
      } else {
        int pasos = Serial.parseInt();
        if (canal >= 0 && canal <= 15 && pasos >= 0 && pasos <= 1023) {
          pca.setPWM(canal, 0, calcularPulso(pasos));
          Serial.print(F(" -> Canal "));
          Serial.print(canal);
          Serial.print(F(" movido a "));
          Serial.println(pasos);
        }
      }
      limpiarBuffer();
    }
  }
  Serial.println(F("\n[Saliendo de Modo Manual...]"));
}

// --- 2: SORPRESA ---
void GestoSorprendido() {
  Serial.println(F("[GESTO] Sorprendido"));
  pca.setPWM(0, 0, calcularPulso(480));
  pca.setPWM(6, 0, calcularPulso(300));
  pca.setPWM(13, 0, calcularPulso(250));
  pca.setPWM(12, 0, calcularPulso(510));
  pca.setPWM(3, 0, calcularPulso(500));
  pca.setPWM(2, 0, calcularPulso(300));
  pca.setPWM(10, 0, calcularPulso(310));
  pca.setPWM(1, 0, calcularPulso(420));
}

// --- 3: FURIA ---
void GestoFurioso() {
  Serial.println(F("[GESTO] Furioso"));
  pca.setPWM(15, 0, calcularPulso(450));
  pca.setPWM(13, 0, calcularPulso(500));
  pca.setPWM(14, 0, calcularPulso(350));
  pca.setPWM(12, 0, calcularPulso(280));
  pca.setPWM(2, 0, calcularPulso(370));
  pca.setPWM(1, 0, calcularPulso(330));
}

// --- 4: FELIZ ---
void GestoFeliz() {
  Serial.println(F("[GESTO] Feliz"));
  pca.setPWM(2, 0, calcularPulso(400));
  pca.setPWM(1, 0, calcularPulso(350));
  pca.setPWM(0, 0, calcularPulso(430));
  pca.setPWM(6, 0, calcularPulso(300));
}

// --- 5: PARPADEO ---
void GestoParpadeo() {
  Serial.println(F("[GESTO] Parpadeo"));
  pca.setPWM(10, 0, calcularPulso(520));
  pca.setPWM(1, 0, calcularPulso(320));
  pca.setPWM(2, 0, calcularPulso(480));
  pca.setPWM(3, 0, calcularPulso(250));
  delay(150);
  pca.setPWM(3, 0, calcularPulso(350)); // Regresa a neutro personalizado
  pca.setPWM(2, 0, calcularPulso(300));
  pca.setPWM(10, 0, calcularPulso(350)); // Regresa a neutro personalizado
  pca.setPWM(1, 0, calcularPulso(500));  // Regresa a neutro personalizado
}

// --- 6: GUIÑO IZQUIERDO ---
void GuinoIzquierdo() {
  Serial.println(F("[GESTO] Guiño Izquierdo"));
  pca.setPWM(10, 0, calcularPulso(520));
  pca.setPWM(1, 0, calcularPulso(320));
  delay(150);
  pca.setPWM(10, 0, calcularPulso(350));
  pca.setPWM(1, 0, calcularPulso(500));
}

// --- 7: GUIÑO DERECHO ---
void GuinoDerecho() {
  Serial.println(F("[GESTO] Guiño Derecho"));
  pca.setPWM(2, 0, calcularPulso(470));
  pca.setPWM(3, 0, calcularPulso(250));
  delay(150);
  pca.setPWM(3, 0, calcularPulso(350)); // Regresa a neutro personalizado
  pca.setPWM(2, 0, calcularPulso(300));
}

// --- 8: DORMIR ---
void GestoDormir() {
  Serial.println(F("[GESTO] Dormir"));
  pca.setPWM(10, 0, calcularPulso(520));
  pca.setPWM(1, 0, calcularPulso(320));
  pca.setPWM(2, 0, calcularPulso(480));
  pca.setPWM(3, 0, calcularPulso(250));
}

// --- 9: SOSPECHOSO ---
void Sospechoso() {
  Serial.println(F("[GESTO] Sospechoso"));
  pca.setPWM(3, 0, calcularPulso(230));
  pca.setPWM(10, 0, calcularPulso(400));
  pca.setPWM(2, 0, calcularPulso(350));
  pca.setPWM(1, 0, calcularPulso(330));
  pca.setPWM(13, 0, calcularPulso(310));
  pca.setPWM(12, 0, calcularPulso(320));
}

// --- 10: RISA ---
void Risa() { Serial.println(F("[GESTO] Risa (Sin mandíbula)")); }

// --- 11: TRISTE ---
void Triste() {
  Serial.println(F("[GESTO] Triste"));
  pca.setPWM(7, 0, calcularPulso(460));
  pca.setPWM(5, 0, calcularPulso(340));
  pca.setPWM(12, 0, calcularPulso(500));
  pca.setPWM(13, 0, calcularPulso(280));
}

// --- 12: Despertar
void Despertar() {
  Serial.println(F("[GESTO] Despertar"));
  pca.setPWM(10, 0, calcularPulso(400));
  pca.setPWM(1, 0, calcularPulso(400));
  pca.setPWM(2, 0, calcularPulso(350));
  pca.setPWM(3, 0, calcularPulso(370));
}

// --- 13: Ojo Izquierdo ---
void OjoIzq() {
  Serial.println(F("[GESTO] Ojo Izq"));
  pca.setPWM(1, 0, calcularPulso(520));
  pca.setPWM(10, 0, calcularPulso(520));
}

// --- UTILIDADES ---
int calcularPulso(int pasos) { return map(pasos, 0, 1023, SERVOMIN, SERVOMAX); }

void limpiarBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void mostrarMenu() {
  Serial.println(F("\n===================================="));
  Serial.println(F("    SISTEMA OPERATIVO HEAD R-ONE    "));
  Serial.println(F("===================================="));
  Serial.println(F("\n----------- MENÚ DE CONTROL Head R-One -----------"));
  Serial.println(F(" 0: Neutro         |  1: MODO MANUAL"));
  Serial.println(F(" 2: Sorpresa       |  3: Furia"));
  Serial.println(F(" 4: Feliz          |  5: Parpadeo"));
  Serial.println(F(" 6: Guiño Izq.     |  7: Guiño Der."));
  Serial.println(F(" 8: Dormir         |  9: Sospechoso"));
  Serial.println(F(" 10: Risa          |  11: Triste"));
  Serial.println(F("--------------------------------------------------"));
  Serial.print(F("Seleccione una opción: "));
}

void ejecutarComando(int id) {
  for (int i = 0; i < commandCount; i++) {
    if (commands[i].id == id) {
      commands[i].fn();
      return;
    }
  }
  Serial.println(F(" [!] Opción no válida."));
}

// --- -1: LISTAR COMANDOS ---
void listarComandosJSON() {
  Serial.println("JSON_BEGIN");
  Serial.println("{\"commands\":[");

  for (int i = 0; i < commandCount; i++) {
    Serial.print("{\"id\":");
    Serial.print(commands[i].id);
    Serial.print(",\"name\":\"");
    Serial.print(commands[i].name);
    Serial.print("\"}");

    if (i < commandCount - 1) {
      Serial.println(",");
    } else {
      Serial.println();
    }
  }

  Serial.println("]}");
  Serial.println("JSON_END");
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);

  pca.begin();
  pca.setPWMFreq(50);
}

void loop() {
  if (Serial.available() > 0) {
    int opcion = Serial.parseInt();

    if (!opcion) {
      mostrarMenu();
    }

    ejecutarComando(opcion);
    limpiarBuffer();
  }
}