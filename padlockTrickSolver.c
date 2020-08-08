// ------------------------------------------------------------------
//
// - Code to resolve a trick
// - Coded by Anzollitto Francesco
//
// ------------------------------------------------------------------


#include "stdio.h"

#define	ROWS_DIGITS			4	// Number of rows
#define	COLUMNS_DIGITS			4	// Number of digits
#define	COLUMNS_ARRAY			6	// 4 digits for each digit + 2 (number of correct digit + number of correct position)
#define	COLUMN_CORRECT_DIGITS		4	// column to store the specified number of correct digits in each row.
#define	COLUMN_CORRECT_POS		5	// Column to store the correct specified position of digits in each row.

// tristate value of each digit
#define	DIGIT_UNDEFINED			2
#define	DIGIT_TRUE			1
#define	DIGIT_FALSE			0



// insert given data in the array (In the first 4 columns are stored the specified digits. 
//In the last 2 columns are stored the given hints for correct digits and correct positions

void fillDigitsTable(char digits[ROWS_DIGITS][COLUMNS_ARRAY], char stateDigitsTF[ROWS_DIGITS][COLUMNS_DIGITS], unsigned char padlock_Result[COLUMNS_DIGITS]) {
	
	
	char dgt[] = 	{2, 6, 5, 7, 2, 0,
			 0, 4, 1, 5, 1, 0,
			 4, 2, 6, 8, 0, 0,
			 1, 7, 4, 9, 2, 2};
	int pos = 0;
	for (int row=0;row<ROWS_DIGITS;row++) {
		for (int column=0;column<COLUMNS_ARRAY;column++) {
			digits[row][column]=dgt[pos];
			
			if (column<(COLUMNS_DIGITS)) {
				stateDigitsTF[row][column]= DIGIT_UNDEFINED;
			}
			pos++;
		}
	}
	
	for (int n=0;n<COLUMNS_DIGITS;n++) {
		padlock_Result[n]=0xFF;  // (__0xFF__) Chosen value to set undiscovered digit in the padlock result.
	}
	
}



// This function is called twice a each digit. Each digit is compared with all the ones in followes rows.
// (It is not invoke for last row digits as there isn't further row to be compared). 
// The first time (first step) it verify and store uncorrected digits. 
// On second time (second step), both, correct and  uncorrected digits.
void compareDigits(char step, int currentRow, int currentColumn, char digits[ROWS_DIGITS][COLUMNS_ARRAY], char stateDigitsTF[ROWS_DIGITS][(COLUMNS_DIGITS)] ) {
	
	char dgtCountCurrentRow=0;
	char boolStatoDigit;
	
	// each row is compared with the following rows. When it reaches the last one, it exits function.
	if (currentRow==(ROWS_DIGITS-1)) {
		return;
	}


			switch (step)
			{
				// firt step - Verify whether all digits in a rows are no correct.
				case 1:
				
				if (currentColumn==(COLUMNS_DIGITS-1) && digits[currentRow][COLUMN_CORRECT_DIGITS]==0) {
					for (int n=0;n<COLUMNS_DIGITS;n++) {
						stateDigitsTF [currentRow][n] = DIGIT_FALSE;
					}
					
				}

				for (int row=(currentRow+1);row<ROWS_DIGITS;row++) {
					for (int column=0;column<COLUMNS_DIGITS;column++) {
						if ((digits[row][COLUMN_CORRECT_DIGITS]==0) || (digits[currentRow][COLUMN_CORRECT_DIGITS]==0)) {
							if (digits[row][column]==digits[currentRow][currentColumn]) {
								stateDigitsTF [currentRow][currentColumn] = DIGIT_FALSE;
								stateDigitsTF [row][column] = DIGIT_FALSE;
							}
						}
					}
				}
				break;
				
				// second step
				case 2:
				
				if (stateDigitsTF[currentRow][currentColumn]==DIGIT_FALSE) {
				break;
				}
				
				for (int n=0;n<COLUMNS_DIGITS;n++) {
					if (stateDigitsTF [currentRow][n]==DIGIT_TRUE) {
						dgtCountCurrentRow++;
					}
				}
				
				if (digits[currentRow][COLUMN_CORRECT_DIGITS]>dgtCountCurrentRow) {
					boolStatoDigit = DIGIT_TRUE;
				} else {
					boolStatoDigit = DIGIT_FALSE;
					if (stateDigitsTF [currentRow][currentColumn]==DIGIT_UNDEFINED) {
						stateDigitsTF [currentRow][currentColumn] = DIGIT_FALSE;
					}
					}
				
				
				for (int row=(currentRow+1);row<(ROWS_DIGITS);row++) {
					for (int column=0;column<COLUMNS_DIGITS;column++) {
						if (digits[row][column]==digits[currentRow][currentColumn]) {
							stateDigitsTF [row][column] = boolStatoDigit;
							stateDigitsTF [currentRow][currentColumn] = boolStatoDigit;
						}
					}
				}
				break;
				
				default:
				break;
			}	// end_switch

}


// Third step - Check digits state that hasn't been still undefined. (_DIGIT_UNDEFINED_)
// For each row, it's verified whether the sum of digits with 'DIGIT_TRUE' current value match the given value hint,
// stored in the 'COLUMN_CORRECT_POS' column. If not, it set digits, whose state is 'DIGIT_UNDEFINE' as  'DIGIT_TRUE'.

