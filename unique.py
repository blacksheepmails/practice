import math
import p042_words
import cProfile
from functools import reduce
from random import randint
import threading
from multiprocessing import Pool


def isAnagram(s1,s2):
	return sorted(s1) == sorted(s2)
def isSubstring(s,xs):
	for i in range(0,len(xs)-len(s)):
		if xs[i:len(s)+i] == s:
			return True
	return False

def isRotation(s1,s2):
	return isAnagram(s1,s2) and isSubstring(s1,s2+s2)
def removeDupe(xs):
	newxs=list(xs)
	hashset={}
	numdupe=0
	for i in range(0,len(xs)):
		if xs[i] in hashset:
			newxs.pop(i-numdupe)
			numdupe+=1
		else:
			hashset[xs[i]] = 'weepy'
	return ''.join(newxs)
class Stack():
	def __init__(self, initialStack=None):
		self.stack = [] if initialStack==None else initialStack
	def push(self, e):
		self.stack.append(e)
	def pop(self):
		return self.stack.pop()
	def peek(self):
		return self.stack[-1]
	def isEmpty(self):
		return self.stack == []
	def __str__(self):
		return 'Stack('+str(self.stack)+')'
	def __repr__(self):
		return str(self)

class StackQ():
	def __init__(self):
		self.q = Stack()
	def push(self, e):
		self.q.push(e)
	def pop(self):
		temp = Stack()
		while not self.q.isEmpty():
			temp.push(self.q.pop())
		ans = temp.pop()
		while not temp.isEmpty():
			self.q.push(temp.pop())
		return ans
	def __str__(self):
		return 'StackQ(' + str(self.q) + ')'
	def __str__(self):
		return str(self)

def mergeSort(xs):
	def merge(l1, l2):
		if len(l1) == 0:
			return l2
		if len(l2) == 0:
			return l1
		if l1[0] < l2[0]:
			return [l1[0]] + merge(l1[1:],l2)
		else:
			return [l2[0]] + merge(l2[1:],l1)
	if len(xs) <= 1:
		return xs
	middle = len(xs)//2
	return merge(mergeSort(xs[0:middle]), mergeSort(xs[middle:]))

class Heap():
	def __init__(self, initialHeap=[]):
		self.heap = []
		for x in initialHeap:
			self.insert(x)
	def getParentIndex(self,i): #should be private
		return (i-1)//2
	def getChild1Index(self,i):
		return i*2+1
	def getChild2Index(self,i):
		return i*2+2
	def isRoot(self,i):
		return i == 0
	def isLeaf(self,i):
		return self.getChild1Index(i) >= len(self.heap)
	def hasChild2(self,i):
		return self.getChild2Index(i) < len(self.heap)
	def heapDown(self,i):
		if self.isLeaf(i):
			return
		swapIndex = None
		if self.heap[i] < self.heap[self.getChild1Index(i)]:
			swapIndex = self.getChild1Index(i)
		if self.hasChild2(i) and self.heap[i] < self.heap[self.getChild2Index(i)] and self.heap[self.getChild1Index(i)] < self.heap[self.getChild2Index(i)]:
			swapIndex = self.getChild2Index(i)
		if swapIndex != None:
			temp = self.heap[i]
			self.heap[i] = self.heap[swapIndex]
			self.heap[swapIndex] = temp
			self.heapDown(swapIndex)
	def heapUp(self,i):
		if self.isRoot(i):
			return
		upIndex = self.getParentIndex(i)
		if self.heap[i] > self.heap[upIndex]:
			temp = self.heap[i]
			self.heap[i] = self.heap[upIndex]
			self.heap[upIndex] = temp
			self.heapUp(upIndex) 
	def insert(self, x):
		self.heap.append(x)
		self.heapUp(len(self.heap)-1)
	def buildHeap(self,xs):
		self.heap = xs
		for i in range(0, len(self.heap)).reversed():
			heapDown(i)
	def pop(self):
		temp = self.heap[0]
		self.heap[0] = self.heap.pop()
		self.heapDown(0)
		return temp
	def merge(self, heap):
		self.buildHeap(self.heap.concat(heap))

def quicksort(xs):
	if len(xs) <= 1:
		return xs
	pivot = xs[0]
	before = []
	after = []
	pivots = []
	for x in xs:
		if x < pivot:
			before.append(x)
		elif x > pivot:
			after.append(x)
		else:
			pivots.append(x)
	return quicksort(before) + pivots + quicksort(after)

