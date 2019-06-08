from enum import Enum, auto

from controller.rules.Capture import Capture
from controller.rules.CaptureGameEnding import CaptureGameEnding
from controller.rules.NoDoubleThree import NoDoubleThree
from controller.rules.ARule import ARule



class RuleFactory:

	class Name(Enum):
		CAPTURE = auto(),
		CAPTURE_GAME_ENDING = auto(),
		NO_DOUBLE_THREE = auto()

	type_to_class = {
		Name.CAPTURE: Capture,
		Name.CAPTURE_GAME_ENDING: CaptureGameEnding,
		Name.NO_DOUBLE_THREE: NoDoubleThree
	}

	@staticmethod
	def instantiate(name):
		if name in RuleFactory.type_to_class:
			return RuleFactory.type_to_class[name](name)

