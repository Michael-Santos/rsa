/*
 * @Author Michael dos Santos
 * @RA 726572
 * 
 * @Author Victor Watanabe
 * @RA 726591
*/

#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include <time.h> 

#define RSA_MAX_NUM_BITS 2048

gmp_randstate_t state;

/*
 * Lê um número digitado no input
 * 
 * @params number - Variável que receberá o valor
 * @params base - A base em que o valor lido está
*/
void read_entry(mpz_t number, int base) {
   char entry[1000];
   
   scanf("%s", entry);
   mpz_set_str(number, entry, base);
}

/*
 * Calcula o gdc(a, b) e inverso modular de a, b
 * 
 * @returns gdc - gdc(a, b)
 * @returns modular_inverse - Inverso modular a, b
 * @params a - Primeiro número
 * @params b - Segundo número
*/
void extended_euclidean(mpz_t gdc, mpz_t modular_inverse, mpz_t a, mpz_t b) {
   mpz_t remainder1, remainder2, s, t, s1, s2, t1, t2, quotient, aux;

   // Inicialização
   mpz_init(remainder1);
   mpz_init(remainder2);
   mpz_init(s);
   mpz_init(t);
   mpz_init(s1);
   mpz_init(s2);
   mpz_init(t1);
   mpz_init(t2);
   mpz_init(quotient);
   mpz_init(aux);

   mpz_set(remainder1, a);
   mpz_set(remainder2, b);
   mpz_set_ui(s1, 1);
   mpz_set_ui(s2, 0);
   mpz_set_ui(t1, 0);
   mpz_set_ui(t2, 1);

   do {
      mpz_tdiv_qr(quotient, aux, remainder1, remainder2);
      mpz_set(remainder1, remainder2);
      mpz_set(remainder2, aux);
      
      /*
       * s = s1 - quotient * s2;
       * s1 = s2;
       * s2 = s;
      */
      mpz_mul(aux, quotient, s2);
      mpz_sub(s, s1, aux);
      mpz_set(s1, s2);
      mpz_set(s2, s);
      
      /*
       * t = t1 - quotient * t2;
       * t1 = t2;
       * t2 = t;
      */
      mpz_mul(aux, quotient, t2);
      mpz_sub(t, t1, aux);
      mpz_set(t1, t2);
      mpz_set(t2, t);

   } while(mpz_cmp_ui(remainder2, 0) != 0);
   
   mpz_set(gdc, remainder1);
   mpz_set(modular_inverse, t1);

   mpz_clear(remainder1);
   mpz_clear(remainder2);
   mpz_clear(s);
   mpz_clear(s1);
   mpz_clear(s2);
   mpz_clear(t);
   mpz_clear(t1);
   mpz_clear(t2);
   mpz_clear(quotient);
   mpz_clear(aux);
}

/*
 * Calcula o phi(n) em que n = p*q
 * 
 * @returns result - Calculo de phi de n
 * @params p - Primo composto em n
 * @params q - Primo composto em n
*/
void calc_phi_n(mpz_t result, mpz_t p, mpz_t q) {
   mpz_t p_aux, q_aux;
   
   mpz_init(p_aux);
   mpz_init(q_aux);

   // (p - 1) * (q - 1)   
   mpz_sub_ui(p_aux, p, 1);
   mpz_sub_ui(q_aux, q, 1);
   mpz_mul(result, p_aux, q_aux);

   mpz_clear(p_aux);
   mpz_clear(q_aux);
}

/*
 * Gera número primo aleatório
 * 
 * @returns prime - Varíavel que irá armazenar o número
 * @params size - Tamanho máximo de bits do número
*/
void generate_prime(mpz_t prime, int size) {
   mpz_t max_number;

   // Inicialização
   mpz_init(max_number);

   //max_number = 2^size
   mpz_ui_pow_ui(max_number, 2, size);
   do {
      mpz_urandomb(prime, state, size);
      mpz_nextprime(prime, prime);
   } while(mpz_cmp(prime, max_number) >= 0);

   mpz_clear(max_number);
}