def findKth(k,xs): # O(n) find median
	if len(xs) <= 1:
		return xs
	pivot = xs[0]
	before = []
	after = []
	pivots = []
	for x in xs:
		if x < pivot:
			before.append(x)
		elif x > pivot:
			after.append(x)
		else:
			pivots.append(x)
	if k <= len(before):
		return findKth(k,before)
	if k <= len(before) + len(pivots):
		return pivot
	else:
		return findKth(k-len(before)-len(pivots), after)

# print(findKth(13,[4,2,6,8,4,2,8,9,0,5,6,22,4,1,54,13]))
# print(quicksort([4,2,6,8,4,2,8,9,0,5,6,22,4,1,54,13]))

class IntervalNode():
	def __init__(self, interval):
		self.interval = interval
		self.maxEndpoint = interval[1]
		self.left = None
		self.right = None
		self.parent = None
class IntervalSearchTree():
	def __init__(self, initialTree=[]):
		self.tree = None
		for x in initialTree:
			self.insert(x)
	def insert(self, interval):
		node = IntervalNode(interval)
		if self.tree == None:
			self.tree = node
			return
		currNode = self.tree
		while True:
			currNode.maxEndpoint = max(currNode.maxEndpoint, node.maxEndpoint)
			if interval[0] <= currNode.interval[0]:
				if currNode.left == None:
					currNode.left = node
					node.parent = currNode
					return
				currNode = currNode.left
			else:
				if currNode.right == None:
					currNode.right = node
					node.parent = currNode
					return
				currNode = currNode.right
	def replaceNode(self,node,replacement):
		if node.parent.left == node:
			node.parent.left = replacement
		else:
			node.parent.right = replacement
	def delete(self, interval):
		if self.tree == None:
			return
		currNode = self.tree
		while currNode != None:
			if currNode.interval == interval:
				if currNode.left == None and currNode.right == None:
					self.replaceNode(currNode, None)
				elif currNode.left == None or currNode.right == None:
					child = currNode.left if currNode.right == None else currNode.right
					child.parent = currNode.parent
					self.replaceNode(currNode, child)
				else:
					newNode = self.getSuccessor(currNode)
					self.delete(newNode.interval)
					newNode.parent = currNode.parent
					newNode.left = currNode.left
					newNode.right = currNode.right
					self.replaceNode(currNode, newNode)
				return
			if currNode.maxEndpoint == interval[1]:
				currNode.maxEndpoint = max(currNode.left.maxEndpoint, currNode.right.maxEndpoint) #maybe should check things arent null
			currNode = currNode.left if interval[0] <= currNode.interval[0] else currNode.right
	def getSuccessor(self, node):
		succ = node.right
		while succ.left != None:
			succ = succ.left
		return succ
	def search(self, interval, root='root'): #maybe an implementation using a heap would be better
		currNode = self.tree if root == 'root' else root
		if currNode == None:
			return []
		if currNode.maxEndpoint < interval[0]:
			return [] #no intersections in entire tree rooted at currNode
		if currNode.interval[0] > interval[0]:
			if currNode.interval[0] > interval[1]:
				return self.search(interval, currNode.left)
			else:
				return self.search(interval, currNode.left) + self.search(interval, currNode.right) + [currNode]
		else:
			if currNode.interval[1] < interval[0]:
				return self.search(interval, currNode.left) + self.search(interval, currNode.right)
			else:
				return self.search(interval, currNode.left) + self.search(interval, currNode.right) + [currNode]

a = IntervalSearchTree([(4,7),(2,5),(6,8),(1,6),(1,2),(5,9),(4,9),(3,4),(8,11),(7,8)])
# print([x.interval for x in a.search([3,5])])


def sumTo(n, coins): #euler 31
	if len(coins) == 1 or n == 0:
		return 1
	ans = 0
	for taken in range(0, n+1, coins[0]):
		remaining = n-taken
		ans += sumTo(remaining, coins[1:])
	return ans
# print(sumTo(200, [200,100,50,20,10,5,2,1]))

def digits(n):
	return map(int, list(str(n)))
def factorial(n):
	ans = 1
	for i in range(1, n+1):
		ans *= i
	return ans
def sumFactorialDigits(n):
	return sum(map(factorial, digits(n)))
def euler34():
	ans = 0
	for n in range(10, 2540160):
		if sumFactorialDigits(n) == n:
			ans += n
	return ans

