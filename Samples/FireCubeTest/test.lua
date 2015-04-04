Rotator = {}
function Rotator:Init()
	self.rotationSpeed = vec3(0, 0.5, 0)
end

function Rotator:SetRotationSpeed(speed)
	self.rotationSpeed = speed
end

function Rotator:Update(time)
	self.node:Rotate(self.rotationSpeed * time)
end
