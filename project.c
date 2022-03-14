#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct node;
typedef struct node *ptr;
typedef ptr list;
typedef ptr pos;

/* Structure to represent the doubly linked list */
struct node {
    ptr previous;
    double coefficient;
    double power;
    ptr next;
};

/* A variable that stores the number of equations we will read */
int EquationsNo;

/* Explanation of all functions under the main function */

void printMenu();

void replaceMinus(char equation[]);

void removeSpaces(char equ[]);

void insert(list l, double coe, double p);

void showEquations(list header);

list createList(list l);

pos addition(list l1, list l2);

pos addition_Many(list l[], int sizeOfArray);

pos subtraction(list l1, list l2);

pos subtraction_Many(list l[], int sizeOfArray);

pos multiplication(list l1, list l2);

pos multiplication_Many(list l[], int sizeOfArray);

double substitute(list l, double value);

void writeOnFile(FILE *write, list header[], int sizeOfArray);

int main() {
    EquationsNo = 0;
    FILE *file, *write;
    list header[20];
    int choice;
    double value = 0, subs;
    printMenu();
    scanf("%d", &choice);
    while (choice != 7) {
        switch (choice) {
            case 1:
                file = fopen("C:\\Users\\HP\\CLionProjects\\untitled2\\equations.txt", "r");
                char equations[20][100];
                char terms[20][15][20];
                char *i;
                int c;
                int flag;
                do {
                    i = fgets(equations[EquationsNo], 100, file);
                    if (i != NULL) {
                        replaceMinus(equations[EquationsNo]);
                        removeSpaces(equations[EquationsNo]);
                        c = 0;
                        char *p = strtok(equations[EquationsNo], "+\n");
                        while (p != NULL) {
                            strcpy(terms[EquationsNo][c], p);
                            c++;
                            p = strtok(NULL, "+\n");
                        }
                        terms[EquationsNo][c][0] = '\0';
                        EquationsNo++;
                    }
                } while (i != NULL);
                //store in linked list
                for (int k = 0; k < EquationsNo; k++) {
                    header[k] = createList(header[k]);
                }
                for (int k = 0; k < EquationsNo; ++k) {
                    for (int l = 0; l < sizeof(terms[k]) / sizeof(*terms[k]) && terms[k][l][0] != '\0'; ++l) {
                        flag = 0;
                        for (int m = 0; m < strlen(terms[k][l]); ++m) {
                            if (terms[k][l][m] == 'x') {
                                flag++;
                            }
                            if (terms[k][l][m] == '^') {
                                flag++;
                            }
                        }
                        if (flag == 1) {
                            char *remain1;
                            double co = strtod(terms[k][l], &remain1);
                            if (co == 0) {
                                if (terms[k][l][0] == '-') {
                                    co = -1;
                                } else {
                                    co = 1;
                                }
                            }
                            double ex = 1;
                            insert(header[k], co, ex);
                        } else if (flag == 2) {
                            char *remain1;
                            double co = strtod(terms[k][l], &remain1);
                            if (co == 0) {
                                if (terms[k][l][0] == '-') {
                                    co = -1;
                                } else {
                                    co = 1;
                                }
                            }
                            char *pointer = strtok(remain1, "-x^");
                            double ex = strtod(pointer, &remain1);
                            insert(header[k], co, ex);
                        } else {
                            char *remain1;
                            double co = strtod(terms[k][l], &remain1);
                            double ex = 0;
                            insert(header[k], co, ex);
                        }
                    }
                }
                fclose(file);
                printf("The file was read successfully\n");
                break;
            case 2:
                if (EquationsNo != 0) {
                    printf("The Equations:\n");
                    for (int j = 0; j < EquationsNo; ++j) {
                        showEquations(header[j]);
                    }
                } else
                    printf("You must read the equations first!\n");
                break;
            case 3:
                if (EquationsNo != 0) {
                    printf("The result of the addition:\n");
                    list result = addition_Many(header, EquationsNo);
                    showEquations(result);
                    printf("Please enter a value to substitute in the equation:\n");
                    scanf("%lf", &value);
                    subs = substitute(result, value);
                    printf("The answer is: %.0lf ^-^\n", subs);
                } else
                    printf("You must read the equations first!\n");
                break;
            case 4:
                if (EquationsNo != 0) {
                    printf("The result of the subtraction:\n");
                    list result = subtraction_Many(header, EquationsNo);
                    showEquations(result);
                    printf("Please enter a value to substitute in the equation:\n");
                    scanf("%lf", &value);
                    printf("The answer is: %.0lf ^-^\n", substitute(result, value));
                } else
                    printf("You must read the equations first!\n");
                break;
            case 5:
                if (EquationsNo != 0) {
                    printf("The result of the multiplication:\n");
                    list result = multiplication_Many(header, EquationsNo);
                    showEquations(result);
                    printf("Please enter a value to substitute in the equation:\n");
                    scanf("%lf", &value);
                    printf("The answer is: %.0lf ^-^\n", substitute(result, value));
                } else
                    printf("You must read the equations first!\n");
                break;
            case 6:
                if (EquationsNo != 0) {
                    write = fopen("C:\\Users\\HP\\CLionProjects\\untitled2\\results.txt", "w");
                    writeOnFile(write, header, EquationsNo);
                    printf("DONE!\n");
                    fclose(write);
                } else
                    printf("You must read the equations first!\n");
                break;
            default:
                printf("Invalid value!\n");
        }
        printMenu();
        scanf("%d", &choice);
    }
    return 0;
}

