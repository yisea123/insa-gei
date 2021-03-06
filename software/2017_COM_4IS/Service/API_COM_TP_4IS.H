

//=======================================================================================
// 	Auteur T.R.
//  Juin 2017 , 
//  Processeur : STM32F103RB
//  Logiciel dev : �Vision 5.23
//  API pour les TP de Soft embarqu� COM, 4IS
//  N�cessite :
//  => Drivers_STM32F103_107_Juin2017a.lib (=lib perso. Usart n'est pas utilis�)
//  => clock.c indispensable pour faire tourner les lib perso
//  => USART_rev.c m�me version que USART (inclus dans la lib) mais dont le .h a �t� partitionn�
//             un nouveau .h, USART_User_Conf.h qui permet de param�trer USART_rev.c.
//=======================================================================================


#ifndef _API_Com_TP_4IS_H__
#define _API_Com_TP_4IS_H__

/* =============================================
				GPIO
 ============================================= */
// les GPIO
#define Port_A 		0
#define Port_B 		1
#define Port_C 		2
#define Port_D 		3
#define Port_E		4
   

// Sens
#define INPUT 				'i'
#define OUTPUT 				'o'

// Techno pour pin en entr�e (INPUT)
#define ANALOG 				0
#define INPUT_FLOATING 		1
#define INPUT_PULL_DOWN_UP 	2

// Techno pour pin en sortie (OUTPUT)
#define OUTPUT_PPULL 		0
#define OUTPUT_OPDRAIN 		1
#define ALT_PPULL 			2
#define ALT_OPDRAIN 		3

/*
Fonction permettant de configurer une Pin de mani�re individuelle.
Les param�tres possibles sont donn�s ci-dessus.
Exemple : GPIO_Pin_Conf(Port_B, 4, INPUT, INPUT_FLOATING);

*/
void GPIO_Pin_Conf(int Port, int Broche, int Sens, int Techno);

/*
Fonction d'utilisation d'une PIN
Ex : GPIO_Pin_Set(Port_B,4);
La fonction GPIO_Pin_Read renvoie 0 si la PIN est au niveau logique 0
Sinon, c'est que l'entr�e est au niveau logique 1
*/
void GPIO_Pin_Set(int Port,int Broche);
void GPIO_Pin_Clear(int Port,int Broche);
void GPIO_Pin_Toggle(int Port,int Broche);
short GPIO_Pin_Read(int Port,int Broche);




/* =============================================
				LES TIMERS
 ============================================= */
#define Timer_1 				1
#define Timer_2 				2
#define Timer_3 				3
#define Timer_4 				4
#define Timer_Systick		5

/*
La configuration globale d'un Timer (param�tres ci dessus) permet
de remettre � 0 le timer apr�s le temps pass� en param�tre (en �s).
Dans le d�tail, un Timer est un compteur cadenc� � 72MHz. La fonction calcule
automatiquement le nombre � compter pour atteindre la dur�e voulue.
Par exemple 1s de dur�e implique que le Timer compte jusqu'� 71 999 999.
NB : Le Systick ne permet d'atteindre un peu plus d'1 seconde
     les Timers 1 � 4, permettent d'atteindre 59 sec environ
*/
void Timer_Global_Conf(int Timer, float Duree_us );
//_____________________________________________________________________________

/*
La fonction permet de d�clencher une interruption lorsque la dur�e est �coul�e.
Le param�tre Prio correspond � la priorit� d'interruption. Elle va de 0 � 15.
0 est la priorit� la plus �lev�e
Le second param�tre est un pointeur de fonction. Il suffit de lui donner le nom
de la fonction � �x�cuter.
Par exemple :
Timer_IT_Conf(Timer_1, 3, IT_Tim);
permet de lancer la fonction void IT_TIM(void) lors des d�bordements du Timer 1.
La priorit� d'interruption est 3.

*/
void Timer_IT_Conf(int Timer, char Prio, void (*IT_function) (void));
//_____________________________________________________________________________






/* =============================================
				L'ADC
 ============================================= */
#define		PA0 0
#define		PA1 1
#define		PA2 2
#define		PA3 3
#define		PA4 4
#define		PA5 5
#define		PA6 6
#define		PA7 7
#define		PB0 8
#define		PB1 9
#define		PC0 10
#define		PC1 11
#define		PC2 12
#define		PC3 13
#define		PC4 14
#define		PC5 15

