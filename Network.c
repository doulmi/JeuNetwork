#include "Network.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int up( int tube ) {
	return ( tube >> 3 ) & 1;
}

int right( int tube ) {
	return ( tube >> 2 ) & 1; 
}

int down( int tube ) {
	return ( tube >> 1 ) & 1; 
}

int left( int tube ) {
	return tube & 1;
}

void setUp( int* tube, int value ) {
	*tube &= 7; /*  7=(0111)2 */
	*tube |= value << 3;
}

void setRight( int* tube, int value ) {
	*tube &= 11; /* 11 = (1011)2 */
	*tube |= value << 2;
}
void setDown( int* tube, int value ) {
	*tube &= 13; /* 13 = (1101)2 */
	*tube |= value << 1;
}

void setLeft( int* tube, int value ) {
	*tube &= 14; /* 14 = (1110)2 */
	*tube |= value;
}

int upTernary( int need ) {
	return need / 27 % 3;
}

int rightTernary( int need ) {
	return need / 9 % 3;
}

int downTernary( int need ) {
	return need / 3 % 3;
}

int leftTernary( int need ) {
	return need % 3;
}

void setUpTernary( int* need, int up ) {
	*need += 27 * ( up - upTernary( *need ) );
}
void setRightTernary( int* need, int right ) {
	*need += 9 * ( right - rightTernary( *need ) );
}
void setDownTernary( int* need, int down ) {
	*need += 3 * ( down - downTernary( *need ) );
}
void setLeftTernary( int* need, int left ) {
	*need += ( left - leftTernary( *need ) );
}

void change( int* tube ) {
	*tube = ( *tube >> 1 ) + ( ( *tube & 1 ) << 3 );
}

void change180( int* tube ) {
	*tube = ( *tube >> 2 ) + ( ( *tube & 3 ) << 2 );
}

void change270( int* tube ) {
	*tube = ( *tube >> 3 ) + ( ( * tube & 7 ) << 1 );
}

int get1Number( int tube ) {
	switch( tube ) {
		case 1: case 2: case 4: case 8: return 1;
		case 3: case 6: case 9: case 12: case 5: case 10: return 2;
		case 7: case 11: case 13: case 14: return 3;
		default: return 0;
	}
}

void getNeed( int** map, int i, int j, int n, int** needTable ) {
	if ( i - 1 >= 0 && ( get1Number( map[ i ][ j ] ) != 1 || get1Number( map[ i - 1 ][ j ] ) != 1 ) ) {
		setUpTernary( &needTable[ i ][ j ], downTernary( needTable[ i - 1 ][ j ] ) );
	} else {
		setUpTernary( &needTable[ i ][ j ], 0 );
	}

	if ( j + 1 < n && ( get1Number( map[ i ][ j ] ) != 1 || get1Number( map[ i ][ j + 1 ] ) != 1 ) ) {
		setRightTernary( &needTable[ i ][ j ], leftTernary( needTable[ i ][ j + 1 ] ) );
	} else {
		setRightTernary( &needTable[ i ][ j ], 0 );
	}

	if ( i + 1 < n && ( get1Number( map[ i ][ j ] ) != 1 || get1Number( map[ i + 1 ][ j ] ) != 1 )) {
		setDownTernary( &needTable[ i ][ j ], upTernary( needTable[ i + 1 ][ j ] ) );
	} else {
		setDownTernary( &needTable[ i ][ j ], 0 );
	}

	if ( j - 1 >= 0 && ( get1Number( map[ i ][ j ] ) != 1 || get1Number( map[ i ][ j - 1 ] ) != 1 ) ) {
		setLeftTernary( &needTable[ i ][ j ], rightTernary( needTable[ i ][ j - 1 ] ) );
	} else {
		setLeftTernary( &needTable[ i ][ j ], 0 );
	}
}

int isSubset( int need, int tube ) {
	if ( ( upTernary( need ) != 2 && up( tube ) != upTernary( need ) ) ||
		( rightTernary( need ) != 2 && right( tube ) != rightTernary( need ) ) ||
		( downTernary( need ) != 2 && down( tube ) != downTernary( need ) ) ||
		( leftTernary( need ) != 2 && left( tube ) != leftTernary( need ) ) ){
		return 0;
	}
	return 1;
}

