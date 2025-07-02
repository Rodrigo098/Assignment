#include <stdio.h>
#include <math.h>

#define FRACTIONAL_BITS 8 // Duas casas iria ser necessário pelo menos 7 bits outra opção seria realizar a multiplicação por 100


int int_sqrt(long value) { 
    // sqrt  do math.h nao iria dar pois retorna um double


    if (value <= 0) return 0;
    
    int result = 0;
    int bit = 1 << 30;
    
    while (bit > value) {
        bit >>= 2;
    }
    
    while (bit != 0) {
        if (value >= result + bit) {
            value -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result << (FRACTIONAL_BITS / 2);
}

int convert_to_fixed_arithmetic(int number){
    int temp= (int)number*(1<<FRACTIONAL_BITS);
    return temp;
}
long convert_long_to_fixed_arithmetic(long number){
    long temp= (long)number*(1<<FRACTIONAL_BITS);
    return temp;
}

int convert_from_fixed_arithmetic_to_int(int number){
    int temp= (int)number/(1<<FRACTIONAL_BITS);
    return temp;
}

int compute_rms_fixed(const int *samples, int len) {
// Your implementation here
// Irei adicionar comentários a explicar o meu raciocinio apesar do código não ser complexo
// A expressão matemática do root mean square é a raiz quadrada da media de todos os valores ao quadrado
// Primeiramente irei fazer um pequeno ciclo for para obter o valor do somatorio

    int i=0;
    long soma=0; // Usei o long porque com int teria um overflow
    for(i=0;i<len;i++){
        int temp=samples[i];
        soma+= temp*temp;
    }
    soma= convert_long_to_fixed_arithmetic(soma);// converto a soma para a divisão que vai ser feita
    len=convert_to_fixed_arithmetic(len);
    long divisao= (long) (soma<<FRACTIONAL_BITS)/ len;
    int raiz= int_sqrt(divisao);
    return convert_from_fixed_arithmetic_to_int(raiz);

}
//(...)
int main(void) {
const int samples[50] = {
    1010, 980, 1024, 990, 1005, 1018, 975, 992, 1003, 998,
    1012, 984, 1021, 995, 1008, 1015, 982, 987, 1000, 997,
    1007, 978, 1030, 993, 1002, 1022, 979, 989, 1004, 996,
    1011, 985, 1025, 994, 1006, 1016, 980, 986, 1001, 995,
    1009, 977, 1028, 991, 1000, 1020, 981, 990, 1002, 998
    };
    
    int rms = compute_rms_fixed(samples, 50);
    printf("Integer sqrt RMS: %d\n", rms);
    return 0;
}