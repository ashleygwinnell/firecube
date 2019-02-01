function Script()
	return
	{
		SubscribeToEvent = function(self, ...) self.script:SubscribeToEvent(...) end
	}
end

EventManager = {
	events = {}
}

function EventManager:SubscribeToEvent(...)
	local args = {...}
	local event, object, callback, sender
	if #args == 3 then
		event, object, callback = table.unpack(args)
	else
		event, sender, object, callback = table.unpack(args)
	end
	
	local evt = self:_GetEvent(event)

	local found = false
	for index, value in ipairs (evt.listeners) do
		if value[1] == object and value[3] == sender then
			found = true
			break
		end
	end

	if not found then
		table.insert(evt.listeners, {object, callback, sender})
	end
end

function EventManager:UnSubscribeFromEvent(event, object)
	local evt = self:_GetEvent(event)

	for index = #evt.listeners, 1, -1 do
		local value = evt.listeners[index]
		if value[1] == object then
			table.remove(evt.listeners, index)
		end
	end
end

function EventManager:UnSubscribeFromAllEvents(object)
	for key, evt in pairs (self.events) do
		for index = #evt.listeners, 1, -1 do
			local value = evt.listeners[index]
			if value[1] == object then
				table.remove(evt.listeners, index)
			end
		end
	end

end

function EventManager.CloneTable(org)
	return {table.unpack(org)}
end

function EventManager.IsItemInTable(tbl, item)
	for index, value in ipairs(tbl) do
		if value == item then
			return true
		end
	end
	
	return false
end

function EventManager:EmitEvent(event, sender, ...)
	local evt = self:_GetEvent(event)
	local originalListeners = EventManager.CloneTable(evt.listeners)
	
	for index, value in ipairs(originalListeners) do
		if EventManager.IsItemInTable(evt.listeners, value) then
			local obj = value[1]
			local func = value[2]
			local handlerSender = value[3]
			if handlerSender == nil or handlerSender == sender then
				func(obj, ...)
			end
		end
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