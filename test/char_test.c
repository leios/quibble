#include <stdio.h>

int main(void){
    unsigned char x=5;
    unsigned char y = 15 % 4;
    y = (x * y);

    printf("%i\n", y);
}
