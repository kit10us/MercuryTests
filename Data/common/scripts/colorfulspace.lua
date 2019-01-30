local axisIndex = 1
local totalRotation = 0
local autoRotate = true

function OnBeforeStartup()
	local scene1 = Scene()
	local root = scene1:FindObject( "root" )
	
	color3d = Effect( "color3d", "EffectColor.effect" )
	borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	
	local proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1600 )
	
	-- Add camera...
	camera = root:AddCamera( "camera", proj )	
	camera:Transform():SetPosition( V3( 0, 5, -250 ) )
	camera:Transform():LookAt( V3.Zero() ) 

	scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

	group = root:AddChild( "group" )
	
	cubeParameters = ShapeParameters( "cube" )
	cubeParameters:SetEffect( color3d )
	cubeParameters:SetSize3( Size3.New( 20, 20, 20 ) )
	cubeParameters:SetDiffuseFaces( Color.NewRed(), Color.NewGreen(), Color.NewBlue(), Color.NewRGB( 1, 1, 0 ), Color.NewRGB( 0, 1, 1 ), Color.NewRGB( 1, 0, 1 ) )
	cube = group:AddChild( "cube" )
	cube:AddGeometry( Geometry( cubeParameters ) )
	cube:Transform():SetPosition( V3( -4.5, 3, 0 ) )	

	local pointfieldParameters = ShapeParameters( "pointfield" )
	pointfieldParameters:SetEffect( color3d )
	pointfieldParameters:SetMinorRadius( 1000 )
	pointfieldParameters:SetMajorRadius( 1001.0 )
	pointfieldParameters:SetCount( 10000 )
	pointfieldParameters:SetDiffuse( Color.NewWhite() )
	local spacefield = root:AddChild( "spacefield" )
	spacefield:AddGeometry( Geometry( pointfieldParameters ) )
	
	local pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( color3d )
	pointringParameters:SetMinorRadius( 10 )
	pointringParameters:SetMajorRadius( 100 )
	pointringParameters:SetCount( 100000 )
	pointringParameters:SetDiffuse( Color.NewRed() )
	local myring = group:AddChild( "myring" )
	myring:AddGeometry( Geometry( pointringParameters ) )
	
	local pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( color3d )
	pointringParameters:SetMinorRadius( 5 )
	pointringParameters:SetMajorRadius( 50 )
	pointringParameters:SetCount( 100000 )
	pointringParameters:SetDiffuse( Color.NewBlue() )
	local myring2 = group:AddChild( "myring2" )
	myring2:AddGeometry( Geometry( pointringParameters ) )
	myring2:AddScript( "rotatex", "lua", "script/rotatex.lua" )
	myring2:AddScript( "rotatez", "lua", "script/rotatez.lua" )
	myring2:AddScript( "rotatey", "lua", "script/rotatey.lua" )

	local pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( color3d )
	pointringParameters:SetMinorRadius( 20 )
	pointringParameters:SetMajorRadius( 150 )
	pointringParameters:SetCount( 1000000 )
	pointringParameters:SetDiffuse( Color.NewGreen() )
	local myring3 = group:AddChild( "myring3" )
	myring3:AddGeometry( Geometry( pointringParameters ) )
	myring3:AddScript( "rotatey", "lua", "script/rotatey.lua" )
	myring3:AddScript( "rotatex", "lua", "script/rotatex.lua" )
	myring3:AddScript( "rotatez", "lua", "script/rotatez.lua" )
	myring3:AddScript( "rotatey", "lua", "script/rotatey.lua" )
end

