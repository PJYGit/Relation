#include "stdio.h"
#include <iostream>
#include <fstream>
#include "relation.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

//---------------------------------------------------------------------------------------------------

Relation::Relation(int d)
{
    //Construct an empty relation of dimension d

    dimension = d;
    Matrix = new int[dimension * dimension];
    memset(Matrix, 0, sizeof(int)*dimension * dimension);
}

Relation::Relation(int d, const int *M)
{
    //Construct a relation from the dimension of its relational matrix and the array of the relational matrix

    dimension = d;
    Matrix = new int[dimension * dimension];
    memcpy(Matrix, M, sizeof(int)*dimension * dimension);
}

Relation::Relation(const char *filename)
{
    //Read the matrix of a relation from a file

    ifstream readfile(filename);
    if(!readfile)
    {
        printf("Fail to open.\n");
        exit(1);
    }
    else
    {
        int row, column;
        readfile>>dimension;
        Matrix = new int[dimension * dimension];
        for (row = 0; row < dimension; row++)
            for (column = 0; column < dimension; column++)
                readfile>>Matrix[row * dimension + column];
    }
    readfile.close();
}

Relation::~Relation()
{
    //Destructor

    delete []Matrix;
}

Relation::Relation(const Relation &r)
{
    dimension = r.dimension;
    Matrix = new int[dimension * dimension];
    memcpy(Matrix, r.Matrix, sizeof(int)*dimension * dimension);
}

Relation &Relation::operator =(const Relation &r)
{
    dimension = r.dimension;
    Matrix = new int[dimension * dimension];
    memcpy(Matrix, r.Matrix, sizeof(int)*dimension * dimension);
    return *this;
}

//---------------------------------------------------------------------------------------------------

int Relation::GetDimension() const
{
    //Get the dimension of relational matrix

    return dimension;
}

int Relation::GetatPosition(int row, int column) const
{
    //Gets the value at the cross position in column-th column and row-th row of the relational matrix, and this function returns -1 if it is over the boundary.

    if (row >= 0 && row < dimension && column >= 0 && column < dimension)
        return Matrix[row * dimension + column];
    else
        return -1;
}

int Relation::operator()(int row, int column) const
{
    //You can use R (i, j) to get the value at the cross position in column-th column and row-th row of the relational matrix, and this function returns - 1 if it is over the boundary.

    if (row >= 0 && row < dimension && column >= 0 && column < dimension)
        return Matrix[row * dimension + column];
    else
        return -1;
}

bool Relation::GetMatrix(int *result) const
{
    //Get the relational matrix
    if (result != NULL)
    {
        memcpy(result, Matrix, sizeof(int)*dimension * dimension);
        return true;
    }
    return false;
}

void Relation::Output() const
{
    //Display the matrix of the relation on the screen
    int row, column;
    //QString temp = QString::number(dimension);

    cout<<dimension<<endl;

    for (row = 0; row < dimension; row++)
    {
        for (column = 0; column < dimension; column++)
        {
            cout << (*this)(row, column);
        }
        cout << "\n";
    }
}

bool Relation::OutputtoFile(const char *filename) const
{
    //Write the relation matrix to a file in the same format as the read file

    ofstream writefile(filename);
    if(!writefile)
    {
        cout << "Fail to open.\n";
        return false;
    }
    else
    {
        int row, column;
        writefile << dimension << endl;
        for (row = 0; row < dimension; row++)
        {
            for (column = 0; column < dimension; column++)
                writefile << Matrix[row * dimension + column]<<' ';
            writefile << endl;
        }
    }
    writefile.flush();
    writefile.close();
    return true;
}

bool Relation::IsReflexive() const
{
    //ToDo：determine whether this relation has this property or not
    //int num = 0;
    for (int i = 0; i < dimension; i++)
    {
        if (GetatPosition(i, i) != 1)
            return false;
        else if(i==3)
            return true;
    }
}

bool Relation::IsIrreflexive() const
{
    //ToDo：determine whether this relation has this property or not
    for (int i = 0; i < dimension; i++)
    {
        if (GetatPosition(i, i) != 0)
            return false;
        else if (i == 3)
            return true;
    }
}

