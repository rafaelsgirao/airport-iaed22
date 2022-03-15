#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LIM_NOME_AEROPORTO 30+1
#define LIM_NOME_CIDADE 50+1
#define LIM_CODIGO_VOO 6+1
#define LIM_DATA 10+1

#define MAX_AEROPORTOS 40


/*-------------------*/
/*----Protótipos-----*/
/*-------------------*/
void novoAeroporto();
void listaAeroportos();
void adicionaVoo();
void listaVoosAeroporto();
void avancaData();

/*-------------------*/
/*-----Estruturas----*/
/*-------------------*/
typedef struct {
	int dia;
	int mes;
	int ano;
	int hora;
	int minuto;
} Data;
/*-------------------*/
/*-Variáveis Globais-*/
/*-------------------*/

/*long data; TODO*/ 

/*-------------------*/
/*------Funções------*/
/*-------------------*/
int main() {
	char command;
	while ((command = getchar()) != 'q') {
		switch (command) {
		case 'a':
			novoAeroporto();
			break;
		case 'l':
			listaAeroportos();
			break;
		case 'v':
			adicionaVoo();
			break;
		case 'p':
			listaVoosAeroporto();
			break;
		case 't':
			avancaData();
		}
	}
	return 0;
}

void avancaData()
{
	Data data;
	int dia, mes, ano
	scanf("%d%d%d", &dia, &mes, &ano);
	if (dia <= 0 || mes <= 0 || ano <= 0) {
		printf("invalid date\n");
		return;
	}

	/*data.ano += ano; - irrelevant because max data <= 1 yr*/
	data.mes += mes;
	data.dia += dia;
	

	return;
}

/*
	Returns -1 if data1 in the past relative to data2
	0 if both are the same date
	1 if date is in the future
*/
int comparaData(Data data1, Data data2) {
	/*Comparar ano*/
	if (data1.ano > data2.ano) {
		return 1;
	}
	else if (data1.ano > data2.ano) {
		return -1;
	}
	/*Comparar mês*/
	if (data1.mes > data2.mes) {
		return 1;
	}
	else if (data1.mes < data2.mes) {
		return -1;
	}
	/*Comparar dia*/
	if (data1.dia > data2.dia) {
		return 1;
	}
	else if(data1.dia < data2.dia) {
		return -1;
	}
	/*Datas são iguais */
	return 0;
}