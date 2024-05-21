#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_SIZE 20

typedef struct Node {
    char * data;
    struct Node* next;
} Node;

typedef struct {
    Node* front; 
    Node* rear; 
} queue_t;

typedef struct{
    Node* top;
} stack_t;

typedef struct stack{
    double data;
    struct stack * next;
} rpnStack;

void replace_char(char *str, const char *replace);
int isOperator(char* str);
int isFunction(char *str, int *len);
Node* createNode(const char * data);
int stackEmpty(stack_t* stack);
void push(stack_t* stack, const char *data);
char * pop(stack_t* stack);
char * peek(stack_t * stack);
int queueEmpty(queue_t* queue);
void enqueue(queue_t* queue, const char * data);
char * dequeue(queue_t* queue);
void rpn_push(rpnStack** stack, double value);
double rpn_pop(rpnStack** stack);
void displayQueue(queue_t* queue);
void displayStack(stack_t* stack);
int precedence(char * op);
void  shunting(char * equation, stack_t * stack, queue_t * queue);
double rpn(queue_t * queue, rpnStack * stack, double x);
void bisection();
void regula_falsi();
void newton_raphson();
double derivative(char * expression, double x, double h);
double derivativeF(char * expression, double x, double h);
double derivativeB(char * expression, double x, double h);
void simpson();
void trapzoid();
void interpolation();
void mat_inverse();
void gaussian_elimination();
void gauss_seidal();


int main() {
    char choice[10] = "1";
    printf("HELLO WELCOME TO BABAKS MATH CALCULATOR!\n"); 
    while(strcmp(choice, "0") != 0) {
        printf("-----------------------------------------\n");
        printf("Methods:\n1. Bisection method\n2. Regula-Falsi method\n3. Newton-Raphson method\n4. Inverse of NxN matrix\n5. Gaussian Elimination\n6. Gauss-Seidel method\n7. Numerical derivative (center)\n8. Simpson method\n9. Trapezoidal method\n10. Gregory-Newton Interpolation\n");
        printf("-----------------------------------------\n");
        printf("Which method you want to use:");
        scanf("%s", choice);
        if (choice[strlen(choice) - 1] == '\n') {
            choice[strlen(choice) - 1] = '\0';
        }
        double h;
        char expression[MAX_SIZE];
        double x;
        double result;
        if (strcmp(choice, "1") == 0) {
            printf("BISECTION method selected.\n");
            bisection();
        } else if (strcmp(choice, "2") == 0) {
            printf("REGULA FALSI method selected.\n");
            regula_falsi();
        } else if (strcmp(choice, "3") == 0) {
            printf("NEWTON RAPHSON method selected.\n");
            newton_raphson();
        } else if (strcmp(choice, "4") == 0) {
            printf("MAT INVERSE method selected.\n");
            mat_inverse();
        } else if (strcmp(choice, "5") == 0) {
            printf("GAUSSIAN ELIMINATION method selected.\n");
            gaussian_elimination();
        } else if (strcmp(choice, "6") == 0) {
            printf("GAUSS SEIDAL method selected.\n");
            gauss_seidal();
        } else if (strcmp(choice, "7") == 0) {
            printf("DERIVATIVE(CENTER) method selected.\n");
            printf("please enter your expression:"); scanf("%s", &expression);
            printf("please enter precesion(h):"); scanf("%lf", &h);
            printf("please enter x value"); scanf("%lf", &x);
            result = derivativeF(expression, x, h);
            printf("forward derivative is %lf\n", result);
            result = derivativeB(expression, x, h);
            printf("backward derivative is %lf\n", result);
            result = derivative(expression, x, h);
            printf("center derivative is %lf\n", result);
        } else if (strcmp(choice, "8") == 0) {
            printf("SIMPSON method selected.\n");
            simpson();
        } else if (strcmp(choice, "9") == 0) {
            printf("TRAPZOID method selected.\n");
            trapzoid();
        } else if (strcmp(choice, "10") == 0) {
            printf("interpolation() method selected.\n");
            interpolation();
        }
        printf("enter any button to continue | enter 0 to quit:"); scanf("%s", choice);
    }
}



