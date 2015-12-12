from random import randint
import math

from flask import Flask, jsonify, redirect, url_for, escape, request

app = Flask(__name__)
#app.config['SECRET_KEY'] = 'secret!'
app.secret_key = 'A0Zr98j/3yX R~XHH!jmN]LWX/,?RT'

def concat(xs):
	ans=[]
	for x in xs:
		ans.extend(x)
	return ans
def makeKnightCrossword(wordList, width, height, density, difficulty):
	def printBoard(m):
		for row in m:
			line = [l[0] for l in row]
			print(' '.join(line))
	def stringifyBoard(m):
		s = ''
		for line in m:
			s = s + ' '.join(line) + '<br>'
		return s
	def htmlBoard(m):
		s = '<link rel="stylesheet" href="crossword.css">'
		s += '<table>'
		for line in m:
			s = s + '<tr>'
			for (letter, function) in line:
				s = s + '<td class="' + function + '">' + letter + '</td>'
			s = s + '</tr>'
		return s + '</table>'
	def fillBoard(m):
		letters = ['q','z','x']
		if difficulty == 'hard':
			letters = concat(wordList)
		width = max([p[0] for p in m]) + 1
		height = max([p[1] for p in m]) + 1
		matrix = [[(letters[randint(0,len(letters)-1)], 'unused') for i in range(width)] for j in range(height)]
		for (i,j) in m:
			matrix[j][i] = (m[(i,j)], 'used')
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
	finalBoard = fillBoard(prevBoards[0])
	printBoard(finalBoard)
	return htmlBoard(finalBoard)
	# return prevBoards[0]


# ws = ['sheepy', 'weepy', 'weepleton', 'rexabottom', 'noahbottom', 'grumplesaur','noahboa','fish','fraug','froglett']
# size = int(math.sqrt(len(concat(ws))))
# print('easy')
# makeKnightCrossword(ws, size, size, 2, 'easy')
# print('medium')
# makeKnightCrossword(ws, size, size, 1000, 'hard')
# print('hard')
# makeKnightCrossword(ws, size*2, size*2, 2, 'hard')

@app.route('/<path:path>')
def static_proxy(path):
	return app.send_static_file(path)

@app.route('/crossword')
def root():
	ws = ['sheep', 'dinosaur', 'fish', 'frog']
	size = int(math.sqrt(len(concat(ws))))
	crossword = makeKnightCrossword(ws, size+5, size+5, 1, 'hard')
	return crossword


if __name__ == "__main__":
	app.run(debug=True)