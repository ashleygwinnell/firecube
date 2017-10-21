function Script()
	return
	{
		SubscribeToEvent = function(self, ...) self.script:SubscribeToEvent(...) end
	}
end

EventManager = {
	events = {}
}

function EventManager:SubscribeToEvent(event, object, callback)
	local evt = self:_GetEvent(event)

	local found = false
	for index, value in ipairs (evt.listeners) do
		if value[1] == object then
			found = true
			break
		end
	end

	if not found then
		table.insert(evt.listeners, {object, callback})
	end
end

function EventManager:UnSubscribeFromEvent(event, object)
	local evt = self:_GetEvent(event)

	for index, value in ipairs (evt.listeners) do
		if value[1] == object then
			table.remove(evt.listeners, index)
			break
		end
	end
end

function EventManager:UnSubscribeFromAllEvents(object)
	for key, evt in pairs (self.events) do
		for index, value in ipairs (evt.listeners) do
			if value[1] == object then
				table.remove(evt.listeners, index)
				break
			end
		end
	end

end

function EventManager:EmitEvent(event, ...)
	local evt = self:_GetEvent(event)

	for index, value in ipairs (evt.listeners) do
		local obj = value[1]
		local func = value[2]

		func(obj, ...)
	end
end

function EventManager:_GetEvent(event)
	local ret = self.events[event]

	if not ret then
		ret = {
			name = event,
			listeners = {}
		}

		self.events[event] = ret
	end

	return ret
end