void thirdStepAdjustment(char digits[ROWS_DIGITS][COLUMNS_ARRAY], char stateDigitsTF[ROWS_DIGITS][COLUMNS_DIGITS]) {
	for (int row=0;row<(ROWS_DIGITS);row++) {
		int totCorrect=0;
		for (int column=0;column<COLUMNS_DIGITS;column++) {
			if (stateDigitsTF[row][column]==DIGIT_TRUE) {
				totCorrect++;
			}
		}
		if (digits[row][COLUMN_CORRECT_POS]>totCorrect) {
			for (int column=0;column<COLUMNS_DIGITS;column++) {
				if (stateDigitsTF[row][column]==DIGIT_UNDEFINED) {
					if (digits[row][COLUMN_CORRECT_POS]>totCorrect) {
						stateDigitsTF[row][column]=DIGIT_TRUE;
						totCorrect++;
					} else {
						stateDigitsTF[row][column]=DIGIT_FALSE;
						}
				}
			}
		}
	}
}



// Fourth step - This function is called to get the correct position of digits and, finally, to get the result.

void findRightPosDigits(char digits[ROWS_DIGITS][COLUMNS_ARRAY], char stateDigitsTF[ROWS_DIGITS][COLUMNS_DIGITS], unsigned char padlock_Result[4]) {
	
	char boolNotAllFound= 0;
	char missedDgt=0;
	unsigned char nextPos= 0xFF; // next position in result digits.
	char boolisNotInsDigit09[10] = {1,1,1,1,1,1,1,1,1,1};
	
	for (int row=0;row<(ROWS_DIGITS);row++) {
		if (digits[row][COLUMN_CORRECT_POS]>0) {
			for (int column=0;column<COLUMNS_DIGITS;column++) {
				if (stateDigitsTF[row][column]==DIGIT_TRUE) {
					padlock_Result[column]=digits[row][column];
				}
			}
		}
	}


	for (int n=0; n<(ROWS_DIGITS); n++) {
		if (padlock_Result[n]==0xFF) {
			boolNotAllFound=1;
			missedDgt++; 
		}
	}
	
	if (boolNotAllFound) {
		for (int row=0;row<(ROWS_DIGITS);row++) {
			for (int column=0;column<COLUMNS_DIGITS;column++) {
				if (stateDigitsTF[row][column]==DIGIT_TRUE) {
					
					char boolAlreadyInserted = 0;
					
					for (int n=0;n<COLUMNS_DIGITS; n++) {
						if ((padlock_Result[n]==0xFF) && (nextPos==0xFF)) {
							nextPos = n;
						} else if (padlock_Result[n]==digits[row][column]) {
							boolAlreadyInserted++;
							}
					}
					if (!boolAlreadyInserted) {
						padlock_Result[nextPos]= digits[row][column];
						missedDgt--;
						nextPos = 0xFF;
						boolAlreadyInserted=0;
					}
					
				}
				// In order to find which digits (from 0 to 9) is not used, all read digits from the table are indicated in the relative index.
				boolisNotInsDigit09[((int)digits[row][column])] = 0;
			}
		}
	}
	
	// ---------
	
	for (int n=0; n<10; n++) {
		if ((boolisNotInsDigit09[n]) && nextPos!=0xFF) {
			padlock_Result[nextPos] = n;
			missedDgt--;
			
			if (!missedDgt) break;
			nextPos=0xFF;
			for (int c = 0; c<COLUMNS_DIGITS; c++) {
				if (padlock_Result[c]!=0xFF) {
					nextPos= c;
					break;
				}
			}
			if (nextPos== 0xFF) break;
		}
	}
	
	printf("List of digits inserted/not inserted in the table:\n");
	for (int n=0; n<10; n++) {
		printf("\n digit %d: %s ", n, (boolisNotInsDigit09[n]) ? "Not inserted":"Inserted");
		}
		printf("\n");

}



int main(void) {
	
	char digits [ROWS_DIGITS][COLUMNS_ARRAY];
	char stateDigitsTF [ROWS_DIGITS][COLUMNS_DIGITS];
	unsigned char padlock_Result[COLUMNS_DIGITS];
	
	fillDigitsTable(digits, stateDigitsTF, padlock_Result); // insert given values
	
	// First compared step
	for (int row=0;row<ROWS_DIGITS;row++) {
		for (int column=0;column<COLUMNS_DIGITS;column++) {
			compareDigits(1, row, column, digits, stateDigitsTF);
		}
	}

	// Second compared step
	for (int row=0;row<ROWS_DIGITS;row++) {
		for (int column=0;column<COLUMNS_DIGITS;column++) {
			compareDigits(2, row, column, digits, stateDigitsTF);
		}
	}

	// Third step
	thirdStepAdjustment(digits, stateDigitsTF);

	// Fourth step - find right positioned digits
	findRightPosDigits(digits, stateDigitsTF, padlock_Result);


	// ------ print result -------
	// -
	printf("\n\n\nResulting schema of correct/incorrect digits (C = correct; U = uncorrected\n\n");
	for (int row=0;row<ROWS_DIGITS;row++) {
		for (int column=0;column<COLUMNS_DIGITS;column++) {
			printf("%s - ", (stateDigitsTF[row][column]) ? "C": "U");
		}
		printf("\n");
	}
	printf("\n\n");
	
	// -------
	printf("table of given digits\n\n");
	for (int row=0;row<ROWS_DIGITS;row++) {
		for (int column=0;column<COLUMNS_DIGITS;column++) {
			printf("- %d ", digits[row][column]);
		}
		printf("\n");
	}
		printf("\n\n\nPADLOCK TRICK RESULT: ");
	for (int n=0;n<4;n++) {
		 printf("- %d ", padlock_Result[n]);
	}

	return 0;
}
