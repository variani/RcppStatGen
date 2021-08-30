X <- as.matrix(iris[, 1:3])
y <- iris[, 4]

X1 <- X
X2 <- cbind(X[, c(1,1)], X)

C1 <- cor(X1)
C2 <- cor(X2)

# R version
C2 <- matrix(5, 5, 5)
out <- eigen(C2)
D <- out$values
V <- out$vectors
sel <- which(D > 1e-6)
C2r <- V[, sel, drop = FALSE] %*% diag(D[sel], length(sel)) %*% t(V[,sel, drop = FALSE])
C2i <- V[, sel, drop = FALSE] %*% diag(1/D[sel], length(sel)) %*% t(V[,sel, drop = FALSE])
# D[D < 1e-6] <- 1e-6
# C2i <- V %*% diag(1/D) %*% t(V)
P2 <- (C2 %*% C2i) %>% round(2)

# X16 %>% eigen_qr_keep(1e-14) %>% X16[, .] %>% cor -> C
# (C %*% eigen_inverse_evd(C)) %>% round(2)
