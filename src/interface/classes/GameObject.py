# python has no abstract classes...

class GameObject(object):
	def render(self, screen):
		raise Exception('Must overload abstract class method')