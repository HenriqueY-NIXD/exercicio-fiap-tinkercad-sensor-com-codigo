#include <Adafruit_LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>

#define BUZZER 13
#define LUZ_VERMELHA 12
#define LUZ_VERDE 11

#define DOS 528
#define DO 262

Adafruit_LiquidCrystal lcd(0);

char teclas[4][4]= 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char teclas_letras[4] = {'A', 'B', 'C', 'D'};

int index_coluna = 0;
int index_linha = 0;

int is_resetar_senha = 0;

char senha[6] = {'1', '2', '3', '4', '5', '6'};

char senha_escrita[6] = { '_', '_', '_', '_', '_', '_'};

byte linhas[4] = {10,9,8,7};
byte colunas[4]= {6,5,4,3};

Keypad keypad= Keypad(makeKeymap(teclas), linhas, colunas, 4, 4);

void setup()
{
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  pinMode(LUZ_VERMELHA, OUTPUT);
  pinMode(LUZ_VERDE, OUTPUT);
  
  lcd.begin(16, 2);
  /*
  lcd.print("HELLO WORLD!");
  digitalWrite(11, HIGH);
  delay(250);
  digitalWrite(11, LOW);
  delay(250);
  digitalWrite(11, HIGH);
  delay(250);
  digitalWrite(11, LOW);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FIAP");
  lcd.setCursor(0, 1);
  lcd.print("CYBER SEGURANCA");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INTEGRANTES:");
  delay(1000);
  
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("ANDERSON CARLOS");
  lcd.setCursor(0, 1);
  lcd.print("94982");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HENRIQUE YAMADA");
  lcd.setCursor(0, 1);
  lcd.print("95863");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ISABELA SANTANA");
  lcd.setCursor(0, 1);
  lcd.print("96066");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WALLACE SAMPAIO");
  lcd.setCursor(0, 1);
  lcd.print("95958");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  
  for(int i = 0; i < 3; i++) {
    lcd.setCursor(0, 0);
  	lcd.print("CARREGADO.");
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("CARREGADO..");
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("CARREGADO...");
    delay(500);
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  digitalWrite(LUZ_VERDE, HIGH);
  delay(250);
  digitalWrite(LUZ_VERDE, LOW);
  delay(250);
  digitalWrite(LUZ_VERDE, HIGH);
  delay(250);
  digitalWrite(LUZ_VERDE, LOW);*/
}

void loop()
{
	lcd.setCursor(index_coluna, index_linha);
	char tecla_apertada = keypad.getKey();

	if (tecla_apertada)
	{
		int is_tecla_apertada_number = 0;
		for(int i = 0; i < 4; i++) {
			if (tecla_apertada == teclas_letras[i]) {
				is_tecla_apertada_number = 1;
				break;
			}
		}
		if(is_tecla_apertada_number == 0){
			if (tecla_apertada == '#'){
				if (index_coluna != 0){
					lcd.clear();
					for(int i = 0; i < index_coluna-1; i++){
						lcd.setCursor(i, index_linha);
						lcd.print(senha_escrita[i]);
					}
					--index_coluna; 
				}
			} else if (tecla_apertada == '*'){
				lcd.clear();
				for(int i = 0; i < 6; i++) {
					senha_escrita[i] = -1;
				}
				index_coluna = 0;
			} else {
				lcd.print(tecla_apertada);
				senha_escrita[index_coluna] = tecla_apertada;

				if(index_coluna == 5){
					Serial.println("");
					for(int i=0; i<6; i++) {
						Serial.print(senha_escrita[i]);
					}
					Serial.println("");
					
					delay(500);
					
					lcd.clear();
					lcd.setCursor(0, 0);
					
					if (!(strncmp(senha_escrita, senha, 6)))
					{
						lcd.print("SENHA CERTA");
						Serial.println("Senha Certa");
						digitalWrite(LUZ_VERDE, HIGH);
						tone(BUZZER, DOS, 250);
						delay(250);
						digitalWrite(LUZ_VERDE, LOW);
						delay(250);
						digitalWrite(LUZ_VERDE, HIGH);
						tone(BUZZER, DOS, 250);
						delay(250);
						digitalWrite(LUZ_VERDE, LOW);
					} else {
						lcd.print("SENHA ERRADA");
						digitalWrite(LUZ_VERMELHA, HIGH);
						tone(BUZZER, DO, 250);
						delay(250);
						digitalWrite(LUZ_VERMELHA, LOW);
						delay(250);
						digitalWrite(LUZ_VERMELHA, HIGH);
						tone(BUZZER, DO, 250);
						delay(250);
						digitalWrite(LUZ_VERMELHA, LOW);
					}
					
					index_coluna = 0;
					for(int i = 0; i < 6; i++) {
						senha_escrita[i] = '_';
					}
					lcd.clear();
				} else {
					++index_coluna;
				}
			}
		} else if (index_coluna == 0) {
			lcd.setCursor(0, 0);
			switch(tecla_apertada){
				case 'A':
					lcd.print("ANDERSON CARLOS");
					lcd.setCursor(0, 1);
					lcd.print("94982");
					delay(1000);
					lcd.clear();
					break;
				case 'B':
					lcd.print("HENRIQUE YAMADA");
					lcd.setCursor(0, 1);
					lcd.print("95863");
					delay(1000);
					lcd.clear();
					break;
				case 'C':
					lcd.print("ISABELA SANTANA");
					lcd.setCursor(0, 1);
					lcd.print("96066");
					delay(1000);
					lcd.clear();
					break;
				case 'D':
					lcd.print("WALLACE SAMPAIO");
					lcd.setCursor(0, 1);
					lcd.print("95958");
					delay(1000);
					lcd.clear();
					break;
				default:
					lcd.print("??????????");
					delay(1000);
					Serial.println(tecla_apertada);
					break;
			}
		}
	}
}
















