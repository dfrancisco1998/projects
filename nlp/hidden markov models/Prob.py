import numpy as np
from scipy.special import logsumexp

class Prob():
    """
        Class to represent probabilities internally as log probabilities.

        Notes:
        -Probabilities of 0 are treated as the smallest possible float64 in Python, 5e-324.
        -All probabilities are stored internally as logs but converted to standard probabilities for output of
         operations such as addition, multiplication, or just general access.
    """

    def __init__(self, p, space='prob'):
        self.__zero_prob = -np.inf#np.log(np.nextafter(0, 1)) # Constant value used to represent 0 probability
        self.prob = None

        if space == 'prob':
            assert p >= 0
            self.prob = np.log(p) if p != 0 else self.__zero_prob
        elif space == 'log':
            self.prob = p

        assert self.prob is not None

    def get_real_prob(self):
        return np.exp(self.prob) if self.prob != self.__zero_prob else 0.0

    def get_log_prob(self):
        return self.prob

    def get_log2_prob(self):
        return self.prob / np.log(2)

    def __add__(self, other):
        assert isinstance(other, Prob)
        if self.prob == other.prob == self.__zero_prob:
            p = self.__zero_prob
        else:
            #p = logsumexp([self.prob, other.prob])
            p = np.logaddexp(self.prob, other.prob)
        return Prob(p, space='log')

    def __mul__(self, other):
        assert isinstance(other, Prob)
        p = self.prob + other.prob
        return Prob(p, space='log')

    def __truediv__(self, other):
        assert isinstance(other, Prob)
        p = self.prob - other.prob
        return Prob(p, space='log')

    def max(self, other):
        assert isinstance(other, Prob)
        if self.prob == other.prob == self.__zero_prob:
            return Prob(0.0, space='prob')
        return Prob(max(self.prob, other.prob), space='log')

    def __gt__(self, other):
        assert isinstance(other, Prob)
        return self.prob > other.prob

    def __ge__(self, other):
        assert isinstance(other, Prob)
        return self.prob >= other.prob

    def __lt__(self, other):
        assert isinstance(other, Prob)
        return self.prob < other.prob

    def __le__(self, other):
        assert isinstance(other, Prob)
        return self.prob <= other.prob

    def __eq__(self, other):
        assert isinstance(other, Prob)
        return self.prob == other.prob

    def __ne__(self, other):
        assert isinstance(other, Prob)
        return self.prob != other.prob

    def __str__(self):
        return str(np.exp(self.prob)) if self.prob != self.__zero_prob else str(0.0)


if __name__ == '__main__':
    P1 = Prob(0)
    print(P1.get_log_prob())
    #P2 = Prob(0.)
    #print(P1 + P2)
    #print(P1 * P2)
    #print(P1.max(P2))
    #print(P1 != P2)
