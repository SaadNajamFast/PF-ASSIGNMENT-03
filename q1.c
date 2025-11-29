#include <stdio.h>

double calculateRepayment(double loan, double interestRate, int years) {

    if (loan <= 0 || years == 0) {
        return 0;
    }

    double installment = 20000;


    loan = loan + (loan * interestRate);


    loan = loan - installment;


    printf("Year %d: Remaining loan = %.2f\n", 4 - years, loan);

    return installment + calculateRepayment(loan, interestRate, years - 1);
}

int main() {

    double loan = 100000;
    double interestRate = 0.05;
    int years = 3;

    printf("Loan Repayment Schedule:\n");
    double total = calculateRepayment(loan, interestRate, years);

    printf("\nTotal repayment in %d years = %.2f\n", years, total);

    return 0;
}
