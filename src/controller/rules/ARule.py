class ARule:

	def __init__(self, name):
		self.name = name

	def can_place(self, gomuku, pos):
		return True

	def trigger_effect(self, gomuku, interface, pos):
		return
