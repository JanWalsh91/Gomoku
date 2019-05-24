from classes.Minmax import Minmax
from classes.Board import Board

def main():
	board = Board(3)
	board.place([0, 0], 1)
	board.place([0, 1], 1)
	board.place([1, 1], 1)
	board.place([2, 1], 2)
	board.print()
	print(board.get_moves())
	print(board.heuristic(1))
	# minmax = Minmax(
	# 	heuristic=lambda b: b.heuristic(0), #heuristic for player 0 
	# 	get_child_nodes=lambda b: b.get_moves(),
	# 	depth=10)



if __name__ == '__main__':
	main()