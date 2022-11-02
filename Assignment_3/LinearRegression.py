import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets

class LinearRegression:
    def __init__(self) -> None:
        self._coeff = None
        self._intercept = None
        

    def __repr__(self) -> str:
        return "Linear Regression Model from  Parent Class"

    def predict(self, X):
        """Output model prediction.

        Arguments:
        X: 1D or 2D numpy array
        """
        if len(X.shape) == 1: # check if X is 1D or 2D array
            X = X.reshape(-1,1) #reshape to column vector if 1D 

        
        self._predicted = self._intercept + np.dot(X, self._coeff)
        return self._predicted

        