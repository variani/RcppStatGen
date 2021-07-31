#include <Rcpp.h>
#include <RcppEigen.h>

#include <iostream>
#include<vector>

using namespace Rcpp;
using namespace Eigen;
using namespace std;

template<typename Derived>

void _eigen_print_sum(const Eigen::MatrixBase<Derived>& X)
{
  cout << X.array().sum() << endl;
}

//' @export
// [[Rcpp::export]]
void eigen_print_sum(SEXP X)
{
  switch (TYPEOF(X)) {
    case INTSXP: { _eigen_print_sum(Rcpp::as<MatrixXi>(X)); break; }
    case REALSXP: { _eigen_print_sum(Rcpp::as<MatrixXd>(X)); break; }
    default: Rcpp::stop("incompatible SEXP encountered");
  }
}
