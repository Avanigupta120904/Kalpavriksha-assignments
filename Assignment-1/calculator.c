#include<stdio.h>
#define MAX_SIZE 10000

int errorCode = 0;

struct stack {
    int operatorStackTop;
    int numberStackTop;
    char operator[MAX_SIZE];
    int numbers[MAX_SIZE];
};

void initializeStack(struct stack *st) {
    st->operatorStackTop = -1;
    st->numberStackTop = -1;
}

void pushNum(struct stack *st, int num) {
    if(st->numberStackTop+1 == MAX_SIZE){ 
        errorCode = 3;
        return;
    }
    st->numberStackTop++;
    st->numbers[st->numberStackTop] = num;
}

int check(struct stack *st, char ch) {
    if(st->operatorStackTop == -1) return 0;

    switch(st->operator[st->operatorStackTop]) {
        case '/':
            return 1;
        case '*':
            if(ch != '/') return 1;
            return 0;
    }
    return 0;

}

int popFromNumStack(struct stack *st) {
    if(st->numberStackTop == -1) return 0;
    if(st->numberStackTop == MAX_SIZE) {
        errorCode = 3;
        return 0;
    }

    int temp = st->numbers[st->numberStackTop];
    st->numberStackTop--;
    return temp;

}

char popFromOperatorStack(struct stack *st) {
    if(st->operatorStackTop == -1) return 0;

    char temp = st->operator[st->operatorStackTop];
    st->operatorStackTop--;
    return temp;

}

void performOpration(struct stack *st) {
    int num2 = popFromNumStack(st);
    int num1 = popFromNumStack(st);
    if(errorCode != 0) return;
    int ans;
    char oper = popFromOperatorStack(st);
    switch(oper) {
        case '+':
            ans = num1+num2;
            break;
        case '-':
            ans = num1-num2;
            break;
        case '*':
            ans = num1*num2;
            break;
        case '/':
            if(num2 == 0) {
                errorCode = 2;
                return;
            }
            ans = num1/num2;
            break;
    }
    pushNum(st, ans);
}

void pushInOpratorStack(struct stack *st, char op) {
    if(st->operatorStackTop == MAX_SIZE) {
        errorCode = 3;
        return;
    }
    if(check(st, op)) {
        performOpration(st);
    }
    st->operatorStackTop++;
    st->operator[st->operatorStackTop] = op;
}

int main() {
    int n = 10000;
    char expression[n];
    fgets(expression, n, stdin);
    struct stack st;
    initializeStack(&st);
    int i = 0,  num = 0, pushed = 0;
    char last = 'a';
    while(expression[i] == ' ') i++;
    while(expression[i] != '\0' && expression[i] != '\n' && errorCode == 0) {
        if(expression[i] >= '0' && expression[i] <= '9') {
            num *= 10;
            num += (expression[i] - '0');
            pushed = 0;
            last = expression[i];
        }
        else if(expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            if(last == '+' || last == '-' || last == '*' || last == '/') {
                errorCode = 1;
                break;
            }
            pushNum(&st, num);
            pushed = 1;
            num = 0;
            pushInOpratorStack(&st, expression[i]);
            last = expression[i];
        }
        else {
            errorCode = 1;
        }
        i++;
        while(expression[i] != '\0' && expression[i] == ' ') i++;
    }
    if(errorCode == 0) {
        if(!pushed) {
            pushNum(&st, num);
        }
        while(st.operatorStackTop >= 0 && errorCode == 0) {
            performOpration(&st);
        }
    }
    if(errorCode == 0) printf("%d\n", st.numbers[0]);
    else if(errorCode == 1) printf("Error: Invalid Expression\n");
    else if(errorCode == 2) printf("Error: Division by zero\n");
    else printf("Error: Stack Overflow\n");

    return 0;
}