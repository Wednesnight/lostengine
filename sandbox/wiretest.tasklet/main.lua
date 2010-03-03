
local windowParams = WindowParams("wiretest", Rect(200,200,640,480))
local rootNode = nil

function startup(tasklet)
	rootNode = dcl.rg:Node
	{
		{
		  dcl.rg:ClearColor
		  {
		    color = Color(0,0,0)
		  },
		  dcl.rg:Clear
		  {
		    mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
		  },
		  dcl.rg:Camera3D
		  {
		    viewport = Rect(0, 0, 320, 240),
		    position = camPos,
		    fovY = 45.0,
		    depth = Vec2(1.0, 1000.0),
		    target = Vec3(0, 0, 0),
		    stickToTarget = true
		  },
		  dcl.rg:DepthTest
		  {
		    true
		  },
		  dcl.rg:Draw
		  {
		    name = "mesh",
		    mesh = dcl.mesh:Obj
		    {
		      filename = "magnolia_tri.obj",
		      material =
		      {
		        shader = dcl.gl:Shader
		        {
		          filename = "light",
		          params =
		          {
		            LightPosition = camPos,
		            LightColor = Color(1, 1, 1),
		            EyePosition = camPos,
		            Specular = Color(.75, .75, .5),
		            Ambient = Color(.1, .1, .1),
		            Kd = 0.8,
		            Scale = Vec2(0.7, 3.7),
		            Threshold = Vec2(.3, .2),
		            SurfaceColor = Color(1,1,1)
		          }
		        }
		      }
		    }
		  }
		}		
	}
	tasklet.renderNode:add(rootNode)
	return true
end

function update(tasklet)
	return true
end

function shutdown(tasklet)
	return true
end