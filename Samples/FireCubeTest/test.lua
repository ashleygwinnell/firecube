Rotator = {}
function Rotator:Init(instance)
	self.rotationSpeed = vec3(0, 0.5, 0)
	local a = vec3(1, 1,0)
	local b = vec3(1, 2,3)
	local c = a * b
	print(c.y)
end

function Rotator:Update(instance, time)
	self.node:Rotate(self.rotationSpeed * time)
end