void binaryToTernary( int* destValue, int srcValue ) {
	setUpTernary( destValue, up( srcValue ) );
	setRightTernary( destValue, right( srcValue ) );
	setDownTernary( destValue, down( srcValue ) );
	setLeftTernary( destValue, left( srcValue ) );
}

void ternaryToBinary( int* destValue, int srcValue ) {
	setUp( destValue, upTernary( srcValue ) );
	setRight( destValue, rightTernary( srcValue ) );
	setDown( destValue, downTernary( srcValue ) );
	setLeft( destValue, leftTernary( srcValue ) );
}

void mask( int* value, int mask ) {
	if ( ( ( mask >> 3 ) & 1 ) == 0 ) {
		setUpTernary( value, 0 );
	}

	if ( ( ( mask >> 2 ) & 1 ) == 0 ) {
		setRightTernary( value, 0 );
	}

	if ( ( ( mask >> 1 ) & 1 ) == 0 ) {
		setDownTernary( value, 0 );
	}

	if ( ( mask & 1 ) == 0 ) {
		setLeftTernary( value, 0 );
	}
}

void fixTube( int** map, int i, int j, int* rest, int n, int** fixedTable, int** needTable ) {
  	int fixedTime = 0; /* combien de state du tube peuvent satisfaire need */
	int state;

	int needMask = 0;
	/* si need = 6610, c¡¯est ¨¤ dire, pour (row, col), il faut connecter avec gauche et dessus. le chiffre 6 c¡¯est ¨¤ dire pour cette direction on ne connait pas encore.  */
	getNeed( map, i, j, n, needTable );
//printf("fixTube %d %d\n<br/>", i, j );
	
	/* v¨¦rifier map[ i ][ j ] peut ¨ºtre fix¨¦e */
	for ( state = 0; state < 4; state ++ ) {
		/* cette forme seulement deux states */
		if ( ( map[ i ][ j ] == 5 || map[ i ][ j ] == 10 ) && state == 2 ) {
			break;
		}

		/* v¨¦rifier si ce state marche ou pas */
  		if ( isSubset( needTable[ i ][ j ], map[ i ][ j ] ) ) {
//printf( "need=%d, can=%d<br/>", needTable[ i ][ j ], map[ i ][ j ] );
			fixedTime ++;
			needMask |= map[ i ][ j ];
		}
		change( &map[ i ][ j ] );
	}

	mask( &needTable[ i ][ j ], needMask );

	/* s'il y a seulement une solution, on peut fixer cette cellule */
	if ( fixedTime == 1 ) {
		map[ i ][ j ] = needMask;
		( *rest ) --;
		fixedTable[ i ][ j ] = 1;
		binaryToTernary( &needTable[ i ][ j ], map[ i ][ j ] );
//    printf( "%d %d is fixed to %d, rest: %d\n<br/>", i, j, map[ i ][ j ], *rest );
//displayMapInGraph( map, n );
     	if ( i - 1 >= 0 && ! fixedTable[ i - 1 ][ j ] ) {
			fixTube( map, i - 1, j, rest, n, fixedTable, needTable );
		}
		if ( j + 1 < n && ! fixedTable[ i ][ j + 1 ] ) {
			fixTube( map, i, j + 1, rest, n, fixedTable, needTable );
		}
  		if ( i + 1 < n && ! fixedTable[ i + 1 ][ j ]) {
			fixTube( map, i + 1, j, rest, n, fixedTable, needTable );
		}
		if ( j - 1 >= 0 && ! fixedTable[ i ][ j - 1 ]) {	
 			fixTube( map, i, j - 1, rest, n, fixedTable, needTable );
		}
	}
}

void displayMap( int** map, int n ) {
	int i, j;
	for ( i = 0; i < n; i ++ ) {
		for ( j = 0; j < n; j ++ ) {
			printf( "%d ", map[ i ][ j ] );
		}
		printf( "<br/>" );
	}
	printf( "<br/>" );
}

void displayMapInGraph( int** map, int n ) {
	int i, j;
	printf("<table border='1'>\n");
	for ( i = 0; i < n; i ++ ) {
		printf( "<tr>\n");
		for ( j = 0; j < n; j ++ ) {
			printf( "<td><img src='images/%d.gif'/></td>\n", map[ i ][ j ] );
		}
		printf( "</tr>\n" );
	}
	printf( "</table><br/>\n" );
}

