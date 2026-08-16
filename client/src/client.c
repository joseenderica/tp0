#include "client.h"
int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	logger = iniciar_logger();

	logger = log_create("tp0.log","",true,LOG_LEVEL_INFO);
	
	// Usando el r creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	config = iniciar_config();
	config = config_create("cliente.config");
	if (config != NULL){
		log_info(logger,"Se encontro cliente.config!");
		valor = config_get_string_value(config,"CLAVE");
		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");

		if (valor!=NULL){
			log_info(logger,valor);
		}
		if (ip!=NULL){
			log_info(logger,ip);
		}if(puerto!=NULL){
			log_info(logger,puerto);
		}
	// Loggeamos el valor de config
	//valor = config_get_string_value(config,"valor");
	//log_info(logger,valor);
	}

	/* ---------------- LEER DE CONSOLA ---------------- */

	char* lectura =leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);
	
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente

}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	return nuevo_config;
}

char* leer_consola(t_log* logger)
{
	char* leido = NULL;
	char* acumulador = NULL; //de líneas que leímos
	size_t tamaño_acumulador = 0;

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (1) {
		leido = readline("> ");
		if (leido == NULL) {
			break;
		}
		if (strcmp("", leido) == 0) {
			free(leido);
			break;
		}

		size_t nuevo_tamaño = tamaño_acumulador + strlen(leido) + 1;
		char* temp_acumulador = realloc(acumulador, nuevo_tamaño);
		if (temp_acumulador == NULL) {
			free(leido);
			free(acumulador);
			break;
		}
		acumulador = temp_acumulador;
		if (tamaño_acumulador == 0) {
			strcpy(acumulador, leido);
		} else {
			strcat(acumulador, leido);
		}
		tamaño_acumulador = nuevo_tamaño - 1;
		
		free(leido);
	}
	
	
	return acumulador;
	//log_info(logger,acumulador);
	// ¡No te olvides de liberar las lineas antes de regresar!
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();
	// Leemos y esta vez agregamos las lineas al paquete
	leido = leer_consola(NULL);
	agregar_a_paquete(paquete, leido, strlen(leido) + 1);
	enviar_paquete(paquete, conexion);
	
	eliminar_paquete(paquete);
	free(leido);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	terminar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);

}
