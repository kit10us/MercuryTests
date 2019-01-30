function OnBeforeStartup()
end

function OnUpdate()
	local rot = Quaternion.NewRotationZ( Update.GetDelta() * 2 )
	_object:Transform():PreMul( rot )
end