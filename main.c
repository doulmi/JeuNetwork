#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<assert.h>
#include"Network.h"

void testChange();
void testGet1Number();
void testGetResult();
void testGenerateGameMap();
void testDisconnect();
void testFinal();

void testChange() {
	int src[] = { 12, 6, 3, 9, 10, 5, 7, 11, 13, 14, 2, 1, 8, 4 };
	int dest[] = { 6, 3, 9, 12, 5, 10, 11, 13, 14, 7, 1, 8, 4, 2 };
	int i;
	int srcNum;
	for ( i = 0; i < 14; i ++ ) {
		srcNum = src[ i ];
		change( &srcNum );
		assert( srcNum == dest[ i ] );
	}

	for ( i = 0; i < 14; i ++ ) {
		srcNum = src[ i ];
		change( &srcNum );
		change( &srcNum );
		change180( &srcNum );
		assert( srcNum == src[ i ] );
	}

	for ( i = 0; i < 14; i ++ ) {
		srcNum = src[ i ];
		change( &srcNum );
		change270( &srcNum );
		assert( srcNum == src[ i ] );
	}
}

void testGet1Number() {
	assert( get1Number( 1 ) == 1 );
	assert( get1Number( 7 ) == 3 );
	assert( get1Number( 6 ) == 2 );
}

void testGetResult() {
	int **map;
	int n, i, j;
	freopen("maps.txt", "r", stdin); 
	freopen("result.html", "w", stdout);  
	while( scanf( "%d", &n ) != EOF ) {
		map = ( int** )malloc( sizeof( int* ) * ( size_t )n  );
		if ( map == NULL ) {
			printf( "memory is not enough" );
			exit( 0 );
		}
		for ( i = 0; i < n; i ++ ) {
			map[ i ] = ( int* )malloc( sizeof( int ) * ( size_t )n );
			if ( map[ i ] == NULL ) {
				printf( "memory is not enough" );
				exit( 0 );
			}
		}

		for ( i = 0; i < n; i ++ ) {
			for( j = 0; j < n; j ++ ) {
				scanf( "%d", &map[ i ][ j ] );
			}
		}
		displayMapInGraph( map, n );
		getResultHumainLike( map, n );
		displayMapInGraph( map, n );
	}
	fclose(stdin);
	fclose(stdout);
	getchar();
}

void testGenerateGameMap() {
	int n = 6;
	int** map;
	srand( (unsigned)time(NULL) );
	freopen("maps.html", "w", stdout);  
	printf("<html><br/>\n\t<head>\n\t\t<title>Jeu du network</title>\n\t</head>\n<body>");
	map = generateGameMap( n );
	displayMapInGraph( map, n );
	getResult( map, n );
	displayMapInGraph( map, n );
	printf("\t</body></html>");
	fclose(stdout);
}

void testDisconnect() {
	int tube1 = 14;
	int tube2 = 6;
	disconnectUp( &tube1, &tube2 );
	assert( tube1 == 6 );
	assert( tube2 == 4 );

	tube1 = 6;
	tube2 = 11;
	disconnectRight( &tube1, &tube2 );
	assert( tube1 == 2 );
	assert( tube2 == 10 );

	tube1 = 6;
	tube2 = 14;
	disconnectDown( &tube1, &tube2 );
	assert( tube1 == 4 );
	assert( tube2 == 6 );

	tube1 = 11;
	tube2 = 6;
	disconnectLeft( &tube1, &tube2 );
	assert( tube1 == 10 );
	assert( tube2 == 2 );
}

void testFinal() {
	int n = 6;
	int** map;
	int k;
	srand( (unsigned)time(NULL) );
	freopen("maps.html", "w", stdout);  
	printf("<html><br/>\n\t<head>\n\t\t<title>Jeu du network</title>\n\t</head>\n<body>");
	n = 3;
	for ( k = 0; k < 20; k ++ ) {
		printf( "3 * 3 : %d\n", k );
		map = generateGameMap( n );
		displayMapInGraph( map, n );
		//getResultByGuess( map, n );
		getResult( map, n );
		displayMapInGraph( map, n );
	}
	n = 4;
	for ( k = 0; k < 20; k ++ ) {
		printf( "4 * 4 : %d\n", k );
		map = generateGameMap( n );
		displayMapInGraph( map, n );
		//getResultByGuess( map, n );
		getResult( map, n );
		displayMapInGraph( map, n );
	}

	n = 5;
	for ( k = 0; k < 20; k ++ ) {
		printf( "5 * 5 : %d\n", k );
		map = generateGameMap( n );
		displayMapInGraph( map, n );
		//getResultByGuess( map, n );
		getResult( map, n );
		displayMapInGraph( map, n );
	}
	n = 6;
	for ( k = 0; k < 50; k ++ ) {
		printf( "6 * 6 : %d\n", k );
		map = generateGameMap( n );
		//displayMapInGraph( map, n );
		displayMapInGraph( map, n );
		//getResultHumainLike( map, n );
		//displayMapInGraph( map, n );
		//getResultByGuess( map, n );
		getResult( map, n );
		displayMapInGraph( map, n );
	}

	n = 8;
	for ( k = 0; k < 25; k ++ ) {
		printf( "8 * 8 : %d\n", k );
		map = generateGameMap( n );
		displayMapInGraph( map, n );
		//getResultHumainLike( map, n );
		//displayMapInGraph( map, n );
		//getResultByGuess( map, n );
		getResult( map, n );
		displayMapInGraph( map, n );
	}

	n = 12;
	for ( k = 0; k < 12; k ++ ) {
		printf( "12 * 12 : %d\n", k );
		map = generateGameMap( n );
		displayMapInGraph( map, n );
		//getResultHumainLike( map, n );
		//displayMapInGraph( map, n );
		//getResultByGuess( map, n );
		getResult( map, n );
		displayMapInGraph( map, n );
	}
	printf("\t</body></html>");
	fclose(stdout);
}

int main( void ) {
	//testNetwork();
	//testFinal();
	testGenerateGameMap();
	return 1;
}