function OnUpdate()
	local rotation = Update.GetDelta()
	
	local keyboard = Input( "Keyboard" )
	if keyboard then
		if keyboard:GetState( 0, "ESCAPE", "Pressed" ) == 1 then
			Game.Quit()
		end
		
		if keyboard:GetState( 0, "Space", "Pressed" ) == 1 then
			autoRotate = not autoRotate
		end
		
		if not autoRotate then
			if keyboard:GetState( 0, "Left", "Down" ) == 1 then
				local change = 1.0
				local rotation = MatrixRotationY( Update.GetDelta() * change )
				group:Transform():PostMul( rotation )
			elseif keyboard:GetState( 0, "Right", "Down" ) == 1 then
				local change = -1.0
				local rotation = MatrixRotationY( Update.GetDelta() * change )
				group:Transform():PostMul( rotation )			
			end			
			if keyboard:GetState( 0, "Up", "Down" ) == 1 then
				local change = 1.0
				local rotation = MatrixRotationX( Update.GetDelta() * change )
				group:Transform():PostMul( rotation )
			elseif keyboard:GetState( 0, "Down", "Down" ) == 1 then
				local change = -1.0
				local rotation = MatrixRotationX( Update.GetDelta() * change )
				group:Transform():PostMul( rotation )			
			end			
		end		
	end
	
	local mouse = Input( "Mouse" )
	if mouse then
		if mouse:GetState( 0, "RightButton", "Pressed" ) == 1 then
			autoRotate = not autoRotate
		end
		
		if not autoRotate then
			if mouse:GetState( 0, "LeftButton", "Down" ) == 1 then
				local changeX = mouse:GetValue( 0, "ChangeX" )
				local changeY = mouse:GetValue( 0, "ChangeY" )
				
				local rotation = MatrixRotationY( Update.GetDelta() * changeX * 0.2 )
				group:Transform():PostMul( rotation )

				local rotation = MatrixRotationX( Update.GetDelta() * changeY * 0.2 )
				group:Transform():PostMul( rotation )			
			end			
			
			local mouseWheel = mouse:GetValue( 0, "MouseWheel" )
			if mouseWheel > 0 or mouseWheel < 0 then
				local v = 1 + ( mouseWheel * Update.GetDelta() )
				Debug.WriteLine( tostring( v ) ) 
				local scale = MatrixScale( v )
				group:Transform():PostMul( scale )
			end			
		end
	end	
	
	local gamepad = Input( "Gamepad" )
	if gamepad then
		if gamepad:GetState( 0, "Back", "Pressed" ) == 1 then
			Game.Quit()
		end

		if gamepad:GetState( 0, "Y", "Pressed" ) == 1 then
			autoRotate = not autoRotate
		end
		
		if not autoRotate then
			if gamepad:HasValue( 0, "ThumbLX" ) then
				local v = gamepad:GetValue( 0, "ThumbLX" )
				if v > 0.1  or v < -0.1 then
					local rotation = MatrixRotationY( Update.GetDelta() * v * 2.0 )
					group:Transform():PostMul( rotation )
				end
			end	
			if gamepad:HasValue( 0, "ThumbLY" ) then
				local v = gamepad:GetValue( 0, "ThumbLY" )
				if v > 0.1  or v < -0.1 then
					local rotation = MatrixRotationX( Update.GetDelta() * v * 2.0 )
					group:Transform():PostMul( rotation )
				end
			end	
			
			if gamepad:HasValue( 0, "LeftTrigger" ) then
				local v = gamepad:GetValue( 0, "LeftTrigger" )
				local rotation = MatrixRotationZ( Update.GetDelta() * v * 2.0 )
				group:Transform():PreMul( rotation )				
			end
			if gamepad:HasValue( 0, "RightTrigger" ) then
				local v = gamepad:GetValue( 0, "RightTrigger" )
				local rotation = MatrixRotationZ( Update.GetDelta() * v * -2.0 )
				group:Transform():PreMul( rotation )				
			end
			
			if gamepad:HasValue( 0, "ThumbRX" ) then
				local v = gamepad:GetValue( 0, "ThumbRX" )
				if v > 0.1  or v < -0.1 then
					local rotation = MatrixRotationY( Update.GetDelta() * v * 2.0 )
					camera:Transform():PreMul( rotation )
				end
			end	
			if gamepad:HasValue( 0, "ThumbRY" ) then
				local v = gamepad:GetValue( 0, "ThumbRY" )
				if v > 0.1  or v < -0.1 then
					local rotation = MatrixRotationX( Update.GetDelta() * v * -2.0 )
					camera:Transform():PreMul( rotation )
				end
			end	

			if gamepad:GetState( 0, "DPAD_DOWN", "Down" ) == 1 then
				local v = 1 + ( -1 * Update.GetDelta() )
				Debug.WriteLine( tostring( v ) ) 
				local scale = MatrixScale( v )
				group:Transform():PostMul( scale )
			end								
			if gamepad:GetState( 0, "DPAD_UP", "Down" ) == 1 then
				local v = 1 + ( 1 * Update.GetDelta() )
				Debug.WriteLine( tostring( v ) ) 
				local scale = MatrixScale( v )
				group:Transform():PostMul( scale )
			end								
			
		end
	end	
	
	if autoRotate == true then
		totalRotation = totalRotation + rotation
		local pi2 = 3.1415926535 * 2
		if totalRotation > pi2 then
			totalRotation = totalRotation - pi2
			rotation = totalRotation -- Left over
			axisIndex = axisIndex + 1
			if axisIndex >= 3 then
				axisIndex = 0
			end
		end

		local axis = V3.Zero()
		if axisIndex == 0 then
			axis.x = 1
		elseif axisIndex == 1 then
			axis.y = 1
		elseif axisIndex == 2 then
			axis.z = 1
		end
		group:Transform():RotateAbout( axis, rotation )
		
		if axisIndex == 0 then
			axis.y = -1
		elseif axisIndex == 1 then
			axis.x = -1
		elseif axisIndex == 2 then
			axis.z = -1
		end
		
		camera:Transform():RotateAbout( axis, rotation )		
	end
end
