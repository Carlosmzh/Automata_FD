Vamos a crear un programa genérico para un AFD capaz de leer/escribir su configuración en un archivo, y decidir la aceptación o no de una cadena de entrada leída desde teclado.

El programa empleará la función ya desarrollada que toma por parámetros  la palabra, alfabeto y transiciones para el AFD.

MODELO GENERAL DE ALGORTMO DEL AFD
estado_actual = estado_inicial
leer cadena de entrada simbolo por simbolo
mientras (simbolo_actual <> $)
{
	estado_actual = ir_a(estado_Actual, simbolo_actual)	
	simbolo_actual = {avanzar en la entrada}
}
si (estado_actual {pertenece al conjunto de estados finales})
	respuesta = "Palabra aceptada"
sino
	respuesta = "Palabra rechazada"