#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Attributes
in vec4 vertex; //Vertex coordinates in model space
in vec4 normal; //Vertex normal in model space
in vec2 texCoord0;

//Varying variables
//out vec4 lightDir;
out vec2 iTexCoord0;
out vec4 aNormal;
//out vec4 viewDir;
out vec4 fragPos;

void main(void) {

    fragPos = M * vertex;
    
    aNormal = normalize(M * normal); //normal vector in world space

    iTexCoord0 = texCoord0;

    gl_Position = P * V * M * vertex;
}