/*
Fonction qui configure l'ADC sur la Pin souhait�e (16 possibles, voir ci-dessus).
La fonction est � lancer qu'une seule fois. Elle configure automatiquement la Pin
choisie en INPUT, ANALOG.
Si l'utilisateur veut pouvoir traiter une autre entr�e, il suffit simplement de configurer
la Pin voulue en INPUT, ANALOG. Pas besoin de relancer la fonction de config g�n�rale.
NB : le temps de pr�l�vement de la tension est fix�e � 10�s.
*/
void ADC_Global_Conf(int Pin);
//_____________________________________________________________________________

/*
C'est une fonction permettant :
- de lancer une conversion sur la Pin voulue
- d'attendre la fin de conversion
- de r�cup�rer le r�sultat de conversion (entier de 0 � 4095) puisque l'ADC est un 12 bits.
*/
int ADC_Get_Value(int Pin);
//_____________________________________________________________________________






/* =============================================
				Liaison Serie USART

 Rappel sur les ressources sur le STM32F103RB 

 3 USART possibles :
 USART_1_TX = (TIM1_CH2) = PA9
 USART_1_RX = (TIM1_CH3) = PA10
 USART_2_TX = (TIM2_CH3) = PA2
 USART_2_RX = (TIM2_CH4) = PA3
 USART_3_TX = PB10
 USART_3_RX = PB11

 ============================================= */
#define USART_1 0
#define USART_2 1
#define USART_3 2

/*
La fonction configure l'USART voulue (USART_1 � USART_3)
Les Pin associ�es sont automatiquement configur�es pour �tre reli�es � l'USART voulue
Comme toutes les transmissions et r�ception se font sous interruption, il faut
donner une priorit�. On conseille de la fixer � 0 de mani�re � la rendre la plus
prioritaire. Cela permet d'�viter de manquer un caract�re (sauf s'il existe des 
contraintes tps r�el tr�s forte par ailleurs).
NB : les interruptions sont tr�s courtes et non bloquantes (<3�s)
On peut r�gler le d�bit en Baud
NB : l'USART est obligatoirement en mode
- 8 bits
- sans contr�le de flux
- 1 bit de stop
- pas de parit�.

Le fichier USART_User_Conf.h contient les param�tres que l'utilisateur peut ajuster en fonction
de l'application. A lire (et modifier si besoin).

NB: le fichier USART_rev.h contient en commentaire toutes les explications sur le fonctionnement
du module USART. 

*/
void Serial_Global_Conf(int Serial,int baudrate, char Prio_USART);
//_____________________________________________________________________________

/*
Fonctions de test des USART. Elles permettent notamment de savoir si un caract�re vient d'�tre re�u
ou si une ch�ine de caract�re (String) vient d'�tre re�ue. Ce dernier �v�nement survient si un caract�re
sp�cial de fin de cha�ne est re�u. Il est param�trable pour chaque USART (voir  USART_User_Conf.h).
Par d�faut le caract�re de fin de cha�ne est \r (= touche entr�e)
Enfin on peut �ventuellement contr�ler que les transmissions / r�ceptions se d�roulent bien avec
les deux derni�res fonctions overrun.
*/
char Serial_Is_Str_Received(int Serial); 
char Serial_Is_Byte_Received(int Serial);
char Serial_Is_Overrun_Receive_Str(int Serial);
void Serial_Clear_Overrun_Receive_Str(int Serial);
//_____________________________________________________________________________


/*
Fonctions d'�criture et de lecture d'un caract�re ou d'une ch�ine.
Pour les cha�nes de caract�re, voici comment se passe une �mission :

Couche applicative :	Suite des codes ASCII + 0
														||
														||   Serial_Put_String
														\/
Liaison physique :			suite des codes ASCII
(le 0, caract�re nul est enlev�) Aucun caract�re de cha�ne n'est �mis

La r�ception :
Couche applicative :	Suite des codes ASCII + 0
														/\
														||   Sur d�tection du caract�re de terminaison
														||
Liaison physique :			suite des codes ASCII + Term_Car_USARTx (� choisir, par d�faut \r ='Enter'=0x0D)

*/
char Serial_Get_Char(int Serial);
char * Serial_Get_String(int Serial); 
void Serial_Put_Char(int Serial,char Car); 
void Serial_Put_String(int Serial,char * pCar);
//_____________________________________________________________________________

#endif