void replace_char(char *str, const char *replace) {
    char buffer[MAX_SIZE]; 
    while (*str) {
        if (*str == 'x') {
            strcpy(buffer, replace);
            strcat(buffer, str + 1); 
            strcpy(str, buffer);
        }
        str++;
    }
}

int isOperator(char* str) {
    return (strcmp(str, "/") == 0 || strcmp(str, "*") == 0 || 
    strcmp(str, "-") == 0 || strcmp(str, "+") == 0 || strcmp(str, "^") == 0) ? 1 : 0;
}

int isFunction(char *str, int *len) {
    int i = 0;
    if (str[i] == 's' && str[i + 1] == 'i' && str[i + 2] == 'n') {
        *len = 3;
        return 0;
    }
    if (str[i] == 'c' && str[i + 1] == 'o' && str[i + 2] == 's') {
        *len = 3;
        return 0;
    }
    if (str[i] == 't' && str[i + 1] == 'a' && str[i + 2] == 'n') {
        *len = 3;
        return 0;
    }
    if (str[i] == 'c' && str[i + 1] == 'o' && str[i + 2] == 't') {
        *len = 3;
        return 0;
    }
    return -1; 
}


Node* createNode(const char * data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = strdup(data); 
    if (newNode->data == NULL) {
        printf("Memory allocation failed\n");
        free(newNode);
        exit(1);
    }
    newNode->next = NULL;
    return newNode;
}

int stackEmpty(stack_t* stack) {
    return stack->top == NULL;
}