def maximalSumSubarray(xs):
	maxMemo = xs[:]
	arrayMemo = [[x] for x in xs]
	for i in range(1, len(xs)):
		if maxMemo[i-1] > 0:
			maxMemo[i] = maxMemo[i-1]+xs[i]
			arrayMemo[i] = arrayMemo[i-1][:]
			arrayMemo[i].append(xs[i])
		else:
			maxMemo[i] = xs[i]
			arrayMemo[i] = [xs[i]]
	return arrayMemo[maxMemo.index(max(maxMemo))]

xs = [1,-3,5,-2,9,-8,-6,4]
# print(maximalSumSubarray(xs))

def knapsack(xs, size):
	memoMax = [[x[1] if x[0] <= s else 0 for x in xs] for s in range(size+1)]
	memoItems = [[[x] if x[0] <= s else [] for x in xs] for s in range(size+1)]
	for s in range(1,size+1):
		for i in range(1, len(xs)):
			withoutItem = memoMax[s][i-1]
			if s-xs[i][0] >= 0:
				withItem = memoMax[s - xs[i][0]][i-1] + xs[i][1]
			else:
				withItem = 0
			if withItem > withoutItem:
				memoMax[s][i] = withItem
				memoItems[s][i] = memoItems[s-xs[i][0]][i-1][:]
				memoItems[s][i].append(xs[i])
			else:
				memoMax[s][i] = withoutItem
				memoItems[s][i] = memoItems[s][i-1][:]
	print(memoItems)
	print(max(memoMax[size]))
	return memoItems[size][memoMax[size].index(max(memoMax[size]))]
sizeWeight = [(2,3),(4,3),(3,2),(3,3),(4,3),(2,1),(1,1),(5,6),(4,6),(3,4)]
#print(knapsack(sizeWeight, 5))

def euler39():
	d = {}
	for a in range(1, 999):
		for b in range(1, a):
			c = math.sqrt(a*a+b*b)
			if abs(c-round(c)) < 0.0000000001:
				p = a+b+c
				if p <= 1000:
					if p in d:
						d[p].append((a,b,c))
					else:
						d[p] = [(a,b,c)]
				else:
					break
	maxSetSize = 0
	ans = 120
	for p in d:
		if len(d[p]) > maxSetSize:
			ans = p
			maxSetSize = len(d[p])
	return ans

def permute(xs):
	if len(xs) <= 1:
		return [xs]
	permutations = []
	for x in xs:
		xsCopy = xs[:]
		xsCopy.remove(x)
		ps = permute(xsCopy)
		for p in ps:
			p.append(x)
		permutations += ps
	return permutations
def digitsToInt(xs):
	ans = 0
	for x in xs:
		ans = ans*10 + x
	return ans
def euler32():
	permutations = permute([1,2,3,4,5,6,7,8,9])
	products = set()
	for p in permutations:
		a1 = digitsToInt(p[:2])
		b1 = digitsToInt(p[2:5])
		a2 = digitsToInt(p[:1])
		b2 = digitsToInt(p[1:5])
		p = digitsToInt(p[5:9])
		if a1*b1 == p or a2*b2 == p:
			products.add(p)
	return sum(products)
def euler43():
	def isSpecial(p):
		if p[0] == 0:
			return False
		for i in range(1,8):
			if digitsToInt(p[i:i+3]) % primes[i-1] != 0:
				return False
		return True
	specials = []
	permutations = permute([0,1,2,3,4,5,6,7,8,9])
	primes = [2,3,5,7,11,13,17]
	for p in permutations:
		if isSpecial(p):
			specials.append(digitsToInt(p))
	return sum(specials)
def isTriangle(t):
	n = int(math.sqrt(t*2))
	return n*(n+1) == t*2
def euler42():
	def value(xs):
		return sum([ord(x)-ord('A')+1 for x in xs])
	return len([w for w in p042_words.words if isTriangle(value(w))])
def pentagonal(n):
	return (n*(3*n-1))//2
def euler44():
	memoSet = set()
	memo = {}
	for i in range(1,50000):
		memoSet.add(pentagonal(i))
		memo[i] = pentagonal(i)
	for x in range(1,10000):
		d = memo[x]
		for y in range(1,5000):
			py = memo[y]
			if d+2*py in memoSet and d+py in memoSet:
				return d
	return None
def euler52():
	for i in range(1, 10000000):
		ss = sorted(str(i))
		c = i
		for k in range(2,7):
			c += i
			if ss != sorted(str(c)):
				break
		if k == 6:
			return i
	return None
def triangular(n):
	return (n*(n-1))//2
def hexagonal(n):
	return n*(2*n-1)
