/*
 * main.c
 *
 *  Created on: 15 abr. 2018
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

typedef struct Persona {
	char region[30];
	char nombreYapellido[30];
	int edad;
	char telefono[15];
	char dni[10];
	double saldo;
} Persona;

void agregarPersona(char**, t_list*, Persona*);
bool compararPersonas(Persona*, Persona*);
bool menoresDe18(Persona*);
bool saldoPobre(Persona*);
char* obtenerRenglonDeSalida(Persona*);
void personaDestroy(Persona*);

int main(int argc, char *argv[]) {
	FILE *fEntrada, *fSalida;
	char *renglon = malloc(TAMANIORENGLON), *delim = ";", **arrRenglon = malloc(TAMANIORENGLON);
	t_list *listaPersonas = list_create();
	int i = 0;

	if (argc != 2) {
		printf("%s\n", "Debe ingresar 1 archivo de entrada para poder leer");
		return EXIT_FAILURE;
	}

	// ABRO EL ARCHIVO DE ENTRADA (LECTURA)
	fEntrada = fopen(argv[1], "r");
	if (fEntrada) {
		// LEO LOS RENGLONES DE ENTRADA
		while (fgets(renglon, TAMANIORENGLON, fEntrada)) {
			Persona *unaPersona = malloc(sizeof(Persona));
			arrRenglon = string_split(renglon, delim);
			agregarPersona(arrRenglon, listaPersonas, unaPersona);
		}
		fclose(fEntrada);
	} else {
		printf("%s\n", "No se puedo abrir el archivo de entrada");
		return EXIT_FAILURE;
	}

	Persona *pUnaPersona = NULL;

	// ABRO EL ARCHIVO DE SALIDA (LECTURA Y ESCRITURA)
	fSalida = txt_open_for_append("salida.txt");
	if(fSalida) {
		// ORDENO LA LISTA POR REGION Y EDAD
		t_list *listaFiltrada = list_create();
		listaFiltrada = list_filter(listaPersonas, menoresDe18);
		list_sort(listaFiltrada, compararPersonas);
		for(i = 0; i < list_size(listaFiltrada); i++) {
			pUnaPersona = list_get(listaFiltrada, i);
			strcpy(renglon, obtenerRenglonDeSalida(pUnaPersona));
			txt_write_in_file(fSalida, renglon);
		}
		list_destroy(listaFiltrada);
		fclose(fSalida);
	} else {
		printf("%s\n", "No se puedo abrir el archivo de salida");
		return EXIT_FAILURE;
	}

	// ABRO EL ARCHIVO DE LOGGEO
	t_list *listaFiltrada = list_create();
	listaFiltrada = list_filter(listaPersonas, saldoPobre);
	t_log *logger = log_create("log.txt", "ejercicio3", true, LOG_LEVEL_TRACE);
	for (i = 0; i < list_size(listaFiltrada); i++) {
		pUnaPersona = list_get(listaFiltrada, i);
		strcpy(renglon, obtenerRenglonDeSalida(pUnaPersona));
		log_trace(logger, renglon);
	}
	list_destroy(listaFiltrada);

	free(renglon);
	free(arrRenglon);
	log_destroy(logger);

	list_destroy_and_destroy_elements(listaPersonas, (void*) personaDestroy);

	exit(EXIT_SUCCESS);
}

void agregarPersona(char **datosPersona, t_list *listaPersonas, Persona *unaPersona) {
	strcpy(unaPersona->region, datosPersona[0]);
	strcpy(unaPersona->nombreYapellido, datosPersona[1]);
	unaPersona->edad = atoi(datosPersona[2]);
	strcpy(unaPersona->telefono, datosPersona[3]);
	strcpy(unaPersona->dni, datosPersona[4]);
	unaPersona->saldo = atof(datosPersona[5]);

	list_add(listaPersonas, unaPersona);
}

bool compararPersonas(Persona *unaPersona, Persona *otraPersona) {
	return (unaPersona->region < otraPersona->region)
			|| (unaPersona->edad <= otraPersona->edad);
}

bool menoresDe18(Persona *unaPersona) {
	return (unaPersona->edad < 18);
}

bool saldoPobre(Persona *unaPersona) {
	return (unaPersona->saldo < 100);
}

char* obtenerRenglonDeSalida(Persona* unaPersona) {

	char *nuevoRenglon = malloc(1000);
	sprintf(nuevoRenglon, "%s|%d|%s|%s|%s\n", unaPersona->region, unaPersona->edad, unaPersona->dni, unaPersona->nombreYapellido, unaPersona->telefono);

	return nuevoRenglon;
}

void personaDestroy(Persona *persona) {
	free(persona);
}