void push(stack_t* stack, const char *data) {
    Node* newNode = createNode(data);
    newNode->next = stack->top;
    stack->top = newNode;

    // Add null terminator
    size_t len = strlen(data);
    newNode->data = (char*)realloc(newNode->data, len + 1); // Resize to accommodate null terminator
    if (newNode->data == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data[len] = '\0'; // Add null terminator
}


char * pop(stack_t* stack) {
    if (stackEmpty(stack)) {
        printf("Stack is empty\n");
        exit(1);
    }
    char * data = stack->top->data;
    Node* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return data;
}

char * peek(stack_t * stack) {
    if (stackEmpty(stack)) {
        printf("Stack is empty\n");
        exit(1);
    }
    return stack->top->data;
}

int queueEmpty(queue_t* queue) {
    return queue->front == NULL;
}

void enqueue(queue_t* queue, const char * data) {
    Node* newNode = createNode(data);
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

char * dequeue(queue_t* queue) {
    if (queueEmpty(queue)) {
        printf("Queue is empty\n");
        exit(1);
    }
    char * data = queue->front->data;
    Node* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    return data;
}

void rpn_push(rpnStack** stack, double value) {
    rpnStack* newNode = (rpnStack*)malloc(sizeof(rpnStack));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->next = *stack;
    *stack = newNode;
}

double rpn_pop(rpnStack** stack) {
    if (*stack == NULL) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    rpnStack* temp = *stack;
    double value = temp->data;
    *stack = temp->next;
    //free(temp);
    return value;
}

void displayQueue(queue_t* queue) {
    if (queueEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }
    Node* current = queue->front;
    while (current != NULL) {
        printf("%s ", current->data);
        current = current->next;
    }
    printf("\n");
}

void displayStack(stack_t* stack) {
    if (stackEmpty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    Node* current = stack->top;
    while (current != NULL) {
        printf("%s ", current->data);
        current = current->next;
    }
    printf("\n");
}

int precedence(char * op) {
    if (strcmp(op, "sin") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "tan") == 0 || strcmp(op, "cot") == 0 || strcmp(op, "log") == 0
    || strcmp(op, "arccos") == 0 || strcmp(op, "arcsin") == 0 || strcmp(op, "arctan") == 0) return 4;
    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    return 0;
}

void  shunting(char * equation, stack_t * stack, queue_t * queue) {
    int i = 0; int tmp; char buffer[MAX_SIZE]; char * tempo; int len;
    while(equation[i] != '\0') {
        if(isdigit(equation[i]) || equation[i] == '.') {
            tmp = i;
            while(isdigit(equation[i]) || equation[i] == '.') {
                i++;
            }
            strncpy(buffer, &equation[tmp], (i-tmp));
            buffer[i-tmp] = '\0';
            enqueue(queue, buffer);
            i--;
        }
        if(equation[i] == 'x') {
            enqueue(queue, "x");
        }
        if(equation[i] == 'e') {
            enqueue(queue, "e");
        }
        if(equation[i] == '/' || equation[i] == '*' || equation[i] == '-' ||
           equation[i] == '+' || equation[i] == '^' ) {
            char op[2] = {equation[i], '\0'};
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence(op)) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, op);
        }
        if(equation[i] == '(') {
            char op[2] = {'(', '\0'};
            push(stack, op);
        }
        if(equation[i] == ')') {
            while (!stackEmpty(stack) && strcmp(peek(stack), "(") != 0) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            if (stackEmpty(stack)) {
            printf("Mismatched parentheses\n");
            exit(1);
            }
            pop(stack); // to remove '('
        }
        if (equation[i] == 's' && equation[i + 1] == 'i' && equation[i + 2] == 'n') {
            i = i + 2;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("sin")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "sin");
        }
        if (equation[i] == 'c' && equation[i + 1] == 'o' && equation[i + 2] == 's') {
            i = i + 2;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("cos")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "cos");
        }
        if (equation[i] == 't' && equation[i + 1] == 'a' && equation[i + 2] == 'n') {
            i = i + 2;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("tan")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "tan");
        }
        if (equation[i] == 'c' && equation[i + 1] == 'o' && equation[i + 2] == 't') {
            i = i + 2;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("cot")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "cot");
        }
        if (equation[i] == 'a' && equation[i + 1] == 'r' && equation[i + 2] == 'c'
            && equation[i + 3] == 'c' && equation[i + 4] == 'o' && equation[i + 5] == 's') {
            i = i + 5;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("arccos")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "arccos");
        }
        if (equation[i] == 'a' && equation[i + 1] == 'r' && equation[i + 2] == 'c'
            && equation[i + 3] == 's' && equation[i + 4] == 'i' && equation[i + 5] == 'n') {
            i = i + 5;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("arcsin")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "arcsin");
        }
        if (equation[i] == 'a' && equation[i + 1] == 'r' && equation[i + 2] == 'c'
            && equation[i + 3] == 't' && equation[i + 4] == 'a' && equation[i + 5] == 'n') {
            i = i + 5;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("arctan")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "arctan");
        }
        if (equation[i] == 'l' && equation[i + 1] == 'o' && equation[i + 2] == 'g') {
            i = i + 2;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("log")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "l");
        }
        if (equation[i] == 'l' && equation[i + 1] == 'n') {
            i = i + 1;
            while(!stackEmpty(stack) && precedence(peek(stack)) >= precedence("log")) {
                tempo = pop(stack);
                enqueue(queue, tempo);
                tempo = NULL;
            }
            push(stack, "ln");
        }
        i++; 
    }
    while(!stackEmpty(stack)) {
            tempo = pop(stack);
            enqueue(queue, tempo);
            tempo = NULL;
    }
}