bool Relation::IsSymmetric() const
{
    //ToDo：determine whether this relation has this property or not
    int num = 0;
    for (int i = 0; i < dimension; i++)
    {
        for (int j = i; j < dimension; j++)
        {
            if (GetatPosition(i, j) == GetatPosition(j, i))
            {
                num++;
                if (num == (dimension + 1)*dimension / 2)
                {
                    return true;
                }
            }
            else
                return false;
        }
    }
}

bool Relation::IsAsymmetric() const
{
    //ToDo：determine whether this relation has this property or not
    int num = 0;
    for (int i = 0; i < dimension; i++)
    {
        if (GetatPosition(i, i) != 0)
            return false;

        for (int j = i; j < dimension; j++)
        {
            if (GetatPosition(i, j) != GetatPosition(j, i))
            {
                num++;
                if (num == (dimension + 1)*dimension / 2-dimension)
                {
                    return true;
                }
            }
            else
                return false;
        }
    }
}

bool Relation::IsAntisymmetric() const
{
    //ToDo：determine whether this relation has this property or not
    int num = 0;
    for (int i = 0; i < dimension; i++)
    {
        for (int j = i+1; j < dimension; j++)
        {
            if (GetatPosition(i, j) == 0 || GetatPosition(j, i) == 0)
            {
                num++;
                if (num == (dimension + 1)*dimension / 2-dimension)
                {
                    return true;
                }
            }
            else
                return false;
        }
    }
}

bool Relation::IsTransitive() const
{
    //ToDo：determine whether this relation has this property or not
    int p = 0;
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (GetatPosition(i, j) == 1)
            {
                for (int k = 0; k < dimension; k++)
                {
                    if (GetatPosition(j, k) == 1 && GetatPosition(i, k) != 1)
                    {
                        p = 1;
                        break;
                    }
                }
            }
        }
    }
    if (p)
        return false;
    else
        return true;
}

