import matplotlib.pyplot as plt
import numpy as np

FILES = ['2x2', '2x3', '2x4', '3x3']
TIMES = [0, 0, 0, 0]
STDS = [0, 0, 0, 0]

for ind, FILE in enumerate(FILES):
    F = np.loadtxt('out/' + FILE + '.txt')
    TIMES[ind] = np.mean(F)
    STDS[ind] = np.std(F, ddof=1)

for TIME in TIMES:
    print(TIME)
    
for STD in STDS:
    print(STD)
    
plt.title('Média e Desvio Padrão do Sudoku')
plt.ylabel('Tempo em μs')
plt.xlabel('Tamanho do Quadrante')
plt.xticks(np.arange(4), FILES)
plt.plot(TIMES, label='Média')
plt.plot(STDS, label='Desvio Padrão')
plt.legend(loc='best')
plt.savefig('plot.png', bbox_inches='tight', dpi=300)