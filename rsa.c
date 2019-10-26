#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

#define RSA_MAX_NUM_BITS 2048

// TODO: Parte 1
/*
 * 1 - Gerar primos p e q 
 * 2 - Calcular n
 * 3 - Calcular phi(n)
 * 4 - Gerar e
 * 5 - Verificar se e é coprimo de phi(n) (gdc = 1) e obter inverso modular (d)
*/

// TODO: Parte 2
/*
 * 1 - Ler chave base, modulo (n), expoente (e/d), mensagem (m/c)
 * 2 - Fazer encriptação/decriptação (quadrado e multiplicação)  
*/

void read_entry(mpz_t number, int base) {
   char entry[1000];
   
   scanf("%s", entry);

   mpz_set_str(number, entry, base);
}

mp_bitcnt_t find_leftmost_one_bit(mpz_t value) {
   mp_bitcnt_t bit = 0;
    
   for(mp_bitcnt_t i = RSA_MAX_NUM_BITS-1; i > 0 ; i--) {
         bit = mpz_tstbit(value, i);
         if (bit == 1) return i;
   }   
   return(-1);
}

void sqr_n_multiply_module(mpz_t result, mpz_t base, mpz_t power, mpz_t n) {
   int bit, index;

   mpz_add_ui(result, base, 0);
   
   index = find_leftmost_one_bit(power);

   for(int i = --index; i >= 0; i--) {
      bit = mpz_tstbit(power, i);
      mpz_mul(result, result, result);
      mpz_tdiv_r(result, result, n);

      if(bit == 1) {
         mpz_mul(result, result, base);
         mpz_tdiv_r(result, result, n);
      }
   }
}

void generate_keys() {

}

void encript_decript() {
   int base;

   mpz_t n, ed, mc, result;

   // Inicialização
   mpz_init(n);
   mpz_init(ed);
   mpz_init(mc);
   mpz_init(result);

   scanf("%d", &base);
   read_entry(n, base);
   read_entry(mc, base);
   read_entry(ed, base);

   // Encript - m^(e)/Decript - m^(d) 
   sqr_n_multiply_module(result, mc, ed, n);

   gmp_printf("%Zd\n", result);
}

int main() {
   int option;

   printf("O que deseja fazer?\n");
   printf("0 - Gerar chaves\n");
   printf("1 - Encriptar/Decriptar\n");
   printf("2 - Sair\n");
   scanf("%d", &option);

   switch (option) {
      case 0:
      generate_keys();
         break;
      case 1:
         encript_decript();
         break;
      case 2:
         break;
      default:
         printf("Opcao invalida\n");
         break;
   }

   return 0;
}