double rpn(queue_t * queue, rpnStack * stack, double x) {
    char * buffer;
    double tmp;
    double num1; double num2;

    while(!queueEmpty(queue)) {
        buffer = dequeue(queue);
        //if x
        if (strcmp(buffer, "x") == 0) {
            rpn_push(&stack, x);
        }
        if(strcmp(buffer, "e") == 0) {
            rpn_push(&stack, 2.71828182);
        }
        // if digit 
        if (isdigit(buffer[0])) {
            tmp = atof(buffer);
            rpn_push(&stack, tmp);
        }
        // if operators
        else if (strcmp(buffer, "+") == 0) {
            num1 = rpn_pop(&stack);
            num2 = rpn_pop(&stack);
            rpn_push(&stack, num2 + num1);
        } else if (strcmp(buffer, "*") == 0) {
            num1 = rpn_pop(&stack);
            num2 = rpn_pop(&stack);
            rpn_push(&stack, num2 * num1);
        } else if (strcmp(buffer, "-") == 0) {
            num1 = rpn_pop(&stack);
            num2 = rpn_pop(&stack);
            rpn_push(&stack, num2 - num1);
        } else if (strcmp(buffer, "/") == 0) {
            num1 = rpn_pop(&stack);
            num2 = rpn_pop(&stack);
            rpn_push(&stack, num2 / num1);
        } else if (strcmp(buffer, "^") == 0) {
            num1 = rpn_pop(&stack);
            num2 = rpn_pop(&stack);
            rpn_push(&stack, pow(num2, num1));
        }
        // if trignometric
        else if (strcmp(buffer, "sin") == 0) {
            num1 = rpn_pop(&stack);
            rpn_push(&stack, sin(num1));
        }
        else if (strcmp(buffer, "tan") == 0) {
            num1 = rpn_pop(&stack);
            rpn_push(&stack, tan(num1));
        }
        else if (strcmp(buffer, "cot") == 0) {
            num1 = rpn_pop(&stack);
            rpn_push(&stack, (1/tan(num1)));
        }
        else if (strcmp(buffer, "cos") == 0) {
            num1 = rpn_pop(&stack);
            rpn_push(&stack, cos(num1));
        }
        else if (strcmp(buffer, "arccos") == 0) {
            num1 = rpn_pop(&stack);
            rpn_push(&stack, acos(num1));
        }
        else if (strcmp(buffer, "arcsin") == 0) {
            num1 = rpn_pop(&stack);
            rpn_push(&stack, asin(num1));
        }
        else if (strcmp(buffer, "arctan") == 0) {
            num1 = rpn_pop(&stack);
            rpn_push(&stack, atan(num1));
        }
        // if logarithmic
        else if (strcmp(buffer, "log") == 0) {
            num1 = rpn_pop(&stack);
            num2 = rpn_pop(&stack);
            rpn_push(&stack, (log(num1)/log(num2)));
        }
        else if (strcmp(buffer, "ln") == 0) {
            num1 = rpn_pop(&stack);
            rpn_push(&stack, log(num1));
        }
    }
    

    return rpn_pop(&stack);
}

void bisection() {
    char expression[MAX_SIZE];
    rpnStack rpn_stack;
    stack_t stack;
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    double error = 1;
    double a; double b; double epsilon; double mid; double a_tmp; double b_tmp; double mid_tmp;
    printf("please enter your expression:"); scanf("%s", &expression);
    printf("please enter start value of x:"); scanf("%lf", &a);
    printf("please enter end value of x:"); scanf("%lf", &b);
    printf("please enter accepted error:"); scanf("%lf", &epsilon);
    shunting(expression, &stack, &queue);
    a_tmp = rpn(&queue, &rpn_stack, a);
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    shunting(expression, &stack, &queue);
    b_tmp = rpn(&queue, &rpn_stack, b);
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    if(a_tmp * b_tmp < 0)
    {
        while(error > epsilon)
        {
            mid = (a + b) / 2;
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            shunting(expression, &stack, &queue);
            mid_tmp = rpn(&queue, &rpn_stack, mid);
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            shunting(expression, &stack, &queue);
            a_tmp = rpn(&queue, &rpn_stack, a);
            if(mid_tmp * a_tmp < 0)
            {
                b = mid;
                error = b - a; 
            }
            else
            {
                a = mid;
                error = b - a;
            }
        }
    }
    else
    {
        printf("There is no root between %lf and %lf.\n", a , b); 
    }
    printf("Root is: %lf \n", a);
}

