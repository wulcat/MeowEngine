//
// Created by Akira Mujawar on 03/07/24.
//

in vec3 v_nearPoint;
in vec3 v_farPoint;

layout(location = 0) out vec4 v_fragColor;

float exampleFunction(float x, float y) {
    return x * x + y * y;
}

float partialDerivativeX(float x, float y,float h) {
        h = 1e-5;
    return (exampleFunction(x + h, y) - exampleFunction(x - h, y)) / (2 * h);
}

float partialDerivativeY(float x,float y,float h) {
    h = 1e-5;
return (exampleFunction(x, y + h) - exampleFunction(x, y - h)) / (2 * h);
}

vec2 sumOfAbsoluteDerivatives(float x,float y) {
    float df_dx = abs(partialDerivativeX(x, y, 0));
    float df_dy = abs(partialDerivativeY(x, y, 0));

    return vec2(df_dx, df_dy);
}

vec4 grid(vec3 fragPos3D, float scale) {


    vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
    //vec2 coord = point.xz * scale;
   //     if(coord.y > 0.0)
   //     coord.y = 1.0;
  //  if(coord.x > 0.0)
  //      coord.x = 1.0;
    vec2 derivative = fwidth(coord);

  // Compute derivatives manually
    float h = 1e-5;
    float coord_xh = (fragPos3D.x + h) * scale;
    float coord_yh = (fragPos3D.y + h) * scale;
    vec2 dcoord_dx = (coord_xh - coord) / h;
    vec2 dcoord_dy = (coord_yh - coord) / h;

    // Absolute values of derivatives
  //  vec2 derivative = vec2(abs(dcoord_dx.x), abs(dcoord_dy.y));

   // vec2 derivative = sumOfAbsoluteDerivatives(coord.x, coord.y);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
   // float alpha = 1.0 - smoothstep(0.0, 0.5, line);
    float alpha = 1 - min(line, 1.0);
   // float test = min(0.9,0.3);
    vec4 color = vec4(0.2, 0.2, 0.2, alpha);

    float minimumz = min(derivative.y , 1.0);
    float minimumx = min(derivative.x , 1.0);


    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
    {
        //color.x = 1.0;
        //color.y = 1.0;
        color.z = 1.0;
    }
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz) {
        color.x = 1.0;
        //color.y = 1.0;
       // color.z = 1.0;
    }

   // color.z = 1.0;
   // color.x = 1.0;
   // color.w = 0.7;
   // return  vec4(derivative, 0.0, 1);

    return color;
}

vec4 grid1(vec3 point, float scale, bool is_axis) {
    vec2 coord = point.xz * scale;
    vec2 dd    = fwidth(coord);
    vec2 uv    = fract(coord - 0.5) - 0.5;
    vec2 grid  = abs(uv) / dd;  // TODO: figure this out, adjust the line thickness
    float line = min(grid.x, grid.y);
    float min_z = min(dd.y, 1.0);
    float min_x = min(dd.x, 1.0);
    vec4 col    = vec4(0.3);
    col.a       = 1.0 - min(line, 1.0);

    if (-1.0 * min_x < point.x && point.x < 0.1 * min_x && is_axis)
        col.rgb = vec3(0.427, 0.792, 0.909);
    if (-1.0 * min_z < point.z && point.z < 0.1 * min_z && is_axis)
        col.rgb = vec3(0.984, 0.380, 0.490);
    return col;
}

vec3 test122() {
// Convert fragment coordinates to world space
    vec3 fragCoordWorld = gl_FragCoord.xyz;

    // Calculate offset from camera position to fragment position
    vec3 fragOffset = fragCoordWorld;// - vec3(0,2,-3);

    // Calculate grid coordinates
    vec3 gridCoords = floor(fragOffset / 10);

    // Calculate fractional parts
    vec3 fractPart = fract(fragOffset / 2);

    // Determine grid lines color (black or white based on fractional parts)
    vec3 gridColor = vec3(0.0); // Start with black color
    if (fractPart.x < 0.02 || fractPart.y < 0.02 || fractPart.z < 0.02) {
        gridColor = vec3(1.0); // White color for grid lines
    }
    return gridColor;
}

vec3 test22() {
// Normalize fragment coordinates to [0, 1] range
    vec2 uv = gl_FragCoord.xy / vec2(800.0, 600.0); // Replace 800.0 and 600.0 with your window size
    // vec2 uv = fragPos3D.xz * 10;
    // Calculate the position in grid coordinates
    vec2 gridCoords = (uv - 0.5) * 10;

    // Calculate fractional parts
    vec2 fractPart = fract(gridCoords / 2);

    // Determine grid lines color (black or white based on fractional parts)
    vec3 gridColor = vec3(0.0); // Start with black color
    if (fractPart.x < 0.02 || fractPart.y < 0.02) {
        gridColor = vec3(1.0); // White color for grid lines
    }

    return gridColor;
}

void main() {
    float t = -v_nearPoint.y / (v_farPoint.y - v_nearPoint.y);
    vec3 fragPos3D = v_nearPoint + t * (v_farPoint - v_nearPoint);
    v_fragColor = grid(fragPos3D, 10.0) * float(t > 0);



    //v_fragColor = vec4(test122(), 1.0); // Output grid color
    //v_fragColor = vec4(test22(), 1.0); // Output grid color




  //  v_fragColor = vec4(1.0, 0.0, 0.0, 1.0);// * float(t > 0));

   // vec2 coord = fragPos3D.xz * 10.0; // use the scale variable to set the distance between the lines
  //  if(coord.y > 0.0)
   //     coord.y = 1.0;
  //  if(coord.x > 0.0)
  //      coord.x = 1.0;
   // vec2 derivative = fwidth(coord);
  //  vec2 derivative = sumOfAbsoluteDerivatives(coord.x, coord.y) ;
  //  vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
   // float line = min(grid.x, grid.y);
  //  float alpha = 1.0 - min(line, 1.0);
    //v_fragColor = vec4(alpha, 0.0, 0.0, 1.0);
}