bool Relation::IsCycle() const
{

    TransitiveClosure();
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            for (int k = 0; k < dimension; k++)
            {
                if (GetatPosition(i, j) == 1 && GetatPosition(j, k) == 1)
                {
                    if (GetatPosition(k, i) == 1) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Relation::IsEquivalence() const
{
    //ToDo：determine whether this relation is an equivalence relation or not
    bool a = IsReflexive();
    bool b = IsSymmetric();
    bool c = IsTransitive();
    if (a&&b&&c)
    {
        return true;
    }
    else
        return false;
}

bool Relation::IsPartialOrder() const
{
    bool a = IsReflexive();
    bool b = IsAntisymmetric();
    bool c = IsTransitive();
    if (a&&b&&c)
    {
        return true;
    }
    else
        return false;
}

Relation Relation::ReflexiveClosure() const
{
    int *m = new int[dimension * dimension];
    GetMatrix(m);

    /*
    ToDo: Modify *m to get the relational matrix of its reflexive closure, from which the object r is constructed and returned
    */
    for (int i = 0; i < dimension; i++)
    {
        if (GetatPosition(i, i) == 0)
            m[i*dimension + i] = 1;
    }
    Relation r(dimension, m);
    return r;
}

Relation Relation::SymmetricClosure() const
{
    int *m = new int[dimension * dimension];
    GetMatrix(m);

    /*
    ToDo: Modify *m to get the relational matrix of its symmetric closure , from which the object r is constructed and returned
    */
    for (int i = 0; i < dimension; i++)
    {
        for (int j = i; j < dimension; j++)
        {
            if (GetatPosition(i, j) == 1 || GetatPosition(j, i) == 1)
            {
                m[i*dimension + j] = 1;
                m[j*dimension + i] = 1;
            }
        }
    }
    Relation r(dimension, m);
    return r;
}

Relation Relation::TransitiveClosure() const
{
    int *m = new int[dimension * dimension];
    GetMatrix(m);

    /*
    ToDo: Modify *m to get the relational matrix of its transitive closure , from which the object r is constructed and returned
    */
    int i, j, k;
    for (k = 0; k < GetDimension(); k++) {
        for (i = 0; i < GetDimension(); i++) {
            for (j = 0; j < GetDimension(); j++) {
                if (m[k*dimension + i] == 1 && m[j*dimension + k] == 1)
                {
                    m[j*dimension + i] = 1;
                }
            }
        }
    }
    Relation r(dimension, m);
    return r;
}

Relation Relation::EquivalenceClosure() const
{
    int *m = new int[dimension * dimension];
    GetMatrix(m);
    for (int i = 0; i < dimension; i++)
    {
        if (GetatPosition(i, i) == 0)
            m[i*dimension + i] = 1;
    }
    for (int i = 0; i < dimension; i++)
    {
        for (int j = i; j < dimension; j++)
        {
            if (GetatPosition(i, j) == 1 || GetatPosition(j, i) == 1)
            {
                m[i*dimension + j] = 1;
                m[j*dimension + i] = 1;
            }
        }
    }
    int i, j, k;
    for (k = 0; k < GetDimension(); k++) {
        for (i = 0; i < GetDimension(); i++) {
            for (j = 0; j < GetDimension(); j++) {
                if (m[k*dimension + i] == 1 && m[j*dimension + k] == 1)
                {
                    m[j*dimension + i] = 1;
                }
            }
        }
    }
    Relation r(dimension, m);
    return r;
}

Relation Relation::PartialorderClosure() const
{
    int *m = new int[dimension * dimension];
    GetMatrix(m);
    for (int i = 0; i < dimension; i++)
    {
        if (GetatPosition(i, i) == 0)
            m[i*dimension + i] = 1;
    }
    int i, j, k;
    for (k = 0; k < GetDimension(); k++) {
        for (i = 0; i < GetDimension(); i++) {
            for (j = 0; j < GetDimension(); j++) {
                if (m[k*dimension + i] == 1 && m[j*dimension + k] == 1)
                {
                    m[j*dimension + i] = 1;
                }
            }
        }
    }
    Relation r(dimension, m);
    return r;
}

Relation Relation::Hasse() const
{
    int *m = new int[dimension * dimension];
    GetMatrix(m);
    for (int i = 0; i < dimension; i++)
    {
        if (GetatPosition(i, i) == 1)
            m[i*dimension + i] = 0;
    }
    int i, j, k;
    for (k = 0; k < GetDimension(); k++) {
        for (i = 0; i < GetDimension(); i++) {
            for (j = 0; j < GetDimension(); j++) {
                if (m[k*dimension + i] == 1 && m[j*dimension + k] == 1)
                {
                    m[j*dimension + i] = 0;
                }
            }
        }
    }
    Relation r(dimension, m);
    return r;
}

bool Relation::EquiClasses(int *result) const
{
    /*
    ToDo: result is an array of the dimension in size.
    If this relation is an equivalence relation, then stores the number of the equivalent class
    of the elements of the current relation (the number is started from 0) in the array result,
    and returnes true.
    Otherwise, the value of any item in the array result is set as -1 and returnes false.
    For an example, see the comment at the function declaration.
    */
    if (IsEquivalence() == true) {
        memset(result, 255, sizeof(int) * dimension);
        int x = 0;
        int i, j;
        for (i = 0; i < GetDimension(); i++) {
            if (result[i] == -1) {
                result[i] = x;
                x++;
            }
            for (j = 0; j < GetDimension(); j++) {
                if (GetatPosition(i, j) == 1) {
                    result[j] = result[i];
                }
            }
        }
        return true;
    }
    else {
        memset(result, 255, sizeof(int) * dimension);
        return false;
    }
}

bool Relation::Topology(int * tuopu) const
{
    int *m = new int[dimension * dimension];
    GetMatrix(m);
    int x = 0;
    int y = 0;
    int *a = new int[dimension];
    for (int k = 0; k < dimension; k++) {
        a[k] = k;
    }
    memset(tuopu, 255, sizeof(int) * dimension);
    for (int k = 0; k < dimension; k++) {

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                if (a[i] != -1) {
                    if (m[j*dimension + a[i]] == 1) {
                        x = 1;
                    }
                }
                else {
                    x = 1;
                }
            }
            if (x == 0) {
                y = i;

            }
            x = 0;
        }
        tuopu[k] = y;
        for (int c = 0; c< dimension; c++) {
            m[y*dimension + c] = 0;
        }
        a[y] = -1;
    }
    return true;
}