void getResultHumainLike( int** map, int n ) {
	int i, j;

	//le nombre de cellule qui ne traitent pas
	int rest = n * n;

	//allouer la m¨¦moire pour fixedTable
	int ** fixedTable = generateMatrice( n, 0 );
	int ** needTable = generateMatrice( n, 80 );
	int step = 0;
	while ( rest > 0 && step <= n ) {
		for ( i = 0; i < n; i ++ ) {
			for ( j = 0; j < n; j ++ ) {
				if ( ! fixedTable[ i ][ j ] ) {
					fixTube( map, i, j, &rest, n, fixedTable, needTable );
				}
			}
		}
		step ++;
	}
	free( fixedTable );
}

void getResult( int** map, int n ) {
	int i, j;

	//le nombre de cellule qui ne traitent pas
	int rest = n * n;

	//allouer la m¨¦moire pour fixedTable
	int ** fixedTable = generateMatrice( n, 0 );
	int ** needTable = generateMatrice( n, 80 );
	int step = 0;
	int finish = 0;
	while ( rest > 0 && step <= n ) {
		for ( i = 0; i < n; i ++ ) {
			for ( j = 0; j < n; j ++ ) {
				if ( ! fixedTable[ i ][ j ] ) {
					fixTube( map, i, j, &rest, n, fixedTable, needTable );
				}
			}
		}
		step ++;
	}
	/* il y a encore quelques tube on ne fixe pas, du coup on devine pour resoudre ce problem*/
	if ( rest > 0 ) {
		guessRestTube( map, fixedTable, n, 0, &finish );
	}
	free( fixedTable );
}

void guessRestTube( int** map, int** fixedTable, int n, int tubeNo, int* finish ) {
	int row, col;
	int state;
	int maxTubeNo = n * n;
	int needUp = 0;
	int needLeft = 0;
	if ( tubeNo < maxTubeNo && ! *finish ) {
		row = tubeNo / n;
		col = tubeNo % n;	
		if ( fixedTable[ row ][ col ] ) {
			guessRestTube( map, fixedTable, n, tubeNo + 1, finish );
			if ( tubeNo == maxTubeNo - 1 && isFinished( map, n ) ) {
				*finish = 1;
				return;
			}
		} else {
			if ( row - 1 >= 0 && down( map[ row - 1 ][ col ] ) ) {
				needUp = 1;
			}
			if ( col - 1 >= 0 && right( map[ row ][ col - 1 ] ) ) {
				needLeft = 1;
			}
			for( state = 0; state < 4; state ++ ) {
				if ( up( map[ row ][ col ] ) == needUp && left( map[ row ][ col ] ) == needLeft ) {
					guessRestTube( map, fixedTable, n, tubeNo + 1, finish );
					if ( *finish ) {
						return;
					}
					if ( tubeNo == maxTubeNo - 1 && isFinished( map, n ) ) {
						*finish = 1;
						return;
					}
				}
				change( &map[ row ][ col ] );
			}
		}
	}
}

void getResultByGuess( int** map, int n ) {
	int finish = 0;
	guess( map, n, 0, &finish );
}

void guess( int** map, int n, int tubeNo, int* finish ) {
	int row, col;
	int state;
	int maxTubeNo = n * n;
	int needUp = 0;
	int needLeft = 0;
	if ( tubeNo < maxTubeNo && ! *finish ) {
		row = tubeNo / n;
		col = tubeNo % n;
		if ( row - 1 >= 0 && down( map[ row - 1 ][ col ] ) ) {
			needUp = 1;
		}
		if ( col - 1 >= 0 && right( map[ row ][ col - 1 ] ) ) {
			needLeft = 1;
		}
		for( state = 0; state < 4; state ++ ) {
			if ( up( map[ row ][ col ] ) == needUp && left( map[ row ][ col ] ) == needLeft ) {
				guess( map, n, tubeNo + 1, finish );
				if ( *finish ) {
					return;
				}
				if ( tubeNo == maxTubeNo - 1 && isFinished( map, n ) ) {
					*finish = 1;
					return;
				}
			}
			change( &map[ row ][ col ] );
		}
	}
}