def isPentagon(p):
	n = int(1+math.sqrt(1+24*p)/6)
	return p == (n*(3*n-1))//2
def euler45():
	i = 144
	while True:
		n = hexagonal(i)
		if isPentagon(n) and isTriangle(n):
			return n
		i += 1
def euler40():
	c = 0
	xs = []
	while len(xs) <= 1000000:
		xs.extend(str(c))
		c += 1
	return int(xs[1]) * int(xs[10]) * int(xs[100]) * int(xs[1000]) * int(xs[10000]) * int(xs[100000]) * int(xs[1000000])
def euler53():
	memo = {1:1}
	def fastFactorial(n):
		if n in memo:
			return memo[n]
		return n * fastFactorial(n-1)
	def choose(n,r):
		return fastFactorial(n)//(fastFactorial(r)*fastFactorial(n-r))
	count = 0
	for n in range(23,101):
		for r in range(1,n):
			if choose(n,r) > 1000000:
				count += 1
	return count
def euler38():
	digits = set(['1','2','3','4','5','6','7','8','9'])
	def lte(xs, digits):
		sxs = set(xs)
		return len(sxs) == len(xs) and len(sxs - digits) <= 0
	def panm(n, m, digits):
		nm = str(n*m)
		currDigits = set(digits)
		if not lte(nm, digits):
			return ''
		return nm + panm(n, m+1, digits-set(nm))
	def defaultZero(x):
		s = ''.join(x)
		return 0 if s == '' else int(s)
	ans = 0
	for i in range(1,100000):
		temp = ans
		ans = max(defaultZero(panm(i, 1, digits)), ans)
	return ans

def primes(n):
	ps = [True] * n
	ps[0] = ps[1] = False
	for (p, isPrime) in enumerate(ps):
		if isPrime:
			yield p
			for k in range(p*p, n, p):
				ps[k] = False
def euler27():
	primesList = list(primes(100000))
	ans=(-1, None)
	for a in range(-999,1000):
		for b in primes(1000):
			x = 0
			while x*x + a*x + b in primesList:
				x+=1
			if x > ans[0]:
				ans = (x, (a,b))
				print(ans)

def euler35():
	ps = set(primes(1000000))
	def isCircularPrime(n):
		for r in rotations(str(n)):
			if int(r) not in ps:
				return False
		return True
	def rotations(xs):
		return [xs[i:]+xs[:i] for i in range(len(xs))]
	c = 0
	for p in ps:
		if isCircularPrime(p):
			c+=1
	return c

def euler37():
	c = 0
	ps = set(primes(1000000))
	tp = []
	def truncations(xs):
		return [xs[i:] for i in range(len(xs))] + [xs[:i] for i in range(1, len(xs))]
	def isTruncPrime(n):
		for t in truncations(str(n)):
			if int(t) not in ps:
				return False
		return True
	for p in ps:
		if isTruncPrime(p):
			c += 1
			tp.append(p)
			if c >= 15:
				return sum(tp)-17
	return tp

def gcd(a,b):
	def gcdHelper(a,b):
		diff = b%a
		if diff == 0:
			return a
		else:
			return gcd(diff, a)
	x = min(a,b)
	y = max(a,b)
	return gcdHelper(x,y)
def prod(xs):
	return reduce(lambda x,y: x * y, xs, 1)
def euler33():
	fracs = []
	for d in range(10,100):
		sd = str(d)
		for n in range(10,d):
			sn = str(n)
			n1 = 0
			d1 = 0
			if sn[0] == sd[1]:
				n1 = int(sn[1])
				d1 = int(sd[0])
			if sn[1] == sd[0]:
				n1 = int(sn[0])
				d1 = int(sd[1])
			if d1 != 0:
				if n1/d1 == n/d:
					fracs.append((n,d))
	ns = prod(map(lambda x: x[0], fracs))
	ds = prod(map(lambda x: x[1], fracs))
	return ds//gcd(ns,ds)

def modExp(x,e,m):
	ans = 1
	binE = "{0:b}".format(e)
	p = x
	for d in reversed(binE):
		if d == '1':
			ans = (ans*p) % m
		p = (p*p) % m
	return ans
def isPrimeFermat(n):
	for a in primes(10):
		if n == a:
			return True
		if n == 1:
			return False
		if n % a == 0:
			return False
		if modExp(a, n-1, n) != 1:
			return False
	return True
