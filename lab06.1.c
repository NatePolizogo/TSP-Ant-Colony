#include <stdbool.h>
#include "myheader.h"

#define Dim 2
#define Mapx 1000
#define Cities 1000
#define ANTS 4

float map[Cities][Dim], Dist[Cities][Cities] ;
float T[Cities][Cities] ;

void map_init() {
	for ( int i = 0 ; i < Cities ; i++ ) 
		for ( int j = 0 ; j < Dim ; j++ )
			map[i][j] = randfl(Mapx) ;
	return ;
}

float eucldist(int j, int k) {
	float dist = 0 ;
	for ( int i = 0 ; i < Dim ; i++ )
		dist += ( map[j][i] - map[k][i] ) * ( map[j][i] - map[k][i] ) ;
	return sqrt(dist) ;
}

void dist_init() { // Down Triagonal
    for ( int i = 1 ; i < Cities ; i++ ) 
        for ( int j = 0 ; j < i ; j++ ) 
            Dist[i][j] = eucldist(i, j) ;
}

float dist_read(int i, int j) {
    if ( i < j ) swap(&i, &j) ;
    return Dist[i][j] ;
}

void pheromone_init(const float a){ // Down Triagonal
    for ( int i = 1 ; i < Cities ; i++ )
        for ( int j = 0 ; j < i ; j++ )
            T[i][j] = a ;
    return ;
}

void pheromone_decay(float decay_rate) {
    for ( int i = 1 ; i < Cities ; i++ )
        for ( int j = 0 ; j < i ; j++ )
            T[i][j] = (1-decay_rate) * T[i][j] ;
    return ;
}

float pheromone_read(int i, int j) {
    if (i < j) swap(&i, &j) ;
    return T[i][j] ;
}

float pheromone_write(float val, int i, int j) {
    if (i < j) swap(&i, &j) ;
    T[i][j] = val ;
    return T[i][j] ;
}

void pheromone_refresh(float Lk, int route[]) {
    float t ;
    float dt = 1.0 / Lk ;
    for( int i = 1 ; i < Cities ; i++ ) {
        t = pheromone_read(route[i-1], route[i]) + dt ;
        pheromone_write(t, route[i-1], route[i]) ;
    }
    t = pheromone_read(route[Cities-1], route[0]) + dt ;
    pheromone_write(t, route[Cities-1], route[0]) ;
    return ;
}

void assign_city(int city[]) {
    for ( int i = 0 ; i < ANTS ; i++ )
        city[i] = randrange(0,Cities-1) ;
    return ;
}

void unexplored_init(int unexplored[], int city) {
	for ( int i = 0 ; i < Cities ; i++ )
        unexplored[i] = i ;
    swap(&unexplored[city], &unexplored[0]) ;
	return ;
}

float probability(float t, float locDist, float den) {
    float num = t*(1.0/locDist) ;
    return num/den ;
}

float compute_denom(int dest[], int state, int explored) {
    float den = 0 ;
    for ( int i = explored ; i < Cities ; i++ ){
        float n = 1.0 / dist_read(state, dest[i]) ;
        den += pheromone_read(state, dest[i]) * n;
    }
    return den ;
}

int select_city(float P[], int dest[], int expl) {
    float rnd = randfl(1) ;
    //P[expl-1] = 0 ;
    for ( int i = expl ; i < Cities ; i++ ){
        if ( P[i-1] < rnd && rnd <= P[i]) {
            return i ;
        }
    }
}

float visit_next(int next, int dest[], int expl ) {
    if ( next != 0 ) {
        swap(&dest[expl], &dest[next]) ;
        return dist_read(dest[expl-1], dest[expl]) ;
    }
    else {
        return dist_read(dest[expl-1], dest[0]) ;
    }
}

void save_Lk(float Lk, float L[], int ant) {
    L[ant] = Lk ;
    return ;
}



int main(int argc, const char * argv[]) {
    map_init() ;
    dist_init() ;
    pheromone_init(1) ;
    int cntr = 0 ;
    int C[ANTS], route[ANTS][Cities] ;
    float L[ANTS] ;
        
    int min_pos ;
    float mean_Lk ;//= FLT_MAX, old = FLT_MAX ;
    do {
        assign_city(C) ;
        for ( int ant = 0 ; ant < ANTS ; ant++ ) {
            int state = C[ant] ;
            float Lk = 0 ;
            unexplored_init(route[ant], C[ant]) ;

            // Doing a complete route
            for ( int explored = 1 ; explored < Cities ; explored++ ) {
                float sum = 0, P[Cities] ; //, p[Cities] ;
                P[explored-1] = 0 ;
                float den = compute_denom(route[ant], state, explored) ;

                // Calculate Probability for each City
                for ( int k = explored ; k < Cities ; k++ ){
                    float dist = dist_read(state, route[ant][k]) ;
                    float t = pheromone_read(state, route[ant][k]) ;
                    float pr = probability(t, dist, den) ;
                    sum += pr ;
                    //p[k] = pr ;
                    P[k] = sum ;
                }
                
                int next = select_city(P, route[ant], explored) ;
                Lk += visit_next(next, route[ant], explored) ;
                state = route[ant][explored] ;
            }
            Lk += visit_next(0, route[ant], Cities) ;
            save_Lk(Lk, L, ant) ;
        }

        min_pos = arg_min(L, ANTS) ;
        mean_Lk = mean(L, ANTS) ;
        if ( cntr%1 == 0) printf("Gen : %d \t Best : %f \t Mean : %f.\n", cntr, L[min_pos], mean_Lk) ;
        pheromone_decay(0.3) ;
        for (int ant = 0 ; ant < ANTS ; ant++ )
            pheromone_refresh(L[ant], route[ant]) ;
    
        // print2DArray(T,Cities,Cities) ;
        if (cntr == 272) {
            int o ;
            o++ ;
        }
        //printf("Press Any Key to Continue\n");  
        // getchar();
        cntr++ ;
    } while (true) ;


    return 0 ;
}