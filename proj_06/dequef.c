#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "dequef.h"


/**
   Create an empty deque of floats.

   capacity is both the initial and minimum capacity.
   factor is the resizing factor, larger than 1.0.

   On success it returns the address of a new dequef.
   On failure it returns NULL.
**/
dequef* df_alloc(long capacity, double factor) {
   dequef* deque = malloc(sizeof(dequef));
   if (!deque) return NULL;

   deque->data = malloc(capacity * sizeof(float));
   if (!deque->data) {
        free(deque);
        return NULL; 
   }
   
   // Inicializar a memória com zeros
   memset(deque->data, 0, capacity * sizeof(float));

   deque->first = 0;    
   deque->size = 0;
   deque->cap = capacity;
   deque->mincap = capacity;
   deque->factor = factor;

   return deque;
}

/**
  Release a dequef and its data.
**/
void df_free(dequef* D) {
   free(D->data);
   free(D);
}

/**
   The size of the deque.
**/
long df_size(dequef* D) {
   return D->size;
}

/**
   Add x to the end of D.

   If the array is full, it first tries to increase the array size to
   capacity*factor.

   On success it returns 1.
   If attempting to resize the array fails then it returns 0 and D remains unchanged.
**/
int df_push(dequef* D, float x) {
    if (D->size == D->cap) {
        long nova_cap = (long)(D->cap * D->factor);

        float* novo_data = malloc(nova_cap * sizeof(float));
        if (novo_data == NULL) {
            return 0;  
        }
        
        // Inicializar a memória com zeros
        memset(novo_data, 0, nova_cap * sizeof(float));

        // Copiar elementos do array antigo para o novo
        for (long i = 0; i < D->size; i++) {
            novo_data[i] = D->data[(D->first + i) % D->cap];
        }

        free(D->data);
        D->data = novo_data;
        D->cap = nova_cap;
        D->first = 0;  // Resetar o índice inicial
    }

    // Calcular o índice final DEPOIS de redimensionar o array
    int int_fim = (D->first + D->size) % D->cap;
    
    // Adicionar o elemento no final
    D->data[int_fim] = x;
    D->size += 1;

    return 1;
}

/**
   Remove a float from the end of D.

   If the deque has capacity/(factor^2) it tries to reduce the array size to
   capacity/factor.  If capacity/factor is smaller than the minimum capacity,
   the minimum capacity is used instead.  If it is not possible to resize, then
   the array size remains unchanged.

   This function returns the float removed from D.
   If D was empty prior to invocation, the returned float is meaningless.
**/
float df_pop(dequef* D) {
   if (D->size == 0) {
       return 0.0f;
   }
   
   int int_fim = (D->first + D->size - 1) % D->cap;
   float valor = D->data[int_fim];

   D->size--;
   long limite = (long)(D->cap / (D->factor * D->factor));
   if (D->size <= limite && D->cap > D->mincap) {
      long nova_cap = (long)(D->cap / D->factor);
      if (nova_cap < D->mincap) {
         nova_cap = D->mincap;
      }

      float* novo_data = malloc(nova_cap * sizeof(float));
      if (novo_data == NULL) {
         return valor; // Retorna o valor mesmo que o redimensionamento falhe
      }
      
      // Inicializar a memória com zeros
      memset(novo_data, 0, nova_cap * sizeof(float));
      
      // Copiar elementos para o novo array
      for (long i = 0; i < D->size; i++) {
          novo_data[i] = D->data[(D->first + i) % D->cap];
      }
      
      free(D->data);
      D->data = novo_data;
      D->cap = nova_cap;
      D->first = 0; // Resetar o índice inicial
   }

   return valor;
}

/**
   Add x to the beginning of D.

   If the array is full, it first tries to increase the array size to
   capacity*factor.

   On success it returns 1.
   If attempting to resize the array fails then it returns 0 and D remains unchanged.
**/
int df_inject(dequef* D, float x) {
   if (D->size == D->cap) {
      long nova_cap = (long)(D->cap * D->factor);
      
      float* novo_data = malloc(nova_cap * sizeof(float));
      if (novo_data == NULL) {
         return 0;
      }
      
      // Inicializar a memória com zeros
      memset(novo_data, 0, nova_cap * sizeof(float));
      
      // Copiar os elementos para o novo array, começando na posição 1
      // para deixar espaço para o novo elemento no início
      for (long i = 0; i < D->size; i++) {
          novo_data[i+1] = D->data[(D->first + i) % D->cap];
      }
      
      free(D->data);
      D->data = novo_data;
      D->cap = nova_cap;
      D->first = 0; // O novo elemento vai em D->data[0]
      
      // Adicionar o elemento no início
      D->data[0] = x;
      D->size += 1;
      
      return 1;
   }
   
   // Se não precisou redimensionar
   int int_ini;
   if (D->first == 0) {
      int_ini = D->cap - 1;
   } else {
      int_ini = D->first - 1;
   }
   
   D->data[int_ini] = x;
   D->first = int_ini;
   D->size += 1;
   
   return 1;
}

/**
   Remove a float from the beginning of D.

   If the deque has capacity/(factor^2) elements, this function tries to reduce
   the array size to capacity/factor.  If capacity/factor is smaller than the
   minimum capacity, the minimum capacity is used instead.

   If it is not possible to resize, then the array size remains unchanged.

   This function returns the float removed from D.
   If D was empty prior to invocation, the returned float is meaningless.
**/
float df_eject(dequef* D) {
   if (D->size == 0) {
      return 0.0f;
   }

   int int_ini = D->first;
   float valor = D->data[int_ini];

   D->first = (D->first + 1) % D->cap;
   D->size--;

   long limite = (long)(D->cap / (D->factor * D->factor));
   if (D->size <= limite && D->cap > D->mincap) {
      long nova_cap = (long)(D->cap / D->factor);
      if (nova_cap < D->mincap) {
         nova_cap = D->mincap;
      }

      float* novo_data = malloc(nova_cap * sizeof(float));
      if (novo_data == NULL) {
         return valor; // Retorna o valor mesmo que o redimensionamento falhe
      }
      
      // Inicializar a memória com zeros
      memset(novo_data, 0, nova_cap * sizeof(float));
      
      // Copiar elementos para o novo array
      for (long i = 0; i < D->size; i++) {
          novo_data[i] = D->data[(D->first + i) % D->cap];
      }
      
      free(D->data);
      D->data = novo_data;
      D->cap = nova_cap;
      D->first = 0; // Resetar o índice inicial
   }

   return valor;
}

/**
   Set D[i] to x.

   If i is not in [0,|D|-1]] then D remains unchanged.
**/
void df_set(dequef* D, long i, float x) {
   if (i < 0 || i >= D->size) {
      return; // Índice fora dos limites
   }
   D->data[(D->first + i) % D->cap] = x;
}

/**
   Return D[i].

   If i is not in [0,|D|-1]] the returned float is meaningless.
**/
float df_get(dequef* D, long i) {
   if (i < 0 || i >= D->size) {
      return 0.0f; // Retornar 0 para índices inválidos
   }
   return D->data[(D->first + i) % D->cap];
}

/**
   Print the elements of D in a line.
**/
void df_print(dequef* D) {
   printf("deque (%ld):", D->size);
   for (long k = 0; k < D->size; k++) {
      long idx = (D->first + k) % D->cap;
      printf(" %.1f", D->data[idx]);
   }
   printf("\n");
}