int isFinished( int** map, int n ) {
	int ** unionMap = generateMatrice( n, 0 );
	int tubeCount = 0;
	DSF( map, n, 0, 0, unionMap, &tubeCount );
	return tubeCount == n * n;
}

void DSF( int ** map, int n, int row, int col, int** unionMap, int* tubeCount ) {
	unionMap[ row ][ col ] = 1;
	( *tubeCount ) ++;
	if ( up( map[ row ][ col ] ) && row - 1 >= 0 && ! unionMap[ row - 1 ][ col ] ){
		DSF( map, n, row - 1, col, unionMap, tubeCount );
	}
	
	if ( right( map[ row ][ col ] ) && col + 1 < n && ! unionMap[ row ][ col + 1 ] ){
		DSF( map, n, row, col + 1, unionMap, tubeCount );
	}

	if ( down( map[ row ][ col ] ) && row + 1 < n && ! unionMap[ row + 1 ][ col ] ){
		DSF( map, n, row + 1, col, unionMap, tubeCount );
	}

	if ( left( map[ row ][ col ] ) && col - 1 >= 0 && ! unionMap[ row ][ col - 1 ] ){
		DSF( map, n, row, col - 1, unionMap, tubeCount );
	}
}

int** generateMatrice( int n, int initialVal ) {
	int ** matrice;
	int i, j;
	//allouer la m¨¦moire pour la matrice
	matrice = ( int** )malloc( sizeof( int* ) * ( size_t )n );
	if ( matrice == NULL ) {
		printf( "memory is not enough" );
		exit( 0 );
	}
	for ( i = 0; i < n; i ++ ) {
		matrice[ i ] = ( int* )malloc( sizeof( int ) * ( size_t )n );
		if ( matrice[ i ] == NULL ) {
			printf( "memory is not enough" );
			exit( 0 );
		}
	}

	//initialiser la matrice;
	for ( i = 0; i < n; i ++ ) {
		for ( j = 0; j < n; j ++ ) {
			matrice[ i ][ j ] = initialVal;
		}
	}
	return matrice;
}

int** generateGameMap( int n ) {
	int row, col;
	int** map = generateMatrice( n, 0 );
	int** needTable = generateMatrice( n, 80 );
	for ( row = 0; row < n; row ++ ) {
		for ( col = 0; col < n; col ++ ) {
			if ( ! map[ row ][ col ] ) {
				ternaryToBinary( &map[ row ][ col ], generateSuitableNum( map, row, col, n, needTable ) );
			}
		}
	}
	printf("tout d'abord\n");
	displayMapInGraph( map, n );
	connectAll( map, n );
	printf("final\n");
	displayMapInGraph( map, n );
	shuffleState( map, n );
	return map;
}

int getRandomBinary() {
	return rand() % 2;
}

int generateSuitableNum( int** map, int row, int col, int n, int** needTable ) {
	int last2 = 0;

	getNeed( map, row, col, n, needTable ); /* utilise map comme fixedTable, quand map[ row ][ col ] ne fixe pas, map[ row ][ col ] = 0 */
	if ( upTernary( needTable[ row ][ col ] ) == 2 ) {
		setUpTernary( &needTable[ row ][ col ], getRandomBinary() );
	}
	if ( rightTernary( needTable[ row ][ col ] ) == 2 ) {
		setRightTernary( &needTable[ row ][ col ], getRandomBinary() );
		last2 = 1;
	}
	if ( downTernary( needTable[ row ][ col ] ) == 2 ) {
		setDownTernary( &needTable[ row ][ col ], getRandomBinary() );
		last2 = 2;
	}
	if ( leftTernary( needTable[ row ][ col ] ) == 2 ) {
		setLeftTernary( &needTable[ row ][ col ], getRandomBinary() );
		last2 = 3;
	}

	if ( needTable[ row ][ col ] == 0 ) {
		switch( last2 ) {
			case 0: setUpTernary( &needTable[ row ][ col ], 1 ); break;
			case 1: setRightTernary( &needTable[ row ][ col ], 1 ); break;
			case 2: setDownTernary( &needTable[ row ][ col ], 1 ); break;
			case 3: setLeftTernary( &needTable[ row ][ col ], 1 ); break;
		}
	} else if ( needTable[ row ][ col ] == 40 ) { /* 40 = (1111)3 */
		switch( last2 ) {
			case 0: setUpTernary( &needTable[ row ][ col ], 0 ); break;
			case 1: setRightTernary( &needTable[ row ][ col ], 0 ); break;
			case 2: setDownTernary( &needTable[ row ][ col ], 0 ); break;
			case 3: setLeftTernary( &needTable[ row ][ col ], 0 ); break;
		}
	}
	return needTable[ row ][ col ];
}

