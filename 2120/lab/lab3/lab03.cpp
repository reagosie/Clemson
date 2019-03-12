#include <iostream>
#include <string>
#include "Stack.h"
#include "Queue.h"

using namespace std;

//Test the Stack class in Stack.h
void stack_test();

//Test the Queue class in Queue.h
void queue_test();

int main()
{
    stack_test();
    queue_test();

    return 0;
}

void stack_test()
{
    Stack<int> s1;
    for (int i = 1; i < 30; i++)
    {
        s1.push(i);
        if (i % 3 == 0)
        {
            s1.pop();
            s1.pop();
        }
    }
    cout << "s1: size = " << s1.size() << ", stack = ";
    s1.print();

    Stack<char> s2;
    int j = 1;
    for (char c = 'a'; c <= 'z'; c++, j++)
    {
        if (j % 4 == 0)
        {
            s2.push('#');
        }
        else
        {
            s2.push(c);
        }
        if (j % 6 == 0)
        {
            s2.pop();
            s2.pop();
        }
    }
    cout << "s2: size = " << s2.size() << ", stack = ";
    s2.print();

    Stack<double> s3;
    for (double k = 1; k <= 1000000; k++)
    {
        double value = 0.5 * k;
        s3.push(value);
    }
    for (double k = 1; k <= 1000000 - 6; k++)
    {
        s3.pop();
    }
    cout << "s3: size = " << s3.size() << ", stack = ";
    s3.print();

    Stack<int> s4;
    for (int l = 1; l < 30; l++)
    {
        s4.push(l);
        if (l % 8 == 0)
        {
            s4.pop();
            s4.pop();
            s4.pop();
        }
    }
    cout << "s4: size = " << s4.size() << ", stack = ";
    s4.print();

    Stack<char> s5;
    int m = 1;
    for (char c = 'a'; c <= 'z'; c++, m++)
    {
        if (m % 2 == 0)
        {
            s5.push('~');
        }
        else
        {
            s5.push(c);
        }
        if (m % 7 == 0)
        {
            s5.pop();
            s5.pop();
        }
    }
    cout << "s5: size = " << s5.size() << ", stack = ";
    s5.print();

    Stack<double> s6;
    for (double n = 100; n <= 1000000; n++)
    {
        double value = 4.2 * n;
        s6.push(value);
    }
    for (double n = 100; n <= 1000000 - 9; n++)
    {
        s6.pop();
    }
    cout << "s6: size = " << s6.size() << ", stack = ";
    s6.print();
}

void queue_test()
{
    Queue<int> q1;
    for (int i = 1; i < 30; i++)
    {
        q1.push(i);
        if (i % 3 == 0)
        {
            q1.pop();
            q1.pop();
        }
    }
    cout << "q1: size = " << q1.size() << ", queue = ";
    q1.print();

    Queue<char> q2;
    int j = 1;
    for (char c = 'a'; c <= 'z'; c++, j++)
    {
        if (j % 3 == 0)
        {
            char d = (char)(c - 32);
            q2.enqueue(d);
        }
        else
        {
            q2.enqueue(c);
        }
        if (j % 6 == 0)
        {
            q2.dequeue();
            q2.dequeue();
        }
    }
    cout << "q2: size = " << q2.size() << ", queue = ";
    q2.print();

    Queue<double> q3;
    for (double k = 1; k <= 1000000; k++)
    {
        q3.push(k);
    }
    for (double k = 1; k <= 1000000 - 6; k++)
    {
        q3.pop();
    }
    cout << "q3: size = " << q3.size() << ", queue = ";
    q3.print();

    Queue<int> q4;
    for (int l = 8; l < 33; l++)
    {
        q4.push(l);
        if (l % 5 == 0)
        {
            q4.pop();
            q4.pop();
        }
    }
    cout << "q4: size = " << q4.size() << ", queue = ";
    q4.print();

    Queue<char> q5;
    int m = 1;
    for (char c = 'a'; c <= 'z'; c++, m++)
    {
        if (m % 6 == 0)
        {
            char d = (char)(c - 32);
            q5.enqueue(d);
        }
        else
        {
            q5.enqueue(c);
        }
        if (m % 3 == 0)
        {
            q5.dequeue();
            q5.dequeue();
        }
    }
    cout << "q5: size = " << q5.size() << ", queue = ";
    q5.print();

    Queue<double> q6;
    for (double n = 15; n <= 19359; n++)
    {
        q6.push(n);
    }
    for (double n = 15; n <= 19359 - 11; n++)
    {
        q6.pop();
    }
    cout << "q6: size = " << q6.size() << ", queue = ";
    q6.print();
}
