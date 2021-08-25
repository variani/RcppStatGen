X <- as.matrix(iris[, 1:3])
y <- iris[, 4]

X1 <- X
X2 <- cbind(X[, c(1,1)], X)

P1 <- X2[, c(1, 4)] # selected
P2 <- X2[, c(2, 3, 5)] # unselected

U <- qr.Q(qr(P1))
P3 <- P2 - U %*% crossprod(U, P2)

# Dataset #2
data(X16) # X16, sel, unsel

P1 <- X16[, sel]
P2 <- X16[, unsel]

U <- qr.Q(qr(P1))
P3 <- P2 - U %*% crossprod(U, P2)

try(eigen_qri_keep(X16, sel))

length(eigen_qr_keep(P3)) + length(sel)
qr(X16)$rank



