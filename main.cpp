//
//  main.cpp
//  power method
//
//  Created by 罗杰榕 on 11/7/19.
//  Copyright © 2019 Jierong. All rights reserved.
//


#include<iostream>
#include<vector>
#include<cmath>
using namespace std;

//Implement matrix multiplication in a function
vector<double> matrix_multiply_array(vector<vector<double>> matrix, vector<double> array)
{
    //Get the row of matrix
    const unsigned short row = matrix.size();
    //Get the column of matrix
    const unsigned short col = matrix[0].size();
    //Get the length of array
    const unsigned short len_of_vector = array.size();


    vector<double>  res;
    if (col != len_of_vector)//if it can't be multiplied, return None
    {
        return res;
    }
    else
    {
        //initialize the result array
        res.resize(row);


        //multiplication
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {

                res[i] += matrix[i][j] * array[j];
            }
        }
    }

    return res;
}


//calcuate the norm of a vector
double getnorm(vector<double> array){
    double res = 0;
    const unsigned short len = array.size();
    for (unsigned i = 0;i < len;i++){
        res += pow(array.at(i),2);
    }
    res = sqrt(res);
    return res;
}
//mutiply a vector by a number
vector<double> number_multiply_vector(vector<double> array, double number){
    vector<double> ans;
    const short unsigned len = array.size();
    unsigned i;
    ans.resize(len);
    for (i=0;i<len;i++){
        ans[i] = array[i] * number;
    }
    return ans;
}
//compute the tensor product of certain vector, return a matrix
vector<vector<double>> tensor_product_of_vector(vector<double> array, double mutipler){
    vector<vector<double>> ans;
    const short unsigned len = array.size();
    unsigned i,j;
    ans.resize(len);

    for (i=0;i<len;i++){
        ans[i].resize(len);
        for (j=0;j<len;j++){
            ans[i][j] = array[i]*array[j]*mutipler;
        }
    }
    return ans;
}
//Compute the subtract of matrix
vector<vector<double>> subtract_of_matrix(vector<vector<double>> matrix1, vector<vector<double>> matrix2){
    vector<vector<double>> ans;
    //Get the row of matrix
    const unsigned short row = matrix1.size();
    //Get the column of matrix
    const unsigned short col = matrix1[0].size();
    ans.resize(row);
    for (unsigned i=0;i<row;i++){
        ans[i].resize(col);
        for (unsigned j=0;j<col;j++){
            ans[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }


    return ans;
}



//Implement the power mathod to compute first k eigenvalues, return them as a vector
vector<double> power(int k,vector<vector<double>> matrix){
    vector<double> res;
    const int max_iters = 1000;
    double oldnorm2, norm2;
    const double tolerance = 1e-10;
    double lamda;
    vector<double> u;
    vector<double> v,v1;
    // Generate the random vector u
    srand((unsigned)time(0));
    for (unsigned i=0;i<matrix[0].size();i++){
        u.push_back((rand() % (10) + 1));
    }
    // Compute its k largest eigenvalue by recursion
    for (unsigned i=0;i<k;i++){
        lamda=0;
        // Initiailize norm
        norm2 = getnorm(u);
        // normalize u
        v = number_multiply_vector(u, 1/norm2);
        for (unsigned j=1;j<=max_iters;j++){
            // Store the previous norm
            oldnorm2 = norm2;
            // Compute new vector
            v1 = matrix_multiply_array(matrix, v);
            //Renew the norm
            norm2 = getnorm(v1);
            // normalize v
            v = number_multiply_vector(v1, 1/norm2);
            //Renew "eigenvalue"
            lamda = norm2;
            //Stopping rule
            if (fabs(norm2-oldnorm2) < tolerance){
                break;
            }

        }
        // Store the eigenvalue in result array one by one
        res.push_back(lamda);
        // renew the matrix in order to compute the next eigenvalue
        matrix = subtract_of_matrix(matrix, tensor_product_of_vector(v, lamda));
    }


    return res;
}

int main(void)
{
    //vector<vector<int>> matrix = { { 2, 1 }, { 4, 3 } };
    //vector<double> arrB = { { 1, 2 }, { 1, 0 } };
    vector<vector<double>> matrix = { { 0.7676, 0.1907, -0.1723,-0.0707 }, { 0.1907, 0.6843, 0.2717 ,0.1093} ,{-0.1723,0.2717,0.4841,-0.1342},{-0.0707,0.1093,-0.1342,0.8610}};
    vector<double> res = power(2, matrix);
    for (unsigned i=0;i<res.size();i++){

        cout<<res[i]<<" ";

    }
    return 0;
}

