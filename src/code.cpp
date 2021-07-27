#include <Rcpp.h>
#include <RcppEigen.h>

#include <iostream>
#include<vector>

using namespace Rcpp;
using namespace Eigen;
using namespace std;

typedef Array<bool,Dynamic,Dynamic> ArrayXb;

//' @export
// [[Rcpp::export]]
void eigen_version()
{
  cout << "Eigen library version: " << EIGEN_WORLD_VERSION << "." <<
    EIGEN_MAJOR_VERSION << "." << EIGEN_MINOR_VERSION << endl;
}

//' @export
// [[Rcpp::export]]
Eigen::MatrixXd eigen_scale(Eigen::MatrixXd X0, int verbose = 0)
{
  MatrixXd X = X0;
  const int n = X.rows(), p = X.cols();

  // init
  ArrayXb M = X.array().isNaN(); // boolean mask matrix: is X[i,j] NAN?
  ArrayXi n_nan = M.cast<int>().colwise().sum(); // Number of NAN per column
  ArrayXi n_val(p);
  n_val.fill(n);
  n_val -= n_nan;
  if(verbose > 1) {
    cout << "Input Matrix X:" << endl << X << endl;
    cout << "Mask Matrix M (NAN):" << endl << M << endl;
    cout << " -- " << n_nan.transpose() << endl;
    cout << " -- " << n_val.transpose() << endl;
  }


  // center
  /* VectorXd means = X.colwise().mean(); */
  ArrayXd sums = M.select(0.0, X).colwise().sum();
  ArrayXd means = sums / n_val.cast<double>();
  if(verbose) {
    cout << "means = " << means.transpose() << endl;
  }
  X.array().rowwise() -= means.transpose();

  // scale
  /* ArrayXd sds = X.colwise().norm() / sqrt(p - 1); */ 
  ArrayXd norms = M.select(0.0, X).colwise().norm();
  ArrayXd sds = norms / (n_val - 1).cast<double>().sqrt();
  if(verbose) {
    cout << "norms = " << norms.transpose() << endl;
    cout << "sds = " << sds.transpose() << endl;
  }
  bool any_sds0 = (sds == 0.0).any();
  if(any_sds0) {
    throw std::range_error("some sds are zero");
  }
  X.array().rowwise() /= sds.transpose();

  return X;
}

// eigen_scale_naive(matrix(1:4, 2, 2))
//' @export
// [[Rcpp::export]]
Eigen::MatrixXd eigen_scale_naive(Eigen::MatrixXd X0)
{
  MatrixXd X = X0;
  const int p = X.cols();

  // center
  ArrayXd means = X.colwise().mean();
  X.array().rowwise() -= means.transpose();

  // scale
  ArrayXd sds = X.colwise().norm() / sqrt(p - 1); 
  X.array().rowwise() /= sds.transpose();

  return X;
}

/* // [[Rcpp::export]] */
/* Eigen::MatrixXd eigen_scale_naive_matrix(Eigen::MatrixXd X) */
/* { */
/*   const int p = X.cols(); */

/*   // center */
/*   MatrixXd means = X.colwise().mean(); */
/*   X.array().rowwise() -= means.transpose().array(); */

/*   // scale */
/*   MatrixXd sds = X.colwise().norm() / sqrt(p - 1); */ 
/*   X.array().rowwise() /= sds.transpose().array(); */ 

/*   return X; */
/* } */