def euler41():
	ans = -1
	for i in range(3,10):
		permutations = permute(list(map(str,range(1,i))))
		for perm in permutations:
			n = int(''.join([] + perm))
			if isPrimeFermat(n):
				if n > ans:
					ans = n
	return ans
def squares():
	n = 1
	while True:
		yield n*n
		n += 1
def euler46():				
	squares = [x*x for x in range(100000)]
	def isSum(x):
		for s in squares:
			if 2*s >= x:
				return False
			if isPrimeFermat(x-2*s):
				return True
		return False #shouldnt get here
	x = 3
	while True:
		if not isSum(x):
			return x
		x = x + 2
def primeFactorize(n, ps):
		if isPrimeFermat(n):
			return [n]
		for p in ps:
			if n % p == 0:
				return primeFactorize(n//p,ps) + [p]
		print('oh no!!')
def euler47():
	ps = list(primes(10000))
	def has4Factors(n):
		return len(set(primeFactorize(n, ps))) == 4
	x = 2
	c = 0
	while True:
		if has4Factors(x):
			c += 1
			if c == 4:
				return x - 3
		else:
			c = 0
		x += 1
def combinate(xs, k):
	xs = sorted(xs)
	if k > len(xs):
		return []
	if k == len(xs):
		return [xs]
	if k == 1:
		return [[x] for x in set(xs)]
	c = 1
	while c < len(xs):
		if xs[c] != xs[0]:
			break
		c += 1
	return [x + [xs[0]] for x in combinate(xs[1:], k-1)] + combinate(xs[c:], k)
def euler49():
	def isArithmeticSequence(xs):
		d = xs[1] - xs[0]
		for i in range(1, len(xs)-1):
			if xs[i+1] - xs[i] != d:
				return False
		return True
	for x in range(1001,10000,2):
		sx = str(x)
		if '0' in sx:
			continue
		primePerms = sorted(filter(lambda x: isPrimeFermat(int(''.join(x))), set(map(tuple,permute(list(sx)))) ))
		if len(primePerms) >= 3:
			for ps in combinate(primePerms, 3):
				numps = [int(''.join(p)) for p in sorted(ps)]
				if isArithmeticSequence(numps) and 1487 not in numps:
					return numps
def euler50():
	memo = [0]
	for p in primes(500000):
		memo.append(p + memo[-1])
	def sumInterval(i,j):
		return memo[j] - memo[i]
	ans = 0
	for size in range(500,100000):
		for i in range(len(memo)-size):
			n = sumInterval(i,i+size)
			if isPrimeFermat(n):
				if n < 1000000:
					ans = n
					print(n)
					break
	return ans
def concat(xs):
	ans=[]
	for x in xs:
		ans.extend(x)
	return ans
def euler65():
	e = concat([[1,2*i,1] for i in range(1,34)])[:99]
	def reduceFraction(xs, z):
		if xs == []:
			return z
		else:
			(n,d) = z
			return reduceFraction(xs[:-1], (xs[-1]*n + d, n))
	return sum(map(int, list(str(reduceFraction([2]+e[:-1],(e[-1], 1))[0]))))
def euler89():
	ans = 0
	def savings(s):
		saved = 0
		if 'IIII' in s:
			saved += 3 if 'V' in s else 2
		if 'XXXX' in s:
			saved += 3 if 'L' in s else 2
		if 'CCCC' in s:
			saved += 3 if 'D' in s else 2
		return saved
	f = open("p089_roman.txt", "r")
	for line in f:
		ans += savings(line)
	return ans
def euler79():
	memo = {}
	f = open('p079_keylog.txt', 'r')
	for line in f:
		for i in range(len(line)-1):
			if line[i] in memo:
				memo[line[i]].update(line[i+1:-1])
			else:
				memo[line[i]] = set(line[i+1:-1])
	return ''.join(sorted(memo, key = lambda x: -1*len(memo[x])))
def euler92():
	memo = {1:1, 89:89}
	def end(n):
		if n in memo:
			return memo[n]
		else:
			next = sum([int(x)**2 for x in str(n)])
			e = end(next)
			if n < 568:
				memo[n] = e
			return e
	c = 0
	for i in range(1,10000000):
		if end(i) == 89:
			c+=1
	return c
def euler81():
	infinity = 10000000
	m = []
	f = open('p081_matrix.txt', 'r')
	for line in f:
		m.append(list(map(int, line[:-1].split(','))))
	s = len(m)
	for i in range(0,s):
		for j in range(0,s):
			if i == 0 and j == 0:
				top = left = 0
			elif i == 0:
				top = infinity
				left = m[i][j-1]
			elif j == 0:
				top = m[i-1][j]
				left = infinity
			else:
				top = m[i-1][j]
				left = m[i][j-1]
			m[i][j] = min(top,left) + m[i][j]
	return m[s-1][s-1]

def bins(xs):
	b = {}
	for x in xs:
		if x not in b:
			b[x] = 1
		else:
			b[x] += 1
	return b
def euler243():
	r = 15499/94744
	print(r)
	ps = list(primes(1000000))
	def totient(n):
		factors = bins(primeFactorize(n, ps))
		size = 1
		for p in factors:
			k=factors[p]
			size *= p**k - p**(k-1)
		return size
	def R(d):
		return totient(d)/(d-1)
	d = 1
	for i in range(100):
		d *= ps[i]
		if R(d) < r:
			d = d//(ps[i])
			while R(d) > r:
				d *= 2
			return d
def euler64():
	def next(n, k, sq, d):
		a = int((n*k*math.sqrt(sq)+n*d)/(k*k*sq-d*d))
		n1 = k*k*sq-d*d
		k1 = n*k
		d1 = a*(k*k*sq-d*d)-n*d
		f = gcd(gcd(n1,k1),d1)
		return (a, (n1//f, k1//f, sq, d1//f))
	oddLengthCounter = 0
	for i in range(10001):
		rt = int(math.sqrt(i))
		if rt*rt != i:
			lenCounter = 1
			f = (1,1,i,rt)
			while True:
				f = next(*f)[1]
				if f == (1,1,i,rt):
					break
				lenCounter += 1
			oddLengthCounter += lenCounter%2
	return oddLengthCounter
def wholeRoot(a,b,c):
	sq = b*b-4*a*c
	rt = int(math.sqrt(sq))
	if rt*rt == sq and (-1*b+rt)%(2*a) == 0:
		return int((-1*b+rt)/(2*a))
	return None
def euler100():
	b = 15
	s = 21
	while s <= 10**12: #diaphantine equations
		b1 = 3*b + 2*s - 2
		s = 4*b + 3*s - 3
		b = b1
	return b
def choose(n,r):
	return factorial(n)//(factorial(r)*factorial(n-r))
def printMatrix(m):
	for r in m:
		print(r)
def euler121():
	memo=[[1]]
	n=15
	for r in range(1,n+1):
		row = []
		for i in range(r+1):
			left = 0 if i == 0 else memo[r-1][i-1]
			right = 0 if i == r else memo[r-1][i]
			row.append(r*left + right)
		memo.append(row)
	total = sum(memo[n])
	wins = sum(memo[n][:math.ceil(n/2)])
	losses = total - wins
	return math.ceil(losses/wins)
def euler169():
	n = 10**25
	def expand(n):
		if n == 1:
			return [[1]]
		return [[n]] + [[n//2]+x for x in expand(n//2)]
	def f(d,sillyN):
		if d in fmemo:
			return fmemo[d]
		if d == 0:
			fmemo[0] = [(x,1) for x in expand(2**sillyN[0])]
			return fmemo[0]
		fmemo[d] = []
		for x in expand(2**sillyN[d]):
			v = 0
			for (y,v1) in f(d-1,sillyN):
				if len(set(x).intersection(set(y))) == 0:
					v += v1
			fmemo[d].append((x,v))
		return fmemo[d]
	fmemo = {}
	binN = list("{0:b}".format(n))
	binN.reverse()
	sillyN = []
	for i in range(len(binN)):
		if binN[i] == '1':
			sillyN.append(i)
	return sum([v for (x,v) in f(len(sillyN)-1, sillyN)])
def quadForm(a,b,c):
	sq = b*b - 4*a*c
	if sq < 0:
		return []
	return ((-b+math.sqrt(sq))/(2*a), (-b-math.sqrt(sq))/(2*a))
def euler144():
	def tangent(x,y):
		return -4*x/y
	def reflect(x, m):
		return (m*m*x+2*m-x)/(1+2*m*x-m*m)
	def hits(x,y):
		return not (-0.01 <= x and x <= 0.01 and y>0)
	def bounce(m,p):
		(x1,y1) = p
		(r1,r2) = quadForm((4+m*m), (2*m*y1-2*m*m*x1), (m*m*x1*x1-2*m*x1*y1+y1*y1-100))
		x = r1 if abs(r1-x1) > abs(r2-x1) else r2
		y = m*(x-x1)+y1
		newM = reflect(m, tangent(x,y))
		return (newM, (x,y))
	c = 0
	(m, (x,y)) = bounce((19.7/-1.4),(0,10.1))
	while hits(x,y):
		(m,(x,y)) = bounce(m,(x,y))
		c += 1
	return c
def euler55():
	def isPalindrome(n):
		s = list(str(n))
		return s == list(reversed(s))
	def lychrel(n,c):
		if c > 50:
			return True
		n1 = n + int(''.join(list(reversed(str(n)))))
		if isPalindrome(n1):
			return False
		return lychrel(n1,c+1)
	c = 0
	for i in range(10000):
		if lychrel(i,0):
			c+=1
	return c
def euler233():
	primeslist = list(primes(5000000))
	primes1 = [x for x in primeslist if x%4==1]
	primes23 = [x for x in primeslist if x%4!=1]
	freq = [3,2,1]
	freq1 = [7,3]
	freq2 = [10,2]
	ans = set([])
	bound = 10**11
	def pad(n):
		nonlocal ans
		if n > bound:
			return
		ans.add(n)
		for p in primes23:
			if p*n > bound:
				break
			if p*n not in ans:
				pad(p*n)
	for p1 in primes1:
		x1 = p1**freq[0]
		if x1 > bound:
			break
		for p2 in primes1:
			x12 = x1 * p2**freq[1]
			if x12 > bound:
				break
			for p3 in primes1:
				x123 = x12 * p3**freq[2]
				if x123 > bound:
					break
				if p1!=p2 and p2!=p3 and p1!=p3:
					pad(x123)
	for p1 in primes1:
		x1 = p1**freq1[0]
		y1 = p1**freq2[0]
		if x1 > bound:
			break
		for p2 in primes1:
			x12 = x1 * p2**freq1[1]
			y12 = y1 * p2**freq2[1]
			if x12 > bound and y12 > bound:
				break
			if p1 != p2:
				pad(x12)
				pad(y12)
	return sum(ans)

def sumToX(n,x):
	digits = str(n)
	ans = [digits[0]] # ans is set of strings
	for digit in digits[1:]:
		for (i,a) in enumerate(ans[:]): # a is a string
			ans.append(a + '+' + digit)
			ans.append(a + '-' + digit)
			ans[i] = a + digit
	return set(filter(lambda z: eval(z) == x, ans))
def testSumToX():
	assert sumToX(1,1) == set(['1'])
	assert sumToX(111,3) == set(['1+1+1'])
	assert '1+2+3-4+5+6+78+9' in sumToX(123456789, 100)
def foo(n,f):
	def step(ps):
		w = 1
		ans = 0
		for p in reversed(ps):
			ans += p*w
			w += 1
		ps.append(ans)
	c = 1
	ps = [1]
	while ps[-1] < n:
		step(ps)
		c += 1
	return c
def rotate90(m):
	printMatrix(m)
	size = len(m)
	for i in range(size//2):
		for j in range(i,size-i-1):
			temp = m[i][j]
			m[i][j] = m[-1*j-1][i]
			m[size-j-1][i] = m[size-i-1][size-j-1]
			m[size-i-1][size-j-1] = m[j][size-i-1]
			m[j][size-i-1] = temp
	printMatrix(m)
# m = [[1,2,3],[1,2,3],[1,2,3]]
# # m = [[1,2],[3,4]]
# rotate90(m)
def hanoiProblem(n):
	def hanoi(src, middle, target, size):
		if size == 1:
			target.push(src.pop())
		else:
			hanoi(src, target, middle, size-1)
			target.push(src.pop())
			hanoi(middle, src, target, size-1)
	src = Stack(list(reversed(range(n))))
	middle = Stack()
	target = Stack()
	hanoi(src, middle, target, n)
	return (src, middle, target)
def sortedStack(s):
	temp = Stack()
	while not s.isEmpty():
		e = s.pop()
		while not temp.isEmpty() and temp.peek() > e:
			s.push(temp.pop())
		temp.push(e)
	return temp
class Node():
	def __init__(self, value=None, parent=None):
		self.value = value
		self.parent = parent
		self.left = None
		self.right = None
	def __str__(self):
		return 'Node(' + str(self.left) + ' ' + str(self.value) + ' ' + str(self.right) + ')'
	def __repr__(self):
		return str(self)
class BST(Node):
	def __init__(self, xs=None):
		if type(xs) is list:
			Node.__init__(self)
			for x in xs:
				self.insert(x)
		elif type(xs) is Node:
			Node.__init__(self, xs.value, xs.parent)
		else:
			Node.__init__(self)
	def insert(self, x):
		if self.value == None:
			self.value = x
		elif x <= self.value:
			if self.left == None:
				self.left = BST(Node(x, self))
			else:
				self.left.insert(x)
		else:
			if self.right == None:
				self.right = BST(Node(x, self))
			else:
				self.right.insert(x)

# print(BST([4,3,1,6,7,9]))
def printSparseMatrix(m):
	width = max([p[0] for p in m]) + 1
	height = max([p[1] for p in m]) + 1
	matrix = [[0] * width for j in range(height)]
	for (i,j) in m:
		matrix[j][i] = m[(i,j)]
	printMatrix(matrix)

def makeKnightCrossword(wordList, width, height, density, difficulty):
	def printBoard(m):
		for line in m:
			print(' '.join(line))
	def fillBoard(m):
		letters = ['q','z','x']
		if difficulty == 'hard':
			letters = concat(wordList)
		width = max([p[0] for p in m]) + 1
		height = max([p[1] for p in m]) + 1
		matrix = [[letters[randint(0,len(letters)-1)] for i in range(width)] for j in range(height)]
		for (i,j) in m:
			matrix[j][i] = m[(i,j)]
		return matrix
	def inBounds(pos):
		(i,j) = pos
		return i >= 0 and j >= 0 and i < width and j < height
	def nextMoves(pos):
		(i,j) = pos
		moves = [(i-2,j-1), (i-2,j+1), (i+2,j-1), (i+2,j+1), (i-1,j-2), (i-1,j+2), (i+1,j-2), (i+1,j+2)]
		if difficulty == 'hard' and (i+j)%2 == 0:
		 	moves.reverse()
		return filter(inBounds, moves)
	def placeWord(pos, word, board):
		if pos in board and board[pos] != word[0]:
			return []
		if len(word) == 1:
			newBoard = board.copy()
			if pos not in board:
				newBoard[pos] = word[0]
			return [newBoard]
		newBoard = board.copy()
		if pos not in board:
			newBoard[pos] = word[0]

		for p in nextMoves(pos):
			moves = placeWord(p, word[1:], newBoard)
			if moves != []:
				return moves
		return []
	prevBoards = [{}]
	for word in wordList:
		boards = []
		for i in range(density):
			pos = (randint(0,width-1), randint(0,height-1))
			boards = concat([placeWord(pos, word, board.copy()) for board in prevBoards])
			if boards != []:
				break
		prevBoards = boards
	if prevBoards == []:
		return makeKnightCrossword(wordList,width+1,height+1,density,difficulty)
	printBoard(fillBoard(prevBoards[0]))
	return prevBoards[0]
# ws = ['sheepy', 'weepy', 'weepleton', 'rexabottom', 'noahbottom', 'grumplesaur','noahboa','fish','fraug','froglett']
# size = int(math.sqrt(len(concat(ws))))
# print('easy')
# makeKnightCrossword(ws, size, size, 2, 'easy')
# print('medium')
# makeKnightCrossword(ws, size, size, 1000, 'hard')
# print('hard')
# makeKnightCrossword(ws, size*2, size*2, 2, 'hard')

def isPalindrome(xs):
	return xs == reversed(xs)
# s = "abcba" * 99999
# cProfile.run('print(isPalindrome(s))')


class myThreadTest(threading.Thread):
	def __init__(self, lock, gc):
		super(myThreadTest, self).__init__()
		self.c = 0
		self.lock = lock
		self.gc = gc
	def run(self):
		while True:
			self.lock.acquire()
			if self.gc[0] < 100000:
				self.gc[0] += 1
				self.c += 1
				self.lock.release()
			else:
				self.lock.release()
				break
		print(self.c, self.gc)

def threadTest():
	threads = []
	gcLock = threading.Lock()
	gc = [0]
	for i in range(10):
		threads.append(myThreadTest(gcLock, gc))
		threads[-1].start()

def eat(g):
	lines = []
	print(__name__)
	for line in g:
		lines.append(line)
	return lines

def multiprocessingTest():
	lines = []
	def cb(ls):
		nonlocal lines
		print('here44')
		lines += ls
	with open('testWords.txt') as f:
		with Pool(3) as p:
			print('here')
			p1 = p.apply_async(eat, args = (f.readlines()[:],), callback = cb)
			p2 = p.apply_async(eat, args = (f,), callback = cb)
			p1.wait()
			p2.wait()
	print(lines)

if __name__ == '__main__':
	multiprocessingTest()