#include <iostream>
#include "gurobi_c++.h"
#include <vector>
#include <cmath>
using namespace std;

int M = 1000;


// the gurobi function to implement frank-wolfe
GRBLinExpr dot_product(vector<double> vect_A, vector<GRBVar>& vect_B)
{
    GRBLinExpr product = 0;
    // Loop for calculate cot product
    for (int i = 0; i < vect_A.size(); i++)
        product += vect_A[i] * vect_B[i];
    return product;
}

vector<double> optimize(vector<double> delta_S, vector<double> X, int T)
{
    try {

        // Create an environment
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "mip1.log");
        env.start();

        // Create an empty model
        GRBModel model = GRBModel(env);

        // Create variables
        vector<GRBVar> y;
        y.resize(T);
        for (int i = 0; i < T; i++)
        {
            y[i] = model.addVar(-GRB_INFINITY, GRB_INFINITY, 0.0, GRB_BINARY, "day" + to_string(i));
        }

        // Set objective: maximize the dot product of delta_s and y
        GRBLinExpr prod = dot_product(delta_S, y);
        model.setObjective(prod, GRB_MAXIMIZE);

        // Add constraint: sum(y) == 0
        GRBLinExpr sum_y = 0;
        for (int i = 0; i < T; i++)
        {
            sum_y += y[i];
        }
        model.addConstr(sum_y == 0, "c0");

        // add all the other constraints respectively
        for (int i = 0; i < T; i++)
        {
            model.addConstr(X[i] + y[i] >= 0, "c" + to_string(i + 1));
        }

        // Optimize model
        model.optimize();

        vector<double> ret;

        for (int i = 0; i < T; i++)
        {
            ret.push_back(y[i].get(GRB_DoubleAttr_X));
        }

        return ret;

    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}


//define main function, taking T,N alpha and pi as input
void optimized_solve (int T, int N, double alpha, double pi)
{
    vector<double> X;
    unsigned int i,t;
    // Initialize value of target function
    double last_obj = 0;
    double obj;
    double best_obj;
    //Gradient vector
    vector<double> delta_s;
    //Product basis
    vector<double> pre_prod;
    vector<double> all_term;
    //Sum basis
    vector<double> pre_sum_all_term;
    //A certain part in derivative term
    vector<double> deriv;
    vector<double> y;
    vector<double> best_X;
    vector<double> cur_X;

    // Initialize a bunch of variables
    for (i = 0; i < T; i++)
    {
        delta_s.push_back(0);
        pre_prod.push_back(0);
        all_term.push_back(0);
        pre_sum_all_term.push_back(0);
        deriv.push_back(0);
        y.push_back(0);
        cur_X.push_back(0);
        best_X.push_back(0);

    }
    // Initialize strategy
    X.push_back(N);
    for (i = 1; i < T; i++)
    {
        X.push_back(0);
    }

    for (t = 1; t <= 10000; t++)
    {
        //Initialize a bunch of variables
        delta_s[0] = 0;
        pre_prod[0] = 1 - alpha * pow(X[0], pi);
        all_term[0] = pre_prod[0] * X[0];
        pre_sum_all_term[0] = all_term[0];
        if (X[0] > 0)
        {
            deriv[0] = -1 * alpha * pi * pow(X[0], pi - 1);
        }
        else
        {
            deriv[0] = 0;
        }

        for (unsigned j = 1; j < T; j++)
        {
            delta_s[j] = 0;
            pre_prod[j] = pre_prod[j - 1] * (1 - alpha * pow(X[j], pi));
            all_term[j] = X[j] * pre_prod[j];
            pre_sum_all_term[j] = pre_sum_all_term[j - 1] + all_term[j];
            if (X[j] > 0)
            {
                deriv[j] = -1 * alpha * pi * pow(X[j], pi - 1);
            }
            else
            {
                deriv[j] = 0;
            }
        }
        last_obj = pre_sum_all_term[T - 1];


        //Compute derivative function
        for (unsigned j = 0; j < T; j++)
        {
            delta_s[j] = pre_prod[j] \
            + (pre_sum_all_term[T - 1] - pre_sum_all_term[j]) / (1 - alpha * pow(X[j], pi)) * deriv[j];
            if (j > 0)
            {
                delta_s[j] += X[j] * deriv[j] * pre_prod[j-1];
            }
            else
            {
                delta_s[j] += X[j] * deriv[j];
            }
        }
        //Get our optimal direction
        y = optimize(delta_s, X, T);
        best_X = X;
        best_obj = 0;
        //Initialize maximal value in terms of step size choice
        for (unsigned j = 0; j < T; j++)
        {
            best_obj += X[j] * pre_prod[j];

        }

        for (unsigned int j = 1; j <= M; j++)
        {
            //Get a partition of [0,1] to M part
            for (unsigned k = 0; k < T; k++)
            {
                cur_X[k] = X[k] + y[k] * (j * 1.0 / M);
            }

            //Compute current value of function
            pre_prod[0] = 1 - alpha * pow(cur_X[0], pi);
            obj = pre_prod[0] * cur_X[0];
            for (unsigned int k = 1; k < T; k++)
            {
                pre_prod[k] = pre_prod[k-1] * (1 - alpha * pow(cur_X[k], pi));
                obj += pre_prod[k] * cur_X[k];

            }
            // Renew the best step size
            if (obj > best_obj)
            {
                best_X = cur_X;
                best_obj = obj;
            }
        }
        // Renew current strategy
        X = best_X;
        //Stopping rule
        if (best_obj - last_obj < 1e-7)
        {
            break;
        }
    }
    cout << "The optimal execution in each period is" << " ";
    for (unsigned j=0; j < T; j++){
        cout << X[j] << " ";
    }
    cout << endl;
    cout << "The optimal value we got is " << best_obj << endl;
}


// driver
int main() {
    optimized_solve(20,10000,0.001,0.5);
    return 0;
}

//int main() {
//    int t = 5;
//    vector<double> x = {0.2, 0.2, 0.2, 0.2, 0.2};
//    vector<double> triangle = {-1, 1, 3, 2, 5};
//    vector<double> ans = optimize(triangle, x, t);
//    for (int i = 0; i < ans.size(); i++)
//    {
//        cout << ans[i] << endl;
//    }
//    return 0;
//}