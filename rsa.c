#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

#define RSA_MAX_NUM_BITS 2048

// TODO: Parte 1
/*
 * 1 - Gerar primos p e q (pendente)
 * 2 - Calcular n (concluído)
 * 3 - Calcular phi(n) (concluído)
 * 4 - Gerar e até que e seja coprimo de phi(n) (gdc = 1) (pendente)
 * 5 - Obter inverso modular (d) (concluído)
*/

// TODO: Parte 2 (Concluído)
/*
 * 1 - Ler chave base, modulo (n), expoente (e/d), mensagem (m/c) (concluído)
 * 2 - Fazer encriptação/decriptação (quadrado e multiplicação) (concluído)
*/

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

   mpz_add_ui(remainder1, a, 0);
   mpz_add_ui(remainder2, b, 0);
   mpz_set_ui(s1, 1);
   mpz_set_ui(s2, 0);
   mpz_set_ui(t1, 0);
   mpz_set_ui(t2, 1);

   do {
      mpz_tdiv_qr(quotient, aux, remainder1, remainder2);
      mpz_add_ui(remainder1, remainder2, 0);
      mpz_add_ui(remainder2, aux, 0);
      
      /*
       * s = s1 - quotient * s2;
       * s1 = s2;
       * s2 = s;
      */
      mpz_mul(aux, quotient, s2);
      mpz_sub(s, s1, aux);
      mpz_add_ui(s1, s2, 0);
      mpz_add_ui(s2, s, 0);
      
      /*
       * t = t1 - quotient * t2;
       * t1 = t2;
       * t2 = t;
      */
      mpz_mul(aux, quotient, t2);
      mpz_sub(t, t1, aux);
      mpz_add_ui(t1, t2, 0);
      mpz_add_ui(t2, t, 0);

   } while(mpz_cmp_ui(remainder2, 0) != 0);
   
   mpz_add_ui(gdc, remainder1, 0);
   mpz_add_ui(modular_inverse, t1, 0);
}

/*
 * Calcula o phi de n em que n = p*q
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
}

/*
 * Gera chaves públicas e privadas para RSA
*/
void generate_rsa_keys() {
   mpz_t p, q, n, phi_n, e, d, gdc;
   
   mpz_init(phi_n);
   mpz_init(d);
   mpz_init(gdc);
   mpz_init(n);

   // TODO: Gerar os primos 'p', 'q' e o expoente 'e'

   // Irei considerar que estou recebendo os primos 'p' e 'q' e 'e'
   // mpz_init_set_str(p, "11", 10);
   // mpz_init_set_str(q, "13", 10);
   // mpz_init_set_str(e, "23", 10);

   mpz_init_set_str(p, "127", 10);
   mpz_init_set_str(q, "13", 10);
   mpz_init_set_str(e, "23", 10);

   // Calcula n
   mpz_mul(n, p, q);

   // Calcula phi(n)
   calc_phi_n(phi_n, p, q);

   // Calcula d
   extended_euclidean(gdc, d, phi_n, e);
   if(mpz_cmp_ui(d, 0) < 0) mpz_add(d, d, phi_n);

   // Saída
   printf("Base: 10\n");
   gmp_printf("Modulo: %Zd\n", n);
   gmp_printf("Expoente publico: %Zd\n", e);
   gmp_printf("Expoente privado: %Zd\n", d);
   gmp_printf("Primo p: %Zd\n", p);
   gmp_printf("Primo q: %Zd\n", q);
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

   return 0;
}