color3d = Effect( "color3d", "EffectColorInstanced3d.effect" )
borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )

scene1 = Scene()	

Game.Command( "AddFPS" )

-- Add PhysX
pxScene = PxSceneComponent()
pxScene:AttachTo( scene1 )

root = scene1:FindObject( "root" )

proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth() / Game.GetHeight(), 1, 1000 )

-- Add camera...
camera = scene1:NewObject( "camera" )
cameraComponent = CameraComponent()
camera:Attach( cameraComponent )
cameraComponent:SetProjection( proj )
camera:Transform():SetPosition( V3( 0, 25, -47 ) )
camera:Transform():LookAt( V3() )

local i = 1

cubeParameters = ShapeParameters( "cube" )
cubeParameters:SetSize3( Size3.New( 2, 2, 2 ) )
cubeParameters:SetEffect( color3d )
cubeParameters:SetDiffuse( Color.NewRed() )
local cubeGeoRed = Geometry( cubeParameters )
cubeParameters:SetDiffuse( Color.NewGreen() )
local cubeGeoGreen = Geometry( cubeParameters )
cubeParameters:SetDiffuse( Color.NewBlue() )
local cubeGeoBlue = Geometry( cubeParameters )

sphereParameters = ShapeParameters( "sphere" )
sphereParameters:SetEffect( color3d )
sphereParameters:SetSegments( 24 )
sphereParameters:SetRadius( 1.0 )
sphereParameters:SetDiffuse( Color.NewRGB( 1, 1, 0 ) )
local sphereCyan = Geometry( sphereParameters )

local numR = 20
local numC = 20
local numL = 2
local offset = 3
for l = 0, numL - 1 do
	for r = 0, numR - 1 do
		for c = 0, numC - 1 do
			local x = (-offset * numR * 0.5) + (r * offset)
			local y = 10 + l * offset
			local z = (-offset * numC * 0.5) + (c * offset) + c % 3
			object = scene1:NewObject( "geo_" .. tostring( i ) ); i = i + 1;
			object:Transform():SetPosition( V3( x, y, z ) )
			
			local sh = i % 2
			if sh == 0 then
				local col = i % 3
				if col == 0 then
					object:AddGeometry(  cubeGeoRed )
				elseif col == 1 then
					object:AddGeometry(  cubeGeoGreen )
				else
					object:AddGeometry(  cubeGeoBlue )
				end
				local pxShape = PhysX.CreateBoxCollider( V3( 1.0, 1.0, 1.0 ) )
				pxShape:AttachTo( object )
			else
				local pxShape = PhysX.CreateSphereCollider( 1.0 )
				pxShape:AttachTo( object )
				object:AddGeometry( sphereCyan )
			end		
			pxBody = PhysX.CreateRigidBody()	
			pxBody:AttachTo( object )
		
		end
	end
end

planeParameters = ShapeParameters( "plane" )
planeParameters:SetEffect( color3d )
planeParameters:SetSegments( 2 )
planeParameters:SetSize2( Size2.New( 40.0, 40.0 ) )
planeParameters:SetDiffuse( Color.NewRGB( 0, 1, 1 ) )
plane = scene1:NewObject( "plane" )
plane:AddGeometry( Geometry( planeParameters ) )
plane:Transform():SetPosition( V3( 0, -20, 0 ) )

local terraParams = TerraParameters()
terraParams:SetEffect( Effect( "pit_image", "EffectPit.effect" ) )
terraParams:SetSize( Size2.New( 30, 30 ) )
terraParams:SetConstant( 0 )
terraParams:SetPoints( 100, 100 )
terraParams:SetHeightMap( Texture( "test256", "steps.bmp", true, true ), Color.NewGrey( 4 ) )
local terraGeo = Terra( terraParams )
local terra = scene1:NewObject( "terra" )
terra:AddGeometry( terraGeo )
terra:Transform():SetPosition( V3( 0, -12, 0 ) );
local pxShape = PhysX.CreateHeightFieldCollider( terraGeo, 30, 30 )
pxShape:AttachTo( terra )
local pxBody = PhysX.CreateRigidStatic()
pxBody:AttachTo( terra )
pxBody:SetDisableGravity( true )

-- Player
local playerGeo = Geometry( "player", "Mickey_Mouse/Mickey_Mouse.dae" )
player = scene1:NewObject( "player" )
player:AddGeometry( playerGeo )
daeMatrix = MatrixIdentity()
daeMatrix = Matrix.Mul( daeMatrix, MatrixScale( 2 ) )
daeMatrix = Matrix.Mul( daeMatrix, MatrixRotationAboutAxis( V3( 1.0, 0, 0 ), Angle.Degrees( 270.0 ) ) )
daeMatrix = Matrix.Mul( daeMatrix, MatrixRotationAboutAxis( V3( 0, 1.0, 0 ), Angle.Degrees( 90.0 ) ) )
player:SetModelMatrix( daeMatrix )
local playerCCT = pxScene:CreateCapsuleController( 1.0, 2 )
playerCCT:AttachTo( player )

--[[
local pxShape = PhysX.CreateCapsuleCollider( 1.0, 2 )
pxShape:AttachTo( player )
pxBody = PhysX.CreateRigidBody()	
pxBody:AttachTo( player )
]]
