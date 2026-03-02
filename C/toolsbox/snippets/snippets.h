// Mcros index
enum funtion_list{
	_ISDIGIT ,		// Check if ASCII code is numeric character. Recive value( n ). Return boolean.
	_ISALPHAU ,		// Check if ASCII code is upper case character. Recive value( n ). Return boolean.
	_ISALPHAL ,		// Check if ASCII code is lower case character. Recive value( n ). Return boolean.
	_ISCONTROL ,		// Check if ASCII code is control character. Recive value( i ). Return boolean.
	_ISBLANK ,		// Check if ASCII code is blank character ( ' ' or '\n' or '\t' or '\r' or '\r' or '\b' or '\v' or '\a' or '\f'). Recive a value( n ). Return boolean.
	_TOUINT ,		// Convert numeric character to decimal value. Recive numeric character( x ). Returns decimal value or 0 for not numeric characters.
	_UPPER ,		// Convert lower case character to upper case. Recive a value( n ). Return upper case ASCII code or input value for not lower case characters. 
	_LOWER ,		// Convert lower case character to lower case. Recive a value( n ). Return lower case ASCII code or input value for not upper case characters.
	_ISNOTPAR ,		// Check if a value is not par. Recive value( n ). Return boolean.
	_BIGGER ,		// Select biggest value. Recive two values( n , m ). Return biggest value.
	_SMALLER ,		// Select smallest value. Recive twuo values( n , m ). Return smallest value.
	_ABS ,			// Calculate absolute value. Recive value( n ). Return | value |.
	_MAXVALUE ,		// Calculate the maximun value posible fot variable type. Recive a variable type( e ). Return value.
	_ROUNDUP ,		// Calculate next value multiple of factor( j ) bigger than base number( i ). Recive two positive values( i , j ). Return next multiple value
	_BOOLEAN ,		// Calculate boolean value. Recive value( n ). Return value 1 for not 0 input.
	_SWITCH ,		// Switch boolean value of a variable. Recive variable( v ). Change varabiale value for its own negate value.
	_SWAP ,			// Exchange the values from two variables. Recive variable type( e ) and two variables( v , w ). Change v value to w value and w value to v value.
	_COUNT ,		// Calculate size of multidimensioanl array. Recive pointer( p ). Return positions in array.
	_ERASE ,		// Set al positions in array to 0. Recive pointer( p ).
	_COPY ,			// Copy array from source( p ) to target( q ). Recive two pointers( p , q ). Macro COPY is a for loop without instruccion end character ";" that use a local variable( unsigned char copy_i ) as array's index, then trasformations can be done durig the copy process.
	_OFFSET ,		// Calculate the offset position of element( t ) from struct( r ). Recive struct type( r ) and struct's elemten( t ). Retun offset pointer value.
	_BIT ,			// Set position bit. Recive positive value( i ). Returns bit flag value.
	_ISBITUP ,		// Check if at least one up bit from a bit mask( m ) is up in base value( n ). Recive values( n , m ). Return bits that are up in both values.
	_ISMASKON ,		// Check is all bits all up bits from a bit mask( m ) are up in base value( n ). Recive values( n , m ). Return boolean.
	_UBITS ,		// Up bitmask( n ) in a variable( v ). Recive variable( v ) and bitmask( n ).
	_DBITS ,		// Down bitmask( n ) in a variable( v ). Recive variable( v ) and bitmask( n ).
	_SBITS ,		// Flip bitmask( n ) in a variable( v ). Recive variable( v ) and bitmask( n ).
	_FUNCTION_LIST		// NUMBER OF MACROS LISTED
} ;
// Variable's definitions
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long custom_t;
//MACROS
// Math
#define BOOLEAN( n )		( !!( n ) )
#define ABS( n )		( ( n ) < 0 ? -( n ) : n )
#define ISNOTPAR( n )		( ABS( n ) & 1 )
#define BIGGER( n , m )		( ( ( n ) >= ( m ) ) ? n : m )
#define SMALLER( n , m )	( ( ( n ) <= ( m ) ) ? n : m )
#define MAXVALUE( e )		( ~( (custom_t)~0 << ( ( SMALLER( sizeof(custom_t) , sizeof( e ) ) ) * 8 - ( ( e )-1 < 0 ) - ( sizeof(custom_t) <= sizeof( e ) ) ) ) )
#define ROUNDUP( n , m )	( ABS( m ) * ( ABS( ( n ) / ( m ) ) + BOOLEAN( ( n ) % ( m ) ) ) )
// BitMasks
#define BIT( n )		( 1 << ABS( n ) )
#define ISBITUP( n , m )	( ( n ) & ( m ) )
#define ISMASKON( n , m )	( !( ISBITUP( n , m ) ^ ( m ) ) )
#define UBITS( v , n )		( v|= ( n ) )
#define DBITS( v , n )		( v&= ~( n ) )
#define SBITS( v , n )		( v^= ( n ) )
#define COUNTBIT( ev )		( sizeof( ev ) * 8 )
// ASCII
#define ESC			127
#define CAPS			32
#define CAPSFLAG		5
#define ISDIGIT( n )		( ( n ) <= '9' && ( n ) >= '0' )
#define ISALPHAU( n )		( ( n ) >= 'A' && ( n ) <= 'Z' )
#define ISALPHAL( n )		( ( n ) >= 'a' && ( n ) <= 'z' )
#define ISCONTROL( n )		( ( n ) < ' ' || ( n ) == ESC )
#define ISBLANK( n )		( ( n ) == ' ' || ( n ) == '\n' || ( n ) == '\t' || ( n ) == '\r' || ( n ) == '\r' || ( n ) == '\b' || ( n ) == '\v' || ( n ) == '\a' || ( n ) == '\f' )
#define TOUINT( x )		( ISDIGIT( x ) ? ( x ) - '0' : 0 )
#define UPPER( a )		( ISALPHAL( a ) ? DBITS( a , CAPS ) : a )
#define LOWER( a )		( ISALPHAU( a ) ? UBITS( a , CAPS ) : a )
// Variables manipulation
#define SWITCH( v )		( v= !( v ) )
#define SWAP( e , n , m )	if( sizeof( e ) == sizeof( n ) && sizeof( e ) == sizeof( m ) ) for( e s= ( n ) ; ( ( m ) != s ) ; n= ( m ) , m= s ) ; ;
// Arrays
#define COUNT( p )		( sizeof ( p ) / sizeof ( *p ) )
#define ERASE( p )		for ( uchar i = (uchar)COUNT( p ) ; i ; p[--i]= 0 ) ;
#define COPY( p , q )		for ( uchar copy_i= 0 , STOP= (uchar)COUNT( q ) ; ( copy_i < STOP ) && ( q[copy_i]= p[copy_i] ) ; copy_i++ )
// Structs
#define OFFSET( r , t )		( &( ( r * )0 )->t )
//FUNCTIONS
uint strtouint( uchar *string ){
	uchar i ;
	for ( i=0 ; ISDIGIT( *string ) ; string++ ) i= ( i*= 10 ) + TOUINT( *string ) ;
	return i ;
}
int strtoint( uchar *string ){
	uchar n ;
	int i= strtouint( &string[n= *string == '-'] ) ;
	return n ? -( i ) : i ;
}
uchar comparr( uchar *string_a , uchar *string_b , uint legth){
	uint i ;
	for ( i= 0 ;  i < legth && string_a[i] && string_b[i] && ( string_a[i] == string_b[i] ) ; i++ ) ;
	return string_a[i] == string_b[i] ;
}
