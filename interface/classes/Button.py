class Button(GameObject):
	class States(Enum):
		NORMAL = auto()
		HOVER = auto()
		DOWN = auto()

	def __init__(self, position, width, height, text, background_color, text_color):
		self.state = Button.States.NORMAL
		pass

	def handle_event(self):
		pass