void connectAll( int** map, int n ) {
	//displayMapInGraph( map, n );
	int** unionMap = generateMatrice( n, 0 );
	int unionNum = 0;
	int objetctUnionNum = 2;
	int i, j;
	int step = 0;
	int maxStep = n;
	preConnectAll( map, n, unionMap, &unionNum );
	displayMap( unionMap, n );
	printf("apres avoir detruit des circuits\n");
	displayMapInGraph( map, n );

	/* si ce n¡¯est pas un grille connexe( unionNum > 1 ), on les tous connecte */
	while( objetctUnionNum <= unionNum && step < maxStep ) {
		for ( i = 0; i < n; i ++ ) {
			for ( j = 0; j < n; j ++ ) {
				if ( unionMap[ i ][ j ] == objetctUnionNum ) {
					if ( i - 1 >= 0 && get1Number( map[ i ][ j ] ) < 3 && 
						get1Number( map[ i - 1 ][ j ] ) < 3 && unionMap[ i - 1 ][ j ] != objetctUnionNum ) {
						changeUnionNumber( unionMap, map, i, j, unionMap[ i - 1 ][ j ], objetctUnionNum );
						connectUp( &map[ i ][ j ], &map[ i - 1 ][ j ] );
						objetctUnionNum ++; 
					} else if ( j - 1 >= 0 && get1Number( map[ i ][ j ] ) < 3 && 
						get1Number( map[ i ][ j - 1 ] ) < 3 && unionMap[ i ][ j - 1 ] != objetctUnionNum ) {
						changeUnionNumber( unionMap, map, i, j, unionMap[ i ][ j - 1 ], objetctUnionNum );
						connectLeft( &map[ i ][ j ], &map[ i ][ j - 1 ] );
						objetctUnionNum ++; 
					} else if ( j + 1 < n && get1Number( map[ i ][ j ] ) < 3 && 
						get1Number( map[ i ][ j + 1 ] ) < 3 && unionMap[ i ][ j + 1 ] != objetctUnionNum ) {
						changeUnionNumber( unionMap, map, i, j, unionMap[ i ][ j + 1 ], objetctUnionNum );
						connectRight( &map[ i ][ j ], &map[ i ][ j + 1 ] );
						objetctUnionNum ++; 
					} else if ( i + 1 < n && get1Number( map[ i ][ j ] ) < 3 && 
						get1Number( map[ i + 1 ][ j ] ) < 3 && unionMap[ i + 1 ][ j ] != objetctUnionNum ) {
						changeUnionNumber( unionMap, map, i, j, unionMap[ i + 1 ][ j ], objetctUnionNum );
						connectDown( &map[ i + 1 ][ j ], &map[ i ][ j ] );
						objetctUnionNum ++; 
					}
				}
			}
		}
		step ++;
	}
	displayMap( unionMap, n );
	free( unionMap );
}

void changeUnionNumber( int** unionMap, int** map, int row, int col, int destNum, int srcNum ) {
	unionMap[ row ][ col ] = destNum;
	if ( up( map[ row ][ col ] ) && unionMap[ row - 1 ][ col ] == srcNum ) {
		changeUnionNumber( unionMap, map, row - 1, col, destNum, srcNum );
	}
	if ( right( map[ row ][ col ] ) && unionMap[ row ][ col + 1 ] == srcNum ) {
		changeUnionNumber( unionMap, map, row, col + 1, destNum, srcNum );
	}
	if ( down( map[ row ][ col ] ) && unionMap[ row + 1 ][ col ] == srcNum  ) {
		changeUnionNumber( unionMap, map, row + 1, col, destNum, srcNum );
	}
	if ( left( map[ row ][ col ] ) && unionMap[ row ][ col - 1 ] == srcNum  ) {
		changeUnionNumber( unionMap, map, row, col - 1, destNum, srcNum );
	}
}

