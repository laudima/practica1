# Practica 1


**Nombre**: Laura Itzel Rodriguez Dimayuga

**Nombre**: Félix Roberto Gómez Cedillo 
			
## Algoritmo Dijkstra 

1. Se calcula la ruta con retraso de comunicación mínimo desde el nodo 0 hacia todos los demás

2. Los retrasos se simularan tomando números aleatorios entre 1 y 100 

3. Al final, basta con que se imprima el retraso total de la ruta desde el nodo 0 
hasta cada nodo. Sin embargo, si también se reporta cual es la ruta. 


## Pseudocodigo para dijkstra 

1. Inicializamos todas las distancias en $D$ con un valor infinito relativo (el nodo de inicio es 0)

2. Para todo vértice $i \in C$ se estable que $P[i]=a$ 

3. Se obtiene el vértice $s$ en $C$, y obtenemos el vértice con menor distancia. Se envía un mensaje al nodo correspondiente y se regresa un mensaje de de respuesta en donde se toma el tiempo y se le asigna su distancia correspondiente. De manera concurrente el nodo destino realiza el mismo procedimiento para calcular su distancia a sus nodos vecinos que no han sido visitados. 

4. Se elimina de $C$ el vértice $s$ 

5. Para cada arista $e$ en $E$ de longitud $l$, que une el vértice $s$ con algún otro vértice $t$ en $C$ 

```
Si l + D[s]< D[t], entonces:

$D[t] := l + D[s]$ 
$P[t] := s$ 
```

6. Se regresa al paso 4. 

## Recibir yu enviar mensajes 

Muchas de las funciones que estan en `main` son para recirbir y enviar informacion a los nodos y fueron proporcionados en la ayudantia. 


## Para corer la práctica  

Para compilar 

```
mpicc practica1.c
```

Para correr el archivo
```
mpirun -np 4  --oversubscribe ./a.out
```

