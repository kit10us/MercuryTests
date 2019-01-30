color3d = Effect( "color3d", "EffectColor.effect" )

scene1 = Scene()
root = scene1:FindObject( "root" )

proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )

-- Add camera...
camera = scene1:NewCamera( "camera", proj )	
camera:Transform():SetPosition( V3( 0, 5, -17 ) )
camera:Transform():LookAt( V3() )

if Game.GetRendererCount() > 1 then
	proj2 = MatrixPerspectiveFovLH( 1, Game.GetWidth() / Game.GetHeight(), 1, 1000 )		--proj2 = MatrixOrthoOffCenterLH( 0, Game.GetWidth(), 0, Game.GetHeight(), -100, 100 )
	camera2 = scene1:NewCamera( "Camera2", proj )
	camera2:Transform():SetPosition( V3( 0, 0, -27 ) )
	camera2:Transform():LookAt( V3() )
	print( "--------------" );
	cameraComponent = camera2:GetComponent( "Camera" )
	cameraComponent:SetRenderer( 1 );
end

cube = scene1:NewObject( "cube" )
cube:AddGeometry( Geometry( "cube", "ShapeCube.shape" ) )
cube:Transform():SetPosition( V3( -4.5, 3, 0 ) )

pointfield = scene1:NewObject( "pointfield" )
pointfield:AddGeometry( Geometry( "pointfield", "ShapePointField.shape" ) )
pointfield:Transform():SetPosition( V3( -1.5, 3, 0 ) )
	
pointring = scene1:NewObject( "pointring" )
pointring:AddGeometry( Geometry( "pointring", "ShapePointRing.shape" ) )
pointring:Transform():SetPosition( V3( 1.5, 3, 0 ) )

dashring = scene1:NewObject( "dashring" )
dashring:AddGeometry( Geometry( "dashring", "ShapeDashRing.shape" ) )
dashring:Transform():SetPosition( V3( 4.5, 3, 0 ) )

pyramid = scene1:NewObject( "pyramid" )
pyramid:AddGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
pyramid:Transform():SetPosition( V3( -4.5, 0, 0 ) )
	
circle = scene1:NewObject( "circle" )
circle:AddGeometry( Geometry( "circle", "ShapeCircle.shape" ) )
circle:Transform():SetPosition( V3( -1.5, 0, 0 ) )

sphere = scene1:NewObject( "sphere" )
sphere:AddGeometry( Geometry( "sphere", "ShapeSphere.shape" ) )
sphere:Transform():SetPosition( V3( 1.5, 0, 0 ) )
   
cylinder = scene1:NewObject( "cylinder" )
cylinder:AddGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
cylinder:Transform():SetPosition( V3( 4.5, 0, 0 ) )

tube = scene1:NewObject( "tube" )
tube:AddGeometry( Geometry( "tube", "ShapeTube.shape" ) )
tube:Transform():SetPosition( V3( -4.5, -3, 0 ) )
	
plane = scene1:NewObject( "plane" )
plane:AddGeometry( Geometry( "plane", "ShapePlane.shape" ) )
plane:Transform():SetPosition( V3( -1.5, -3, 0 ) )
	
cone = scene1:NewObject( "cone" )
cone:AddGeometry( Geometry( "cone", "ShapeCone.shape" ) )
cone:Transform():SetPosition( V3( 1.5, -3, 0 ) )


-- Front Back up down left right..
frontShapeParameters = ShapeParameters( "cone" )
frontShapeParameters:SetEffect( color3d )
frontShapeParameters:SetSegments( 24 )
frontShapeParameters:SetRadius( 1 )
frontShapeParameters:SetHeight( 2 )
frontShapeParameters:SetDiffuse( Color.NewGreen() )
frontShapeParameters:SetCaps( true )
frontShape = Geometry( frontShapeParameters )
front = scene1:NewObject( "front" )
front:AddGeometry( frontShape )
front:Transform():SetPosition( V3( 0, 0, 10 ) );

back = scene1:NewObject( "back" )
back:AddGeometry( "sphere" )
back:Transform():SetPosition( V3( 0, 0, -10 ) );

up = scene1:NewObject( "pyramid" )
up:AddGeometry( "pyramid" )
up:Transform():SetPosition( V3( 0, 10, 0 ) );

down = scene1:NewObject( "down" )
down:AddGeometry( "cylinder" )
down:Transform():SetPosition( V3( 0, -10, 0 ) );	