/*
 * Gera expoente e aleatório
 * 
 * @returns e - Varíavel que irá armazenar o número
 * @params size - Tamanho máximo de bits do número
*/
void generate_expoent_e(mpz_t e, int size) {
   mpz_urandomb(e, state, size);
}

/*
 * Gera chaves públicas e privadas para o RSA
*/
void generate_rsa_keys() {
   int key_size;
   mpz_t p, q, n, phi_n, e, d, gdc;
   
   mpz_init(p);
   mpz_init(q);
   mpz_init(n);
   mpz_init(phi_n);
   mpz_init(e);
   mpz_init(d);
   mpz_init(gdc);

   printf("Tamanho da chave: \n");
   scanf("%d", &key_size);
   
   // mpz_init_set_str(p, "11", 10);
   // mpz_init_set_str(q, "13", 10);
   // mpz_init_set_str(e, "23", 10);
   generate_prime(p, key_size / 2);
   generate_prime(q, key_size / 2);  

   /* 
    * Gera novo expoent e enquanto gdc(phi(n), e) != 1, ou seja,
    * não forem coprimos
   */ 
   do {
      generate_expoent_e(e, 128);

      // Calcula n
      mpz_mul(n, p, q);

      // Calcula phi(n)
      calc_phi_n(phi_n, p, q);

      // Calcula d
      extended_euclidean(gdc, d, phi_n, e);
      if(mpz_cmp_ui(d, 0) < 0) mpz_add(d, d, phi_n);
   } while(mpz_cmp_ui(gdc, 1) != 0);

   // Saída
   printf("Base: 10\n");
   gmp_printf("Modulo: %Zd\n", n);
   gmp_printf("Expoente publico: %Zd\n", e);
   gmp_printf("Expoente privado: %Zd\n", d);
   gmp_printf("Primo p: %Zd\n", p);
   gmp_printf("Primo q: %Zd\n", q);

   mpz_clear(p);
   mpz_clear(q); 
   mpz_clear(n);
   mpz_clear(phi_n);
   mpz_clear(e);
   mpz_clear(d);
   mpz_clear(gdc); 
}

/*
 * Encontra o bit mais a esquerda que possui valor 1
 * 
 * @params value - Número para encontrar o bit mais à esquerda com valor 1 
 * @returns Índice do bit
*/
mp_bitcnt_t find_leftmost_one_bit(mpz_t value) {
   mp_bitcnt_t bit = 0;
    
   for(mp_bitcnt_t i = RSA_MAX_NUM_BITS-1; i > 0 ; i--) {
         bit = mpz_tstbit(value, i);
         if (bit == 1) return i;
   }   
   return(-1);
}

/*
 * Modo eficiente de calcular exponenciação modular usando operação 
 * quadrado e multiplicação
 * 
 * @retuns result - Resultado da exponenciação
 * @params base - Base da operação
 * @params power - Potência da operação
 * @params n - Módulo da operação
*/
void sqr_n_multiply_module(mpz_t result, mpz_t base, mpz_t power, mpz_t n) {
   int bit, index;

   mpz_set(result, base);
   
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

/*
 * Faz a encriptação ou decriptação da mensagem
*/
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
   read_entry(ed, base);
   read_entry(mc, base);

   // Encript - m^(e)/Decript - m^(d) 
   sqr_n_multiply_module(result, mc, ed, n);

   // Saída
   printf("Base: 10\n");
   gmp_printf("Mensagem: %Zd\n", result);

   mpz_clears(n, ed, mc, result);
}

int main() {
   int option;
   unsigned long seed = time(NULL);

   // Inicializa random
   gmp_randinit_default(state);
   gmp_randseed_ui(state, seed);

   printf("O que deseja fazer?\n");
   printf("0 - Gerar chaves\n");
   printf("1 - Encriptar/Decriptar\n");
   printf("2 - Sair\n");
   scanf("%d", &option);

   switch (option) {
      case 0:
      generate_rsa_keys();
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

   gmp_randclear(state);
   return 0;
}