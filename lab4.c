#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
pthread_mutex_t mutex;
#define maxLength 100
#define scaleFactor 10
int answer = 0;

int plusMinus();
int para();
int timesDiv();

// intializing a struct for the parts of the expression given by the user
typedef struct
{
    int first;
    int second;
} Expression;

// global variables- user input
char input[maxLength];
int pos;

// functions that compute an operation for a thread

/**
 * thread taking care of adding numbers
 * @param a universal pointer
 * @return no return value
 *
 **/
void *add(void *param)
{
    Expression *ep = param;
    ep->first = ep->first + ep->second;
}

/**
 * thread taking care of subtracting numbers
 * @param a universal pointer
 * @return no return value
 *
 **/
void *sub(void *param)
{
    Expression *ep = param;
    ep->first = ep->first - ep->second;
}

/**
 * thread taking care of multiplying numbers
 * @param a universal pointer
 * @return no return value
 *
 **/
void *multi(void *param)
{
    Expression *ep = param;
    ep->first = ep->first * ep->second;
}

/**
 * thread taking care of divide numbers
 * @param a universal pointer
 * @return no return value
 *
 **/
void *divide(void *param)
{
    Expression *ep = param;
    ep->first = ep->first / ep->second;
}

// functions to create the thread

/**
 * function to create the thread process for the add operation
 * @param an Expression object from function plusMinus()
 * @return n value, the sum
 *
 **/
int addCompute(Expression ep)
{
    pthread_t tid;
    pthread_mutex_lock(&mutex);
    pos++;
    ep.second = timesDiv();
    pthread_create(&tid, NULL, add, &ep);
    pthread_mutex_unlock(&mutex);
    pthread_join(tid, NULL);
    return ep.first;
}

/**
 * function to create the thread process for the subtract operation
 * @param an Expression object from function plusMinus()
 * @return n value, the difference
 *
 **/
int subCompute(Expression ep)
{
    pthread_t tid;
    pthread_mutex_lock(&mutex);
    pos++;
    ep.second = timesDiv();
    pthread_create(&tid, NULL, sub, &ep);
    pthread_mutex_unlock(&mutex);
    pthread_join(tid, NULL);
    return ep.first;
}

/**
 * function to create the thread process for the multiplication operation
 * @param an Expression object from function timesDiv()
 * @return n value, the product
 *
 **/
int multiCompute(Expression ep)
{
    pthread_t tid;
    pthread_mutex_lock(&mutex);
    pos++;
    ep.second = para();
    pthread_create(&tid, NULL, multi, &ep);
    pthread_mutex_unlock(&mutex);
    pthread_join(tid, NULL);
    return ep.first;
}

/**
 * function to create the thread process for the division operation
 * @param an Expression object from function timesDiv()
 * @return n value, the quotient
 *
 **/
int diviCompute(Expression ep)
{
    pthread_t tid;
    pthread_mutex_lock(&mutex);
    pos++;
    ep.second = para();
    pthread_create(&tid, NULL, divide, &ep);
    pthread_mutex_unlock(&mutex);
    pthread_join(tid, NULL);
    return ep.first;
}

// functions that parse through the arithmetic expression and gets the resulting value

/**
 * expression to evalulate addition and subtraction (same precedence)
 * @param no parameter
 * @return ep.first value, the value of two digits being added or subtracted
 *
 **/
int plusMinus()
{
    Expression ep;
    ep.first = timesDiv();
    if (input[pos] == '+')
    {
        ep.first = addCompute(ep);
    }
    else if (input[pos] == '-')
    {
        ep.first = subCompute(ep);
    }

    return ep.first;
}

/**
 * expression to evalulate mult[plication and division (same precedence)
 * @param no parameter
 * @return ep.first value, the value of two digits being multiplied or subtracted
 *
 **/
int timesDiv()
{
    Expression ep;
    ep.first = para();
    if (input[pos] == '*')
    {
        ep.first = multiCompute(ep);
    }
    else if (input[pos] == '/')
    {
        ep.first = diviCompute(ep);
    }
    return ep.first;
}

/**
 * function to parse through values within the parantheses
 * @param no parameter
 * @return n value, the digit of the expression
 *
 **/
int para()
{
    int n = 0;

    if (input[pos] == '(')
    {
        pos++;
        n = plusMinus();
        if (input[pos] == ')')
        {
            pos++;
            return n;
        }
    }
    else
    {
        while ('0' <= input[pos] && input[pos] <= '9')
        {
            n = n * scaleFactor + (input[pos] - '0');
            pos++;
        }
    }
    return n;
}

/**
 * gets the arithmetic expression from the user and writes to a new file
 * @param no parameter
 * @return return 0 if file cannot be opened
 *
 **/
int newFileOutput()
{
    // writing to a file for a system call
    FILE *fptr = fopen("calculator.txt", "w");
    if (fptr == NULL)
    {
        printf("Could not open file");
        return 0;
    }
    fprintf(fptr, "Enter an expression with parantheses: \n");
    fscanf(fptr, "%s", input);
    fprintf(fptr, "%d\n", answer);

    fclose(fptr);
}

/**
 * gets the arithmetic expression from the user
 * @param no parameter
 * @return an int value of zero
 *
 **/
int main()
{

    printf("Enter an expression with parantheses: \n");
    scanf("%s", input);
    answer = plusMinus();
    printf("Answer: %d\n", answer);
    newFileOutput();
    return 0;
}