/* Displaying the menu on the screen */
void printMenu() {
    printf("****************************************************\n");
    printf("Please select an option:\n");
    printf("1- Read File\n");
    printf("2- Show Equations\n");
    printf("3- Perform Addition\n");
    printf("4- Perform Subtraction\n");
    printf("5- Perform multiplication\n");
    printf("6- Save Results in \"result.txt\"\n");
    printf("7- Exit\n");
    printf("****************************************************\n");
}

/* Removing blank spaces from the word */
void removeSpaces(char equ[]) {
    char newEqu[strlen(equ)];
    int i, j;
    /* Go on all the characters in the string and
     * add them to the new string, unless there
     * are spaces */
    for (i = 0, j = 0; i < strlen(equ); i++) {
        if (equ[i] != ' ') {
            newEqu[j] = equ[i];
            j++;
        }
    }
    newEqu[j] = '\0';
    strcpy(equ, newEqu);
}

/* Replacing all '-' in the string by "+-" to make it easy for trim the string later */
void replaceMinus(char equation[]) {
    int counter = 0;
    /* Count the number of '-' character in the equation */
    for (int i = 0; i < strlen(equation); i++) {
        if (equation[i] == '-') {
            counter++;
        }
    }
    /* The length of the new equation equals the length of
     * the primary equation in addition to the length of '+'
     * that we will add to the equation */
    char newEquation[strlen(equation) + (counter * (strlen("+-") - strlen("-")))];
    int i, j;
    //here
    for (i = 0, j = 0; j < strlen(equation); i++, j++) {
        if (equation[j] == '-') {
            newEquation[i] = '+';
            i++;
            newEquation[i] = '-';
        } else {
            newEquation[i] = equation[j];
        }
    }
    newEquation[i] = '\0';
    strcpy(equation, newEquation);
}

/* Inserts new items at the end of doubly linked list */
void insert(list l, double coe, double p) {
    ptr tmp, pointer;
    //creating the node we want to insert
    tmp = malloc(sizeof(struct node));
    if (tmp == NULL)
        exit(0);
    tmp->previous = NULL;
    tmp->coefficient = coe;
    tmp->power = p;
    tmp->next = NULL;
    pointer = l;
    while (pointer->next != NULL)
        pointer = pointer->next;
    pointer->next = tmp;
    tmp->previous = pointer;
}

