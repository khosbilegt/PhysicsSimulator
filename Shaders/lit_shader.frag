#version 450
#extension GL_KHR_vulkan_glsl: enable
layout(set = 1, binding = 0) uniform sampler2D baseColorSampler;
layout(set = 1, binding = 1) uniform sampler2D emissiveSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;


void main() {
    if(fragTexCoord.x < 0) {
        outColor = fragColor;
    } else {
        vec4 baseColor = texture(baseColorSampler,fragTexCoord) * fragColor;
        vec4 emissiveColor = texture(emissiveSampler,fragTexCoord);
        outColor = baseColor + emissiveColor;
    }
}