import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
import sys 
red_black = LinearSegmentedColormap.from_list('GreenBlack',[(0,0,0),(0,1,0)])
# Load the matrix from the text file
matrix = np.loadtxt(sys.argv[1])
matrix = matrix.astype(int)
 
# Get the shape of the matrix
rows, cols = matrix.shape
figsize = (matrix.shape[1]/4 ,matrix.shape[0]/4)
fig,ax = plt.subplots(figsize = figsize)
ax.matshow(matrix, cmap=red_black,vmin = matrix.min(),vmax = matrix.max())
# Loop over each element in the matrix and display its value
for row in range(rows):
    for col in range(cols):
        if matrix[row][col] > matrix.max()/2:
            color = 'black'
        else:
            color = 'white'
        ax.text(col, row, str(matrix[row][col]), color=color, fontsize=10 - matrix.shape[0]//8,
                 horizontalalignment='center', verticalalignment='center')
 
# Show the plot
# Plot the matrix as an image
plt.show()