void regula_falsi() {
    char expression[MAX_SIZE];
    rpnStack rpn_stack;
    stack_t stack;
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    double a; double b; double epsilon; double a_tmp; double b_tmp; double c; double c_tmp;
    printf("please enter your expression:"); scanf("%s", &expression);
    printf("please enter start value of x:"); scanf("%lf", &a);
    printf("please enter end value of x:"); scanf("%lf", &b);
    printf("please enter accepted error:"); scanf("%lf", &epsilon);
    shunting(expression, &stack, &queue);
    a_tmp = rpn(&queue, &rpn_stack, a);
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    shunting(expression, &stack, &queue);
    b_tmp = rpn(&queue, &rpn_stack, b);
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    int count = 0;
    if (a_tmp * b_tmp < 0) {
        double error = fabs(b - a);
        while (error > epsilon) {
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            shunting(expression, &stack, &queue);
            a_tmp = rpn(&queue, &rpn_stack, a);
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            shunting(expression, &stack, &queue);
            b_tmp = rpn(&queue, &rpn_stack, b);
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            c = ((b * a_tmp - a * b_tmp) / (a_tmp - b_tmp));
            shunting(expression, &stack, &queue);
            c_tmp = rpn(&queue, &rpn_stack, c);
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            if (c_tmp * a_tmp < 0) {
                b = c;
            } else {
                a = c;
            }

            count++;
            error = fabs(b - a) / pow(count, 2);
        }
        printf("Root is %lf\n", a);
    } else {
        printf("There is no root between %lf and %lf.\n", a, b);
    }
}

void newton_raphson() {
    char expression[MAX_SIZE];
    rpnStack rpn_stack;
    stack_t stack;
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    double a; double epsilon; double a_tmp;
    printf("please enter your expression:"); scanf("%s", &expression);
    printf("please enter value of x:"); scanf("%lf", &a);
    printf("please enter accepted error:"); scanf("%lf", &epsilon);
    double error = 1.0;
    double tmp;
    shunting(expression, &stack, &queue);
    a_tmp = rpn(&queue, &rpn_stack, a);
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    a = a - (a_tmp/derivative(expression, a, 0.0001));
    while(error > epsilon) {
        tmp = a;
        shunting(expression, &stack, &queue);
        a_tmp = rpn(&queue, &rpn_stack, a);
        stack.top = NULL;
        rpn_stack.next = NULL;
        queue.front = NULL;
        queue.rear = NULL;
        a = a - (a_tmp/derivative(expression, a, 0.0001));
        error = fabs(a - tmp);
    }
    printf("Root is: %lf\n", a);
}

double derivative(char * expression, double x, double h) {
    rpnStack rpn_stack;
    stack_t stack;
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    double tmp1; double tmp2;
    shunting(expression, &stack, &queue);
    tmp1 = rpn(&queue, &rpn_stack, (x+h));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    shunting(expression, &stack, &queue);
    tmp2 = rpn(&queue, &rpn_stack, (x-h));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    return (tmp1 - tmp2) / (2*h);
}
double derivativeB(char * expression, double x, double h) {
    rpnStack rpn_stack;
    stack_t stack;
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    double tmp1; double tmp2;
    shunting(expression, &stack, &queue);
    tmp1 = rpn(&queue, &rpn_stack, (x));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    shunting(expression, &stack, &queue);
    tmp2 = rpn(&queue, &rpn_stack, (x-h));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    return (tmp1 - tmp2) / (h);
}

double derivativeF(char * expression, double x, double h) {
    rpnStack rpn_stack;
    stack_t stack;
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    double tmp1; double tmp2;
    shunting(expression, &stack, &queue);
    tmp1 = rpn(&queue, &rpn_stack, (x+h));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    shunting(expression, &stack, &queue);
    tmp2 = rpn(&queue, &rpn_stack, (x));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    return (tmp1 - tmp2) / (h);
}

void simpson() {
    char expression[MAX_SIZE];
    rpnStack rpn_stack;
    stack_t stack;
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    double a; int n; double a_tmp; double b; double b_tmp; double x; double x_tmp;
    printf("please enter your expression:"); scanf("%s", expression);
    printf("Enter the lower limit of integration:"); scanf("%lf", &a);
    printf("Enter the upper limit of integration:"); scanf("%lf", &b);
    printf("Enter the number of subintervals (must be even):"); scanf("%d", &n);

    while (n % 2 != 0) {
        printf("Error: Number of subintervals must be even.");
        printf("Enter the number of subintervals (must be even):"); scanf("%lf", &n);
    }

    double h = (b - a) / n;
    shunting(expression, &stack, &queue);
    a_tmp = rpn(&queue, &rpn_stack, (a));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    shunting(expression, &stack, &queue);
    b_tmp = rpn(&queue, &rpn_stack, (b));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;

    double sum = a_tmp + b_tmp;

    for (int i = 1; i < n; i++) {
        x = a + i * h;
        if (i % 2 == 0) {// even
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            shunting(expression, &stack, &queue);
            x_tmp = rpn(&queue, &rpn_stack, (x));
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            sum += 2 * x_tmp;
        }
        else {// odd
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            shunting(expression, &stack, &queue);
            x_tmp = rpn(&queue, &rpn_stack, (x));
            stack.top = NULL;
            rpn_stack.next = NULL;
            queue.front = NULL;
            queue.rear = NULL;
            sum += 4 * x_tmp;
        }
    }
    double result = (h / 3) * sum;

    printf("result is %lf\n", result);
}

