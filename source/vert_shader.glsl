#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 MVP = mat4(1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0);

void main()
{
  gl_Position = MVP*vec4(aPos.x, aPos.y, aPos.z, 1.0);
};
