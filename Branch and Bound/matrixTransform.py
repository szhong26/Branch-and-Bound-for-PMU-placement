
# coding: utf-8

from scipy import io
matfile = './Theta.mat'
mat = io.loadmat(matfile)


theta = mat['Theta']


outfile = open('/home/laozzzzz/matrix.txt','w')
row,col = theta.shape
outfile.write(str(row)+'\t')
outfile.write(str(col)+'\t')
outfile.write('\t'.join(map(str,theta.flatten())) + '\n')
outfile.close()



