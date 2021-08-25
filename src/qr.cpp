#include <Rcpp.h>
#include <RcppEigen.h>

#include <iostream>
#include<vector>

using namespace Rcpp;
using namespace Eigen;
using namespace std;

typedef Array<bool,Dynamic,1> ArrayXb;

//' @export
// [[Rcpp::export]]
Eigen::ArrayXi eigen_qr_keep(const Eigen::MatrixXd &X,
    double tol = 1e-7, int verbose = 0)
{
  ArrayXi cols_keep;
  int rank;

  // find set of linearly independent cols
  ColPivHouseholderQR<MatrixXd> qrA(X);
  qrA.setThreshold(tol); 
  rank = qrA.rank();

  // error: rank = 0
  if(rank == 0) {
    throw std::range_error("rank returned by QR is 0");
  } else {
    cols_keep = qrA.colsPermutation().indices().head(rank);
  }

  // return
  cols_keep += 1; // indices in R starts with 1 rather than 0

  return(cols_keep);
}

//' @export
// [[Rcpp::export]]
Eigen::ArrayXi eigen_qri_keep(const Eigen::MatrixXd &X0,
    Eigen::ArrayXi cols_selected,
    double tol = 1e-7, int verbose = 0)
{
  int n = X0.rows(), p = X0.cols();
  MatrixXd X(n, p);
  ArrayXi cols_keep;
  int rank;

  // reorder columns in X accroding to cols_selected
  int p_selected = cols_selected.size();
  // convert from R column indices to C++ indices
  cols_selected -= 1;
  // p-length array of boolean status: selected or not selected
  ArrayXb status_selected = ArrayXb::Constant(p, false);
  for(int i = 0; i < p_selected; i++) {
    int sel = cols_selected[i];
    // check 1: selected columns are in range [0; p)
    if((sel < 0) | (sel >= p)) {
      throw std::range_error("some selected columns are out of range [0; p)");
    }
    status_selected[cols_selected[i]] = true;
  }
  // p-length array of new order of columns: selected columns go first
  ArrayXi cols_reordered(p);
  for(int i = 0, i_selected = 0, i_unselected = 0; i < p; i++) {
    if(status_selected[i]) {
      cols_reordered[i_selected++] = i;
    } else {
      cols_reordered[p_selected + i_unselected++] = i;
    }
  }
  if(verbose) {
    cout << " new order of columns:\n" << cols_reordered << endl;
  }
  // fill X, i.e., swap columns in input X0 accroding to cols_reordered & store in X
  for(int i = 0; i < p; i++) {
    X.col(i) = X0.col(cols_reordered[i]);
  }

  // find set of linearly independent cols
  ColPivHouseholderQR<MatrixXd> qrA(X);
  qrA.setThreshold(tol); 
  rank = qrA.rank();

  // error: rank = 0
  if(rank == 0) {
    throw std::range_error("rank returned by QR is 0");
  } else {
    cols_keep = qrA.colsPermutation().indices().head(rank);
  }

  // update cols_keep according to cols_reordered &
  // status_keep: columns that are kept by QR 
  ArrayXb status_keep = ArrayXb::Constant(p, false);
  for(int i = 0; i < rank; i++) {
    cols_keep[i]= cols_reordered[cols_keep[i]];
    // fill status
    status_keep[cols_keep[i]] = true;
  }

  // check 2: all selected columns are kept &
  // final list of columns are ordered
  for(int i = 0, i2 = 0; i < p; i++) {
    if(status_selected[i]) {
      if(!status_keep[i]) {
        throw std::range_error("some selected columns were not kept by QR");
      }
    }
    if(status_keep[i]) {
      cols_keep[i2++] = i;
    }
  }
  
  // return
  cols_keep += 1; // indices in R starts with 1 rather than 0

  return(cols_keep);
}
