#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>



// *************************************************
// 					Random Float [Upper]
// *************************************************
float randfl( float Up ) {
	return Up * ( 1.0*rand() / RAND_MAX ) ;
}

// *************************************************
//          Random Range [lower,upper]
// *************************************************
int randrange(int lower, int upper) {
	int num = (rand() % (upper - lower + 1)) + lower; 
    //printf("%d ", num);
	return num ;  
}

// *************************************************
//					  Swap Int
// *************************************************
void swap(int* a, int* b) {
	int tmp = *a ;
	*a = *b ;
	*b = tmp ;
	return ;
}

int arg_min(float arr[], int length) {
	float min = FLT_MAX ;
	int pos ;
	for ( int i = 0 ; i < length ; i++ ){
		if (arr[i] < min) {
			min = arr[i] ;
			pos = i ;
		}
	}
	return pos ;
}

float mean(float arr[], int length) {
	float sum = 0 ;
	for ( int i = 0 ; i < length ; i++ )
		sum += arr[i] ;
	return sum/length ;
}


void print1DArray(float arr[], int length) {
	for ( int i = 0 ; i < length ; i++){
		printf("%f ", arr[i]) ;
	}
	return ;
}

void print2DArray(float arr[][10], int length, int width) {
	for ( int i = 0 ; i < length ; i++) {
		for ( int j = 0 ; j < width ; j++) {
			printf("%f ", arr[i][j]) ;
		}
		printf("\n") ;
	}
	return ;
}

