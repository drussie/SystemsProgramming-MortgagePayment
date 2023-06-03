/*	Student Name: Marcos Ondruska
 *	Student ID: 2685885
 *	
 *	I affirm that I wrote this program without any help from any other
 *	people or sources from the internet.
 *
 *	Program description: This program will compute your monthly mortgage payment for a 
 *	30 or 15 year mortgage given an interest rate, a downpayment, and a price. 
	
   
   	example of command line parsing via mortgagepmt
	usage: mortgagepmt [-ds] -r rate [-p downpayment] homeprice 

	Marcos Ondruska
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>

int debug = 0;

int main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int sflag=0, rflag=0, pflag = 0;
	char *downpayment = "default_downpayment", *rate;
	static char usage[] = "usage: %s [-ds] -r rate [-p downpayment] homeprice\n";
	double price = 0.00;  /* double for homeprice */
	double rateDouble = 0.000; /*double to check if rate complies within 3 and 10 percent */
	double monthlyRate = 0.000; /* monthly interest rate */
	double numberOfMonthlyPayments = 0.00;	/* total amount of monthly payments to be made during term */ 
	double payment = 0.00; /* Initializing monthly payment amount and setting to zero */
	double loanAmount = 0.00; /*Initializing loanAmount */
	double downPaymentDouble = 0.00; /* used to convert downpayment string to double */
	double pmi = 0.00; /* initializing pmi option */

	while ((c = getopt(argc, argv, "dsr:p:")) != -1) /* options for debug, 30/15y, rate, and price */
		switch (c) {
		case 'd':  /* debug */
			debug = 1;
			break;
		case 's': /* if selected 15y option is used */ 
            	        sflag = 1;
			break;
		case 'r':  /* rate */
			rflag = 1;
			rate = optarg;
			rateDouble = atof(rate); /* converting string rate to float rateDouble */ 
			if ((rateDouble < 3) || (rateDouble > 10)) { /* making sure rate is between (including) 3 and 10 percent */  
				printf("rate(-r) must be between (including)  3%% and 10%%\n");
				exit(1);
			}
			break;
		case 'p': /* downpyment */
			pflag = 1;
			downpayment = optarg;
			break;
		case '?':
			err = 1;
			break;
		}
	if (rflag == 0) {	/* -r (rate) was mandatory, print error message if missing */
		fprintf(stderr, "%s: missing -r option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} else if ((optind+1) > argc) {	/* Using one argument to read the price for the house */
		printf("optind = %d, argc=%d\n", optind, argc);
		fprintf(stderr, "%s: missing value\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} else if (err) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}

	downPaymentDouble = atof(downpayment); /* Converting string downpayment to float */ 
	
	if (optind < argc) {	/* this is the argument after the command-line options */ 
		for (;optind < argc; optind++) {
			price = atof(argv[optind]); /* Convert Argument string to float */
			loanAmount = (price - downPaymentDouble); 

			if (!(sflag == 1)) { /*Base case is 30y mortgage */
			numberOfMonthlyPayments = 30 * 12; /* 30 year mortgage in months */ 
			}
			else { /*otherwise if -s is selected a 15y mortgage */
			numberOfMonthlyPayments = 15 * 12; /* 15 year mortgage in months */
			}

			if ((price / downPaymentDouble) > 5) { /* if downpayment is less than 20 percent pmi is enabled*/
			pmi = ((loanAmount / 12) * 0.01);
			}

			if (price < downPaymentDouble) { /* downpayment cannot be larger than homeprice */
			printf("Downpayment cannot be larger than the price of your home.\n");
		        exit(1);	
			}	

			/* calculations as per instructions to compute mortgage payment */
			monthlyRate = (rateDouble / 12.0)+ 1 ;		
			payment = pow(monthlyRate, -numberOfMonthlyPayments);
			payment = 1 - payment;
			payment = (monthlyRate) / payment;
			payment = (payment * (loanAmount)) / numberOfMonthlyPayments;
			payment = payment + pmi;
			setlocale(LC_NUMERIC, ""); /* format dollar amounts so that 1000s columns are formatted with commas */
			printf("The payment on a loan of $%'.2f with an interest rate of %.3f%% for a term of %.0f years will be $%'.2f\n",
				       	loanAmount, rateDouble, numberOfMonthlyPayments / 12, payment); 

			
		}
	}

	else {
		printf("no arguments left to process\n");
	}
	
	exit(0);
}
