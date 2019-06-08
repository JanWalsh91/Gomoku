class ARule:

	def __init__(self, name):
		self.name = name

	def can_place(self, board, player, pos):
		return True

	def trigger_effect(self, board, player, pos, interface):
		return
