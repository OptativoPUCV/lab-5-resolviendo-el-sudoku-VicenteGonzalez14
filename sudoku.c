#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int seen[10];

    // Validar filas
    for(int i = 0; i < 9; i++){
        for(int k = 1; k <= 9; k++) seen[k] = 0;
        for(int j = 0; j < 9; j++){
            int val = n->sudo[i][j];
            if(val == 0) continue;
            if(seen[val]) return 0;
            seen[val] = 1;
        }
    }

    // Validar columnas
    for(int j = 0; j < 9; j++){
        for(int k = 1; k <= 9; k++) seen[k] = 0;
        for(int i = 0; i < 9; i++){
            int val = n->sudo[i][j];
            if(val == 0) continue;
            if(seen[val]) return 0;
            seen[val] = 1;
        }
    }

    // Validar submatrices 3x3 usando k y p
    for(int k = 0; k < 9; k++){
        for(int i = 1; i <= 9; i++) seen[i] = 0;
        for(int p = 0; p < 9; p++){
            int x = 3 * (k / 3) + (p / 3);  // fila dentro de submatriz
            int y = 3 * (k % 3) + (p % 3);  // columna dentro de submatriz
            int val = n->sudo[x][y];
            if(val == 0) continue;
            if(seen[val]) return 0;
            seen[val] = 1;
        }
    }

    return 1; // Si pasa todas las validaciones
}



List* get_adj_nodes(Node* n){
    List* list = createList();
    int i, j;
    int found = 0;

    // Buscar la primera celda vacía (igual a 0)
    for(i = 0; i < 9 && !found; i++){
        for(j = 0; j < 9 && !found; j++){
            if(n->sudo[i][j] == 0){
                found = 1; // Salimos del bucle una vez encontrada
                i--; j--; // Corrección por el último incremento en el for
            }
        }
    }

    // Si no se encontró casilla vacía, no se generan nodos
    if(!found) return list;

    // Generar 9 nodos adyacentes, con valores del 1 al 9 en la casilla vacía
    for(int val = 1; val <= 9; val++){
        Node* newNode = copy(n);
        newNode->sudo[i][j] = val;
        pushBack(list, newNode); // Agregar a la lista
    }

    return list;
}


int is_final(Node* n){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(n->sudo[i][j] == 0)
                return 0;
        }
    }
    return 1;
}

Node* DFS(Node* n, int* cont){
    List* stack = createList();        // usamos lista como pila
    pushFront(stack, n);               // insertar nodo inicial

    while (first(stack) != NULL){      // mientras la pila no esté vacía
        Node* current = first(stack);  // obtenemos el primer nodo
        popFront(stack);               // lo sacamos
        (*cont)++;

        if (is_final(current))         // si es estado final, retornamos
            return current;

        if (is_valid(current)){        // si es válido, expandimos
            List* adj = get_adj_nodes(current);
            Node* adjNode = first(adj);

            while (adjNode != NULL){
                pushFront(stack, adjNode);   // agregamos al stack (como pila)
                adjNode = next(adj);
            }
        }

        free(current);  // liberamos memoria del nodo actual
    }

    return NULL;  // si no se encontró solución
}


int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}