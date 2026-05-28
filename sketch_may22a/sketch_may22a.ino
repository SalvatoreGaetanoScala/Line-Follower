/*

Scala Salvatore Gaetano 1000100183


####################
#   COLLEGAMENTI   #
####################

1. motori e driver L298N
•	motore sx: collega i due fili ai morsetti OUT1 e OUT2 del L298N.
•	motore dx: collega i due fili ai morsetti OUT3 e OUT4 del L298N.

2. batteria 1 e interruttore (motori)
•	prendi il filo rosso (+) della prima batteria da 9V e collegalo a un piedino dell'interruttore nero.
•	dall'altro piedino dell'interruttore, porta un cavetto al morsetto 12V del L298N.
•	collega il filo nero (-) della batteria direttamente al morsetto GND del L298N.


3. breadboard e sensori
•	usa un cavetto per collegare il pin 5V di Arduino alla linea rossa (+) della breadboard.
•	usa un cavetto per collegare un pin GND di Arduino alla linea blu o nera (-) della breadboard.
•	occhio sx: collega VCC alla linea rossa, GND alla linea blu. (A0 rimane scollegato).
•	occhio dx: collega VCC alla linea rossa, GND alla linea blu. (A0 rimane scollegato).


4. tutti i pin verso Elengoo uno R3
•	la massa comune: collega un cavetto dal morsetto GND del L298N a uno dei pin GND.
•	velocità motore sx: Pin ENA (senza cappuccetto) collegato al pin 5.
•	direzione sx: pin IN1 collegato al pin 8.
•	direzione sx: pin IN2 collegato al pin 9.
•	direzione dx: pin IN3 collegato al pin 10.
•	direzione dx: pin IN4 collegato al pin 11.
•	velocità motore dx: pin ENB (senza cappuccetto) collegato al pin 6.
•	IR sx: pin D0 del sensore sinistro collegato al pin 2.
•	IR dx: pin D0 del sensore destro collegato al pin 3.


5. batteria 2 (l'energia per il microcontrollore)
•	collega la seconda batteria da 9V direttamente ad Arduino 
(tramite lo spinotto nero rotondo oppure collegando il filo rosso al pin VIN e il nero a GND).

*/

// pin:
#define ENA 5      // acceleratore destro
#define ENB 6      // acceleratore sinistro
#define IN1 8      // direzione destra 
#define IN2 9      // direzione destra 
#define IN3 10     // direzione sinistra 
#define IN4 11     // direzione sinistra 

#define SENS_SX 2  // occhio sinistro
#define SENS_DX 3  // occhio destro 

// velocità posta 200 per vincere l'attrito dei motori (ed evitare che va con una sola ruota).
// se va ancora solo una ruota, alzarlo a 255 (velocità massima).
int velocita = 200; 


void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(SENS_SX, INPUT);
  pinMode(SENS_DX, INPUT);
}

void loop() {
  // garantiamo che la potenza sia sempre inviata agli acceleratori
  analogWrite(ENA, velocita);
  analogWrite(ENB, velocita);

  // lettura sensori: BIANCO = 0 (LOW), NERO/ARIA = 1 (HIGH)
  int occhioSinistro = digitalRead(SENS_SX);
  int occhioDestro = digitalRead(SENS_DX);

  // 1): entrambi sul BIANCO (0, 0) -> VAI DRITTO
  if (occhioSinistro == LOW && occhioDestro == LOW) {
    vaiDritto();
  }
  
  // 2): occhio SX sul NERO (1, 0) -> GIRA A SINISTRA
  else if (occhioSinistro == HIGH && occhioDestro == LOW) {
    giraSinistra();
  }
  
  // 3): occhio DX sul NERO (0, 1) -> GIRA A DESTRA
  else if (occhioSinistro == LOW && occhioDestro == HIGH) {
    giraDestra();
  }
  
  // 4): entrambi sul NERO o in aria (1, 1) -> FERMO
  else if (occhioSinistro == HIGH && occhioDestro == HIGH) {
    fermo();
  }
}

// --- i vari movimenti ---

void vaiDritto() {
  digitalWrite(IN1, LOW);  // ruota DX avanti
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); // ruota SX avanti
  digitalWrite(IN4, LOW);
}

void giraSinistra() {
  digitalWrite(IN1, LOW);  // ruota DX avanti
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  // ruota SX indietro
  digitalWrite(IN4, HIGH);
}

void giraDestra() {
  digitalWrite(IN1, HIGH); // ruota DX indietro
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); // ruota SX avanti
  digitalWrite(IN4, LOW);
}

void fermo() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


/*
#################################################
#   La logica di movimento (Cosa fa in pista)   #
#################################################

L'obiettivo del robot è avanzare tenendo la linea nera esattamente in mezzo ai due sensori. 
Il codice impone queste 4 regole:

	1.	VAI DRITTO: se entrambi gli "occhi" vedono il bianco, significa che la linea nera si 
    trova perfettamente al centro tra i due sensori. Il robot sa di essere sulla strada giusta e 
    spinge entrambe le ruote in avanti;

	2.	CORREGGI A SINISTRA: se l'occhio sinistro tocca il nero, significa che il robot sta sbandando 
  troppo verso destra. Il cervello interviene subito facendo girare la ruota destra in avanti e la 
  sinistra all'indietro per "ruotare" rapidamente a sinistra e rimettersi in carreggiata.
	
  3.	CORREGGI A DESTRA: se l'occhio destro tocca il nero, il robot sta sbandando verso sinistra. 
  Il cervello fa l'esatto opposto: spinge la ruota sinistra in avanti e la destra all'indietro per 
  sterzare a destra.
	
  4.	FERMATI: Se entrambi gli occhi vedono il nero contemporaneamente (ad esempio se incontra una 
  linea orizzontale di fine percorso) oppure se il robot viene sollevato in aria (i sensori non rilevano 
  più il bianco del pavimento), i motori si spengono e il robot si ferma.


In sintesi si può considerare  un sistema a correzione continua. Mentre avanza, il robot farà dei continui 
e piccolissimi  "zig-zag" (sterzate a destra e a sinistra) per rimbalzare virtualmente tra i bordi della 
linea nera.
*/