/* Display the equations to the user in a tidy way */
void showEquations(list header) {
    pos temp = header->next;
    while (temp != NULL) {
        if (temp->power == 0)
            printf("%.0lf", temp->coefficient);
        else if (temp->power == 1) {
            if (temp->coefficient == 1)
                printf("x");
            else if (temp->coefficient == -1)
                printf("-x");
            else
                printf("%.0lfx", temp->coefficient);
        } else {
            if (temp->coefficient == 1)
                printf("x^%.0lf", temp->power);
            else if (temp->coefficient == -1)
                printf("-x^%.0lf", temp->power);
            else
                printf("%.0lfx^%.0lf", temp->coefficient, temp->power);
        }
        temp = temp->next;
        if (temp != NULL)
            printf(" + ");
    }
    printf("\n");
}

/* Create an empty linked list and make it pointing at NULL */
list createList(list l) {
    l = malloc(sizeof(struct node));
    if (l == NULL)
        exit(0);
    l->next = NULL;
    l->power = 0;
    l->coefficient = 0;
    return l;
}

/* Performs the addition operation for two equations and return the result equation */
pos addition(list l1, list l2) {
    double co, ex;
    list temp1 = l1->next;
    list temp2 = l2->next;
    list result = NULL;
    result = createList(result);
    while (temp1 != NULL && temp2 != NULL) {
        if (temp1->power == temp2->power) {
            co = temp1->coefficient + temp2->coefficient;
            ex = temp1->power;
            insert(result, co, ex);
            temp1 = temp1->next;
            temp2 = l2->next;
        } else {
            if (temp2->next != NULL) {
                temp2 = temp2->next;
            } else {
                insert(result, temp1->coefficient, temp1->power);
                temp1 = temp1->next;
                temp2 = l2->next;
            }
        }
    }
    temp2 = l2->next;
    temp1 = result->next;
    while (temp2 != NULL && temp1 != NULL) {
        if (temp2->power == temp1->power) {
            temp1 = result->next;
            temp2 = temp2->next;
            if (temp2 == NULL)
                break;
        } else {
            if (temp1->next != NULL) {
                temp1 = temp1->next;
            } else {
                insert(result, temp2->coefficient, temp2->power);
                temp2 = temp2->next;
                temp1 = result->next;
            }
        }
    }
    temp2 = result->next;
    while (temp2 != NULL) {
        if (temp2->coefficient == 0) {
            temp1 = temp2;
            temp2 = temp2->next;
            (temp1->previous)->next = temp1->next;
            if (temp1->next != NULL)
                (temp1->next)->previous = temp1->previous;
            free(temp1);
        } else
            temp2 = temp2->next;
    }
    return result;
}

/* Performs the addition operation for many equations and return the result equation */
pos addition_Many(list l[], int sizeOfArray) {
    list result = addition(l[0], l[1]);
    for (int j = 2; j < sizeOfArray; ++j) {
        result = addition(result, l[j]);
    }
    return result;
}

/* Performs the subtraction operation for two equations and return the result equation */
pos subtraction(list l1, list l2) {
    double co, ex;
    list temp1 = l1->next;
    list temp2 = l2->next;
    list result = NULL;
    result = createList(result);
    while (temp1 != NULL && temp2 != NULL) {
        if (temp1->power == temp2->power) {
            co = temp1->coefficient - temp2->coefficient;
            ex = temp1->power;
            insert(result, co, ex);
            temp1 = temp1->next;
            temp2 = l2->next;
        } else {
            if (temp2->next != NULL) {//true
                temp2 = temp2->next;//
            } else {
                insert(result, temp1->coefficient, temp1->power);
                temp1 = temp1->next;
                temp2 = l2->next;
            }
        }
    }
    temp2 = l2->next;
    temp1 = result->next;
    while (temp2 != NULL && temp1 != NULL) {
        if (temp2->power == temp1->power) {
            temp1 = result->next;
            temp2 = temp2->next;
            if (temp2 == NULL)
                break;
        } else {
            if (temp1->next != NULL) {
                temp1 = temp1->next;
            } else {
                insert(result, -1 * temp2->coefficient, temp2->power);
                temp2 = temp2->next;
                temp1 = result->next;
            }
        }
    }
    temp2 = result->next;
    while (temp2 != NULL) {
        if (temp2->coefficient == 0) {
            temp1 = temp2;
            temp2 = temp2->next;
            (temp1->previous)->next = temp1->next;
            if (temp1->next != NULL)
                (temp1->next)->previous = temp1->previous;
            free(temp1);
        } else
            temp2 = temp2->next;
    }
    return result;
}

