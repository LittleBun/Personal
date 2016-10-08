def collect_data(filename='training_points.txt'):
  f = open(filename, 'r')
  points =[]
  for line in f:
    point =[]
    for word in line.split():
      point.append(word)
    points.append(point)
# print points
  return points

def calc_weight(points, w, rate):
  error = 0
  output = 0
  for p in points:
    y = w[0] * p[0] + w[1]
    if p[1] > y:
      output = 0
    else:
      output = 1
    if output != p[2]:
      w[0] += rate * (p[1] - y) * p[0]
      w[1] += rate * (p[1] - y)
  return w

def perceptron(filename='training_points.txt', rate=0.1):
  #Initial parameter
  points = collect_data(filename)
  weight = [int(0),int(0)]
  it = 0
#Iterate until no misclassified examples:
  new_weight = calc_weight(points, weight, rate)
  while new_weight != weight:
    it += 1
    if it > 300:
      print("No solution\n")
      break
    weight = new_weight
    new_weight = calc_weight(points, weight, rate)
  print("solution: y = %f*x + %f" %(weight[0], weight[1]))

perceptron();
    
