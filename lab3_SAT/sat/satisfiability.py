import random

import matplotlib.pyplot as plt
import numpy as np
import pycosat


def calculate_SAT_probabilities_and_plot() -> None:
    """
    Calculate probability of random formula being satisfiable based on its
    size n (number of variables). The result is plotted and shown.

    Conclusion: TODO write conclusion
    """

    T = 100
    step = 0.1 
    # k = random.choice(range(1,3))
    k = 3 #SAT_CNF3

    n = 50
    Sign = [1,-1]            # lista +/-
    V = range(1,n+1)      # lista zmiennych 1...n
    
    a = 1
    end = 10
    res = []
    with open("output.txt", 'w') as file:
        while a < end:
            S = 0
            for _ in range(T):
                cnf = []
            
                for _ in range(int(a*n)):
                    tab = [random.choice(V)*random.choice(Sign) for _ in range(k)]
                    cnf.append(tab)

                sol = pycosat.solve(cnf)
                if (sol != "UNSAT"):
                    S += 1
            
            file.write(f"{a:.2f} {S/T}\n")
            print(f"{a:.2f}, {S/T}")
            res.append(f"{a:.2f}, {S/T}")
            a+=step
    
    plt.plot([float(x.split(",")[0]) for x in res], [float(x.split(",")[1]) for x in res])
    plt.show()