Rotator = Script()
function Rotator:Init()
	print("Init")
	self.rotationSpeed = vec3.new(0, 0.5, 0)
	self.script:SubscribeToEvent("HandleInput", Rotator.HandleInput)
end

function Rotator:Awake()
	print("Awake")
end

function Rotator:SetRotationSpeed(speed)
	self.rotationSpeed = speed
end

function Rotator:Update(time)
	self.node:Rotate(self.rotationSpeed * time)
end

function Rotator:HandleInput(time, mappedInput)
	if mappedInput:IsActionTriggered("Test") then
		print("Received input")
	end
end