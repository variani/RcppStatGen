#include <Rcpp.h>
#include <RcppEigen.h>

#include <iostream>
#include<vector>

using namespace Rcpp;
using namespace Eigen;
using namespace std;

//' @export
// [[Rcpp::export]]
Eigen::MatrixXd eigen_inverse_chol(const Eigen::MatrixXd &X)
{
   int n = X.rows();
   MatrixXd Xinv = X.llt().solve(MatrixXd::Identity(n, n));
   return(Xinv);
}

//' @export
// [[Rcpp::export]]
Eigen::MatrixXd eigen_inverse_evd(const Eigen::MatrixXd &X,
    double tol = 1e-6)
{
  // eigen-values are sorted in increasing order, https://eigen.tuxfamily.org/dox/classEigen_1_1SelfAdjointEigenSolver.html#a3df8721abcc71132f7f02bf9dfe78e41
  // Example: [-3.09e-16, 0, 3]
   SelfAdjointEigenSolver<MatrixXd> solver(X);
   VectorXd D = solver.eigenvalues();
   MatrixXd V = solver.eigenvectors();

   int k = (D.array() > tol).count();

   cout << "D = " << D << endl;
   cout << "k = " << k << endl;
   cout << "Vk = " << V.rightCols(k) << endl;

   MatrixXd Dk_inverse = D.tail(k).array().inverse().matrix().asDiagonal();
   MatrixXd Xk = V.rightCols(k) * Dk_inverse * V.rightCols(k).transpose(); 

   return Xk;
}

//' @export
// [[Rcpp::export]]
Eigen::MatrixXd eigen_inverse_pseudo(const Eigen::MatrixXd &X)
{
   MatrixXd Xinv = X.completeOrthogonalDecomposition().pseudoInverse();
   return(Xinv);
}

