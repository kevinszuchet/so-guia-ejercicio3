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
char* obtenerRenglonDeSalida(struct Persona*);

int main(int argc, char *argv[]) {
	FILE *fEntrada;
	FILE *fSalida;
	char *renglon = malloc(TAMANIORENGLON);
	char *delim = ";";
	//le agrego el malloc porque fgets lo necesita
	char **arrRenglon = malloc(TAMANIORENGLON);
	int i = 0;
	t_list* listaPersonas = list_create();

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

			//yo me ocuparia aca de hacer el filtro porque no tiene mucho sentido guardar en el array personas que no se van a
			//guardar en el archivo nuevo. en cualquier caso, esas personas ya estan en el archivo viejo
			//(no es que las vas a estar perdiendo)

			// AGREGO PERSONAS A LA LISTA CON LA INFO SPLITEADA
			agregarPersonas(arrRenglon, listaPersonas);
		}
		fclose(fEntrada);
	} else {
		printf("%s\n", "No se puedo abrir el archivo de entrada");
		return EXIT_FAILURE;
	}

	// ABRO EL ARCHIVO DE SALIDA (LECTURA Y ESCRITURA)
	/*fSalida = txt_open_for_append("salida.txt");
	 if(fSalida) {
	 // ORDENO LA LISTA POR REGION Y EDAD
	 t_list* personasFiltradas = list_filter(listaPersonas, menoresDe18);
	 list_sort(listaPersonas, compararPersonas);
	 for(i = 0; i < list_size(personasFiltradas); i++) {
	 struct Persona* unaPersona = list_get(personasFiltradas, i);
	 txt_write_in_file(fSalida, obtenerRenglonDeSalida(unaPersona));
	 }

	 fclose(fSalida);
	 } else {
	 printf("%s\n", "No se puedo abrir el archivo de salida");
	 return EXIT_FAILURE;
	 }*/

	return 0;
}

void agregarPersonas(char** datosPersona, t_list* listaPersonas) {
	struct Persona unaPersona;
	//le agregue lugar a la region porque el strcpy lo necesita!
	strcpy(unaPersona.region, datosPersona[0]);
	strcpy(unaPersona.nombreYapellido, datosPersona[1]);
	unaPersona.edad = atoi(datosPersona[2]);
	strcpy(unaPersona.telefono, datosPersona[3]);
	strcpy(unaPersona.dni, datosPersona[4]);

	//si haces lo que te decia en la linea 56: hace falta guardar el saldo? si mal no recuerdo, no lo pide en el archivo nuevo
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

char* obtenerRenglonDeSalida(struct Persona* unaPersona) {

	//investiga la funcion sprintf, te va a ahorrar bastantes lineas en esta funcion!

	char *nuevoRenglon = string_new();
	string_append(&nuevoRenglon, unaPersona->region);
	string_append(&nuevoRenglon, "|");
	string_append(&nuevoRenglon, string_itoa(unaPersona->edad));
	string_append(&nuevoRenglon, "|");
	string_append(&nuevoRenglon, unaPersona->dni);
	string_append(&nuevoRenglon, "|");
	string_append(&nuevoRenglon, unaPersona->nombreYapellido);
	string_append(&nuevoRenglon, "|");
	string_append(&nuevoRenglon, unaPersona->telefono);

	return nuevoRenglon;
}
