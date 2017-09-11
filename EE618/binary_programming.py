import numpy as np
import matplotlib.pyplot as plt

N = 10
M = 9


def calculate(V, q):
    alphas = []
    betas = []
    ymin = 0
    ymax = 0
    for j in range(0, M, 1):
        ymin = np.log(V[j] * (np.prod(q[:, j])))
        ymax = np.log(V[j])

        A = [
            [(2./3.) * (np.power(ymax, 3) - np.power(ymin, 3)), (np.power(ymax, 2) - np.power(ymin, 2))],
            [(1.) * np.power(ymax, 2) - np.power(ymin, 2), 2. * (ymax - ymin)]
        ]

        B = [2 * (np.exp(ymax) * (ymax - 1.) - np.exp(ymin) * (ymin - 1.)), 2 * (np.exp(ymax) - np.exp(ymin))]

        # answer = np.linalg.inv(A) * B
        answer = np.linalg.solve(np.array(A), np.array(B))
        alphas.append(answer[0])
        betas.append(answer[1])
    return ymin, ymax, alphas, betas


def main():
    V = np.arange(1., 1.+M)
    p = abs(np.sin(np.tile(np.arange(1.,1.+N)[:, np.newaxis], (1, M))) * np.sin(np.tile(np.arange(1., 1.+M), (N, 1))))
    q = 1 - p

    ymin, ymax, alphas, betas = calculate(V, q)

    y = np.arange(ymin, ymax, 0.01)
    plt.plot(y, np.exp(y), y, np.array(alphas[M-1]) * (y) + np.array(betas[M-1]))
    plt.show()



if __name__ == "__main__":
    main()