function OnBeforeStartup()
end

function OnUpdate()
	local rot = Quaternion.NewRotationX( Update.GetDelta() * 2 )
	_object:Transform():PreMul( rot )
end