#include <Adafruit_LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>
#include <Wire.h>

#define BUZZER 13
#define LUZ_VERMELHA 12
#define LUZ_VERDE 11
#define PIN_SENSOR 2
#define SECOND_ARD_ADDR 9
#define ANSWER_SIZE 5

#define DOS 528
#define DO 262

#define slaveAdress 0x08

Adafruit_LiquidCrystal lcd(0);

char teclas[4][4]= 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char teclas_letras[4] = {'A', 'B', 'C', 'D'};

int previous_time = millis();
int alarm_pitch = LOW;
int buzzer_state = LOW;
int last_buzzer_state = LOW;

int index_coluna = 0;
int index_linha = 1;

int is_resetar_senha = 0;
int is_senha_atual_certa = 0;
int is_intruder_alert = 0;
int is_entry_permitted = 0;

unsigned long lastDebounceTime = 0;

char senha[6] = {'1', '2', '3', '4', '5', '6'};

char senha_escrita[6] = { '_', '_', '_', '_', '_', '_'};

byte linhas[4] = {10,9,8,7};
byte colunas[4]= {6,5,4,3};

Keypad keypad= Keypad(makeKeymap(teclas), linhas, colunas, 4, 4);

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(BUZZER, OUTPUT);
  pinMode(LUZ_VERMELHA, OUTPUT);
  pinMode(LUZ_VERDE, OUTPUT);
  pinMode(PIN_SENSOR, INPUT);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  printarMensagem();
}

void loop()
{
  int sensor_pin_status = digitalRead(PIN_SENSOR);

  if(sensor_pin_status == HIGH)
    is_intruder_alert = 1;

  if (is_intruder_alert == 1 && is_entry_permitted == 0)
    tone(BUZZER, DOS);

  int current_time = millis();
  char tecla_apertada = keypad.getKey();

  if (tecla_apertada) {
    int is_tecla_apertada_number = 1;
		for(int i = 0; i < 4; i++) {
			if (tecla_apertada == teclas_letras[i]) {
				is_tecla_apertada_number = 0;
				break;
			}
		}

    if (is_tecla_apertada_number == 0) {
      switch(tecla_apertada){
				case 'A':
          is_entry_permitted = 0;
          is_resetar_senha = 0;
          is_senha_atual_certa = 0;
          index_coluna = 0;
          zerarSenha();
          lcd.clear();
          lcd.setCursor(0, 0);
          printarMensagem();
					break;
				case 'B':
          if (is_intruder_alert == 0 || is_entry_permitted == 1) {
            if (is_resetar_senha == 0) {
              is_resetar_senha = 1;
            } else {
              is_resetar_senha = 0;
              is_senha_atual_certa = 0;
            }
            index_coluna = 0;
            zerarSenha();
            lcd.clear();
            lcd.setCursor(0, 0);
            printarMensagem();
          }
					break;
				case 'C':
				case 'D':
          is_entry_permitted = 0;
          is_resetar_senha = 0;
          is_senha_atual_certa = 0;
          is_intruder_alert = 1;
          index_coluna = 0;
          zerarSenha();
          lcd.clear();
          lcd.setCursor(0, 0);
          printarMensagem();
					break;
				default:
					Serial.println(tecla_apertada);
					break;
			}
    } else if((is_resetar_senha == 1 || is_senha_atual_certa == 1) && is_entry_permitted == 1 || is_entry_permitted == 0){
      if (tecla_apertada == '#'){
				if (index_coluna != 0){
					lcd.clear();
          lcd.setCursor(0, 0);
          
          printarMensagem();
          
					for(int i = 0; i < index_coluna-1; i++){
						lcd.setCursor(i, index_linha);
						lcd.print(senha_escrita[i]);
					}
					--index_coluna; 
				}
			} else if (tecla_apertada == '*'){
				lcd.clear();
				zerarSenha();
        lcd.setCursor(0, 0);
        
        printarMensagem();

				index_coluna = 0;
			} else{
        lcd.setCursor(index_coluna, index_linha);
        lcd.print(tecla_apertada);
				senha_escrita[index_coluna] = tecla_apertada;

        if (index_coluna == 5) {
          lcd.clear();
					lcd.setCursor(0, 0);
          if (is_resetar_senha == 1 && is_senha_atual_certa == 1) {
            for(int i=0; i<6; i++) {
              senha[i] = senha_escrita[i];
            }
            is_resetar_senha = 0;
            is_senha_atual_certa = 0;
          } else if(!(strncmp(senha_escrita, senha, 6))) {
            if (is_resetar_senha == 1) {
              is_senha_atual_certa = 1;
            } else {
              lcd.print("SENHA CERTA");
              noTone(BUZZER);
              
              is_intruder_alert = 0;
              is_entry_permitted = 1;

              digitalWrite(LUZ_VERDE, HIGH);
              delay(100);
              digitalWrite(LUZ_VERDE, LOW);
              delay(100);
              digitalWrite(LUZ_VERDE, HIGH);
              delay(100);
              digitalWrite(LUZ_VERDE, LOW);
              delay(500);
            }
          }
          index_coluna = 0;
          zerarSenha();
          lcd.clear();
          lcd.setCursor(0, 0);
          printarMensagem();
        } else {
          ++index_coluna;
        }
      }
		}
  }
}

void zerarSenha() {
  for(int i = 0; i < 6; i++) {
    senha_escrita[i] = '_';
  }
}

void printarMensagem() {
  if (is_resetar_senha == 0){
    if (is_entry_permitted == 1)
      _entryPermitedMessage();
    else
      _initialMessage();
  }
  else {
    if (is_senha_atual_certa == 0)
      _resetPasswordMessage();
    else
      _newPasswordMessage();
  }
}

void _initialMessage() {
  lcd.print("Digite a senha:");
}

void _resetPasswordMessage() {
  lcd.print("SENHA ATUAL:");
}

void _newPasswordMessage() {
  lcd.print("NOVA SENHA:");
}

void _entryPermitedMessage() {
  lcd.print("ACESSO LIBERADO");
}
















