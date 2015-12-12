def expectedGenerator(letterToNumber):
	n = 1
	while True:
		result = ''
		for l in letterToNumber:
			if n % letterToNumber[l] == 0:
				result += l
		if result != '':
			yield set(result)
		n += 1

		
# eg = expectedGenerator({'a':3,'b':2})
# for i in range(10):
# 	print(next(eg))
def charRange(start, end): #like range for chars, but inclusive both sides
	for i in range(ord(start), ord(end)+1):
		yield chr(i)

def betterFizzBuzz():
	with open("fizzBuzzData.txt") as f:
		stringList = f.read().split('\n')
		xs = [set(x) for x in stringList]
	
	letterToNumber = {}
	maxLetter = max(''.join(stringList))
	
	for letter in charRange('a', maxLetter):
		letterToNumber[letter] = 1
	
	i = 0
	g = expectedGenerator(letterToNumber)

	while i < len(xs):
		expected = next(g)
	
		if sorted(xs[i]) != sorted(expected):
			for e in expected - xs[i]:
				letterToNumber[e] += 1
	
			if xs[i] - expected != set() and len(xs[i]) != 1:
			 	for e in set(letterToNumber) - ((xs[i] - expected) | (expected - xs[i])):
			 		letterToNumber[e] += 1
						 
			g = expectedGenerator(letterToNumber)
			i = 0
		else:
			i += 1
	
	return [x[1] for x in sorted(letterToNumber.items(), key = lambda x: x[0])] #turns dict into expected output format

print(betterFizzBuzz())