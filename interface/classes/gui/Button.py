from enum import Enum, auto
from classes.gui.Gui import Gui

class Button(Gui):
	class States(Enum):
		NORMAL = auto()
		HOVER = auto()
		DOWN = auto()

	def __init__(self, position=(0, 0), dimensions=(100, 100), rect=None, text='', background_color=(250, 250, 250), text_color=(0, 0, 0)):
		super().__init__(position, dimensions, background_color=background_color)
		self.color = background_color
		self.state = Button.States.NORMAL
		pass

	def handle_event(self):
		pass