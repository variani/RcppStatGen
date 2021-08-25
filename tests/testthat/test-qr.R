# Dataset 1: iris
X <- as.matrix(iris[, 1:3])
y <- iris[, 4]

X1 <- X
X2 <- cbind(X[, c(1,1)], X)

# Dataset 2: eigen_qri_keep fails, eigen_qrp_keep works
data(X16) # X16, sel, unsel

## 4. eigen_qrp_keep works on X16
test_that("eigen_qrp_keep", {
  expect_equal(eigen_qrp_keep(X2, c(2, ncol(X2))), 4)
  expect_equal(eigen_qrp_keep(X1, 1:2), 3)
})

## 3. eigen_qrp_keep works on X16
test_that("X16", {
  rank_r <- qr(X16)$rank
  keep <- eigen_qrp_keep(X16, sel)
  rank_eigen <- length(sel) + length(keep)
  expect_equal(rank_r, rank_eigen)
})

## 2. eigen_qri_keep fails on X16
test_that("X16", {
  expect_error(eigen_qri_keep(X16, sel), "not kept")
})

## 1. eigen_qri_keep works
test_that("range of selected columns", {
  expect_error(eigen_qri_keep(X2, 0), "range")
  expect_error(eigen_qri_keep(X2, ncol(X2) + 1), "range")
})

## eigen_qri works
test_that("range of selected columns", {
  expect_error(eigen_qri_keep(X2, 0), "range")
  expect_error(eigen_qri_keep(X2, ncol(X2) + 1), "range")
})

test_that("selected columns are not kept by QR", { 
  expect_error(eigen_qri_keep(X2, 1:2), "not kept")
})

test_that("order", { 
  keep <- eigen_qri_keep(X2, c(2, ncol(X2)))
  diff <- diff(order(keep))
  expect_true(all(diff > 0))
})
