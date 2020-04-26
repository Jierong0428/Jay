#include <iostream>
#include <vector>
#include <math.h>       /* pow */
#include <stdio.h>
#include<cmath>
#include<ctime>
#include<cstdlib>
using namespace std;


// a bunch of useful functions first
// implement matrix multiplication in a function
vector<double> array_multiply_matrix(vector<double> array, vector< vector<double> > matrix) {
    //Get the row of matrix
    const unsigned short row = matrix.size();
    //Get the column of matrix
    const unsigned short col = matrix[0].size();
    //Get the length of array
    const unsigned short len_of_vector = array.size();

    vector<double>  res;
    if(row != len_of_vector) {  // if it can't be multiplied, return None
        cout << "error input"<<endl;
        return res;
    }
    else {
        //initialize the result array
        res.resize(col);
        //multiplication
        for(int j = 0; j < col; j++) {
            for(int i = 0; i < row; i++) {
                res[j] += matrix[i][j] * array[i];
            }
        }
    }
    return res;
}


double GaussRand(double dExpect, double dVariance) {
    static double V1, V2, S;
    static int phase = 0;
    double X;
    srand(time(0));
    if(phase == 0) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);
        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);
    phase = 1 - phase;
    return X * dVariance + dExpect;
}
// derivative of activate function
double f_prime(double num) {
    if (num > 0) {
        return 1;
    }
    else if(num == 0) {
        return 0.5;
    }
    else {
        return 0;
    }
}


// activation function
double ReLU(double input) {
    if(input > 0) {
        return input;
    }
    else {
        return 0;
    }
}


// activate vector aka use ReLU
vector<double> activate(vector<double> input){
    vector<double> output;
    for(int i = 0; i < input.size(); i++) {
        output.push_back(ReLU(input[i]));
    }
    return output;
}


// initialize output for each neuron each day
vector< vector<double> > X;
vector< vector<double> > Y;
vector< vector<double> > Z;
vector< double> F;  // final layer output for each day


// initialize the matrices to hold output vectors for all dates
void calc_output(vector< vector<double> > weights_1, vector< vector<double> > weights_2,
                 vector<double> weights_3, vector< vector<double> > df) {


    for (int i = 0; i < df.size(); i++) {
        F[i] = 0.0;
    }
    for (int i = 0; i < X.size(); i++) {
        // x's, y's and z's hold the output of each layer
        X[i] = activate(df[i]);
        Y[i] = activate(array_multiply_matrix(X[i], weights_1));
        Z[i] = activate(array_multiply_matrix(Y[i], weights_2));
        for (int j = 0; j < 5; j++) {
            F[i] += Z[i][j] * weights_3[j];

        }

        F = activate(F);

    }
}


// loss function
double get_loss(const vector<double> volume ) {
    double ret = 0;
    int T = volume.size();
    for(int i = 0; i < T; i++) {
        ret += pow((F[i] - volume[i]), 2);
        //cout << " Output :" << F[i] << endl;
    }
    ret /= T;
    return ret;
}


vector<double> normalize(vector<double> vec) {
    double s_mean = 0;
    double s_var = 0;
    for(int i = 0; i < vec.size(); i++) {
        s_mean += vec[i];
    }
    s_mean /= vec.size();
    for(int i = 0; i < vec.size(); i++) {
        s_var += (s_mean - vec[i]) * (s_mean - vec[i]);
    }
    s_var /= vec.size();
    for(int i = 0; i < vec.size(); i++) {
        vec[i] = (vec[i] - s_mean) / pow(s_var, 0.5);
    }
    return vec;
}


vector<double> get_column(vector< vector<double> > df, int col) {
    vector<double> ans;
    ans.resize(df.size());
    for(int i = 0; i < df.size(); i++) {
        ans[i] = df[i][col];
    }
    return ans;
}


