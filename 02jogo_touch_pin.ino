#define TOUCH1_PIN 27
#define TOUCH2_PIN 14
#define TOUCH3_PIN 12
#define LED1_PIN 18
#define LED2_PIN 19
#define LED3_PIN 21

#define BLINK_LED1_INTERVAL 1000

int player1_score = 0;
int player2_score = 0;

// Valores dos touchs
int touch1 = 100;
int touch2 = 100;
int touch3 = 100;

int current_millis = 0; // Variável para armazenar o milisegundo atual

boolean started_game = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  delay(1000);

  digitalWrite(LED1_PIN, HIGH);
  Serial.println("Toque no pino touch 1 para inciar o jogo");
}

void loop() {
  if(started_game) {
    touch2 = touchRead(TOUCH2_PIN);
    touch3 = touchRead(TOUCH3_PIN);

    boolean touch2IsPressed = touch2 < 30;
    boolean touch3IsPressed = touch3 < 30;

    if(touch2IsPressed && !touch3IsPressed) {
      Serial.println("O jogador 1 ganhou!");
      Serial.print("Tempo decorrido até o toque: ");
      Serial.print((double)(millis() - current_millis)/1000);
      Serial.println(" segundos.");

      started_game = false;
      player1_score++;
      
      digitalWrite(LED2_PIN,HIGH);
      Serial.println("Toque no pino touch 1 para reinciar o jogo");

      Serial.println("_________Pontuações_________");
      Serial.print("Jogador 1: ");
      Serial.println(player1_score);
      Serial.print("Jogador 2: ");
      Serial.println(player2_score);      
      return;
    }

    if(!touch2IsPressed && touch3IsPressed){
      Serial.println("O jogador 2 ganhou!");
      Serial.print("Tempo decorrido até o toque: ");
      Serial.print((double)(millis() - current_millis)/1000);
      Serial.println(" segundos.");
      
      digitalWrite(LED3_PIN,HIGH);
      started_game = false;
      player2_score++;

      Serial.println("_________Pontuações_________");
      Serial.print("Jogador 1: ");
      Serial.println(player1_score);
      Serial.print("Jogador 2: ");
      Serial.println(player2_score);       
      
      Serial.println("Toque no pino touch 1 para reinciar o jogo");      
      return;      
    }
    
    return;   
  }
  
  touch1 = touchRead(TOUCH1_PIN);
  if(touch1 < 30){
    start_game();  
    Serial.println("O jogo começou");
  }
}

void start_game(){
  digitalWrite(LED2_PIN,LOW);
  digitalWrite(LED3_PIN,LOW);
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED1_PIN, HIGH);
    delay(BLINK_LED1_INTERVAL);
    digitalWrite(LED1_PIN, LOW);
    delay(BLINK_LED1_INTERVAL);
  }
  
  digitalWrite(LED1_PIN, HIGH);
  current_millis = millis();
  started_game = true;
}
