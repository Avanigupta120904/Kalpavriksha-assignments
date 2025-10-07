#include<stdio.h>
#define MAX_SIZE 10000

struct stack {
    int opTop;
    int numTop;
    char operation[MAX_SIZE];
    int numbers[MAX_SIZE];
};

void initialize(struct stack *st) {
    st->opTop = -1;
    st->numTop = -1;
}

void pushNum(struct stack *st, int num) {
    if(st->numTop+1 == MAX_SIZE) return;
    st->numTop++;
    st->numbers[st->numTop] = num;
}

int check(struct stack *st, char ch) {
    if(st->opTop == -1) return 0;

    switch(st->operation[st->opTop]) {
        case '/':
            if(ch != '*') return 1;
            return 0;
        case '*':
            if(ch != '/') return 1;
            return 0;
    }
    return 0;

}

int popNum(struct stack *st) {
    if(st->numTop == -1) return 0;

    int temp = st->numbers[st->numTop];
    st->numTop--;
    return temp;

}

char popOp(struct stack *st) {
    if(st->opTop == -1) return 0;

    char temp = st->operation[st->opTop];
    st->opTop--;
    return temp;

}

int perform(struct stack *st) {
    int num2 = popNum(st);
    int num1 = popNum(st);
    int ans;
    char oper = popOp(st);
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
                printf("Error: Divisible by zero\n");
                return 0;
            }
            ans = num1/num2;
            break;
    }
    pushNum(st, ans);
    return 1;
}

int pushOp(struct stack *st, char op) {
    if(st->opTop == MAX_SIZE) return 0;
    int possible = 1;
    if(check(st, op)) {
        possible = perform(st);
    }
    st->opTop++;
    st->operation[st->opTop] = op;
    return possible;
}


int main() {
    char expression[100000];
    scanf("%[^\n]%*c", expression);
    struct stack st;
    initialize(&st);
    int i = 0;
    int num = 0;
    int possible = 1, pushed = 0;
    char last = 'a';
    while(expression[i] == ' ') i++;
    while(expression[i] != '\0' && possible) {
        if(expression[i] >= '0' && expression[i] <= '9') {
            num *= 10;
            num += (expression[i] - '0');
            pushed = 0;
            last = expression[i];
        }
        else if(expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            if(last == '+' || last == '-' || last == '*' || last == '/') {
                printf("Error: Invalid Expression\n");
                possible = 0;
                break;
            }
            pushNum(&st, num);
            pushed = 1;
            num = 0;
            possible = pushOp(&st, expression[i]);
            last = expression[i];
        }
        else {
            printf("Error: Invalid Expression\n");
            possible = 0;
        }
        i++;
        while(expression[i] != '\0' && expression[i] == ' ') i++;
    }
    if(possible) {
        if(!pushed) {
            pushNum(&st, num);
        }
        while(st.opTop >= 0 && possible) {
            possible = perform(&st);
        }
        if(possible) {
            printf("%d\n", st.numbers[0]);
        }
    }

    return 0;
}