int main() {
    freopen("../nnew_500.txt", "r", stdin);
    double val;
    vector< vector<double> > data;
    vector< vector<double> > data_test;
    vector<double> row;
    vector<double> S;
    vector<double> S_test;

    int days, n;
    scanf("%d%d", &days, &n);
    for(int i = 0; i < days; i++) {
        if(i > 232) {
            row.clear();
            for(int j = 0; j < n; j++) {
                scanf("%lf", &val);
                row.push_back(val);
            }
            data_test.push_back(row);
            scanf("%lf", &val);
            S_test.push_back(val);
        }
        if (i <= 232) {
            row.clear();
            for (int j = 0; j < n; j++) {
                scanf("%lf", &val);
                row.push_back(val);
            }
            data.push_back(row);
            scanf("%lf", &val);
            S.push_back(val);
        }
    }  // finished reading in here

    //S = normalize(S);
//    for(int i = 0; i < S.size(); i++) {
//        cout << S[i] << endl;
//    }
//    vector< vector<double> > data1 = data;  // no need to resize then
//    for(int i = 0; i < data[0].size(); i++) {
//        vector<double> temp = normalize(get_column(data, i));
//        for(int j = 0; j < data.size(); j ++) {
//            data1[j][i] = temp[j];
//        }
//    }
    //data = data1;
    // cout << data[0][0] << " " << data[0][1] << " " << data[1][0] << " " << data[1][1] << endl;

    // data 466 * 483
//    // S 466
//    for(int i = 0; i < days; i++) {
//        for(int j = 0; j < n; j++)
//            printf("%.3lf%c", data[i][j], " \n"[j + 1 == n]);
//        printf("\n");
//    }

    // random initialize
    vector<double> zero1;
    vector<double> zero2;
    vector<double> zero3;
    vector< vector<double> > w1;

    for (int i = 0; i < data[0].size();i++){
        zero1.push_back(0);
    }
    for (int i = 0; i < 50 ;i++){
        zero2.push_back(0);
    }
    for (int i = 0; i < 5 ;i++){
        zero3.push_back(0);
    }
    for (int i = 0; i < data.size();i++){
        X.push_back(zero1);
        Y.push_back(zero2);
        Z.push_back(zero3);
        F.push_back(0);
    }
//    X.resize(data.size());
//    Y.resize(data.size());
//    Z.resize(data.size());
//    F.resize(data.size());

//    for(int i = 0; i < X.size(); i++) {
//        X[i].resize(data[i].size());
//        Y[i].resize(50);
//        Z[i].resize(5);
//    }



    for(int i = 0; i < 483; i++){
        w1.push_back(zero2);
    }
    for(int i = 0; i < 483; i++) {
        for(int j = 0; j < 50; j++){
            w1[i][j] = GaussRand(0,1);  // random
        }
    }
    vector< vector<double> > w2;

    for(int i = 0; i < 50; i++){
        w2.push_back(zero3);
    }
    for(int i = 0; i < 50; i++) {
        for(int j = 0; j < 5; j++){
            w2[i][j] = GaussRand(0,1);  // same, random
        }
    }
    vector<double> w3;
    for(int i = 0; i < 5; i++){
        w3.push_back(GaussRand(0,1));
    }

    // initialize weights's, and a trivial loss value
    // double loss = 0;
    // main recursion of GD
    double loss = 0;
    double new_loss = 0;
    double trial_loss = 0;
    double trial_loss_2 = 0;
    double loss_test = 0;
    vector<vector<double> > charlie;
    for (int i = 0; i < 483; i++){
        charlie.push_back(zero2);
    }

    vector<vector<double> > bravo;
    for (int i = 0; i < 50; i++){
        bravo.push_back(zero3);
    }


    vector<double> alpha;
    alpha = zero3;

    long double lambda;
    vector<vector<double> > v1;
    vector<vector<double> > v2;
    vector<double> v3;
    for(int _ = 0; _ < 25; _++) {
        calc_output(w1, w2, w3, data);
        new_loss = get_loss(S);

//        for(int t = 0; t < data.size(); t++) {
//            cout << S[t] << endl;
//        }
        cout << new_loss << endl;
      //  if(abs(loss - new_loss) / loss < 0.0001) {
        //    break;
   // }
        //loss = new_loss;
        // let's start calculating the gradient

        for (int i = 0; i < 483; i++) {
            for (int j = 0; j < 50; j++) {
                charlie[i][j] = 0;  // random
            }
        }

        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 5; j++) {
                bravo[i][j] = 0;  // same, random
            }
        }

        for (int i = 0; i < 5; i++) {
            alpha[i] = 0;
        }

        // iterate over the dates to calc the average gradient
        for (int t = 0; t < data.size(); t++) {
            // last layer gradient
            for (int i = 0; i < 5; i++) {
                alpha[i] += 2 * (F[t] - S[t]) * Z[t][i] / data.size();  // just to avoid infinite divisions in the end
            }
            // second layer gradient
            for (int i = 0; i < 50; i++) {
                for (int j = 0; j < 5; j++) {
                    bravo[i][j] += 2 * (F[t] - S[t]) * w3[j] * f_prime(Z[t][j]) * Y[t][i] / data.size();
                }
            }
            // first layer gradient
            for (int k = 0; k < 5; k++) {
                // printf("%d\n", k);
                for (int i = 0; i < 483; i++) {
                    for (int j = 0; j < 50; j++) {
                        charlie[i][j] += 2 * (F[t] - S[t]) *
                                         w3[k] * f_prime(Z[t][k]) * w2[k][j] * f_prime(Y[t][j]) * X[t][i] / data.size();
                    }
                }
            }

        }

        // w1 - charlie, w2 - bravo, w3 - alpha
        lambda = 1e-13;
        loss = new_loss;


        v1 = w1;
        v2 = w2;
        v3 = w3;

        for (int i = 0; i < 483; i++) {
            for (int j = 0; j < 50; j++) {
                v1[i][j] -= lambda * charlie[i][j];
            }
        }

        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 5; j++) {
                // cout << w2[i][j] << endl;
                v2[i][j] -= lambda * bravo[i][j];
            }
        }

        for (int i = 0; i < 5; i++) {
            v3[i] -= lambda * alpha[i];
            // cout << w3[i] << endl;
        }
        calc_output(v1, v2, v3, data);
        trial_loss = get_loss(S);
            while (trial_loss > loss) {
                lambda /= 2;
//                if (lambda < 1e-20){
//                    lambda = 1e-17;
//                    break;
//                }

                v1 = w1;
                v2 = w2;
                v3 = w3;
                for (int i = 0; i < 483; i++) {
                    for (int j = 0; j < 50; j++) {
                        v1[i][j] -= lambda * charlie[i][j];
                    }
                }

                for (int i = 0; i < 50; i++) {
                    for (int j = 0; j < 5; j++) {
                        // cout << w2[i][j] << endl;
                        v2[i][j] -= lambda * bravo[i][j];
                    }
                }

                for (int i = 0; i < 5; i++) {
                    v3[i] -= lambda * alpha[i];
                    // cout << w3[i] << endl;
                }
                calc_output(v1, v2, v3, data);
                trial_loss = get_loss(S);
                cout << trial_loss<< " "<< loss << endl;
                cout << "lambda" << ' '<< lambda << endl;
            }



            trial_loss_2 = trial_loss;
            while (trial_loss_2 <= trial_loss) {
                lambda /= 1.3;
                v1 = w1;
                v2 = w2;
                v3 = w3;
                for (int i = 0; i < 483; i++) {
                    for (int j = 0; j < 50; j++) {
                        v1[i][j] -= lambda * charlie[i][j];
                    }
                }

                for (int i = 0; i < 50; i++) {
                    for (int j = 0; j < 5; j++) {
                        // cout << w2[i][j] << endl;
                        v2[i][j] -= lambda * bravo[i][j];
                    }
                }

                for (int i = 0; i < 5; i++) {
                    v3[i] -= lambda * alpha[i];
                    // cout << w3[i] << endl;
                }
                calc_output(v1, v2, v3, data);
                trial_loss = trial_loss_2;
                trial_loss_2 = get_loss(S);

                cout << "lamda2" << ' ' << lambda << endl;
                cout << "trial_loss_2"<< trial_loss_2 << endl;

            }
            lambda *= 1.3;
            cout << " Lambda: "<< lambda << endl;
        for(int i = 0; i < 483; i++) {
            for(int j = 0; j < 50; j++) {
                w1[i][j] -= lambda * charlie[i][j];
            }
        }

        for(int i = 0; i < 50; i++) {
            for(int j = 0; j < 5; j++) {
                // cout << w2[i][j] << endl;
                w2[i][j] -= lambda * bravo[i][j];
            }
        }

        for(int i = 0; i < 5; i++) {
            w3[i] -= lambda * alpha[i];
            // cout << w3[i] << endl;
        }
    }

    loss_test = get_loss(S_test);
    cout << "lost_test"<<" "<<loss_test<< endl;
    return 0;
}

//    vector<double> v= {1, 2};
//    vector< vector<double>> w = {{1, 2}, {2, 3}};
//    vector<double> ans = matrix_multiply_array(w, v);
//    for (unsigned i = 0; i < ans.size(); i++) {
//        cout << ans[i] << " ";
//    }
