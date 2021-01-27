#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void separator(int length){
	int i;
	for(i=0; i<=length; i++){
		printf("-");
	}
	printf("\n");
}

int main()
{
    int firstNum;
    int secondNum;

    printf("a = ");
    scanf("%d",&firstNum);
    printf("b = ");
    scanf("%d",&secondNum);
    printf("\n");
	
	// assign bigger value to A, smaller to B
	int A = firstNum > secondNum ? firstNum : secondNum;
	int B = firstNum < secondNum ? firstNum : secondNum;
	
	// length of bigger value
	int lenA = A > 0 ? log10(A) + 1 : 1;
	// length of smaller value
	int lenB = B > 0 ? log10(B) + 1 : 1;
	int i;
	
	// declare table for carried digits
	int carries[lenA][lenB];
	
	// array of carries
	int j;
	int digitA, digitB;
	int tmpA = A;
	int tmpB = B;
	for(j = lenB - 1; j>=0; j--){
		
		digitB = tmpB % 10;
		tmpB /= 10;
		for(i = lenA - 1; i>= 0; i--){
			digitA = tmpA % 10;
			tmpA /= 10;
			
			// check for previous carried number
			if(i == lenA - 1){
				carries[i][j] = (digitB * digitA) / 10;
			}else{
				carries[i][j] = ((digitB * digitA) + carries[i + 1][j]) / 10;
			}
		}
	}
	
	//display carried numbers	
	for(j=0;j<lenB;j++){
		int isEmpty=1;
		for(i=0;i<lenA;i++){
			//chcek if carried number isn't empty
			if(carries[i][j] != 0){
				isEmpty=0;
				break;
			}
		}
		if(!isEmpty){
			for(i=0;i<lenB;i++){
				printf(" ");
			}
			for(i=0;i<lenA;i++){
				if(carries[i][j] != 0){
					printf("%d",carries[i][j]);
				}else{
				printf(" ");
				} 
			}
			printf("\n");
		}
	}
	
	//display multiplied numbers
	
	printf("%*d\n", lenA + lenB +1, A);
	printf("x%*d\n", lenA + lenB, B);
	
	//separator
	separator(lenA+lenB);
	
	//print numbers to sum
	if(lenB >1){
		tmpB = B;
		for(i=0;i<lenB; i++){
			if(i+1 == lenB){
				if((A * (tmpB % 10)) != 0)
					printf("+%*d\n", lenA + lenB - i, A * (tmpB % 10));
				} else{
					if((A * (tmpB % 10)) != 0)
					printf("%*d\n", lenA + lenB + 1 - i, A * (tmpB % 10));
			}
			tmpB /= 10;
		}
		separator(lenA+lenB);
	}
	
	//wynik
	printf("%*d\n", lenA + lenB + 1, A*B);

	return 0;
}
