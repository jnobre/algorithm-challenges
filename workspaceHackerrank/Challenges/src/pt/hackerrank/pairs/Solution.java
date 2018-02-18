package pr.hackerrank.pairs;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;

/**
*https://www.hackerrank.com/challenges/pairs/problem solution
**/
public class Solution {

    /**
	 * Function which creates a an integer array from string consisting of
	 * integers separated by spaces. Throws a {@link IllegalArgumentException}
	 * if the number of integers present in the string is not equal to the size
	 * expected
	 * 
	 * @param elements
	 *            : String containing integers separated by a space
	 * @param size
	 *            : the expected number of integers in the string supplied
	 * @return : an int array which contains size number of integers
	 */
	private static int[] getElements(String elements, int size) {

		String[] elementsArray = elements.trim().split(" ");

		if (elementsArray.length != size)
			throw new IllegalArgumentException("Number of elements in the list does not match the size provided");

		int[] numbers = new int[size];
		int index = 0;

		// extract integers
		for (String ele : elementsArray) {
			numbers[index++] = Integer.parseInt(ele);
		}
		return numbers;
	}
    
    /**
	 * Function to print the number of pairs of elements in a non-decreasing
	 * array which have k difference in their value
	 * 
	 * @param array
	 *            : a non-decreasign array
	 * @param arraySize
	 *            : size of the array
	 * @param k
	 *            : the difference
	 * @return : total numbers of pairs which have k difference between them
	 */
	private static int countAtKDiff(int[] array, int arraySize, int k) {

		int count = 0; // initialize the counter

		for (int i = 0, j = 1; i < arraySize && j < arraySize;) {
			if (array[j] - array[i] == k) { // found a pair
				count++;
				i++;
				j++;
			} else if (array[j] - array[i] < k) { // difference is less than wanted so increment right pointer
				j++;
			} else { // difference is more than wanted so increment the left pointer 
				i++;
			}
		}

		return count;
	}
    
    
    public static void main(String[] args) {
        BufferedReader input = new BufferedReader(new InputStreamReader(System.in));

		try {
			String[] line1 = input.readLine().trim().split(" ");
			int listSize = Integer.parseInt(line1[0]);
			int k = Integer.parseInt(line1[1]);
			int[] list = getElements(input.readLine(), listSize);

			// sort this  array
			Arrays.sort(list);

			// print the count of pairs which are at k distance
			System.out.println(countAtKDiff(list, listSize, k));

			input.close();
		} catch (NumberFormatException e) {
			System.err.println("Caught NumberFormatException: Error in coverting to interger");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("Caught IOException: Error in reading input from stdin");
			e.printStackTrace();
		}
    }
	 

}