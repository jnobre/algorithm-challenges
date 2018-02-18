package pt.hackerrank.bigdecimal;
import java.util.*;
import java.math.*;

public class Solution {
	/** Enter your code here. Read input from STDIN. Print output to STDOUT. 
	 * Your class should be named Solution. **/
	 public static void main(String[] args) {
		/* Enter your code here. Read input from STDIN. Print output to STDOUT. Your class should be named Solution. */
		
		//Input
		Scanner userInput = new Scanner(System.in);
		int arraySize = Integer.parseInt(userInput.nextLine());
		String [ ] array = new String[arraySize];
		for (int i = 0; i < arraySize; i++) {
		    array[ i ] = userInput.nextLine();
		}
		
		//Sorting (Selection)
		for( int i = 0 ; i < ( array.length - 1 ) ; i++ ) {
			for( int k = ( i + 1 ) ; k < array.length ; k++ ) {
				if( new BigDecimal( array[ i ] ).compareTo( new BigDecimal( array[ k ] ) ) < 0 ) {
					String tmpValue = array[ i ];
					array[ i ] = array[ k ];
					array[ k ] = tmpValue;
				}
			}
		}
		
		for( int i = 0 ; i < array.length ; i++ )
			System.out.println( array[ i ] );
		
	}
}
