X <- as.matrix(iris[, 1:3])
y <- iris[, 4]

X1 <- X
X2 <- cbind(X[, c(1,1)], X)

## eigen_qri 
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