void trapzoid() {
    char expression[MAX_SIZE];
    rpnStack rpn_stack;
    stack_t stack;
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue_t queue;
    queue.front = NULL;
    queue.rear = NULL;
    double a; double b; int n; double a_tmp; double b_tmp; double x; double x_tmp;
    printf("please enter your expression:"); scanf("%s", expression);
    printf("Enter the lower limit of integration:"); scanf("%lf", &a);
    printf("Enter the upper limit of integration:"); scanf("%lf", &b);
    printf("Enter the number of subintervals (must be even):"); scanf("%d", &n);

    double h = (b - a) / n;
    shunting(expression, &stack, &queue);
    a_tmp = rpn(&queue, &rpn_stack, (a));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    shunting(expression, &stack, &queue);
    b_tmp = rpn(&queue, &rpn_stack, (b));
    stack.top = NULL;
    rpn_stack.next = NULL;
    queue.front = NULL;
    queue.rear = NULL;
    double sum = 0.5 * (a_tmp + b_tmp); 

    for (int i = 1; i < n; i++) {
        x = a + i * h;
        stack.top = NULL;
        rpn_stack.next = NULL;
        queue.front = NULL;
        queue.rear = NULL;
        shunting(expression, &stack, &queue);
        x_tmp = rpn(&queue, &rpn_stack, (x));
        stack.top = NULL;
        rpn_stack.next = NULL;
        queue.front = NULL;
        queue.rear = NULL;
        sum += x_tmp;
    }

    double result = h * sum;
    printf("Result of integration: %lf\n", result);

}

double com(int n, double s) {
    int fact = 1;
    for (int i = 0 ; i < n; ++i) {
        fact *= i+1;
    }
    double sum = 1;
    for (int i = 0; i < n; ++i) {
        sum = sum * (s-i);
    }
    return sum / fact;
}

void interpolation() {
    int n;
    printf("Enter the amount of coordinates that will be given:"); scanf("%d", &n);
    double * x = (double *)malloc(n * sizeof(double));
    double * y = (double *)malloc(n * sizeof(double));
    for(int i = 0; i < n; i++) {
        printf("%d. x coordinate:", i+1); scanf("%lf", &x[i]);
        printf("%d. y coordinate:", i+1); scanf("%lf", &y[i]);
    }
    double target;
    printf("enter the target:"); scanf("%lf", &target);
    double temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (x[j] > x[j + 1]) {
                temp = x[j];
                x[j] = x[j + 1];
                x[j + 1] = temp;
                temp = y[j];
                y[j] = y[j + 1];
                y[j + 1] = temp;
            }
        }
    }
    double ** mat = (double **)malloc(n * sizeof(double*));
    for(int i = 0; i < n; i++) {
        mat[i] = (double *)malloc(n * sizeof(double));
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            mat[i][j] = 0;
        }
    }
    for(int i = 0; i < n; i++) {
        mat[i][0] = y[i];
    }
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - 1; j++) {
            mat[i+1][j+1] = mat[i+1][j] - mat[i][j];
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%lf  ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    double sum = y[0];
    double s = (target - x[0])/(x[1] - x[0]);
    for(int i = 1; i <n; i++) {
        printf("%lf ", sum);
        printf("%lf ", com(i,s));
        printf("%lf ", mat[i][i]);
        printf("\n");
        sum = sum + (com(i,s)*mat[i][i]);
        printf("\n");
    }

    printf("predicted result is: %lf\n", sum);
}

