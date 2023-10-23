#include <stdio.h>

int main() {



    printf("\nNew Job AI\n--------------");

    float p;
    float threshold = 1.5;

    float input;

    //Better salary 
    printf("\nDoes the job have a better salary? (0 false, 1 true) :");
    scanf("%f", &input);
    float x1 = input;
    float w1 = 0.8;
    
    //Travel Time
    printf("\nIs the travel time better? (0 false, 1 true) :");
    scanf("%f", &input);
    float x2 = input;
    float w2 = 0.4;

    //More Interesting
    printf("\nIs the job more interesting? (0 false, 1 true) :");
    scanf("%f", &input);
    float x3 = input;
    float w3 = 0.6;

    //Better Prospects
    printf("\nDoes the job have bette prospects? (0 false, 1 true) :");
    scanf("%f", &input);
    float x4 = input;
    float w4 = 0.7;

    //Disatisfied
    printf("\nAre you unhappy in your current job? (0 false, 1 true) :");
    scanf("%f", &input);
    float x5 = input;
    float w5 = 0.8;

    p = (x1 * w1) + (x2 * w2) + (x3 * w3)  + (x4 * w4) + (x5 * w5); 


    if (p > threshold) {
        printf("\nYES: You should apply for the new job.");
    } else {
        printf("\nNO: You should not apply for the new job.");
    }



    return 0;
}