/* Performs the subtraction operation for many equations and return the result equation */
pos subtraction_Many(list l[], int sizeOfArray) {
    list result = subtraction(l[0], l[1]);
    for (int j = 2; j < sizeOfArray; ++j) {
        result = subtraction(result, l[j]);
    }
    return result;
}

/* Performs the multiplication operation for two equations and return the result equation */
pos multiplication(list l1, list l2) {
    double co, ex;
    list temp1 = l1->next;
    list temp2 = l2->next;
    pos delete;
    list result = NULL;
    result = createList(result);
    while (temp1 != NULL) {
        while (temp2 != NULL) {
            co = temp1->coefficient * temp2->coefficient;
            ex = temp1->power + temp2->power;
            insert(result, co, ex);
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
        temp2 = l2->next;
    }
    temp1 = result->next;
    while (temp1 != NULL) {
        temp2 = temp1->next;
        while (temp2 != NULL) {
            if (temp1->power == temp2->power) {
                temp1->coefficient += temp2->coefficient;
                delete = temp2;
                temp2 = temp2->next;
                (delete->previous)->next = delete->next;
                if (delete->next != NULL)
                    (delete->next)->previous = delete->previous;
                free(delete);
            } else {
                temp2 = temp2->next;
            }
        }
        temp1 = temp1->next;
    }
    return result;
}

/* Performs the multiplication operation for many equations and return the result equation */
pos multiplication_Many(list l[], int sizeOfArray) {
    list result = multiplication(l[0], l[1]);
    for (int j = 2; j < sizeOfArray; ++j) {
        result = multiplication(result, l[j]);
    }
    return result;
}

/* Substitute the value from the user in the equation */
double substitute(list l, double value) {
    list temp = l->next;
    double total = 0;
    while (temp != NULL) {
            total += temp->coefficient * pow(value, temp->power);
        temp = temp->next;
    }
    return total;
}

/* Save the linked list's element in a text file (print the equations in a tidy way on file) */
void writeOnFile(FILE *write, list header[], int sizeOfArray) {
    list result;
    for (int j = 0; j < 3; ++j) {
        if (j == 0) {
            fprintf(write, "The result of the Addition:\n");
            result = addition_Many(header, sizeOfArray);
        }
        if (j == 1) {
            fprintf(write, "The result of the Subtraction:\n");
            result = subtraction_Many(header, sizeOfArray);
        }
        if (j == 2) {
            fprintf(write, "The result of the Multiplication:\n");
            result = multiplication_Many(header, sizeOfArray);
        }
        pos temp = result->next;
        while (temp != NULL) {
            if (temp->power == 0)
                fprintf(write, "%.0lf", temp->coefficient);
            else if (temp->power == 1) {
                if (temp->coefficient == 1)
                    fprintf(write, "x");
                else if (temp->coefficient == -1)
                    fprintf(write, "-x");
                else
                    fprintf(write, "%.0lfx", temp->coefficient);
            } else {
                if (temp->coefficient == 1)
                    fprintf(write, "x^%.0lf", temp->power);
                else if (temp->coefficient == -1)
                    fprintf(write, "-x^%.0lf", temp->power);
                else
                    fprintf(write, "%.0lfx^%.0lf", temp->coefficient, temp->power);
            }
            temp = temp->next;
            if (temp != NULL)
                fprintf(write, " + ");
        }
        fprintf(write, "\n");
    }
}