// Function to get cofactor of mat[p][q] in temp[][]. n is current dimension of mat[][]
void getCofactor(double **mat, double ** temp, int p, int q, int n) {
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            // Copying into temporary matrix only those elements which are not in given row and column
            if (row != p && col != q) {
                temp[i][j++] = mat[row][col];
                // Row is filled, so increase row index and reset col index
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// Recursive function to find determinant of matrix
double determinant(double ** mat, int n) {
    double det = 0; // Initialize result

    // Base case: if matrix contains single element
    if (n == 1)
        return mat[0][0];

    double ** temp = (double **)malloc( n  * sizeof(double *));
    for(int i = 0; i < n; i++) {
        temp[i] = (double *)malloc(n * sizeof(double));
    }

    int sign = 1; // To store sign multiplier

    // Iterate for each element of first row
    for (int f = 0; f < n; f++) {
        // Getting cofactor of mat[0][f]
        getCofactor(mat, temp, 0, f, n);
        det += sign * mat[0][f] * determinant(temp, n - 1);

        // Terms are to be added with alternate sign
        sign = -sign;
    }

    return det;
}

// Function to get adjoint of mat[][] in adj[][]
void adjoint(double **mat, double **adj, int n) {
    if (n == 1) {
        adj[0][0] = 1;
        return;
    }

    // temp is used to store cofactors of mat[][]
    int sign = 1;
    double **temp = (double **)malloc(n * sizeof(double *));
    for(int i = 0; i < n; i++) {
        temp[i] = (double *)malloc(n * sizeof(double *));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Get cofactor of mat[i][j]
            getCofactor(mat, temp, i, j, n);

            // Sign of adj[j][i] positive if sum of row and column indexes is even
            sign = ((i + j) % 2 == 0) ? 1 : -1;

            // Interchanging rows and columns to get the transpose of the cofactor matrix
            adj[j][i] = (sign) * (determinant(temp, n - 1));
        }
    }
}

// Function to calculate inverse of matrix mat[][]
void inverse(double **mat, double **inv, int n) {
    // Find determinant of mat[][]
    double det = determinant(mat, n);
    if (det == 0) {
        printf("Inverse does not exist");
        return;
    }

    // Find adjoint
    double **adj = (double **)malloc(n * sizeof(double*));
    for(int i = 0; i < n; i++) {
        adj[i] = (double *)malloc(n * sizeof(double));
    }
    adjoint(mat, adj, n);

    // Find Inverse using formula "inverse(mat) = adj(mat)/det(mat)"
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv[i][j] = adj[i][j] / det;
        }
    }
}


void mat_inverse() {
    int n;
    printf("Enter size of square matrix: ");
    scanf("%d", &n);

    double ** mat = (double**)malloc(n * sizeof(double*));
    double ** inv = (double**)malloc(n * sizeof(double*));

    for(int i = 0; i < n; i++) {
        mat[i] = (double*)malloc(n * sizeof(double));
        inv[i] = (double*)malloc(n * sizeof(double));
    }

    printf("Enter elements of matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d.%d. ", i+1, j+1);
            scanf("%lf", &mat[i][j]);
        }
    }

    inverse(mat, inv, n);

    printf("\nInverse of the matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", inv[i][j]);
        }
        printf("\n");
    }
}

void printMatrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void gaussianElimination(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        // Partial pivoting
        for (int k = i + 1; k < n; k++) {
            if (abs(matrix[i][i]) < abs(matrix[k][i])) {
                for (int j = 0; j <= n; j++) {
                    double temp = matrix[i][j];
                    matrix[i][j] = matrix[k][j];
                    matrix[k][j] = temp;
                }
            }
        }
        // Gaussian elimination
        for (int k = i + 1; k < n; k++) {
            double factor = matrix[k][i] / matrix[i][i];
            for (int j = 0; j <= n; j++) {
                matrix[k][j] -= factor * matrix[i][j];
            }
            printf("iteration\n");
            printMatrix(matrix, n);
            printf("\n");
        }
    }
    // Back substitution
    double *solution = (double *)malloc(n * sizeof(double));
    for (int i = n - 1; i >= 0; i--) {
        solution[i] = matrix[i][n];
        for (int j = i + 1; j < n; j++) {
            solution[i] -= matrix[i][j] * solution[j];
        }
        solution[i] /= matrix[i][i];
    }
    printf("\nSolution:\n");
    for (int i = 0; i < n; i++) {
        printf("x%d = %lf\n", i + 1, solution[i]);
    }
    free(solution);
}

