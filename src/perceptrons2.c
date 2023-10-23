#include <stdio.h>
#include <string.h>

#define THRESHOLD 1.5
#define INPUT_COUNT 5

char * get_question(int i, float *wieght){

    switch (i) {
        case 0:
            *wieght = 0.8;
            return "\tDo you like computers?";
            break;
        case 1:
            *wieght = 0.7;
            return "\tDo you like programming?";
            break;
        case 2:
            *wieght = 0.5;
            return "\tAre you dedicated?";
            break;
        case 3:
            *wieght = 0.4;
            return "\tAre you willing to give up your social life?";
            break;
        case 4:
            *wieght = 0.7;
            return "\tCan you copy/paste?";
            break;
        default:
            return "ERROR";
            break;
    }

    return NULL;
}

int set_input(char input_buff, float *input) {

    if (input_buff == 'y' || input_buff == 'Y') {
        *input = 1.0;
    }
    else if (input_buff == 'n' || input_buff == 'N') {
        *input = 0.0;
    }
    else {
        return -1;
    }
    return 0;
}

void output(float p, float p_max) {

    if (p > THRESHOLD) {
        printf("\n\t\033[1;32mYES: Learn C\033[0m");
    } else {
        printf("\n\t\033[1;31mNO: Learn Javascript\033[0m");
    }


    printf("\nP = %f", p);
    printf("\n%%P = %f%%", p / p_max * 100);
}


int main() {

    printf("Perceptron: Answer y/n\n");

    char input_buff;

    float x[INPUT_COUNT];
    float w[INPUT_COUNT];

    float p = 0.0;
    float p_max = 0.0;

    for(int i = 0; i < INPUT_COUNT; i++){
        char *str = get_question(i, &w[i]);
        
        if (!strcmp("ERROR", str)) {
            fprintf(stderr, "ERROR: No qeustion defined. Check Inputs.");
            return 1;
        }
        printf("%s", str);

        scanf(" %c", &input_buff);


        while (getchar() != '\n');


        if (set_input(input_buff, &x[i]) != 0) {
            fprintf(stderr, "ERROR: Invalid input");
            return 1;
        }

        p_max += 1 * w[i];
        p += x[i] * w[i];
    }

    output(p, p_max);

    return 0;
}
