#version 150

layout (points) in;
layout (triangle_strip, max_vertices = 36) out;

uniform mat4 modelViewProjectionMatrix;

in float vVoxelSize[];
in vec4 vVoxelColor[];

out vec2 vUV;
out vec4 vColor;

bool isCulled(vec4 normal) {
    return normal.x > 0;
}

void addQuad(vec4 color, vec4 center, float radius, vec3 topleft, vec3 topright, vec3 bottomleft, vec3 bottomright) {
    vec3 normal = cross(topright - topleft, bottomleft - topright);
    if (isCulled(modelViewProjectionMatrix * vec4(normal, 0.0))) {
        return;
    }
    vColor = color;
    gl_Position = modelViewProjectionMatrix * (center + radius * vec4(topleft, 0.0));
    EmitVertex();
    vColor = color;
    gl_Position = modelViewProjectionMatrix * (center + radius * vec4(topright, 0.0));
    EmitVertex();
    vColor = color;
    gl_Position = modelViewProjectionMatrix * (center + radius * vec4(bottomleft, 0.0));
    EmitVertex();
    EndPrimitive();
    
    vColor = color;
    gl_Position = modelViewProjectionMatrix * (center + radius * vec4(topright, 0.0));
    EmitVertex();
    vColor = color;
    gl_Position = modelViewProjectionMatrix * (center + radius * vec4(bottomright, 0.0));
    EmitVertex();
    vColor = color;
    gl_Position = modelViewProjectionMatrix * (center + radius * vec4(bottomleft, 0.0));
    EmitVertex();
    EndPrimitive();
}

void main(void) {
    for (int i = 0; i < gl_in.length(); ++i) {
        vec4 center = gl_in[i].gl_Position;
//        vec4 dx = modelViewProjectionMatrix[0] / 2.0f * vVoxelSize[i];
//        vec4 dy = modelViewProjectionMatrix[1] / 2.0f * vVoxelSize[i];
//        vec4 dz = modelViewProjectionMatrix[2] / 2.0f * vVoxelSize[i];
//        if (!isCulled(dx)) {
//            addQuad(center + dx, dy, dz, vVoxelColor[i]);
//        }
//        if (!isCulled(-dx)) {
//            addQuad(center - dx, dz, dy, vVoxelColor[i]);
//        }
//        if (!isCulled(dy)) {
//            addQuad(center + dy, dz, dx, vVoxelColor[i]);
//        }
//        if (!isCulled(-dy)) {
//            addQuad(center - dy, dx, dz, vVoxelColor[i]);
//        }
//        if (!isCulled(dz)) {
//            addQuad(center + dz, dx, dy, vVoxelColor[i]);
//        }
//        if (!isCulled(-dz)) {
//            addQuad(center - dz, dy, dx, vVoxelColor[i]);
//        }
        
        float radius = 0.5 * vVoxelSize[i];
        vec4 color = vVoxelColor[i];
        
        // front quad
        addQuad(color,
                center,
                radius,
                vec3(-1.0, -1.0, -1.0),
                vec3( 1.0, -1.0, -1.0),
                vec3( 1.0,  1.0, -1.0),
                vec3(-1.0,  1.0, -1.0));
        
        // back quad
        addQuad(color,
                center,
                radius,
                vec3( 1.0, -1.0, 1.0),
                vec3(-1.0, -1.0, 1.0),
                vec3(-1.0,  1.0, 1.0),
                vec3( 1.0,  1.0, 1.0));
        
        // left quad
        addQuad(color,
                center,
                radius,
                vec3(-1.0, -1.0,  1.0),
                vec3(-1.0, -1.0, -1.0),
                vec3(-1.0,  1.0, -1.0),
                vec3(-1.0,  1.0,  1.0));
        
        // right quad
        addQuad(color,
                center,
                radius,
                vec3(1.0, -1.0, -1.0),
                vec3(1.0, -1.0,  1.0),
                vec3(1.0,  1.0,  1.0),
                vec3(1.0,  1.0, -1.0));
        
        // top quad
        addQuad(color,
                center,
                radius,
                vec3(-1.0, -1.0,  1.0),
                vec3( 1.0, -1.0,  1.0),
                vec3( 1.0, -1.0, -1.0),
                vec3(-1.0, -1.0, -1.0));
        
        // bottom quad
        addQuad(color,
                center,
                radius,
                vec3(-1.0, 1.0, -1.0),
                vec3( 1.0, 1.0, -1.0),
                vec3( 1.0, 1.0,  1.0),
                vec3(-1.0, 1.0,  1.0));
    }
}