void gaussian_elimination() {
    int n;
    printf("Enter the size of the matrix (NxN): ");
    scanf("%d", &n);

    // Allocate memory for matrix
    double **matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *)malloc((n + 1) * sizeof(double));
    }

    // Input matrix values
    printf("Enter the elements of the matrix row-wise:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            if(j < n) {
                printf("%d.%d.", i+1, j+1);
                scanf("%lf", &matrix[i][j]);
            }
            if(j == n) {
                printf("%d.element", i+1);
                scanf("%lf", &matrix[i][j]);
            }
        }
    }

    // Print the input matrix
    printf("\nInput Matrix:\n");
    printMatrix(matrix, n);
    printf("\n");

    // Perform Gaussian elimination
    gaussianElimination(matrix, n);

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

double ** dominant(double ** mat, int n) {
    int i, j, a;
    double tmp;
    
    for (i = 0; i < n; i++) {
        double max = mat[0][i];
        for (j = 0; j < n; j++) {
            if(abs(max) < abs(mat[j][i])) {
                max = mat[j][i];
                a = j;
            }
        }
        if(max != mat[0][i]) {
            for(j = 0; j < n; j++) {
                tmp = mat[i][j];
                mat[i][j] = mat[a][j];
                mat[a][j] = tmp;
            }
        }
    }
    return mat;
}

void gaussSeidel(double** A, double* b, double* x, int n, int iteration_count) {
    int i, j, iteration;
    double* newX = (double*)malloc(n * sizeof(double));
    double error;

    for (iteration = 0; iteration < iteration_count; iteration++) {
        error = 0.0;
        for (i = 0; i < n; i++) {
            newX[i] = b[i];
            for (j = 0; j < n; j++) {
                if (j != i) {
                    newX[i] -= A[i][j] * x[j];
                }
            }
            newX[i] /= A[i][i];
            error += fabs(newX[i] - x[i]);
            x[i] = newX[i];
        }
    }
    free(newX);
}

void gauss_seidal() {
    int n, i, j, iteration;
    // Get the size of the matrix from the user
    printf("Enter the size of the square matrix: ");
    scanf("%d", &n);

    // Allocate memory for matrix A, vector b, and initial guess x
    double** matrix = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }
    double* b = (double*)malloc(n * sizeof(double));
    double* x = (double*)malloc(n * sizeof(double));

    // Input the matrix A
    printf("Enter the elements of the matrix A row-wise:\n");
    for (i = 0; i < n; i++) {
        printf("Enter elements of row %d: ", i + 1);
        for (j = 0; j < n; j++) {
            scanf("%lf", &matrix[i][j]);
        }
    }

    // Input the vector b
    printf("Enter the elements of the vector b:\n");
    for (i = 0; i < n; i++) {
        printf("Enter element %d: ", i + 1);
        scanf("%lf", &b[i]);
    }

    // Initialize x to zeros
    for (i = 0; i < n; i++) {
        x[i] = 0.0;
    }

    printf("enter the iteration count:");
    scanf("%d", &iteration);

    // Print the results
    printf("\nInput Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%lf\t", matrix[i][j]);
        }
        printf("\n");
    }

    matrix = dominant(matrix, n);

    printf("\ndominant matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%lf\t", matrix[i][j]);
        }
        printf("\n");
    }
    // Perform Gauss-Seidel method
    gaussSeidel(matrix, b, x, n, iteration);

    printf("\nInput Vector :\n");
    for (i = 0; i < n; i++) {
        printf("%lf\n", b[i]);
    }

    printf("\nSolution Vector :\n");
    for (i = 0; i < n; i++) {
        printf("%lf\n", x[i]);
    }

    free(b);
    free(x);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
