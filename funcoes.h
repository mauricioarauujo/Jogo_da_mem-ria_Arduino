#include "notas_musicais.h"

#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5

#define QtdDeLEDs 4

#define INDEFINIDO -1

#define buzzer 6

#define BOTAO_VERDE 8
#define BOTAO_AMARELO 9
#define BOTAO_VERMELHO 10
#define BOTAO_AZUL 11


#define TAMANHO_SEQUENCIA 5  //A rodada final terá <TAMANHO_SEQUENCIA> 'piscadas' para responder

enum Estados {               //Estados do jogo.
  PRONTO_PARA_PROXIMA_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

int sequenciaLuzes[TAMANHO_SEQUENCIA];     //Array com a sequencia a ser respondida.

int leds[] = {LED_VERDE, LED_AMARELO, LED_VERMELHO, LED_AZUL};

int rodada;

int ledsRespondidos;



void iniciaPortas() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
}

int sorteiaCor() {
  return random(LED_VERDE, LED_AZUL + 1);
}

void iniciaJogo() {

  int jogo = analogRead(0);
  randomSeed(jogo);

  for (int i = 0; i < TAMANHO_SEQUENCIA; i++) {
    sequenciaLuzes[i] = sorteiaCor();
  }
}









int estadoAtual() {
  if (rodada <= TAMANHO_SEQUENCIA) {
    if (ledsRespondidos == rodada) {
      return PRONTO_PARA_PROXIMA_RODADA;
    } else {
      return USUARIO_RESPONDENDO;
    }
  } else if (rodada == TAMANHO_SEQUENCIA + 1) {
    return JOGO_FINALIZADO_SUCESSO;
  } else {
    return JOGO_FINALIZADO_FALHA;
  }
}
void tocaSom(int frequencia) {
  tone(buzzer, frequencia, 100);
}

void verificaSomDoLed(int portaLed) {
  switch (portaLed) {
    case LED_VERDE:
      tocaSom(2000);
      break;
    case LED_AMARELO:
      tocaSom(2200);
      break;
    case LED_VERMELHO:
      tocaSom(2400);
      break;
    case LED_AZUL:
      tocaSom(2500);
      break;
  }
}
int piscaLed(int portaLed) {

  verificaSomDoLed(portaLed);

  digitalWrite(portaLed, HIGH);
  delay(700);
  digitalWrite(portaLed, LOW);
  delay(300);

  return portaLed;
}
void tocaLedsRodada() {
  for (int i = 0; i < rodada; i++) {
    piscaLed(sequenciaLuzes[i]);
  }
}
void preparaNovaRodada() {
  rodada++;
  ledsRespondidos = 0;

  if (rodada <= TAMANHO_SEQUENCIA) {
    tocaLedsRodada();
  }

}




/*int calculaTempoLed() {
  int potenciometro = analogRead(1);
  Serial.println(potenciometro);
  return potenciometro;
  }
*/

void tocaMusicaMarioVitoria() {

  // change this to make the song slower or faster
  int tempo = 200;


  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
  int melody[] = {

    // Super Mario Bros theme
    // Score available at https://musescore.com/user/2123/scores/2145
    // Theme by Koji Kondo


    NOTE_E5, 8, NOTE_E5, 8, REST, 8, NOTE_E5, 8, REST, 8, NOTE_C5, 8, NOTE_E5, 8, //1
    NOTE_G5, 4, REST, 4, NOTE_G4, 8, REST, 4,
    NOTE_C5, -4, NOTE_G4, 8, REST, 4, NOTE_E4, -4, // 3
    NOTE_A4, 4, NOTE_B4, 4, NOTE_AS4, 8, NOTE_A4, 4,
    NOTE_G4, -8, NOTE_E5, -8, NOTE_G5, -8, NOTE_A5, 4, NOTE_F5, 8, NOTE_G5, 8,
    REST, 8, NOTE_E5, 4, NOTE_C5, 8, NOTE_D5, 8, NOTE_B4, -4,
    NOTE_C5, -4, NOTE_G4, 8, REST, 4, NOTE_E4, -4, // repeats from 3
    NOTE_A4, 4, NOTE_B4, 4, NOTE_AS4, 8, NOTE_A4, 4,
    NOTE_G4, -8, NOTE_E5, -8, NOTE_G5, -8, NOTE_A5, 4, NOTE_F5, 8, NOTE_G5, 8,
    REST, 8, NOTE_E5, 4, NOTE_C5, 8, NOTE_D5, 8, NOTE_B4, -4,


  };

  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }

}
void tocaMusicaMarioDerrota() {


  // change this to make the song slower or faster
  int tempo = 200;

  // change this to whichever pin you want to use



  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
  int melody[] = {

    // Super Mario Bros theme
    // Score available at https://musescore.com/user/2123/scores/2145
    // Theme by Koji Kondo


    NOTE_C5, -4, NOTE_G4, -4, NOTE_E4, 4, //45
    NOTE_A4, -8, NOTE_B4, -8, NOTE_A4, -8, NOTE_GS4, -8, NOTE_AS4, -8, NOTE_GS4, -8,
    NOTE_G4, 8, NOTE_D4, 8, NOTE_E4, -2,


  };

  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }

}

int checaRespostaJogador() {
  if (digitalRead(BOTAO_VERDE) == LOW) {
    return piscaLed(LED_VERDE);
  }
  if (digitalRead(BOTAO_AMARELO) == LOW) {
    return piscaLed(LED_AMARELO);
  }
  if (digitalRead(BOTAO_VERMELHO) == LOW) {
    return piscaLed(LED_VERMELHO);
  }
  if (digitalRead(BOTAO_AZUL) == LOW) {
    return piscaLed(LED_AZUL);
  }

  return INDEFINIDO;
}
void processaRespostaUsuario() {
  int resposta = checaRespostaJogador();

  if (resposta == INDEFINIDO) {
    return;
  }

  if (resposta == sequenciaLuzes[ledsRespondidos]) {
    ledsRespondidos++;
  } else {
    Serial.println("Sequencia errada");
    rodada = TAMANHO_SEQUENCIA + 2; //a variável rodada fica com um valor que configura derrota
  }
}

void jogoFinalizadoSucesso() {

  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
  }
  tocaMusicaMarioVitoria();




  delay(500);
}

void jogoFinalizadoFalha() {

  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
  }
  tocaMusicaMarioDerrota();




  delay(500);
}
