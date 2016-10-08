import numpy as np
import random
import matplotlib.pyplot as plt
import sys

iteration = 1

def get_color(i):
   color = ''
   if i is 0:
      color = 'g'
   elif i is 1:
      color = 'r'
   elif i is 2:
      color = 'b'
   elif i is 3:
      color = 'c'
   elif i is 4:
      color = 'm'
   elif i is 5:
      color = 'y'
   else:
      color = 'k'
   return color

def init_uniform(N):
   X = np.array([(random.uniform(-1, 1), random.uniform(-1, 1)) for i in range(N)])
   return X

def init_gaussian(N, k):
   n = float(N)/k
   X = []
   plt.figure()
   for i in range(k):
      color=get_color(i)
      c = (random.uniform(-1, 1), random.uniform(-1, 1))
      s = random.uniform(0.05, 0.1)
      x =[]
      while len(x) < n:
         a, b = np.array([np.random.normal(c[0], s), np.random.normal(c[1], s)])
         if abs(a) < 1 and abs(b) < 1:
            x.append([a, b])
      X.extend(x)
      plt.plot(zip(*x)[0], zip(*x)[1], color+'s')
   plt.savefig('./data/original')
   X = np.array(X)[:N]
   return X

def init_panda(N, k):
   total = N
   X = []
   plt.figure()
   c = [(-0.2, 0.2),(0.2, 0.2), (0, 0)]
   for i in range(k):
      color=get_color(i)
      #c = (random.uniform(-1, 1), random.uniform(-1, 1))
      if i is not k-1:
         s = random.uniform(0.01, 0.05)
         n = float(N)/k/3
         total -= n;
      else:
         s = random.uniform(0.05, 0.1)
         n = total
      x =[]
      while len(x) < n:
         a, b = np.array([np.random.normal(c[i][0], s), np.random.normal(c[i][1], s)])
         if abs(a) < 1 and abs(b) < 1:
            x.append([a, b])
      X.extend(x)
      plt.plot(zip(*x)[0], zip(*x)[1], color+'s')
   plt.savefig('./data/original')
   X = np.array(X)[:N]
   return X

def init_diff_variance(N, k):
   n = N/2
   X = []
   plt.figure()
   c = [(0.2, 0.2),(-0.05, 0.15)]
   for i in range(k):
      color=get_color(i)
      #c = (random.uniform(-1, 1), random.uniform(-1, 1))
      if i is not k-1:
         s = random.uniform(0.01, 0.05)
      else:
         s = random.uniform(0.05, 0.1)
      x = []
      while len(x) < n:
         a, b = np.array([np.random.normal(c[i][0], s), np.random.normal(c[i][1], s)])
         if abs(a) < 1 and abs(b) < 1:
            x.append([a, b])
      X.extend(x)
      plt.plot(zip(*x)[0], zip(*x)[1], color+'s')
   plt.savefig('./data/original')
   X = np.array(X)[:N]
   return X

def has_converged(center, oldcenter):
   return (set([tuple(a) for a in center]) == set([tuple(a) for a in oldcenter]))

# compute points for each center forming clusters[centers]
def cluster_lists(X, center):
   clusters = {}
   for x in X:
      centerid = min([(i[0], np.linalg.norm(x-center[i[0]])) for i in enumerate(center)], key=lambda t:t[1])[0]
      try:
         clusters[centerid].append(x)
      except KeyError:
         clusters[centerid] = [x]
   return clusters

def check_centers(center, clusters):
   newcenter = []
   keys = sorted(clusters.keys())
   for k in keys:
      newcenter.append(np.mean(clusters[k], axis = 0))
   return newcenter

def compute_center(X, K):
   oldcenter = random.sample(X, K)
   center = random.sample(X, K)
   while not has_converged(center, oldcenter):
      global iteration
      oldcenter = center
      clusters = cluster_lists(X, center)
      center = check_centers(oldcenter, clusters)

      plt.figure()
      for i in range(K):
         color = get_color(i)
         plt.plot(zip(*clusters[i])[0], zip(*clusters[i])[1], color+'o')
         plt.plot(center[i][0],center[i][1], color+'*')
      #plt.plot(zip(*X)[0], zip(*X)[1], 'gs')
      #plt.plot(zip(*center)[0],zip(*center)[1],'yo')
      plt.savefig('./data/iteration'+str(iteration))
      iteration += 1
   return(center, clusters)

random.seed()
#X = init_panda(200, 3)
X = init_diff_variance(600, 2)
#X = init_gaussian(200, 3)
#X = init_uniform(200)
(center, clusters)= compute_center(X, 2)
#plt.plot(zip(*X)[0], zip(*X)[1], 'gs')
#plt.plot(zip(*center)[0],zip(*center)[1],'ro')
#plt.axis([-1, 1, -1, 1])
plt.show()
#print sys.argv
