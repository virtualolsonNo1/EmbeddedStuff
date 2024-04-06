#include <stdio.h>

int main(){
uint8_t var = 12;
uint8_t data[] = {0,1,2,3,4,5,6,7};
uint8_t *p1 = &var;
uint8_t *p2 = data+2;
//Draw picture here
data[3] = *p1;
p2[2] = 0; 

printf("var: %d\n", var);

for i = 

  return(0);
}