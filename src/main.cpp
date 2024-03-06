/*
O jogo implementado neste código é uma versão eletrônica do clássico "Genius".
Nele, o jogador deve repetir corretamente uma sequência de luzes emitidas pelos LEDs,
pressionando os botões correspondentes na ordem correta.
*/

#include <Arduino.h>

// Define os pinos dos leds
#define numLed 4
int leds[numLed] = {2, 3, 4, 5};

// Define os pinos dos botões
#define numBtn 4
int btns[numBtn] = {8, 9, 10, 11};

// Led para aviso de falha
#define ledAviso 13

// Define a sequencia maxima do jogo
#define sequenciaMax 30
int sequencia[sequenciaMax] = {0};

// Define a sequencia inicial do jogo
#define sequenciaInicial 2

// Define o intervalo de tempo entre os leds
#define intervalo 400

// Estado da partida atual
int estadoAtual = 0;
int estadoJogador = 0;

// Variável para indicar se o jogador perdeu
bool perdeu = false;

// Variavel de apoio para contar o numero de botoes pressionados
int numClique;

// Variavel de apoio para o tempo que o btn esta pressionado
unsigned long tempoDeInicio = 0;

// Declarar funções
void confInicial();
int verificarBtnPressionado();
void verificarNumBotoesPressionados();
void adicionarSequencia();
void mostrarSequencia();
void piscarAcerto();
void piscarErro();
void aviso();

void setup()
{
  // Inicialização dos pinos
  for (int i = 0; i < numLed; i++)
  {
    pinMode(leds[i], OUTPUT);
  }

  for (int i = 0; i < numBtn; i++)
  {
    pinMode(btns[i], INPUT);
  }

  // Gera aleatoriedade
  randomSeed(analogRead(0));

  // Led qeu servira de aviso
  pinMode(ledAviso, OUTPUT);

  // Função que estabelece condições iniciais
  confInicial();
}

void loop()
{
  // Mostra a sequência
  mostrarSequencia();

  // Reinicia o estado do jogador
  perdeu = false;
  estadoJogador = 0;

  // Loop para análise dos cliques dos botões
  for (estadoJogador = 0; estadoJogador < estadoAtual; estadoJogador++)
  {
    // Verifica se o botão pressionado é diferente da sequência
    // 6 é a distancia do pino do led para o respectivo do botão
    if (sequencia[estadoJogador] != verificarBtnPressionado() - 6)
    {
      // Reinicia as configurações iniciais
      piscarErro();
      confInicial();
      // Atualiza o valor para idicar que o jogador errou
      perdeu = true;
      break;
    }
  }

  // Se não perdeu, pisca o LED de acerto e adiciona à sequência
  if (!perdeu)
  {
    piscarAcerto();
    adicionarSequencia();
  }
}

// Configura o jogo para uma configuração inicial especifica
void confInicial()
{
  estadoAtual = 0;
  estadoJogador = 0;
  sequencia[sequenciaMax] = {0};
  piscarAcerto(); // Piscar LEDs para indicar início do jogo

  // Inicia a sequência com a quantidade desejada
  for (int i = 0; i < sequenciaInicial; i++)
  {
    adicionarSequencia();
  }
}

// Função para verificar qual botão foi pressionado
int verificarBtnPressionado()
{

  // Loop vai ficar ativo até que um botão seja pressionado
  while (HIGH)
  {
    for (int i = 0; i < numBtn; i++)
    {
      tempoDeInicio = millis();
      if (digitalRead(btns[i]) == HIGH)
      {
        // Depois que o botão for ativo, irá esperar soltar
        while (digitalRead(btns[i]) == HIGH)
        {
          verificarNumBotoesPressionados();
          digitalWrite(leds[i], HIGH); // Acende o LED do botão pressionado

          // Determina que se o botão ficar pressionado por mais de 5 segundos tem erro
          if (millis() - tempoDeInicio > 5000)
          {
            aviso();
          }
        }
        digitalWrite(leds[i], LOW); // Apaga o LED do botão
        delay(intervalo);
        return btns[i]; // Retorna o número do botão pressionado
      }
    }
  }
}

// Verificar o numero de botoes precionados por vez
void verificarNumBotoesPressionados()
{
  numClique = 0;

  // Verifica se ha botoes pressionados, neste caso conta quantos
  for (int i = 0; i < numBtn; i++)
  {
    if (digitalRead(btns[i]) == HIGH)
    {
      numClique++;
      digitalWrite(leds[i], HIGH);
      delay(intervalo);
      digitalWrite(leds[i], LOW);
    }
  }

  // Se o valor for maior que 1, ele dar o aviso
  if (numClique > 1)
  {
    aviso();
  }
}

// Adiciona um valor para a sequência
void adicionarSequencia()
{
  // Verifica se já atingiu a quantidade máxima
  if (estadoAtual < sequenciaMax)
  {
    // Um número inteiro aleatório entre 2 e 5
    sequencia[estadoAtual] = random(2, 6);
    estadoAtual++;
  }
}

// Mostra a sequência de LEDs
void mostrarSequencia()
{
  for (int i = 0; i < estadoAtual; i++)
  {
    digitalWrite(sequencia[i], HIGH); // Acende o LED da sequência
    delay(intervalo);
    digitalWrite(sequencia[i], LOW); // Apaga o LED
    delay(intervalo);
  }
}

// Piscar LEDs para indicar acerto
void piscarAcerto()
{
  for (int i = 0; i < numLed; i++)
  {
    digitalWrite(leds[i], HIGH);
    delay(200);
  }
  delay(1000);
  for (int i = 0; i < numLed; i++)
  {
    digitalWrite(leds[i], LOW);
    delay(200);
  }
  delay(1500);
}

// Piscar LEDs para indicar erro
void piscarErro()
{
  for (int i = 0; i < 5; i++)
  {
    for (int i = 0; i < numLed; i++)
    {
      digitalWrite(leds[i], HIGH);
      delay(50);
    }
    delay(200);
    for (int i = 0; i < numLed; i++)
    {
      digitalWrite(leds[i], LOW);
      delay(50);
    }
  }
  delay(1500);
}

// Avisa sobre falhas
void aviso()
{
  digitalWrite(ledAviso, HIGH);
  delay(intervalo);
  digitalWrite(ledAviso, LOW);
}
