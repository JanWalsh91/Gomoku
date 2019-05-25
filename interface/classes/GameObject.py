# python has no abstract classes...

class GameObject:
	def render(self, screen):
		raise Exception('Must overload abstract class method')