from LinearRegression import *

class LinearRegression1D(LinearRegression):
    
    def fit(self,X,y):
        m = len(X)
        y = y.reshape(-1,1) #Make y a column vector
        self._coeff = ((m*(np.dot(X.T,y)))-((np.sum(X))*(np.sum(y))))/((m*(np.dot(X.T,X)))-((np.sum(X))**2))
        self._intercept = (((np.sum(y))*(np.dot(X.T,X)))-((np.sum(X))*(np.dot(X.T,y))))/((m*(np.dot(X.T,X)))-((np.sum(X))**2))
        print(f"Fit intercept: {self._intercept}\n")
        print(f"Fit coefficients: {self._coeff}\n")