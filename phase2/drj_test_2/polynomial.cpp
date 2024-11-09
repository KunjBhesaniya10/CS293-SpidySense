#include <bits/stdc++.h>
#include <iostream>

using namespace std;

class Polynomial
{
    int *degCoeff; // Name of your array (Don't change this)
    int capacity;
    // Complete the class
    Polynomial()
    {
        degCoeff = new int[10]; // intially 10 elements;
        for (int i = 0; i < 10; i++)
            degCoeff[i] = 0;
        this->capacity = 10;
    }
    void setCoefficient(int degree, int coeff)
    {
        if (degree >= capacity)
        {
            int size = deg + 1;
            int *newarr = new int[size];
            for (int i = 0; i < this->capacity; i++)
                newarr[i] = this->degCoeff[i];
            for (int i = this->capacity; i < size; i++)
                newarr[i] = 0;
            delete[] this->degCoeff;
            this->degCoeff = newarr;
            this->capacity = size;
        }
        this->degCoeff[degree] = coeff;
    }
    Polynomial operator+(Polynomial &b)
    {
        int k;
        int max_capacity = max(this->capacity, b.capacity);
        Polynomial result;
        result.capacity = max_capacity;
        result.degCoeff = new int[max_capacity];
        if (max_capacity > this->capacity)
        {
            for (k = 0; k < this->capacity; k++)
                result.degCoeff[k] = b.degCoeff[k] + this->degCoeff[k];

            for (; k < max_capacity; k++)
                result.degCoeff[k] = b.degCoeff[k];
        }
        else
        {

            for (k = 0; k < b.capacity; k++)
                result.degCoeff[k] = b.degCoeff[k] + this->degCoeff[k];

            for (; k < max_capacity; k++)
                result.degCoeff[k] = this->degCoeff[k];
        }
        return result;
    }
    Polynomial operator-(Polynomial &b)
    {
        int k;
        int max_capacity = max(this->capacity, b.capacity);
        Polynomial result;
        result.capacity = max_capacity;
        result.degCoeff = new int[max_capacity];
        if (max_capacity > this->capacity)
        {
            for (k = 0; k < this->capacity; k++)
                result.degCoeff[k] = -b.degCoeff[k] + this->degCoeff[k];

            for (; k < max_capacity; k++)
                result.degCoeff[k] = -b.degCoeff[k];
        }
        else
        {

            for (k = 0; k < b.capacity; k++)
                result.degCoeff[k] = -b.degCoeff[k] + this->degCoeff[k];

            for (; k < max_capacity; k++)
                result.degCoeff[k] = this->degCoeff[k];
        }
        return result;
    }
    Polynomial operator*(Polynomial &b)
    {
        Polynomial result;
        int max_capacity = this->capacity + b.capacity;
        result.capacity = max_capacity;
        for (int i = 0; i < this->capacity; i++)
        {
            for (int j = 0; j < b.capacity; j++)
            {
                result.degCoeff[i + j] = this->degCoeff[i] + b.degCoeff[j];
            }
        }
        return result;
    }
    Polynomial operator=(Polynomial &b)
    {
        this->capacity = b.capacity;
        this->degCoeff = new int[this->capacity];
        for (int i = 0; i < b.capacity; i++)
            this->degCoeff[i] = b.degCoeff[i];
        return *this;
    }
    //copy constructor
    Polynomial(Polynomial &b)
    {
        this->capacity=b.capacity;
        this->degCoeff=new int[this->capacity];
        for(int i=0;i<this->capacity;i++) this->degCoeff[i]=b.degCoeff[i];
    }
    void print()
    {
        for(int i=0;i<this->capacity;i++)
        {
            if(this->degCoeff[i])
            {
                cout<<this->degCoeff[i]<<"x"<<i<<" ";

            }
            cout<<endl;
        }
    }
};

int main()
{
    int count1, count2, choice;
    cin >> count1;

    int *degree1 = new int[count1];
    int *coeff1 = new int[count1];

    for (int i = 0; i < count1; i++)
    {
        cin >> degree1[i];
    }

    for (int i = 0; i < count1; i++)
    {
        cin >> coeff1[i];
    }

    Polynomial first;
    for (int i = 0; i < count1; i++)
    {
        first.setCoefficient(degree1[i], coeff1[i]);
    }

    cin >> count2;

    int *degree2 = new int[count2];
    int *coeff2 = new int[count2];

    for (int i = 0; i < count2; i++)
    {
        cin >> degree2[i];
    }

    for (int i = 0; i < count2; i++)
    {
        cin >> coeff2[i];
    }

    Polynomial second;
    for (int i = 0; i < count2; i++)
    {
        second.setCoefficient(degree2[i], coeff2[i]);
    }

    cin >> choice;

    Polynomial result;
    switch (choice)
    {
        // Add
    case 1:
        result = first + second;
        result.print();
        break;
        // Subtract
    case 2:
        result = first - second;
        result.print();
        break;
        // Multiply
    case 3:
        result = first * second;
        result.print();
        break;

    case 4: // Copy constructor
    {
        Polynomial third(first);
        if (third.degCoeff == first.degCoeff)
        {
            cout << "false" << endl;
        }
        else
        {
            cout << "true" << endl;
        }
        break;
    }

    case 5: // Copy assignment operator
    {
        Polynomial fourth(first);
        if (fourth.degCoeff == first.degCoeff)
        {
            cout << "false" << endl;
        }
        else
        {
            cout << "true" << endl;
        }
        break;
    }
    }

    return 0;
}