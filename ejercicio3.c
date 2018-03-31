/*
 * ejercicio3.c
 *
 *  Created on: 24/3/2018
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// COMMONS
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

#define TAMANIORENGLON 1000

struct Persona {
	char region[30];
	char nombreYapellido[30];
	int edad;
	char telefono[15];
	char dni[10];
	double saldo;
};

void agregarPersonas(char**, t_list*);
bool compararPersonas(struct Persona, struct Persona);
bool menoresDe18(struct Persona);
bool saldoPobre(struct Persona);
char* obtenerRenglonDeSalida(struct Persona*);

int main(int argc, char *argv[]) {
	FILE *fEntrada, *fSalida, *fLog;
	char *renglon = malloc(TAMANIORENGLON), *delim = ";", **arrRenglon = malloc(TAMANIORENGLON);
	t_list *listaPersonas = list_create(), *listaFiltrada = list_create();
	int i = 0;
	struct Persona *unaPersona = NULL;

	if (argc != 2) {
		printf("%s\n", "Debe ingresar 1 archivo de entrada para poder leer");
		return EXIT_FAILURE;
	}

	// ABRO EL ARCHIVO DE ENTRADA (LECTURA)
	fEntrada = fopen(argv[1], "r");
	if (fEntrada) {
		// LEO LOS RENGLONES DE ENTRADA
		while (fgets(renglon, TAMANIORENGLON, fEntrada)) {
			arrRenglon = string_split(renglon, delim);
			agregarPersonas(arrRenglon, listaPersonas);
		}
		fclose(fEntrada);
	} else {
		printf("%s\n", "No se puedo abrir el archivo de entrada");
		return EXIT_FAILURE;
	}

	// ABRO EL ARCHIVO DE SALIDA (LECTURA Y ESCRITURA)
	fSalida = txt_open_for_append("salida.txt");
	if(fSalida) {
		// ORDENO LA LISTA POR REGION Y EDAD
		listaFiltrada = list_filter(listaPersonas, menoresDe18);
		list_sort(listaFiltrada, compararPersonas);
		for(i = 0; i < list_size(listaFiltrada); i++) {
			unaPersona = list_get(listaFiltrada, i);
			txt_write_in_file(fSalida, obtenerRenglonDeSalida(unaPersona));
		}
		fclose(fSalida);
	} else {
		printf("%s\n", "No se puedo abrir el archivo de salida");
		return EXIT_FAILURE;
	}

	// ABRO EL ARCHIVO DE LOGGEO
	fLog = txt_open_for_append("log.txt");
	if(fLog) {
		listaFiltrada = list_filter(listaPersonas, saldoPobre);
		for(i = 0; i < list_size(listaFiltrada); i++) {
			unaPersona = list_get(listaFiltrada, i);
			t_log *logger = log_create(fLog, "ejercicio3", true, log_level_from_string(obtenerRenglonDeSalida(unaPersona)));
			log_trace(logger, obtenerRenglonDeSalida(unaPersona));
		}
		fclose(fLog);
	} else {
		 printf("%s\n", "No se puedo abrir el archivo de loggeo");
		 return EXIT_FAILURE;
	}

	return 0;
}

void agregarPersonas(char **datosPersona, t_list *listaPersonas) {
	struct Persona unaPersona;
	strcpy(unaPersona.region, datosPersona[0]);
	strcpy(unaPersona.nombreYapellido, datosPersona[1]);
	unaPersona.edad = atoi(datosPersona[2]);
	strcpy(unaPersona.telefono, datosPersona[3]);
	strcpy(unaPersona.dni, datosPersona[4]);
	unaPersona.saldo = atof(datosPersona[5]);

	list_add(listaPersonas, &unaPersona);
}

bool compararPersonas(struct Persona unaPersona, struct Persona otraPersona) {
	return (unaPersona.region < otraPersona.region)
			|| (unaPersona.edad <= otraPersona.edad);
}

bool menoresDe18(struct Persona unaPersona) {
	return unaPersona.edad < 18;
}

bool saldoPobre(struct Persona unaPersona) {
	return unaPersona.saldo < 100;
}

char* obtenerRenglonDeSalida(struct Persona* unaPersona) {

	char *nuevoRenglon = string_new();
	sprintf(nuevoRenglon, "%s|%d|%s|%s|%s\n", unaPersona->region, unaPersona->edad, unaPersona->dni, unaPersona->nombreYapellido, unaPersona->telefono);

	return nuevoRenglon;
}