void connectUp( int* tube1, int* tube2 ) {
	*tube1 |= 8; /* 8 = (1000)2 */
	*tube2 |= 2; /* 2 = (0010)2 */
}

void connectLeft( int* tube1, int* tube2 ) {
	*tube1 |= 1; /* 1 = (0001)2 */
	*tube2 |= 4; /* 4 = (0100)2 */
}

void connectRight( int* tube1, int* tube2 ) {
	*tube1 |= 4; /* 4 = (0100)2 */
	*tube2 |= 1; /* 1 = (0001)2 */
}

void connectDown( int* tube1, int* tube2 ) {
	*tube1 |= 2; /* 2 = (0010)2 */
	*tube2 |= 8; /* 8 = (1000)2 */
}

void preConnectAll( int** map, int n, int** unionMap, int* unionNum ) {
	/* ce peut etre n¡¯est pas un graphe connexe */
	/* on trouve tous les graphe isolide et les remarque dans unionMap en utilisant unionNum */
	int i, j;
	for ( i = 0; i < n; i ++ ) {
		for( j = 0; j < n; j ++ ) {
			if ( unionMap[ i ][ j ] == 0 ) {
				*unionNum += 1;
				destryoCircleDSF( map, i, j, unionMap, unionNum );
			}
		}
	}
}

void destryoCircleDSF( int** map, int row, int col, int** unionMap, int* unionNum ) {
	unionMap[ row ][ col ] = -1;
	if ( up( map[ row ][ col ] ) ){
		if ( unionMap[ row - 1 ][ col ] == 0 ) {
			destryoCircleDSF ( map, row - 1, col, unionMap, unionNum);
		} else if ( unionMap[ row - 1 ][ col ] == *unionNum ) {
			disconnectUp( &map[ row ][ col ] , &map[ row - 1 ][ col ] );
		}
	}
	
	if ( right( map[ row ][ col ] ) ){
		if ( unionMap[ row ][ col + 1 ] == 0 ) {
			destryoCircleDSF ( map, row, col + 1, unionMap, unionNum);
		} else if ( unionMap[ row ][ col + 1 ] == *unionNum ) {
			disconnectRight( &map[ row ][ col ] , &map[ row ][ col + 1 ] );
		}
	}

	if ( down( map[ row ][ col ] ) ){
		if ( unionMap[ row + 1 ][ col ] == 0 ) {
			destryoCircleDSF ( map, row + 1, col, unionMap, unionNum);
		} else if ( unionMap[ row + 1 ][ col ] == *unionNum ) {
			disconnectDown( &map[ row ][ col ] , &map[ row + 1 ][ col ] );
		}
	}

	if ( left( map[ row ][ col ] ) ){
		if ( unionMap[ row ][ col - 1 ] == 0 ) {
			destryoCircleDSF ( map, row, col - 1, unionMap, unionNum);
		} else if ( unionMap[ row ][ col - 1 ] == *unionNum ) {
			disconnectLeft( &map[ row ][ col ] , &map[ row ][ col - 1 ] );
		}
	}
	unionMap[ row ][ col ] = *unionNum;
}

void disconnectUp( int* tube1, int* tube2 ) {
	*tube1 &= 7; // 7 = (0111)2
	*tube2 &= 13; //13 = (1101)2
}

void disconnectRight( int* tube1, int* tube2 ) {
	*tube1 &= 11;// (1011)2
	*tube2 &= 14;// (1110)2
}

void disconnectDown( int* tube1, int* tube2 ) {
	*tube1 &= 13;// (1101)2
	*tube2 &= 7; // (0111)2
}

void disconnectLeft( int* tube1, int* tube2 ) {
	*tube1 &= 14;// (1110)2
	*tube2 &= 11;// (1011)2
}

void shuffleState( int** map, int n ) {
	int i, j;
	int changeStateTime = 0;
	for ( i = 0; i < n; i ++ ) {
		for ( j = 0; j < n; j ++ ) {
			changeStateTime = rand() % 4;
			switch( changeStateTime ) {
				case 1: change( &map[ i ][ j ] ); break;
				case 2: change180( &map[ i ][ j ] ); break;
				case 3: change270( &map[ i ][ j ] ); break;
			}
		}
	}
}