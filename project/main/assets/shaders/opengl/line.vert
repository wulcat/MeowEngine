uniform mat4 MVP;

attribute vec3 aPos;

void main()
{
   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}