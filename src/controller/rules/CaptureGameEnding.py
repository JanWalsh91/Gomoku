from controller.rules.ARule import ARule

class CaptureGameEnding(ARule):
	# def can_place(self, board, player, pos):
	# 	return True
	
	def trigger_effect(self, board, player, pos, interface):
		return